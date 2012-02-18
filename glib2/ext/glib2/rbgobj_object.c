/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2002-2004  Ruby-GNOME2 Project Team
 *  Copyright (C) 2002-2003  Masahiro Sakai
 *  Copyright (C) 1998-2000 Yukihiro Matsumoto,
 *                          Daisuke Kanda,
 *                          Hiroshi Igarashi
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 *  MA  02110-1301  USA
 */

#include "rbgprivate.h"

#define RG_TARGET_NAMESPACE rbgobj_cObject

VALUE RG_TARGET_NAMESPACE;
static VALUE eNoPropertyError;
static GQuark RUBY_GOBJECT_OBJ_KEY;

/* deperecated */
void
rbgobj_add_abstract_but_create_instance_class(G_GNUC_UNUSED GType gtype)
{
}

static void
weak_notify(gpointer data, G_GNUC_UNUSED GObject *where_the_object_was)
{
    gobj_holder *holder = data;

    rbgobj_instance_call_cinfo_free(holder->gobj);
    rbgobj_invalidate_relatives(holder->self);
    holder->destroyed = TRUE;

    g_object_unref(holder->gobj);
    holder->gobj = NULL;
}

static void
holder_mark(gobj_holder *holder)
{
    if (holder->gobj && !holder->destroyed)
        rbgobj_instance_call_cinfo_mark(holder->gobj);
}

static void
holder_free(gobj_holder *holder)
{
    if (holder->gobj) {
        if (!holder->destroyed) {
            g_object_set_qdata(holder->gobj, RUBY_GOBJECT_OBJ_KEY, NULL);
            g_object_weak_unref(holder->gobj, (GWeakNotify)weak_notify, holder);
            weak_notify(holder, holder->gobj);
        }
        holder->gobj = NULL;
    }
    xfree(holder);
}

static VALUE
gobj_s_allocate(VALUE klass)
{
    gobj_holder* holder;
    VALUE result;

    result = Data_Make_Struct(klass, gobj_holder, holder_mark, holder_free, holder);
    holder->self  = result;
    holder->gobj  = NULL;
    holder->cinfo = NULL;
    holder->destroyed = FALSE;

    return result;
}

/* deprecated */
VALUE
rbgobj_create_object(VALUE klass)
{
    return gobj_s_allocate(klass);
}

void
rbgobj_gobject_initialize(VALUE obj, gpointer cobj)
{
    gobj_holder* holder = g_object_get_qdata((GObject*)cobj, RUBY_GOBJECT_OBJ_KEY);
    if (holder)
        rb_raise(rb_eRuntimeError, "ruby wrapper for this GObject* already exists.");
    Data_Get_Struct(obj, gobj_holder, holder);
    holder->cinfo = RVAL2CINFO(obj);
    holder->gobj  = (GObject*)cobj;
    holder->destroyed = FALSE;

    g_object_set_qdata((GObject*)cobj, RUBY_GOBJECT_OBJ_KEY, (gpointer)holder);
    g_object_weak_ref((GObject*)cobj, (GWeakNotify)weak_notify, holder);
    {
        GType t1 = G_TYPE_FROM_INSTANCE(cobj);
        GType t2 = CLASS2GTYPE(CLASS_OF(obj));

        if (t1 != t2) {
            if (!g_type_is_a(t1, t2))
                rb_raise(rb_eTypeError, "%s is not subtype of %s",
                         g_type_name(t1), g_type_name(t2));
        }
    }
}

VALUE
rbgobj_get_ruby_object_from_gobject(GObject* gobj, gboolean alloc)
{
    gobj_holder *holder;

    holder = g_object_get_qdata(gobj, RUBY_GOBJECT_OBJ_KEY);
    if (holder) {
        return holder->self;
    } else if (alloc) {
        VALUE obj;

        obj = gobj_s_allocate(GTYPE2CLASS(G_OBJECT_TYPE(gobj)));
        gobj = g_object_ref(gobj);
        rbgobj_gobject_initialize(obj, (gpointer)gobj);
        return obj;
    } else {
        return Qnil;
    }
}

