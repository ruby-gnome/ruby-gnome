/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/**********************************************************************

  rbgobject.c -

  $Author: sakai $
  $Date: 2006/05/27 12:24:15 $

  Copyright (C) 2003-2006  Ruby-GNOME2 Project Team
  Copyright (C) 2002,2003  Masahiro Sakai

  This file is derived from rbgtkobject.c in Ruby/Gtk distribution.
  rbgtkobject.c -
  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi

**********************************************************************/

#include "ruby.h"
#include "global.h"
#include <ctype.h>

static GQuark RUBY_GOBJECT_OBJ_KEY;

static ID id_relatives;
static ID id_delete;
static ID id_module_eval;

ID rbgobj_id_children;

/**********************************************************************/

void
rbgobj_initialize_object(obj, cobj)
    VALUE obj;
    gpointer cobj;
{
    if (!cobj)
        rb_raise(rb_eRuntimeError, "failed to initialize");

    switch (G_TYPE_FUNDAMENTAL(RVAL2GTYPE(obj))){
    case G_TYPE_OBJECT:
        rbgobj_gobject_initialize(obj, cobj);
        break;
    case G_TYPE_PARAM:
        rbgobj_param_spec_initialize(obj, cobj);
        break;
    case G_TYPE_BOXED:
        rbgobj_boxed_initialize(obj, cobj);
        break;
    default:
        rbgobj_fund_initialize(G_TYPE_FUNDAMENTAL(RVAL2GTYPE(obj)), obj, cobj);
    }
}

gpointer
rbgobj_instance_from_ruby_object(VALUE obj)
{
    GType t;

    if (obj == Qnil)
    	return NULL;

    t = G_TYPE_FUNDAMENTAL(RVAL2GTYPE(obj));
    switch (t){
    case G_TYPE_OBJECT:
        return rbgobj_get_gobject(obj);
    case G_TYPE_PARAM:
        return rbgobj_get_param_spec(obj);
    default:
      {
        gpointer ret = rbgobj_fund_robj2instance(t, obj);
        if (ret == NULL) {
          rb_raise(rb_eTypeError, "%s isn't supported",
               rb_class2name(CLASS_OF(obj)));
        }
        return ret;
      }
    }
}

VALUE
rbgobj_ruby_object_from_instance(gpointer instance)
{
    return rbgobj_ruby_object_from_instance2(instance, TRUE);
}

VALUE
rbgobj_ruby_object_from_instance2(gpointer instance, gboolean alloc)
{
    GType t; 
    
    if (instance == NULL)
    	return Qnil;
    
    t = G_TYPE_FUNDAMENTAL(G_TYPE_FROM_INSTANCE(instance));
    switch (t){
    case G_TYPE_OBJECT:
        return rbgobj_get_value_from_gobject(instance, alloc);
    case G_TYPE_PARAM:
        return rbgobj_get_value_from_param_spec(instance, alloc);
    default:
        /* FIXME */
        if (alloc) {
            VALUE ret = rbgobj_fund_instance2robj(t, instance);
            if (ret == Qnil) {
                rb_raise(rb_eTypeError, "%s isn't supported",
                         rb_class2name(CLASS_OF(instance)));
            }
            return ret;
        } else {
            return Qnil;
        }
    }
}

/**********************************************************************/

static void 
rbgobj_weak_notify(data, where_the_object_was)
    gpointer data;
    GObject* where_the_object_was;
{
    gobj_holder* holder = data;

    rbgobj_instance_call_cinfo_free(holder->gobj);
    if (RTEST(rb_ivar_defined(holder->self, id_relatives)))
        rb_ivar_set(holder->self, id_relatives, Qnil);
    if (RTEST(rb_ivar_defined(holder->self, rbgobj_id_children)))
        rb_ivar_set(holder->self, rbgobj_id_children, Qnil);
    holder->destroyed = TRUE;
}

static void
rbgobj_mark(holder)
    gobj_holder* holder;
{
    if (holder->gobj && !holder->destroyed)
        rbgobj_instance_call_cinfo_mark(holder->gobj);
}

static void
rbgobj_free(gobj_holder* holder)
{
    if (holder->gobj){
        if (!holder->destroyed){
            rbgobj_instance_call_cinfo_free(holder->gobj);
            g_object_set_qdata(holder->gobj, RUBY_GOBJECT_OBJ_KEY, NULL);
            g_object_weak_unref(holder->gobj, (GWeakNotify)rbgobj_weak_notify, holder);
        }
        g_object_unref(holder->gobj);
        /* holder->gobj = NULL; */
    }
    free(holder);
}

