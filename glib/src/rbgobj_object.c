/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/**********************************************************************

  rbgobj_object.c -

  $Author: mutoh $
  $Date: 2004/03/05 15:52:27 $

  Copyright (C) 2002-2004  Ruby-GNOME2 Project Team
  Copyright (C) 2002-2003  Masahiro Sakai

  This file is derived from rbgtkobject.c in Ruby/Gtk distribution.
  rbgtkobject.c -
  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi

**********************************************************************/

#include "ruby.h"
#include "st.h"
#include "global.h"

static VALUE eNoPropertyError;

void
rbgobj_add_abstract_but_create_instance_class(gtype)
    GType gtype;
{
    RGObjClassInfo* cinfo = (RGObjClassInfo*)rbgobj_lookup_class_by_gtype(gtype);
    cinfo->flags |= RBGOBJ_ABSTRACT_BUT_CREATABLE;
}

static VALUE
gobj_s_allocate(klass)
    VALUE klass;
{
    const RGObjClassInfo* cinfo = rbgobj_lookup_class(klass);
    if (G_TYPE_IS_ABSTRACT(cinfo->gtype) &&
        !(cinfo->flags & RBGOBJ_ABSTRACT_BUT_CREATABLE))
        rb_raise(rb_eTypeError, "abstract class");
    return rbgobj_create_object(klass);
}

static gboolean
is_gtkobject(gobj)
    GObject* gobj;
{
    static GType gtype_gtkobject = G_TYPE_INVALID;
    if (!gtype_gtkobject)
        gtype_gtkobject = g_type_from_name("GtkObject");
    return gtype_gtkobject && g_type_is_a(G_OBJECT_TYPE(gobj), gtype_gtkobject);
}

static VALUE
gobj_s_gobject_new(argc, argv, self)
    int argc;
    VALUE* argv;
    VALUE self;
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

static VALUE
gobj_s_install_property(int argc, VALUE* argv, VALUE self)
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
gobj_s_property(self, property_name)
     VALUE self, property_name;
{
    GObjectClass* oclass;
    const char* name;
    GParamSpec* prop;
    VALUE result;

    if (SYMBOL_P(property_name)) {
        name = rb_id2name(SYM2ID(property_name));
    } else {
        StringValue(property_name);
        name = StringValuePtr(property_name);
    }

    oclass = g_type_class_ref(CLASS2GTYPE(self));

    prop = g_object_class_find_property(oclass, name);
    if (!prop){
        g_type_class_unref(oclass);
        rb_raise(eNoPropertyError, "no such property: %s", name);
    }

    result = GOBJ2RVAL(prop);
    g_type_class_unref(oclass);
    return result;
}