GObject*
rbgobj_get_gobject(VALUE obj)
{
    gobj_holder* holder;

    if (!RVAL2CBOOL(rb_obj_is_kind_of(obj, GTYPE2CLASS(G_TYPE_OBJECT))))
        rb_raise(rb_eTypeError, "not a GLib::Object");

    Data_Get_Struct(obj, gobj_holder, holder);

    if (holder->destroyed)
        rb_raise(rb_eTypeError, "destroyed GLib::Object");
    if (!holder->gobj)
        rb_raise(rb_eTypeError, "uninitialize GLib::Object");

    return holder->gobj;
}

static VALUE
dummy_init(int argc, VALUE *argv, VALUE self)
{
    GType gtype = CLASS2GTYPE(CLASS_OF(self));
    if (G_TYPE_IS_ABSTRACT(gtype))
        rb_raise(rb_eTypeError, "initializing abstract class");
    else
        return rb_call_super(argc, argv);
}

void
rbgobj_init_object_class(VALUE klass)
{
    rbgobj_define_property_accessors(klass);
    if (G_TYPE_IS_ABSTRACT(CLASS2GTYPE(klass)))
        rbg_define_method(klass, "initialize", dummy_init, -1);
}

/**********************************************************************/

static gboolean
is_gtkobject(GObject *gobj)
{
    static GType gtype_gtkobject = G_TYPE_INVALID;
    if (!gtype_gtkobject)
        gtype_gtkobject = g_type_from_name("GtkObject");
    return gtype_gtkobject && g_type_is_a(G_OBJECT_TYPE(gobj), gtype_gtkobject);
}

static void
gobj_mark(gpointer ptr)
{
    GObject* gobj = ptr;
    guint n_properties;
    GParamSpec** properties;
    GValue gval = G_VALUE_INIT;
    guint i;

    properties = g_object_class_list_properties(G_OBJECT_GET_CLASS(gobj), &n_properties);

    for (i = 0; i < n_properties; i++) {
        GParamSpec* pspec = properties[i];
        GType value_type = G_PARAM_SPEC_VALUE_TYPE(pspec);
        if (G_TYPE_FUNDAMENTAL(value_type) != G_TYPE_OBJECT) continue;
        if (!(pspec->flags & G_PARAM_READABLE)) continue;
        /* FIXME: exclude types that doesn't have identity. */

        {
            g_value_init(&gval, value_type);
            g_object_get_property(gobj, pspec->name, &gval);
            rbgobj_gc_mark_gvalue(&gval);
            g_value_unset(&gval);
        }
    }

    g_free(properties);
}

static VALUE
rg_s_new_bang(int argc, VALUE *argv, VALUE self)
{
    const RGObjClassInfo* cinfo = rbgobj_lookup_class(self);
    VALUE params_hash;
    GObject* gobj;
    VALUE result;

    rb_scan_args(argc, argv, "01", &params_hash);

    if (!NIL_P(params_hash))
        Check_Type(params_hash, T_HASH);

    if (cinfo->klass != self)
        rb_raise(rb_eTypeError, "%s isn't registered class",
                 rb_class2name(self));

    gobj = rbgobj_gobject_new(cinfo->gtype, params_hash);
    result = GOBJ2RVAL(gobj);

    // XXX: Ughhhhh
    if (is_gtkobject(gobj)){
        // We can't call gtk_object_sink() here.
        // But hopefully someone will call it in the future.
        //gtk_object_sink(gobj);
    } else {
        g_object_unref(gobj);
    }

    return result;
}

struct param_setup_arg {
    GObjectClass* gclass;
    GParameter* params;
    guint param_size;
    VALUE params_hash;
    guint index;
};