VALUE
rbgobj_create_object(klass)
    VALUE klass;
{
    VALUE result;

    if (G_TYPE_FUNDAMENTAL(CLASS2GTYPE(klass)) == G_TYPE_BOXED){
        result = rbgobj_boxed_create(klass);
    } else {
        gobj_holder* holder;
        result = Data_Make_Struct(klass, gobj_holder, rbgobj_mark, rbgobj_free, holder);
        holder->self  = result;
        holder->gobj  = NULL;
        holder->cinfo = NULL;
        holder->destroyed = FALSE;
    }
    return result;
}

void
rbgobj_gobject_initialize(obj, cobj)
    VALUE obj;
    gpointer cobj;
{
    gobj_holder* holder = g_object_get_qdata((GObject*)cobj, RUBY_GOBJECT_OBJ_KEY);
    if (holder)
        rb_raise(rb_eRuntimeError, "ruby wrapper for this GObject* is already exist.");

    Data_Get_Struct(obj, gobj_holder, holder);
    holder->cinfo = RVAL2CINFO(obj);
    holder->gobj  = (GObject*)cobj;
    holder->destroyed = FALSE;

    g_object_set_qdata((GObject*)cobj, RUBY_GOBJECT_OBJ_KEY, (gpointer)holder);
    g_object_weak_ref((GObject*)cobj, (GWeakNotify)rbgobj_weak_notify, holder);
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

GObject*
rbgobj_get_gobject(obj)
    VALUE obj;
{
    gobj_holder* holder;

    if (!RTEST(rb_obj_is_kind_of(obj, GTYPE2CLASS(G_TYPE_OBJECT))))
        rb_raise(rb_eTypeError, "not a GLib::Object");

    Data_Get_Struct(obj, gobj_holder, holder);

    if (holder->destroyed)
        rb_raise(rb_eTypeError, "destroyed GLib::Object");

    if (!holder->gobj)
        rb_raise(rb_eTypeError, "uninitialize GLib::Object");

    return holder->gobj;
}

VALUE
rbgobj_get_value_from_gobject(gobj, alloc)
    GObject* gobj;
    gboolean alloc;
{
    gobj_holder* holder = g_object_get_qdata(gobj, RUBY_GOBJECT_OBJ_KEY);
    if (holder)
        return holder->self;
    else if (alloc) {
        VALUE obj = rbgobj_create_object(GTYPE2CLASS(G_OBJECT_TYPE(gobj)));
        gobj = g_object_ref(gobj);
        rbgobj_gobject_initialize(obj, (gpointer)gobj);
        return obj;
    } else
        return Qnil;
}

void
rbgobj_add_relative(obj, relative)
    VALUE obj, relative;
{
    VALUE hash = Qnil;

    if (RTEST(rb_ivar_defined(obj, id_relatives)))
        hash = rb_ivar_get(obj, id_relatives);

    if (NIL_P(hash) || TYPE(hash) != T_HASH) {
        hash = rb_hash_new();
        rb_ivar_set(obj, id_relatives, hash);
    }
    rb_hash_aset(hash, relative, Qnil);
}

void
rbgobj_add_relative_removable(obj, relative, obj_ivar_id, hash_key)
    VALUE obj, relative, hash_key;
    ID    obj_ivar_id;
{
    VALUE hash = Qnil;

    if (RTEST(rb_ivar_defined(obj, obj_ivar_id)))
        hash = rb_ivar_get(obj, obj_ivar_id);

    if (NIL_P(hash) || TYPE(hash) != T_HASH) {
        hash = rb_hash_new();
        rb_ivar_set(obj, obj_ivar_id, hash);
    }
    rb_hash_aset(hash, hash_key, relative);
}

VALUE
rbgobj_get_relative_removable(obj, obj_ivar_id, hash_key)
    VALUE obj, hash_key;
    ID    obj_ivar_id;
{
    VALUE hash = Qnil;

    if (RTEST(rb_ivar_defined(obj, obj_ivar_id)))
        hash = rb_ivar_get(obj, obj_ivar_id);

    if (NIL_P(hash) || TYPE(hash) != T_HASH) {
        return Qnil;
    }
    return rb_hash_aref(hash, hash_key);
}

void
rbgobj_remove_relative(obj, obj_ivar_id, hash_key)
    VALUE obj, hash_key;
    ID    obj_ivar_id;
{
    VALUE hash = Qnil;

    if (RTEST(rb_ivar_defined(obj, obj_ivar_id)))
        hash = rb_ivar_get(obj, obj_ivar_id);

    if (NIL_P(hash) || TYPE(hash) != T_HASH) {
        /* should not happen. */
    } else {
        rb_funcall(hash, id_delete, 1, hash_key);
    }
}

void
rbgobj_remove_relative_all(obj, obj_ivar_id)
    VALUE obj;
    ID    obj_ivar_id;
{
    rb_ivar_set(obj, obj_ivar_id, Qnil);
}

static GHashTable* prop_exclude_list;

#define IS_FLAG(bitmask, flag) (((bitmask) & (flag)) == (flag))

void
rbgobj_define_property_accessors(klass)
    VALUE klass;
{
    GType gtype;
    GParamSpec** pspecs = NULL;
    int i;
    GString* source = g_string_new(NULL);
    guint n_properties = 0;

    gtype  = CLASS2GTYPE(klass);
    if (G_TYPE_IS_INTERFACE(gtype)){
#if GLIB_CHECK_VERSION(2,4,0)
        gpointer iface = g_type_default_interface_ref(gtype);
        pspecs = g_object_interface_list_properties(iface, &n_properties);
        g_type_default_interface_unref(iface);
#endif
    } else {
        GObjectClass* oclass = G_OBJECT_CLASS(g_type_class_ref(gtype));
        pspecs = g_object_class_list_properties(oclass, &n_properties);
        g_type_class_unref(oclass);
    }

    for (i = 0; i < n_properties; i++){
        GParamSpec* pspec = pspecs[i];
        char* buf;
        char* prop_name;
        char* p;

        if (pspec->owner_type != gtype)
            continue;

        buf = g_strdup(pspec->name);
        for (p = buf; *p; p++)
            if (*p == '-')
                *p = '_';

        if (!strncmp(buf, "is_", 3))
            prop_name = buf + 3;
        else
            prop_name = buf;

        if (g_hash_table_lookup(prop_exclude_list, prop_name)){
            g_free(buf);
            continue;
        }

        if (pspec->flags & G_PARAM_READABLE){
            g_string_append_printf(
                source, 
                "def %s%s; get_property('%s'); end\n",
                prop_name,
                (G_PARAM_SPEC_VALUE_TYPE(pspec) == G_TYPE_BOOLEAN) ? "?" : "",
                pspec->name);
        }

        if (IS_FLAG(pspec->flags, G_PARAM_WRITABLE) && !IS_FLAG(pspec->flags, G_PARAM_CONSTRUCT_ONLY)){
            g_string_append_printf(source,
                "def set_%s(val); set_property('%s', val); end\n",
                prop_name, pspec->name);
#ifdef HAVE_NODE_ATTRASGN
            g_string_append_printf(source, "alias %s= set_%s\n",
                                   prop_name, prop_name);
#else
            g_string_append_printf(source,
                "def %s=(val); set_property('%s', val); val; end\n",
                prop_name, pspec->name);
#endif
        }

        g_free(buf);
    }

    rb_funcall(klass, id_module_eval, 1, rb_str_new2(source->str));
    g_string_free(source, TRUE);
}


struct param_setup_arg {
    GObjectClass* gclass;
    GParameter* params;
    guint param_size;
    VALUE params_hash;
};

static VALUE
_each_with_index(obj)
    VALUE obj;
{
    rb_funcall(obj, rb_intern("each_with_index"), 0);
    return Qnil;
}

static VALUE
_params_setup(arg, param_setup_arg)
    VALUE arg;
    struct param_setup_arg* param_setup_arg;
{
    int n = NUM2INT(rb_ary_entry(arg, 1));
    VALUE name, val;
    GParamSpec* pspec;

    if (n >= param_setup_arg->param_size)
       rb_raise(rb_eArgError, "too many parameters");

    arg = rb_ary_entry(arg, 0);

    name = rb_ary_entry(arg, 0);
    val  = rb_ary_entry(arg, 1);

    if (SYMBOL_P(name)) {
        param_setup_arg->params[n].name = rb_id2name(SYM2ID(name));
    } else {
        StringValue(name);
        param_setup_arg->params[n].name = StringValuePtr(name);
    }

    pspec = g_object_class_find_property(
        param_setup_arg->gclass,
        param_setup_arg->params[n].name);
    if (!pspec)
        rb_raise(rb_eArgError, "No such property: %s",
                 param_setup_arg->params[n].name);

    g_value_init(&(param_setup_arg->params[n].value),
                 G_PARAM_SPEC_VALUE_TYPE(pspec));
    rbgobj_rvalue_to_gvalue(val, &(param_setup_arg->params[n].value));
    
    return Qnil;
}

static VALUE
gobj_new_body(struct param_setup_arg* arg)
{
    rb_iterate((VALUE(*)_((VALUE)))_each_with_index, (VALUE)arg->params_hash, _params_setup, (VALUE)arg);
    return (VALUE)g_object_newv(G_TYPE_FROM_CLASS(arg->gclass),
                                arg->param_size, arg->params);
}

static VALUE
gobj_new_ensure(struct param_setup_arg* arg)
{
    int i;
    g_type_class_unref(arg->gclass);
    for (i = 0; i < arg->param_size; i++) {
        if (G_IS_VALUE(&arg->params[i].value))
            g_value_unset(&arg->params[i].value);
    }
    return Qnil;
}

GObject*
rbgobj_gobject_new(gtype, params_hash)
    GType gtype;
    VALUE params_hash;
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

        result = (GObject*)rb_ensure(&gobj_new_body, (VALUE)&arg,
                                     &gobj_new_ensure, (VALUE)&arg);
    }

    return result;
}

