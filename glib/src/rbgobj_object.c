/* -*- c-file-style: "ruby" -*- */
/**********************************************************************

  rbgobj_object.c -

  $Author: sakai $
  $Date: 2002/07/27 06:23:51 $

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

VALUE rbgobj_cGObject;

static const char* const RUBY_GOBJECT_OBJ_KEY = "__ruby_gobject_object__";

static VALUE rbgobj_make_gobject(VALUE klass, GObject* gobj);
static VALUE rbgobj_make_gobject_auto_type(GObject* gobj);

ID id_relatives;
ID id_relative_callbacks;
ID id_delete;
ID id_class_info;

/**********************************************************************/

#define NO_UNREF_IN_WEAK_NOTIFY

typedef struct {
    VALUE self;
    GObject* gobj;
    const RGObjClassInfo* cinfo;
    gboolean destroyed;
} gobj_holder;

static
void gobj_weak_notify(data, where_the_object_was)
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
gobj_mark(holder)
    gobj_holder* holder;
{
    if (holder->gobj && !holder->destroyed
        && holder->cinfo && holder->cinfo->mark)
        holder->cinfo->mark(holder->gobj);
}

static void
gobj_free(gobj_holder* holder)
{
    if (holder->gobj){
        if (!holder->destroyed){
            if (holder->cinfo && holder->cinfo->free)
                holder->cinfo->free(holder->gobj);
            g_object_set_data(holder->gobj, RUBY_GOBJECT_OBJ_KEY, NULL);
            g_object_weak_unref(holder->gobj, gobj_weak_notify, holder);
        }
        g_object_unref(holder->gobj);
    }
    free(holder);
}

static VALUE
gobj_s_allocate(klass)
    VALUE klass;
{
    gobj_holder* holder;
    VALUE result;

    result = Data_Make_Struct(klass, gobj_holder, gobj_mark, gobj_free, holder);
    holder->self  = result;
    holder->gobj  = NULL;
    holder->cinfo = NULL;
    holder->destroyed = FALSE;
    
    return result;
}

#ifdef HAVE_OBJECT_ALLOCATE
#define rbgobj_s_new rb_class_new_instance
#else
static
VALUE gobj_s_new(int argc, VALUE* argv, VALUE klass)
{
    VALUE obj = gobj_s_allocate(klass);
    rb_obj_call_init(obj, argc, argv);
    return obj;
}
#endif

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
    g_object_weak_ref(gobj, gobj_weak_notify, holder);

    rb_ivar_set(obj, id_relatives, Qnil);
}

/**********************************************************************/

GObject*
rbgobj_get_gobject(obj)
    VALUE obj;
{
    gobj_holder* holder;

    if (!RTEST(rb_obj_is_kind_of(obj, rbgobj_cGObject)))
        rb_raise(rb_eTypeError, "not a GLib::GObject");

    Data_Get_Struct(obj, gobj_holder, holder);

    if (holder->destroyed)
        rb_raise(rb_eArgError, "destroyed GLib::GObject");

    if (!holder->gobj)
        rb_raise(rb_eArgError, "uninitialize GLib::GObject");

    return holder->gobj;
}

static VALUE
null()
{
    return (VALUE)NULL;
}

GObject*
rbgobj_force_get_gobject(self)
    VALUE self;
{
    return (GObject*)rb_rescue((VALUE(*)())rbgobj_get_gobject, self, null, 0);
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
    return holder ? holder->self : rbgobj_make_gobject_auto_type(gobj);
}

VALUE
rbgobj_make_gobject(klass, gobj)
    VALUE klass;
    GObject* gobj;
{
    gobj_holder* holder = g_object_get_data(gobj, RUBY_GOBJECT_OBJ_KEY);

    if (holder) {
        return holder->self;
    } else {
        VALUE obj = gobj_s_allocate(klass);
        gobj = g_object_ref(gobj);
        rbgobj_initialize_gobject(obj, gobj);
        return obj;
    }
}

VALUE
rbgobj_make_gobject_auto_type(gobj)
    GObject* gobj;
{
    return rbgobj_make_gobject(rbgobj_lookup_rbclass(gobj), gobj);
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

/**********************************************************************/

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
    VALUE result;

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

    param_setup_arg.gclass = G_OBJECT_CLASS(g_type_class_ref(gtype)); // FIXME: g_type_peek_class?
    param_setup_arg.params = ALLOCA_N(GParameter, param_size);
    memset(param_setup_arg.params, 0, sizeof(GValue) * param_size);

    rb_iterate(&_each_with_index, params_hash, _params_setup,
               (VALUE)&param_setup_arg);

    return g_object_newv(gtype, param_size, param_setup_arg.params);
}

/**********************************************************************/

