/* -*- c-file-style: "ruby" -*- */
/**********************************************************************

  rbgobject.c -

  $Author: sakai $
  $Date: 2002/08/01 04:59:07 $

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

static const char* const RUBY_GOBJECT_OBJ_KEY = "__ruby_gobject_object__";

ID id_relatives;
ID id_relative_callbacks;
ID id_delete;
ID id_class_info;

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
            g_object_set_data(holder->gobj, RUBY_GOBJECT_OBJ_KEY, NULL);
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
    gobj_holder* holder;
    VALUE result;

    result = Data_Make_Struct(klass, gobj_holder, rbgobj_mark, rbgobj_free, holder);
    holder->self  = result;
    holder->gobj  = NULL;
    holder->cinfo = NULL;
    holder->destroyed = FALSE;
    
    return result;
}

void
rbgobj_initialize_gobject(obj, gobj)
    VALUE obj;
    GObject* gobj;
{
    gobj_holder* holder = g_object_get_data(gobj, RUBY_GOBJECT_OBJ_KEY);
    if (holder)
        rb_raise(rb_eRuntimeError, "ruby wrapper for this GObject* is already exist.");

    Data_Get_Struct(obj, gobj_holder, holder);
    holder->cinfo = rbgobj_lookup_class(rb_class_of(obj));
    holder->gobj  = gobj;
    holder->destroyed = FALSE;

    g_object_set_data(gobj, RUBY_GOBJECT_OBJ_KEY, (gpointer)holder);
    g_object_weak_ref(gobj, rbgobj_weak_notify, holder);

    rb_ivar_set(obj, id_relatives, Qnil);
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
    gobj_holder* holder = g_object_get_data(gobj, RUBY_GOBJECT_OBJ_KEY);
    return holder ? holder->self : Qnil;
}

VALUE
rbgobj_get_value_from_gobject(gobj)
    GObject* gobj;
{
    gobj_holder* holder = g_object_get_data(gobj, RUBY_GOBJECT_OBJ_KEY);
    if (holder)
        return holder->self;
    else {
        VALUE obj = rbgobj_create_object(rbgobj_lookup_rbclass(gobj));
        gobj = g_object_ref(gobj);
        rbgobj_initialize_gobject(obj, gobj);
        return obj;
    }
}

void
rbgobj_add_relative(obj, relative)
    VALUE obj, relative;
{
    VALUE ary = rb_ivar_get(obj, id_relatives);

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
    VALUE hash = rb_ivar_get(obj, obj_ivar_id);

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
    VALUE hash = rb_ivar_get(obj, obj_ivar_id);

    if (NIL_P(hash) || TYPE(hash) != T_HASH) {
        /* should not happen. */
    } else {
        rb_funcall(hash, id_delete, 1, hash_key);
    }
}

void
rbgobj_define_property_acccessors(klass)
    VALUE klass;
{
    GType gtype;
    guint n_properties;
    GParamSpec** pspecs;
    GObjectClass* oclass;
    int i;

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

        if (pspec->flags & G_PARAM_READABLE){
            char* s = g_strdup_printf("def %s; get_property('%s'); end",
                                      prop_name, pspec->name);
            rb_funcall(klass, rb_intern("module_eval"), 3,
                       rb_str_new2(s),
                       rb_str_new2(__FILE__),
                       INT2NUM(__LINE__ - 5));
            g_free(s);
        }
        if (pspec->flags & G_PARAM_WRITABLE){
            char* s = g_strdup_printf("def %s=(val); set_property('%s', val); val; end",
                                      prop_name, pspec->name);
            rb_funcall(klass, rb_intern("module_eval"), 3,
                       rb_str_new2(s),
                       rb_str_new2(__FILE__),
                       INT2NUM(__LINE__ - 5));
            g_free(s);
        }
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
rbgobj_gobject_new(type, params_hash)
    VALUE type, params_hash;
{
    GType gtype;
    size_t param_size;
    struct param_setup_arg param_setup_arg;
    GObject* result;

    if (RTEST(rb_obj_is_kind_of(type, rb_cInteger))) {
        gtype = NUM2INT(type);
    } else {
        StringValue(type);
        gtype = g_type_from_name(StringValuePtr(type));
    }

    if (!g_type_is_a(gtype, G_TYPE_OBJECT))
        rb_raise(rb_eArgError,
                 "type \"%s\" is not descendants if GObject",
                 g_type_name(type));

    param_size = NUM2INT(rb_funcall(params_hash, rb_intern("length"), 0)); 

    param_setup_arg.gclass = G_OBJECT_CLASS(g_type_class_ref(gtype));
    param_setup_arg.params = ALLOCA_N(GParameter, param_size);
    memset(param_setup_arg.params, 0, sizeof(GValue) * param_size);

    // FIXME: use rb_ensure() to ensure following g_type_class_unref() call.
    rb_iterate(&_each_with_index, params_hash, _params_setup,
               (VALUE)&param_setup_arg);

    result = g_object_newv(gtype, param_size, param_setup_arg.params);

    g_type_class_unref(param_setup_arg.gclass);

    return result;
}


void 
Init_gobject()
{
    /* IDs */
    id_relatives = rb_intern("__relatives__");
    id_relative_callbacks = rb_intern("__relative_callbacks__");
    id_class_info = rb_intern("__gobject_class_info__");
    id_delete = rb_intern("delete");

    Init_gobject_gtype();
    Init_gobject_gvalue();
    Init_gobject_gclosure();
    Init_gobject_gparam();

    Init_gobject_gobject();
    Init_gobject_gsignal();

    Init_gobject_gtypemodule();
    Init_gobject_gboxed();
    Init_gobject_genums();
}