void 
Init_gobject()
{
    extern void Init_gobject_gtype();
    extern void Init_gobject_typeinterface();
    extern void Init_gobject_typeinstance();
    extern void Init_gobject_gvalue();
    extern void Init_gobject_gvaluetypes();
    extern void Init_gobject_gboxed();
#if GLIB_CHECK_VERSION(2,6,0)
    extern void Init_gobject_gstrv();
#endif
    extern void Init_gobject_value_array();
    extern void Init_gobject_genums();
    extern void Init_gobject_gparam();
    extern void Init_gobject_gparamspecs();
    extern void Init_gobject_gclosure();
    extern void Init_gobject_gobject();
    extern void Init_gobject_gsignal();
    extern void Init_gobject_gtypeplugin();
    extern void Init_gobject_gtypemodule();
    extern void Init_gobject_fundamental();

    /* Not defined properties. They are already used as methods of Object */
    prop_exclude_list = g_hash_table_new(g_str_hash, g_str_equal);
    g_hash_table_insert(prop_exclude_list, "class", "class");
    g_hash_table_insert(prop_exclude_list, "clone", "clone");
    g_hash_table_insert(prop_exclude_list, "dup", "dup");
    g_hash_table_insert(prop_exclude_list, "extend", "extend");
    g_hash_table_insert(prop_exclude_list, "freeze", "freeze");
    g_hash_table_insert(prop_exclude_list, "hash", "hash");
    g_hash_table_insert(prop_exclude_list, "method", "method");
    g_hash_table_insert(prop_exclude_list, "methods", "methods");
    g_hash_table_insert(prop_exclude_list, "object_id", "object_id");
    g_hash_table_insert(prop_exclude_list, "taint", "taint");
    g_hash_table_insert(prop_exclude_list, "untaint", "untaint");

    RUBY_GOBJECT_OBJ_KEY = g_quark_from_static_string("__ruby_gobject_object__");

    /* IDs */
    id_relatives = rb_intern("__relatives__");
    id_delete = rb_intern("delete");
    id_module_eval = rb_intern("module_eval");

    rbgobj_id_children = rb_intern("__stored_children__");

    Init_gobject_gtype();
    Init_gobject_typeinterface();
    Init_gobject_typeinstance();
    Init_gobject_fundamental();
    Init_gobject_gvalue();
    Init_gobject_gvaluetypes();
    Init_gobject_gboxed();
#if GLIB_CHECK_VERSION(2,6,0)
    Init_gobject_gstrv();
#endif
    Init_gobject_value_array();
    Init_gobject_genums();
    Init_gobject_gparam();
    Init_gobject_gparamspecs();
    Init_gobject_gclosure();
    Init_gobject_gobject();
    Init_gobject_gsignal();

    Init_gobject_gtypeplugin();
    Init_gobject_gtypemodule();
}
