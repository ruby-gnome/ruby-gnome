/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/**********************************************************************

  rbgobj_object.c -

  $Author: sakai $
  $Date: 2003/04/04 13:48:42 $

  Copyright (C) 2002,2003  Masahiro Sakai

  This file is derived from rbgtkobject.c in Ruby/Gtk distribution.
  rbgtkobject.c -
  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi

**********************************************************************/

#include "ruby.h"
#include "st.h"
#include "global.h"

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

#ifdef RBGLIB_ENABLE_EXPERIMENTAL

static VALUE
gobj_s_gobject_new(argc, argv, self)
    int argc;
    VALUE* argv;
    VALUE self;
{
    VALUE params_hash;
    GObject* gobj;
    VALUE result;

    rb_scan_args(argc, argv, "01", &params_hash);

    gobj = rbgobj_gobject_new(CLASS2GTYPE(self), params_hash);
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

#endif /* RBGLIB_ENABLE_EXPERIMENTAL */

#if 0
void        g_object_class_install_property   (GObjectClass   *oclass,
					       guint           property_id,
					       GParamSpec     *pspec);
#endif

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
        rb_raise(rb_eNameError, "no such property: %s", name);
    }

    result = GOBJ2RVAL(prop);
    g_type_class_unref(oclass);
    return result;
}

static VALUE
gobj_s_properties(self)
     VALUE self;
{
    GObjectClass* oclass = g_type_class_ref(CLASS2GTYPE(self));
    gint n_properties;
    GParamSpec** props;
    VALUE ary;
    int i;

    props = g_object_class_list_properties(oclass, &n_properties);

    ary = rb_ary_new2(n_properties);
    for (i = 0; i < n_properties; i++)
        rb_ary_store(ary, i, rb_str_new2(props[i]->name));

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
        rb_raise(rb_eArgError, "No such property: %s", name);
    else {
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
            if (!NIL_P(val))
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
        rb_raise(rb_eArgError, "No such property: %s", name);
    else {
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
    const RGObjClassInfo* cinfo = rbgobj_lookup_class(CLASS_OF(self));
    VALUE params_hash;
    GObject* gobj;
    
    rb_scan_args(argc, argv, "01", &params_hash);

    gobj = rbgobj_gobject_new(cinfo->gtype, params_hash);

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
    
    if (g_signal_lookup(name, G_OBJECT_TYPE(obj))) {
        VALUE method = rb_funcall(self, rb_intern("method"), 1, id);
        g_signal_connect_closure(obj, name,
                                 g_rclosure_new(method, Qnil, NULL), FALSE);
    }

    return Qnil;
}

/**********************************************************************/

#ifdef RBGLIB_ENABLE_EXPERIMENTAL

#if 0
typedef void   (*GBaseInitFunc)              (gpointer         g_class);
typedef void   (*GBaseFinalizeFunc)          (gpointer         g_class);
typedef void   (*GClassFinalizeFunc)         (gpointer         g_class,
					      gpointer         class_data);
typedef void   (*GInterfaceInitFunc)         (gpointer         g_iface,
					      gpointer         iface_data);
typedef void   (*GInterfaceFinalizeFunc)     (gpointer         g_iface,
					      gpointer         iface_data);
typedef gboolean (*GTypeClassCacheFunc)	     (gpointer	       cache_data,
					      GTypeClass      *g_class);
#endif

static void
class_init(gpointer g_class, gpointer class_data)
{
    VALUE class_init_proc = (VALUE)class_data;
    // XXX
    VALUE f = rb_eval_string("lambda{|obj,proc| obj.module_eval(&proc)}");
    rb_funcall(f, rb_intern("call"), 2,
               GTYPE2CLASS(G_TYPE_FROM_CLASS(g_class)), class_init_proc);
}

static void
instance_init(GTypeInstance* instance, gpointer g_class)
{
    VALUE obj = GOBJ2RVAL(instance);
    ID id_instance_init = rb_intern("instance_init");
    if (rb_respond_to(obj, id_instance_init))
        rb_funcall(obj, id_instance_init, 0);
}

static VALUE
noop(int argc, VALUE* argv, VALUE _)
{
    return Qnil;
}

static VALUE
gobject_class_new(int argc, VALUE* argv, VALUE _)
{
    VALUE type_name, flags;
    volatile VALUE class_init_proc;
    GType parent_type;
    GTypeInfo* info;

    {
        VALUE parent;
        rb_scan_args(argc, argv, "21&", &parent, &type_name, &flags, &class_init_proc);
        # FIXME: check arguments
        parent_type = CLASS2GTYPE(parent);
    }

    {
        GTypeQuery query;
        g_type_query(parent_type, &query);

        info = g_new0(GTypeInfo, 1);
        info->class_size     = query.class_size;
        info->base_init      = NULL;
        info->base_finalize  = NULL;
        info->class_init     = class_init;
        info->class_finalize = NULL;
        info->class_data     = (gpointer)class_init_proc;
        info->instance_size  = query.instance_size;
        info->n_preallocs    = 0;
        info->instance_init  = instance_init;
        info->value_table    = NULL;
    }

    {
        GType type = g_type_register_static(parent_type,
                                            StringValuePtr(type_name),
                                            info,
                                            NIL_P(flags) ? 0 : NUM2INT(flags));
        VALUE klass = GTYPE2CLASS(type);
        G_RELATIVE(klass, class_init_proc);

        rb_define_alias(CLASS_OF(klass), "new", "new!");
        rb_define_method(klass, "initialize", noop, -1);

        return klass;
    }
}

static void
Init_gobject_class()
{
    VALUE cGObject = GTYPE2CLASS(G_TYPE_OBJECT);
    rb_define_module_function(cGObject, "class_new", gobject_class_new, -1);
}

#endif

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
#ifdef RBGLIB_ENABLE_EXPERIMENTAL
    rb_define_singleton_method(cGObject, "new!", gobj_s_gobject_new, -1);
#endif

    rb_define_singleton_method(cGObject, "property", &gobj_s_property, 1);
    rb_define_singleton_method(cGObject, "properties", &gobj_s_properties, 0);

    rb_define_method(cGObject, "set_property", gobj_set_property, 2);
    rb_define_method(cGObject, "get_property", gobj_get_property, 1);
    rb_define_alias(cGObject, "property", "get_property");
    rb_define_method(cGObject, "freeze_notify", gobj_freeze_notify, 0);
    rb_define_method(cGObject, "notify", gobj_notify, 1);
    rb_define_method(cGObject, "thaw_notify", gobj_thaw_notify, 0);
    rb_define_method(cGObject, "destroyed?", gobj_is_destroyed, 0);

    rb_define_method(cGObject, "initialize", gobj_initialize, -1);
    rb_define_method(cGObject, "ref_count", gobj_ref_count, 0); /* for debugging */
    rb_define_method(cGObject, "inspect", gobj_inspect, 0);

    rb_define_method(cGObject, "singleton_method_added", gobj_smethod_added, 1);

    rb_global_variable(&type_to_prop_setter_table);
    rb_global_variable(&type_to_prop_getter_table);
    type_to_prop_setter_table = rb_hash_new();
    type_to_prop_getter_table = rb_hash_new();

#ifdef RBGLIB_ENABLE_EXPERIMENTAL
    Init_gobject_class();
#endif
}