static VALUE
_params_setup(VALUE arg, struct param_setup_arg *param_setup_arg)
{
    guint index;
    VALUE name, val;
    GParamSpec* pspec;

    index = param_setup_arg->index;
    if (index >= param_setup_arg->param_size)
       rb_raise(rb_eArgError, "too many parameters");

    name = rb_ary_entry(arg, 0);
    val  = rb_ary_entry(arg, 1);

    if (SYMBOL_P(name))
        param_setup_arg->params[index].name = rb_id2name(SYM2ID(name));
    else
        param_setup_arg->params[index].name = StringValuePtr(name);

    pspec = g_object_class_find_property(
        param_setup_arg->gclass,
        param_setup_arg->params[index].name);
    if (!pspec)
        rb_raise(rb_eArgError, "No such property: %s",
                 param_setup_arg->params[index].name);

    g_value_init(&(param_setup_arg->params[index].value),
                 G_PARAM_SPEC_VALUE_TYPE(pspec));
    rbgobj_rvalue_to_gvalue(val, &(param_setup_arg->params[index].value));

    param_setup_arg->index++;

    return Qnil;
}

static VALUE
gobj_new_body(struct param_setup_arg* arg)
{
    rb_iterate(rb_each, (VALUE)arg->params_hash, _params_setup, (VALUE)arg);
    return (VALUE)g_object_newv(G_TYPE_FROM_CLASS(arg->gclass),
                                arg->param_size, arg->params);
}

static VALUE
gobj_new_ensure(struct param_setup_arg* arg)
{
    guint i;
    g_type_class_unref(arg->gclass);
    for (i = 0; i < arg->param_size; i++) {
        if (G_IS_VALUE(&arg->params[i].value))
            g_value_unset(&arg->params[i].value);
    }
    return Qnil;
}

GObject*
rbgobj_gobject_new(GType gtype, VALUE params_hash)
{
    GObject* result;

    if (!g_type_is_a(gtype, G_TYPE_OBJECT))
        rb_raise(rb_eArgError,
                 "type \"%s\" is not descendant of GObject",
                 g_type_name(gtype));

    if (NIL_P(params_hash)) {
        result = g_object_newv(gtype, 0, NULL);
    } else {
        size_t param_size;
        struct param_setup_arg arg;

        param_size = NUM2INT(rb_funcall(params_hash, rb_intern("length"), 0)); 

        arg.param_size = param_size;
        arg.gclass = G_OBJECT_CLASS(g_type_class_ref(gtype));
        arg.params = ALLOCA_N(GParameter, param_size);
        memset(arg.params, 0, sizeof(GParameter) * param_size);
        arg.params_hash = params_hash;
        arg.index = 0;

        result = (GObject*)rb_ensure(&gobj_new_body, (VALUE)&arg,
                                     &gobj_new_ensure, (VALUE)&arg);
    }

    if (!result)
        rb_raise(rb_eRuntimeError, "g_object_newv failed");

    return result;
}

static VALUE
rg_s_install_property(int argc, VALUE* argv, VALUE self)
{
    const RGObjClassInfo* cinfo = rbgobj_lookup_class(self);
    gpointer gclass;
    GParamSpec* pspec;
    VALUE pspec_obj, prop_id;

    if (cinfo->klass != self)
        rb_raise(rb_eTypeError, "%s isn't registered class",
                 rb_class2name(self));

    rb_scan_args(argc, argv, "11", &pspec_obj, &prop_id);
    pspec = G_PARAM_SPEC(RVAL2GOBJ(pspec_obj));

    gclass = g_type_class_ref(cinfo->gtype);
    g_object_class_install_property(gclass,
                                    NIL_P(prop_id) ? 1 : NUM2UINT(prop_id),
                                    pspec);
    g_type_class_unref(gclass);

    /* FIXME: define accessor methods */

    return Qnil;
}

static VALUE
gobj_s_property(VALUE self, VALUE property_name)
{
    GObjectClass* oclass;
    const char* name;
    GParamSpec* prop;
    VALUE result;

    if (SYMBOL_P(property_name))
        name = rb_id2name(SYM2ID(property_name));
    else
        name = StringValuePtr(property_name);

    oclass = g_type_class_ref(CLASS2GTYPE(self));

    prop = g_object_class_find_property(oclass, name);
    if (!prop){
        g_type_class_unref(oclass);
        rb_raise(eNoPropertyError, "No such property: %s", name);
    }

    result = GOBJ2RVAL(prop);
    g_type_class_unref(oclass);
    return result;
}