static VALUE
gobj_s_properties(int argc, VALUE* argv, VALUE self)
{
    GObjectClass* oclass = g_type_class_ref(CLASS2GTYPE(self));
    gint n_properties;
    GParamSpec** props;
    VALUE inherited_too;
    VALUE ary;
    int i;

    if (rb_scan_args(argc, argv, "01", &inherited_too) == 0)
        inherited_too = Qtrue;

    props = g_object_class_list_properties(oclass, &n_properties);

    ary = rb_ary_new();
    for (i = 0; i < n_properties; i++){
        if (RTEST(inherited_too)
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
rbgobj_register_property_setter(gtype, name, func)
    GType gtype;
    const char* name;
    RValueToGValueFunc func;
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
    
    rb_hash_aset(table, rb_intern(g_param_spec_get_name(pspec)),
                 Data_Wrap_Struct(rb_cData, NULL, NULL, func));

    g_type_class_unref(oclass);
}

void
rbgobj_register_property_getter(gtype, name, func)
    GType gtype;
    const char* name;
    GValueToRValueFunc func;
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

    rb_hash_aset(table, rb_intern(g_param_spec_get_name(pspec)),
                 Data_Wrap_Struct(rb_cData, NULL, NULL, func));
}

static VALUE
gobj_set_property(self, prop_name, val)
    VALUE self, prop_name, val;
{
    GParamSpec* pspec;
    const char* name;

    if (SYMBOL_P(prop_name)) {
        name = rb_id2name(SYM2ID(prop_name));
    } else {
        StringValue(prop_name);
        name = StringValuePtr(prop_name);
    }

    pspec = g_object_class_find_property(G_OBJECT_GET_CLASS(RVAL2GOBJ(self)),
                                         name);

    if (!pspec)
        rb_raise(eNoPropertyError, "No such property: %s", name);
    else {
        // FIXME: use rb_ensure to call g_value_unset()
        RValueToGValueFunc setter = NULL;
        GValue gval = {0,};
        g_value_init(&gval, G_PARAM_SPEC_VALUE_TYPE(pspec));

        {
            VALUE table = rb_hash_aref(type_to_prop_setter_table,
                                       INT2FIX(pspec->owner_type));
            if (!NIL_P(table)){
                VALUE obj = rb_hash_aref(table, rb_intern(g_param_spec_get_name(pspec)));
                if (!NIL_P(obj))
                    Data_Get_Struct(obj, void, setter);
            }
        }

        if (setter)
            setter(val, &gval);
        else {
            rbgobj_rvalue_to_gvalue(val, &gval);
        }

        g_object_set_property(RVAL2GOBJ(self), name, &gval);
        g_value_unset(&gval);
        return self;
    }
}

static VALUE
gobj_get_property(self, prop_name)
    VALUE self, prop_name;
{
    GParamSpec* pspec;
    const char* name;

    if (SYMBOL_P(prop_name)) {
        name = rb_id2name(SYM2ID(prop_name));
    } else {
        StringValue(prop_name);
        name = StringValuePtr(prop_name);
    }

    pspec = g_object_class_find_property(G_OBJECT_GET_CLASS(RVAL2GOBJ(self)),
                                         name);

    if (!pspec)
        rb_raise(eNoPropertyError, "No such property: %s", name);
    else {
        // FIXME: use rb_ensure to call g_value_unset()
        GValueToRValueFunc getter = NULL;
        GValue gval = {0,};
        VALUE ret;

        {
            VALUE table = rb_hash_aref(type_to_prop_getter_table,
                                       INT2FIX(pspec->owner_type));
            if (!NIL_P(table)){
                VALUE obj = rb_hash_aref(table, rb_intern(g_param_spec_get_name(pspec)));
                if (!NIL_P(obj))
                    Data_Get_Struct(obj, void, getter);
            }
        }

        g_value_init(&gval, G_PARAM_SPEC_VALUE_TYPE(pspec));
        g_object_get_property(RVAL2GOBJ(self), name, &gval);
        ret = getter ? getter(&gval) : GVAL2RVAL(&gval);
        g_value_unset(&gval);
        return ret;
    }
}

static VALUE gobj_thaw_notify(VALUE self);

static VALUE
gobj_freeze_notify(self)
    VALUE self;
{
    g_object_freeze_notify(RVAL2GOBJ(self));
    if (rb_block_given_p()) {
        return rb_ensure(rb_yield, self, gobj_thaw_notify, self);
    }
    return self;
}

static VALUE
gobj_notify(self, property_name)
    VALUE self, property_name;
{
    StringValue(property_name);
    g_object_notify(RVAL2GOBJ(self), StringValuePtr(property_name));
    return self;
}

static VALUE
gobj_thaw_notify(self)
    VALUE self;
{
    g_object_thaw_notify(RVAL2GOBJ(self));
    return self;
}

static VALUE
gobj_is_destroyed(self)
    VALUE self;
{
    gobj_holder* holder;

    if (!RTEST(rb_obj_is_kind_of(self, GTYPE2CLASS(G_TYPE_OBJECT))))
        rb_raise(rb_eTypeError, "not a GLib::Object");

    Data_Get_Struct(self, gobj_holder, holder);

    return (holder->destroyed) ? Qtrue : Qfalse;
}

static VALUE
gobj_inspect(self)
    VALUE self;
{
    gobj_holder* holder;
    char *cname = rb_class2name(CLASS_OF(self));
    char *s;
    VALUE result;

    Data_Get_Struct(self, gobj_holder, holder);

    if (!holder->destroyed)
        s = g_strdup_printf("#<%s:%p ptr=%p>", cname, (void *)self, holder->gobj);
    else
        s = g_strdup_printf("#<%s:%p destroyed>", cname, (void *)self);

    result = rb_str_new2(s);
    g_free(s);

    return result;
}

static VALUE
gobj_initialize(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
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
null()
{
    return (VALUE)NULL;
}

static VALUE
gobj_ref_count(self)
    VALUE self;
{
    GObject* gobj = (GObject*)rb_rescue((VALUE(*)())rbgobj_get_gobject, self, null, 0);
    return INT2NUM(gobj ? gobj->ref_count : 0);
}

static VALUE
gobj_smethod_added(self, id)
    VALUE self, id;
{
    GObject *obj = RVAL2GOBJ(self);
    const char* name = rb_id2name(SYM2ID(id));
    guint signal_id = g_signal_lookup(name, G_OBJECT_TYPE(obj));
    
    if (signal_id) {
        VALUE method = rb_funcall(self, rb_intern("method"), 1, id);
        GClosure* closure = g_rclosure_new(method, Qnil,
                                           rbgobj_get_signal_func(signal_id));
        g_signal_connect_closure(obj, name, closure, FALSE);
    }

    return Qnil;
}

/**********************************************************************/

static VALUE proc_mod_eval;
static GQuark q_ruby_setter;
static GQuark q_ruby_getter;

// FIXME: use rb_protect
static void
get_prop_func(GObject* object,
              guint property_id,
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
              guint property_id,
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
class_init_func(gpointer g_class_, gpointer class_data)
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
type_register(int argc, VALUE* argv, VALUE self)
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

    if (NIL_P(type_name)){
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
            const RGObjClassInfo* cinfo = rbgobj_lookup_class_by_gtype(parent);
            VALUE m = rb_define_module_under(self, RubyGObjectHookModule);

            if (! (cinfo->flags & RBGOBJ_DEFINED_BY_RUBY)) {
                rb_define_method(m, "initialize", gobj_initialize, -1);
            }

            rb_include_module(self, m);
        }

        return Qnil;
    }
}

static void
Init_gobject_subclass()
{
    VALUE cGObject = GTYPE2CLASS(G_TYPE_OBJECT);
    rb_define_singleton_method(cGObject, "type_register", type_register, -1);

    rb_global_variable(&proc_mod_eval);
    proc_mod_eval = rb_eval_string("lambda{|obj,proc| obj.module_eval(&proc)}");
}

/**********************************************************************/

void 
Init_gobject_gobject()
{
    VALUE cGObject = G_DEF_CLASS(G_TYPE_OBJECT, "Object", mGLib);

#ifndef HAVE_RB_DEFINE_ALLOC_FUNC
    rb_define_singleton_method(cGObject, "allocate", &gobj_s_allocate, 0);
#else
    rb_define_alloc_func(cGObject, gobj_s_allocate);
#endif
    rb_define_singleton_method(cGObject, "new!", gobj_s_gobject_new, -1);

    rb_define_singleton_method(cGObject, "property", &gobj_s_property, 1);
    rb_define_singleton_method(cGObject, "properties", &gobj_s_properties, -1);
    rb_define_singleton_method(cGObject, "install_property", gobj_s_install_property, -1);
    q_ruby_getter = g_quark_from_static_string("__ruby_getter");
    q_ruby_setter = g_quark_from_static_string("__ruby_setter");

    rb_define_method(cGObject, "set_property", gobj_set_property, 2);
    rb_define_method(cGObject, "get_property", gobj_get_property, 1);
    rb_define_method(cGObject, "freeze_notify", gobj_freeze_notify, 0);
    rb_define_method(cGObject, "notify", gobj_notify, 1);
    rb_define_method(cGObject, "thaw_notify", gobj_thaw_notify, 0);
    rb_define_method(cGObject, "destroyed?", gobj_is_destroyed, 0);

    rb_define_method(cGObject, "initialize", gobj_initialize, -1);
    rb_define_method(cGObject, "ref_count", gobj_ref_count, 0); /* for debugging */
    rb_define_method(cGObject, "inspect", gobj_inspect, 0);

    rb_define_method(cGObject, "singleton_method_added", gobj_smethod_added, 1);

    eNoPropertyError = rb_define_class_under(mGLib, "NoPropertyError",
                                             rb_eNameError);

    rb_global_variable(&type_to_prop_setter_table);
    rb_global_variable(&type_to_prop_getter_table);
    type_to_prop_setter_table = rb_hash_new();
    type_to_prop_getter_table = rb_hash_new();

    Init_gobject_subclass();
}