static VALUE
gobj_s_gobject_new(self, type, params_hash)
    VALUE self, type, params_hash;
{
    GObject* gobj = rbgobj_gobject_new(type, params_hash);
    VALUE result = rbgobj_make_gobject_auto_type(gobj);

    // XXX: Ughhhhh
    if (!strncmp("Gtk", g_type_name(G_OBJECT_TYPE(gobj)), 3))
        gtk_object_sink(gobj);
    else
        g_object_unref(gobj);

    return result;
}

static VALUE
gobj_set_property(self, prop_name, val)
    VALUE self, prop_name, val;
{
    GParamSpec* pspec;

    StringValue(prop_name);

    pspec = g_object_class_find_property(
        G_OBJECT_GET_CLASS(RVAL2GOBJ(self)),
        StringValuePtr(prop_name));

    if (!pspec)
        rb_raise(rb_eArgError, "No such property");
    else {
        GValue tmp = {0,};
        g_value_init(&tmp, G_PARAM_SPEC_VALUE_TYPE(pspec));
        rbgobj_rvalue_to_gvalue(val, &tmp);
        g_object_set_property(RVAL2GOBJ(self), StringValuePtr(prop_name), &tmp);
        g_value_unset(&tmp);
        return self;
    }
}

static VALUE
gobj_get_property(self, prop_name)
    VALUE self, prop_name;
{
    GParamSpec* pspec;

    StringValue(prop_name);

    pspec = g_object_class_find_property(
        G_OBJECT_GET_CLASS(RVAL2GOBJ(self)),
        StringValuePtr(prop_name));

    if (!pspec)
        rb_raise(rb_eArgError, "No such property");
    else {
        GValue tmp = {0,};
        VALUE ret;
        g_value_init(&tmp, G_PARAM_SPEC_VALUE_TYPE(pspec));
        g_object_get_property(RVAL2GOBJ(self), StringValuePtr(prop_name), &tmp);
        ret = rbgobj_gvalue_to_rvalue(&tmp);
        g_value_unset(&tmp);
        return ret;
    }
}

static VALUE
gobj_initialize(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    rb_raise(rb_eRuntimeError, "can't instantiate class %s",
             rb_class2name(CLASS_OF(self)));
}

static VALUE
gobj_inspect(self)
    VALUE self;
{
    gobj_holder* holder;
    char *cname = rb_class2name(CLASS_OF(self));
    char *s;

    Data_Get_Struct(self, gobj_holder, holder);

    if (holder->gobj) {
        s = ALLOCA_N(char, 2+strlen(cname)+1+18+1+4+18+1+1);
        sprintf(s, "#<%s:%p ptr=%p>", cname, (void *)self,
                rbgobj_get_gobject(self));
    } else {
        s = ALLOCA_N(char, 2+strlen(cname)+2+9+1+1);
        sprintf(s, "#<%s: destroyed>", cname);
    }

    return rb_str_new2(s);
}

/**********************************************************************/

static VALUE
gobj_sig_connect(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE sig, after;
    ID id = 0;
    int i;
    GClosure* rclosure;

    rb_scan_args(argc, argv, "11", &sig, &after);
    StringValue(sig);

    rclosure = g_rclosure_new(rb_f_lambda());
    i = g_signal_connect_closure(rbgobj_get_gobject(self),
                                 StringValuePtr(sig), rclosure, RTEST(after));

    return INT2FIX(i);
}

/* XXX */
static VALUE
gobj_sig_emit(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE sig_id;
    VALUE rest;
    GSignalQuery query;
    GValueArray* params;
    GValue return_value = {0,};
    int i;
    VALUE result;

    rb_scan_args(argc, argv, "1*", &sig_id, &rest);
    g_signal_query(NUM2INT(sig_id), &query);

    params = g_value_array_new(query.n_params + 1);
    rbgobj_rvalue_to_gvalue(self, &(params->values[0]));
    for (i = 0; i < query.n_params; i++)
        rbgobj_rvalue_to_gvalue(rb_ary_entry(rest, i), &(params->values[i+1]));

    //g_value_init(&return_value, ); // XXX

    g_signal_emitv(params->values, NUM2INT(sig_id), 0, &return_value);

    g_value_array_free(params);
    result = rbgobj_gvalue_to_rvalue(&return_value);
    g_value_unset(&return_value);
    
    return self;
}

/* TODO */
static VALUE
gobj_sig_emit_by_name(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    rb_notimplement();
    return self;
}

static VALUE
gobj_sig_emit_stop(self, sig_id)
    VALUE self, sig_id;
{
    g_signal_stop_emission(rbgobj_get_gobject(self),
                           NUM2INT(sig_id), 0);
    return self;
}

static VALUE
gobj_sig_emit_stop_by_name(self, sig_name)
    VALUE self, sig_name;
{
    GObject* gobj = rbgobj_get_gobject(self);
    StringValue(sig_name);
    g_signal_stop_emission(gobj,
						   g_signal_lookup(StringValuePtr(sig_name), G_OBJECT_TYPE(gobj)), 0);
    return self;
}