static VALUE
gobj_s_properties(int argc, VALUE* argv, VALUE self)
{
    GObjectClass* oclass = g_type_class_ref(CLASS2GTYPE(self));
    guint n_properties;
    GParamSpec** props;
    VALUE inherited_too;
    VALUE ary;
    guint i;

    if (rb_scan_args(argc, argv, "01", &inherited_too) == 0)
        inherited_too = Qtrue;

    props = g_object_class_list_properties(oclass, &n_properties);

    ary = rb_ary_new();
    for (i = 0; i < n_properties; i++){
        if (RVAL2CBOOL(inherited_too)
            || GTYPE2CLASS(props[i]->owner_type) == self)
            rb_ary_push(ary, rb_str_new2(props[i]->name));
    }
    g_free(props);
    g_type_class_unref(oclass);
    return ary;
}

static VALUE type_to_prop_setter_table;
static VALUE type_to_prop_getter_table;

void
rbgobj_register_property_setter(GType gtype, const char *name, RValueToGValueFunc func)
{
    GObjectClass* oclass;
    GParamSpec* pspec;

    VALUE table = rb_hash_aref(type_to_prop_setter_table, INT2FIX(gtype));
    if (NIL_P(table)){
        table = rb_hash_new();
        rb_hash_aset(type_to_prop_setter_table, INT2FIX(gtype), table);
    }

    oclass = g_type_class_ref(gtype);
    pspec = g_object_class_find_property(oclass, name);

    rb_hash_aset(table, CSTR2RVAL(g_param_spec_get_name(pspec)),
                 Data_Wrap_Struct(rb_cData, NULL, NULL, func));

    g_type_class_unref(oclass);
}

void
rbgobj_register_property_getter(GType gtype, const char *name, GValueToRValueFunc func)
{
    GObjectClass* oclass;
    GParamSpec* pspec;

    VALUE table = rb_hash_aref(type_to_prop_getter_table, INT2FIX(gtype));
    if (NIL_P(table)){
        table = rb_hash_new();
        rb_hash_aset(type_to_prop_getter_table, INT2FIX(gtype), table);
    }

    oclass = g_type_class_ref(gtype);
    pspec = g_object_class_find_property(oclass, name);

    rb_hash_aset(table, CSTR2RVAL(g_param_spec_get_name(pspec)),
                 Data_Wrap_Struct(rb_cData, NULL, NULL, func));

    g_type_class_unref(oclass);
}

static VALUE
rg_set_property(VALUE self, VALUE prop_name, VALUE val)
{
    GParamSpec* pspec;
    const char* name;

    if (SYMBOL_P(prop_name))
        name = rb_id2name(SYM2ID(prop_name));
    else
        name = StringValuePtr(prop_name);

    pspec = g_object_class_find_property(G_OBJECT_GET_CLASS(RVAL2GOBJ(self)),
                                         name);

    if (!pspec)
        rb_raise(eNoPropertyError, "No such property: %s", name);
    else {
        // FIXME: use rb_ensure to call g_value_unset()
        RValueToGValueFunc setter = NULL;
        GValue gval = G_VALUE_INIT;

        g_value_init(&gval, G_PARAM_SPEC_VALUE_TYPE(pspec));

        {
            VALUE table = rb_hash_aref(type_to_prop_setter_table,
                                       INT2FIX(pspec->owner_type));
            if (!NIL_P(table)){
                VALUE obj = rb_hash_aref(table, CSTR2RVAL(g_param_spec_get_name(pspec)));
                if (!NIL_P(obj))
                    Data_Get_Struct(obj, void, setter);
            }
        }
        if (setter) {
            setter(val, &gval);
        } else {
            rbgobj_rvalue_to_gvalue(val, &gval);
        }

        g_object_set_property(RVAL2GOBJ(self), name, &gval);
        g_value_unset(&gval);

        G_CHILD_SET(self, rb_intern(name), val);

        return self;
    }
}

