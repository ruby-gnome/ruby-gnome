/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/**********************************************************************

  rbgobject.c -

  $Author: sakai $
  $Date: 2002/09/21 10:54:49 $

  Copyright (C) 2002  Masahiro Sakai

  This file is derived from rbgtkobject.c in Ruby/Gtk distribution.
  rbgtkobject.c -
  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi

**********************************************************************/

#include "ruby.h"
#include "st.h"
#include "global.h"
#include <ctype.h>

static GQuark RUBY_GOBJECT_OBJ_KEY;

ID id_relatives;
static ID id_delete;
static ID id_module_eval;

/**********************************************************************/

#define NO_UNREF_IN_WEAK_NOTIFY

static void 
rbgobj_weak_notify(data, where_the_object_was)
    gpointer data;
    GObject* where_the_object_was;
{
    gobj_holder* holder = data;
    if (holder->cinfo && holder->cinfo->free)
        holder->cinfo->free(holder->gobj); 
    rb_ivar_set(holder->self, id_relatives, Qnil);
    /*
      XXX. We can't unref() here.
      Because ref_counted have been set to zero in g_object_real_dispose().
    */
#ifndef NO_UNREF_IN_WEAK_NOTIFY
    g_object_unref(holder->gobj);
    holder->gobj = NULL;
#endif
    holder->destroyed = TRUE;
}

static void
rbgobj_mark(holder)
    gobj_holder* holder;
{
    if (holder->gobj && !holder->destroyed
        && holder->cinfo && holder->cinfo->mark)
        holder->cinfo->mark(holder->gobj);
}