static VALUE
gobj_sig_handler_block(self, id)
	VALUE self, id;
{
    g_signal_handler_block(rbgobj_get_gobject(self), NUM2INT(id));
    return self;
}

static VALUE
gobj_sig_handler_unblock(self, id)
	VALUE self, id;
{
    g_signal_handler_unblock(rbgobj_get_gobject(self), NUM2INT(id));
    return self;
}

static VALUE
gobj_sig_handler_disconnect(self, id)
    VALUE self, id;
{
    g_signal_handler_disconnect(rbgobj_get_gobject(self), NUM2INT(id));
    return self;
}

static VALUE
gobj_get_g_type(self)
    VALUE self;
{
    return INT2NUM(G_OBJECT_TYPE(rbgobj_get_gobject(self)));
}

static VALUE
gobj_clone(self)
    VALUE self;
{
    rb_raise(rb_eTypeError, "can't clone %s", rb_class2name(CLASS_OF(self)));
}

static VALUE
gobj_smethod_added(self, id)
    VALUE self, id;
{
    GObject *obj = rbgobj_get_gobject(self);
    const char* name = rb_id2name(SYM2ID(id));
    
    if (g_signal_lookup(name, G_OBJECT_TYPE(obj))) {
        VALUE method = rb_funcall(self, rb_intern("method"), id);
        VALUE args[2] = {method, Qfalse};
        gobj_sig_connect(self, sizeof(args), args);
    }

    return Qnil;
}

static VALUE
gobj_ref_count(self)
    VALUE self;
{
    GObject* gobj = rbgobj_force_get_gobject(self);
    return INT2NUM(gobj ? gobj->ref_count : 0);
}

static VALUE
_gobject_to_ruby(const GValue* from)
{
    return rbgobj_get_value_from_gobject(g_value_get_object(from));
}

static void
_gobject_from_ruby(VALUE from, GValue* to)
{
    g_value_set_object(to, rbgobj_get_gobject(from));
}

void Init_gobject_gobj()
{
    static RGObjClassInfo cinfo;

    rbgobj_cGObject = rb_define_class_under(mGLib, "GObject", rb_cObject);
    cinfo.klass = rbgobj_cGObject;
    cinfo.gtype = G_TYPE_OBJECT;
    cinfo.mark = 0;
    cinfo.free = 0;
    rbgobj_register_class(&cinfo);
    rbgobj_register_r2g_func(rbgobj_cGObject, _gobject_from_ruby);
    rbgobj_register_g2r_func(G_TYPE_OBJECT, _gobject_to_ruby);

#ifdef HAVE_OBJECT_ALLOCATE
    rb_define_singleton_method(rbgobj_cGObject, "allocate", &gobj_s_allocate, 0);
#else
    rb_define_singleton_method(rbgobj_cGObject, "new", &gobj_s_new, -1);
#endif

    rb_define_singleton_method(rbgobj_cGObject, "gobject_new", gobj_s_gobject_new, 2);
    rb_define_method(rbgobj_cGObject, "set_property", gobj_set_property, 2);
    rb_define_method(rbgobj_cGObject, "get_property", gobj_get_property, 1);

    rb_define_method(rbgobj_cGObject, "initialize", gobj_initialize, -1);
    rb_define_method(rbgobj_cGObject, "g_type", gobj_get_g_type, 0);

    rb_define_method(rbgobj_cGObject, "signal_connect", gobj_sig_connect, -1);
/*
  rb_define_method(rbgobj_cGObject, "signal_connect_after",
  gobj_sig_connect_after, -1);
*/

    rb_define_method(rbgobj_cGObject, "signal_emit",
                     gobj_sig_emit, -1);
    rb_define_method(rbgobj_cGObject, "signal_emit_by_name",
                     gobj_sig_emit_by_name, -1);
    rb_define_method(rbgobj_cGObject, "signal_emit_stop",
                     gobj_sig_emit_stop, 1);
    rb_define_method(rbgobj_cGObject, "signal_emit_stop_by_name",
                     gobj_sig_emit_stop_by_name, 1);
    rb_define_method(rbgobj_cGObject, "signal_handler_block",
                     gobj_sig_handler_block, 1);
    rb_define_method(rbgobj_cGObject, "signal_handler_unblock",
                     gobj_sig_handler_unblock, 1);
    rb_define_method(rbgobj_cGObject, "signal_disconnect",
                     gobj_sig_handler_disconnect, 1);

    rb_define_method(rbgobj_cGObject, "singleton_method_added", gobj_smethod_added, 1);
    rb_define_method(rbgobj_cGObject, "inspect", gobj_inspect, 0);
    rb_define_method(rbgobj_cGObject, "clone", gobj_clone, 0);

    /* for debugging */
    rb_define_method(rbgobj_cGObject, "ref_count", gobj_ref_count, 0);
}

/**********************************************************************/

void Init_gobject()
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

    Init_gobject_gobj();
}