static VALUE
rg_get_property(VALUE self, VALUE prop_name)
{
    GParamSpec* pspec;
    const char* name;

    if (SYMBOL_P(prop_name))
        name = rb_id2name(SYM2ID(prop_name));
    else
        name = StringValuePtr(prop_name);

    pspec = g_object_class_find_property(G_OBJECT_GET_CLASS(RVAL2GOBJ(self)),
                                         name);

    if (!pspec)
        rb_raise(eNoPropertyError, "No such property: %s", name);
    else {
        // FIXME: use rb_ensure to call g_value_unset()
        GValueToRValueFunc getter = NULL;
        GValue gval = G_VALUE_INIT;
        VALUE ret;

        {
            VALUE table = rb_hash_aref(type_to_prop_getter_table,
                                       INT2FIX(pspec->owner_type));
            if (!NIL_P(table)){
                VALUE obj = rb_hash_aref(table, CSTR2RVAL(g_param_spec_get_name(pspec)));
                if (!NIL_P(obj))
                    Data_Get_Struct(obj, void, getter);
            }
        }

        g_value_init(&gval, G_PARAM_SPEC_VALUE_TYPE(pspec));
        g_object_get_property(RVAL2GOBJ(self), name, &gval);

        ret = getter ? getter(&gval) : GVAL2RVAL(&gval);
        g_value_unset(&gval);

        G_CHILD_SET(self, rb_intern(name), ret);

        return ret;
    }
}

static VALUE rg_thaw_notify(VALUE self);

static VALUE
rg_freeze_notify(VALUE self)
{
    g_object_freeze_notify(RVAL2GOBJ(self));
    if (rb_block_given_p()) {
        return rb_ensure(rb_yield, self, rg_thaw_notify, self);
    }
    return self;
}

static VALUE
rg_notify(VALUE self, VALUE property_name)
{
    g_object_notify(RVAL2GOBJ(self), StringValuePtr(property_name));
    return self;
}

static VALUE
rg_thaw_notify(VALUE self)
{
    g_object_thaw_notify(RVAL2GOBJ(self));
    return self;
}

static VALUE
rg_destroyed_p(VALUE self)
{
    gobj_holder* holder;

    if (!RVAL2CBOOL(rb_obj_is_kind_of(self, GTYPE2CLASS(G_TYPE_OBJECT))))
        rb_raise(rb_eTypeError, "not a GLib::Object");

    Data_Get_Struct(self, gobj_holder, holder);

    return CBOOL2RVAL(holder->destroyed);
}

static VALUE
rg_inspect(VALUE self)
{
    gobj_holder* holder;
    const char *class_name;
    char *s;
    VALUE result;

    Data_Get_Struct(self, gobj_holder, holder);

    class_name = rb_class2name(CLASS_OF(self));
    if (!holder->destroyed)
        s = g_strdup_printf("#<%s:%p ptr=%p>", class_name, (void *)self,
                            holder->gobj);
    else
        s = g_strdup_printf("#<%s:%p destroyed>", class_name, (void *)self);

    result = rb_str_new2(s);
    g_free(s);

    return result;
}

static VALUE
rg_type_name(VALUE self)
{
    return CSTR2RVAL(G_OBJECT_TYPE_NAME(RVAL2GOBJ(self)));
}

static VALUE
rg_initialize(int argc, VALUE *argv, VALUE self)
{
    VALUE params_hash;
    GObject* gobj;

    rb_scan_args(argc, argv, "01", &params_hash);

    if (!NIL_P(params_hash))
        Check_Type(params_hash, T_HASH);

    gobj = rbgobj_gobject_new(RVAL2GTYPE(self), params_hash);

    if (is_gtkobject(gobj)){
        gobj = g_object_ref(gobj);
        // We can't call gtk_object_sink() here.
        // But hopefully someone will call it in the future.
        //gtk_object_sink(gobj);
    }

    G_INITIALIZE(self, gobj);
    return Qnil;
}