static void
rbgobj_free(gobj_holder* holder)
{
    if (holder->gobj){
        if (!holder->destroyed){
            if (holder->cinfo && holder->cinfo->free)
                holder->cinfo->free(holder->gobj);
            g_object_set_qdata(holder->gobj, RUBY_GOBJECT_OBJ_KEY, NULL);
            g_object_weak_unref(holder->gobj, rbgobj_weak_notify, holder);
        }
        g_object_unref(holder->gobj);
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
rbgobj_initialize_object(obj, cobj)
    VALUE obj;
    gpointer cobj;
{
	if (G_TYPE_FUNDAMENTAL(RVAL2GTYPE(obj)) == G_TYPE_BOXED){
		rbgobj_boxed_initialize(obj, cobj);
	} else {
		gobj_holder* holder = g_object_get_qdata((GObject*)cobj, RUBY_GOBJECT_OBJ_KEY);
		if (holder)
			rb_raise(rb_eRuntimeError, "ruby wrapper for this GObject* is already exist.");

		Data_Get_Struct(obj, gobj_holder, holder);
		holder->cinfo = RVAL2CINFO(obj);
		holder->gobj  = (GObject*)cobj;
		holder->destroyed = FALSE;
		
		g_object_set_qdata((GObject*)cobj, RUBY_GOBJECT_OBJ_KEY, (gpointer)holder);
		g_object_weak_ref((GObject*)cobj, rbgobj_weak_notify, holder);

		rb_ivar_set(obj, id_relatives, Qnil);
	}
}

GObject*
rbgobj_get_gobject(obj)
    VALUE obj;
{
    gobj_holder* holder;

    if (!RTEST(rb_obj_is_kind_of(obj, GTYPE2CLASS(G_TYPE_OBJECT))))
        rb_raise(rb_eTypeError, "not a GLib::GObject");

    Data_Get_Struct(obj, gobj_holder, holder);

    if (holder->destroyed)
        rb_raise(rb_eArgError, "destroyed GLib::GObject");

    if (!holder->gobj)
        rb_raise(rb_eArgError, "uninitialize GLib::GObject");

    return holder->gobj;
}

VALUE
rbgobj_get_value_from_gobject_if_exist(gobj)
    GObject* gobj;
{
    gobj_holder* holder = g_object_get_qdata(gobj, RUBY_GOBJECT_OBJ_KEY);
    return holder ? holder->self : Qnil;
}

VALUE
rbgobj_get_value_from_gobject(gobj)
    GObject* gobj;
{
    gobj_holder* holder = g_object_get_qdata(gobj, RUBY_GOBJECT_OBJ_KEY);
    if (holder)
        return holder->self;
    else {
        VALUE obj = rbgobj_create_object(GTYPE2CLASS(G_OBJECT_TYPE(gobj)));
        gobj = g_object_ref(gobj);
        rbgobj_initialize_object(obj, (gpointer)gobj);
        return obj;
    }
}

void
rbgobj_add_relative(obj, relative)
    VALUE obj, relative;
{
    VALUE ary = Qnil;

    if (RTEST(rb_ivar_defined(obj, id_relatives)))
        ary = rb_ivar_get(obj, id_relatives);

    if (NIL_P(ary) || TYPE(ary) != T_ARRAY) {
        ary = rb_ary_new();
        rb_ivar_set(obj, id_relatives, ary);
    }
    rb_ary_push(ary, relative);
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

static VALUE prop_exclude_list;

void
rbgobj_define_property_accessors(klass)
    VALUE klass;
{
    GType gtype;
    guint n_properties;
    GParamSpec** pspecs;
    GObjectClass* oclass;
    int i;
    GString* source = g_string_new(NULL);

    gtype  = rbgobj_lookup_class(klass)->gtype;
    oclass = G_OBJECT_CLASS(g_type_class_ref(gtype));
    pspecs = g_object_class_list_properties(oclass, &n_properties);

    for (i = 0; i < n_properties; i++){
        GParamSpec* pspec = pspecs[i];
        char* prop_name;
        char* p;

        if (pspec->owner_type != gtype)
            continue;

        prop_name = g_strdup(pspec->name);
        for (p = prop_name; *p; p++)
            if (*p == '-')
                *p = '_';

        if (RTEST(rb_ary_includes(prop_exclude_list, rb_str_new2(prop_name)))){
            g_free(prop_name);
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

        if (pspec->flags & G_PARAM_WRITABLE){
            g_string_append_printf(source,
                "def %s=(val); set_property('%s', val); val; end\n",
                prop_name, pspec->name);
            g_string_append_printf(source,
                "def set_%s(val); set_property('%s', val); end\n",
                prop_name, pspec->name);
        }

        g_free(prop_name);
    }

    rb_funcall(klass, id_module_eval, 1, rb_str_new2(source->str));
    g_string_free(source, TRUE);

    g_type_class_unref(oclass);
}

void
rbgobj_define_signal_constants(klass)
    VALUE klass;
{
    GType gtype;
    gpointer oclass;
    guint n_ids;
    guint* ids;
    int i;

    gtype = rbgobj_lookup_class(klass)->gtype;
    oclass = g_type_class_ref(gtype);

    ids = g_signal_list_ids(gtype, &n_ids);

    for (i = 0; i < n_ids; i++){
        const gchar* sname = g_signal_name(ids[i]);
        gchar* cname;
        gchar* p;

        cname = g_strconcat("SIGNAL_", sname, NULL);
        for (p = cname + strlen("SIGNAL_"); *p; p++){
            if (*p == '-')
                *p = '_';
            else
                *p = toupper(*p);
        }
        
        rb_define_const(klass, cname, rb_str_new2(sname));
        g_free(cname);
    }

    g_type_class_unref(oclass);
}

struct param_setup_arg {
    GObjectClass* gclass;
    GParameter* params;
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

    arg = rb_ary_entry(arg, 0);

    name = rb_ary_entry(arg, 0);
    val  = rb_ary_entry(arg, 1);

    StringValue(name);
    param_setup_arg->params[n].name = StringValuePtr(name);

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
        struct param_setup_arg param_setup_arg;

        param_size = NUM2INT(rb_funcall(params_hash, rb_intern("length"), 0)); 

        param_setup_arg.gclass = G_OBJECT_CLASS(g_type_class_ref(gtype));
        param_setup_arg.params = ALLOCA_N(GParameter, param_size);
        memset(param_setup_arg.params, 0, sizeof(GParameter) * param_size);

        // FIXME: use rb_ensure() to ensure following g_type_class_unref() call.
        rb_iterate(&_each_with_index, params_hash, _params_setup,
                   (VALUE)&param_setup_arg);

        result = g_object_newv(gtype, param_size, param_setup_arg.params);

        g_type_class_unref(param_setup_arg.gclass);
    }

    return result;
}

void 
Init_gobject()
{
    extern void Init_gobject_gtype();
    extern void Init_gobject_gvalue();
    extern void Init_gobject_gvaluetypes();
    extern void Init_gobject_gboxed();
    extern void Init_gobject_genums();
    extern void Init_gobject_gparam();
    extern void Init_gobject_gparamspecs();
    extern void Init_gobject_gclosure();
    extern void Init_gobject_gobject();
    extern void Init_gobject_gsignal();
    extern void Init_gobject_gtypeplugin();
    extern void Init_gobject_gtypemodule();

    
    prop_exclude_list = rb_funcall(rb_cObject, rb_intern("instance_methods"),
                                   1, Qtrue);
    rb_global_variable(&prop_exclude_list);

    RUBY_GOBJECT_OBJ_KEY = g_quark_from_static_string("__ruby_gobject_object__");

    /* IDs */
    id_relatives = rb_intern("__relatives__");
    id_delete = rb_intern("delete");
    id_module_eval = rb_intern("module_eval");

    Init_gobject_gtype();
    Init_gobject_gvalue();
    Init_gobject_gvaluetypes();
    Init_gobject_gboxed();
    Init_gobject_genums();
    Init_gobject_gparam();
    Init_gobject_gparamspecs();
    Init_gobject_gclosure();
    Init_gobject_gobject();
    Init_gobject_gsignal();

    Init_gobject_gtypeplugin();
    Init_gobject_gtypemodule();
}