static VALUE
gobj_ref_count(VALUE self)
{
    gobj_holder* holder;
    Data_Get_Struct(self, gobj_holder, holder);
    return INT2NUM(holder->gobj ? holder->gobj->ref_count : 0);
}

/**********************************************************************/

static VALUE proc_mod_eval;
static GQuark q_ruby_setter;
static GQuark q_ruby_getter;

// FIXME: use rb_protect
static void
get_prop_func(GObject* object,
              G_GNUC_UNUSED guint property_id,
              GValue* value,
              GParamSpec* pspec)
{
    ID ruby_getter = (ID)g_param_spec_get_qdata(pspec, q_ruby_getter);
    if (!ruby_getter) {
        gchar* name = g_strdup(g_param_spec_get_name(pspec));
        gchar* p;
        for (p = name; *p; p++) {
          if (*p == '-')
            *p = '_';
        }
        ruby_getter = rb_intern(name);
        g_param_spec_set_qdata(pspec, q_ruby_getter, (gpointer)ruby_getter);
        g_free(name);
    }

    {
        VALUE ret = rb_funcall(GOBJ2RVAL(object), ruby_getter, 0);
        rbgobj_rvalue_to_gvalue(ret, value);
    }
}

// FIXME: use rb_protect
static void
set_prop_func(GObject* object,
              G_GNUC_UNUSED guint property_id,
              const GValue* value,
              GParamSpec* pspec)
{
    ID ruby_setter = (ID)g_param_spec_get_qdata(pspec, q_ruby_setter);
    if (!ruby_setter) {
        gchar* name = g_strconcat(g_param_spec_get_name(pspec), "=", NULL);
        gchar* p;
        for (p = name; *p; p++) {
          if (*p == '-')
            *p = '_';
        }
        ruby_setter = rb_intern(name);
        g_param_spec_set_qdata(pspec, q_ruby_setter, (gpointer)ruby_setter);
        g_free(name);
    }

    rb_funcall(GOBJ2RVAL(object), ruby_setter, 1, GVAL2RVAL(value));
}

// FIXME: use rb_protect
static void
class_init_func(gpointer g_class_, G_GNUC_UNUSED gpointer class_data)
{
    GObjectClass* g_class = G_OBJECT_CLASS(g_class_);

    g_class->set_property = set_prop_func;
    g_class->get_property = get_prop_func;

#if 0
    VALUE class_init_proc = (VALUE)class_data;
    rb_funcall(proc_mod_eval, rb_intern("call"), 2,
               GTYPE2CLASS(G_TYPE_FROM_CLASS(g_class)), class_init_proc);
#endif
}

static VALUE
rg_s_type_register(int argc, VALUE* argv, VALUE self)
{
    VALUE type_name, flags;
    volatile VALUE class_init_proc = Qnil;
    GType parent_type;
    GTypeInfo* info;

    rb_scan_args(argc, argv, "03", &type_name, &info, &flags);

    {
        const RGObjClassInfo* cinfo = rbgobj_lookup_class(self);
        if (cinfo->klass == self)
            rb_raise(rb_eTypeError, "already registered");
    }

    {
        VALUE superclass = rb_funcall(self, rb_intern("superclass"), 0);
        const RGObjClassInfo* cinfo = rbgobj_lookup_class(superclass);
        if (cinfo->klass != superclass)
            rb_raise(rb_eTypeError, "super class must be registered to GLib");
        parent_type = cinfo->gtype;
    }

    if (NIL_P(type_name)) {
        VALUE s = rb_funcall(self, rb_intern("name"), 0);

        if (strlen(StringValuePtr(s)) == 0)
            rb_raise(rb_eTypeError, "can't determine type name");

        type_name = rb_funcall(
            rb_eval_string("lambda{|x| x.gsub(/::/,'') }"),
            rb_intern("call"), 1, s);
    }

    {
        GTypeQuery query;
        g_type_query(parent_type, &query);

        /* TODO: Why new?  g_type_register_static() doesn’t retain a copy, so
         * this should be allocated on the stack. */
        info = g_new0(GTypeInfo, 1);
        info->class_size     = query.class_size;
        info->base_init      = NULL;
        info->base_finalize  = NULL;
        info->class_init     = class_init_func;
        info->class_finalize = NULL;
        info->class_data     = (gpointer)class_init_proc;
        info->instance_size  = query.instance_size;
        info->n_preallocs    = 0;
        info->instance_init  = NULL;
        info->value_table    = NULL;
    }

    {
        GType type = g_type_register_static(parent_type,
                                            StringValuePtr(type_name),
                                            info,
                                            NIL_P(flags) ? 0 : NUM2INT(flags));
        G_RELATIVE(self, class_init_proc);

        rbgobj_register_class(self, type, TRUE, TRUE);

        {
            RGObjClassInfo* cinfo = (RGObjClassInfo*)rbgobj_lookup_class(self);
            cinfo->flags |= RBGOBJ_DEFINED_BY_RUBY;
        }

        {
            GType parent = g_type_parent(type);
            const RGObjClassInfo* cinfo =  GTYPE2CINFO(parent);
            VALUE m = rb_define_module_under(self, RubyGObjectHookModule);

            if (! (cinfo->flags & RBGOBJ_DEFINED_BY_RUBY)) {
                rbg_define_method(m, "initialize", rg_initialize, -1);
            }

            rb_include_module(self, m);
        }

        return Qnil;
    }
}

/**********************************************************************/

void 
Init_gobject_gobject(void)
{
    RG_TARGET_NAMESPACE = G_DEF_CLASS_WITH_GC_FUNC(G_TYPE_OBJECT, "Object", mGLib,
                                                  gobj_mark, NULL);

#ifdef G_TYPE_INITIALLY_UNOWNED
    G_DEF_CLASS(G_TYPE_INITIALLY_UNOWNED, "InitiallyUnowned", mGLib);
#endif

    RUBY_GOBJECT_OBJ_KEY = g_quark_from_static_string("__ruby_gobject_object__");

    rb_define_alloc_func(RG_TARGET_NAMESPACE, (VALUE(*)_((VALUE)))gobj_s_allocate);
    RG_DEF_SMETHOD_BANG(new, -1);

    rbg_define_singleton_method(RG_TARGET_NAMESPACE, "property", &gobj_s_property, 1);
    rbg_define_singleton_method(RG_TARGET_NAMESPACE, "properties", &gobj_s_properties, -1);
    RG_DEF_SMETHOD(install_property, -1);
    q_ruby_getter = g_quark_from_static_string("__ruby_getter");
    q_ruby_setter = g_quark_from_static_string("__ruby_setter");

    RG_DEF_METHOD(set_property, 2);
    RG_DEF_METHOD(get_property, 1);
    RG_DEF_METHOD(freeze_notify, 0);
    rb_undef_method(RG_TARGET_NAMESPACE, "notify");
    RG_DEF_METHOD(notify, 1);
    RG_DEF_METHOD(thaw_notify, 0);
    RG_DEF_METHOD_P(destroyed, 0);

    RG_DEF_METHOD(initialize, -1);
    rbg_define_method(RG_TARGET_NAMESPACE, "ref_count", gobj_ref_count, 0); /* for debugging */
    RG_DEF_METHOD(inspect, 0);
    RG_DEF_METHOD(type_name, 0);

    eNoPropertyError = rb_define_class_under(mGLib, "NoPropertyError",
                                             rb_eNameError);

    rb_global_variable(&type_to_prop_setter_table);
    rb_global_variable(&type_to_prop_getter_table);
    type_to_prop_setter_table = rb_hash_new();
    type_to_prop_getter_table = rb_hash_new();

    /* subclass */
    RG_DEF_SMETHOD(type_register, -1);

    rb_global_variable(&proc_mod_eval);
    proc_mod_eval = rb_eval_string("lambda{|obj,proc| obj.module_eval(&proc)}");
}
