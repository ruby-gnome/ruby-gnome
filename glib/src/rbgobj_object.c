/* -*- c-file-style: "ruby" -*- */
/**********************************************************************

  rbgobj_object.c -

  $Author: sakai $
  $Date: 2002/06/17 18:14:24 $

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

ID id_gobject_data;
ID id_relatives;
ID id_relative_callbacks;
ID id_delete;
ID id_class_info;

static st_table *gobject_object_list;
static VALUE gobject_object_list_v;

static void
gobj_mark(obj)
    GObject *obj;
{
    /* just for type mark */
}

GObject*
rbgobj_get_gobject(obj)
    VALUE obj;
{
    VALUE data;
    GObject* result;

    if (NIL_P(obj)) { 
	rb_raise(rb_eTypeError, "wrong argument type nil");
    }

    Check_Type(obj, T_OBJECT);
    data = rb_ivar_get(obj, id_gobject_data);

    /* if (NIL_P(data) || data->dmark != gobj_mark) { */
    if (NIL_P(data)) {
	rb_raise(rb_eTypeError, "not a Glib::GObject");
    }

    Data_Get_Struct(data, GObject, result);
    if (!result)
	rb_raise(rb_eArgError, "destroyed GLib::GObject");
    return G_OBJECT(result);
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

static void
delete_gobject(gpointer user_data)
{
    VALUE obj = (VALUE)user_data;
    struct RData *data;

    if (!st_delete(gobject_object_list, (char**)&obj, 0))
        rb_bug("ruby-gobject: already freed object is freed again");
    data = RDATA(rb_ivar_get(obj, id_gobject_data));
    data->dfree = NULL;
    data->data  = NULL;
}

void
rbgobj_set_gobject(obj, gobj)
    VALUE obj;
    GObject* gobj;
{
    VALUE data;

    const rbgobj_class_info* cinfo = rbgobj_lookup_class(rb_class_of(obj));

    /* XXX */
    if (cinfo)
	data = Data_Wrap_Struct(rb_cData, cinfo->mark, g_object_unref, gobj);
    else
	data = Data_Wrap_Struct(rb_cData, gobj_mark, g_object_unref, gobj);

    g_object_set_data_full(gobj, RUBY_GOBJECT_OBJ_KEY,
                           (gpointer)obj, delete_gobject);

    rb_ivar_set(obj, id_relatives, Qnil);
    rb_ivar_set(obj, id_gobject_data, data);

    st_add_direct(gobject_object_list, (char*)obj, (char*)obj);
}

VALUE
rbgobj_get_value_from_gobject(gobj)
    GObject* gobj;
{
    VALUE ret = (VALUE)g_object_get_data(gobj, RUBY_GOBJECT_OBJ_KEY);
    if ( ! ret )
	ret = rbgobj_make_gobject_auto_type(gobj);
    return ret;
}

VALUE
rbgobj_make_gobject(klass, gobj)
    VALUE klass;
    GObject* gobj;
{
    VALUE obj = rb_obj_alloc(klass);
    gobj = g_object_ref(gobj);
    rbgobj_set_gobject(obj, gobj);
    return obj;
}

VALUE
rbgobj_make_gobject_auto_type(gobj)
    GObject* gobj;
{
    return rbgobj_make_gobject(rbgobj_lookup_rbclass(gobj), gobj);
}

void
rbgobjct_add_relative(obj, relative)
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
rbgobjcet_add_relative_removable(obj, relative, obj_ivar_id, hash_key)
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
        rb_funcall(hash, rb_intern("delete"), 1, hash_key);
    }
}

/**********************************************************************/

VALUE rbgobj_cGObject;

static VALUE
_each_with_index(obj)
    VALUE obj;
{
    rb_funcall(obj, rb_intern("each_with_index"), 0);
    return Qnil;
}

static VALUE
_params_setup(arg, parameters)
    VALUE arg;
    GParameter* parameters;
{
    int n = NUM2INT(rb_ary_entry(arg, 1));
    VALUE name, val;
    arg = rb_ary_entry(arg, 0);

    name = rb_ary_entry(arg, 0);
    val  = rb_ary_entry(arg, 1);

    StringValue(name);

    parameters[n].name = StringValuePtr(name);
    rbgobj_rvalue_to_gvalue(val, &parameters[n].value);

    return Qnil;
}

VALUE
rbgobj_gobject_new(gtype, params_hash)
    GType gtype;
    VALUE params_hash;
{
    size_t param_size = rb_enum_length(params_hash);
    GParameter* parameters = ALLOCA_N(GParameter, param_size);
    GObject* gobj;

    rb_iterate(&_each_with_index, params_hash, _params_setup, (VALUE)parameters);
    gobj = g_object_newv(gtype, param_size, parameters);

    return rbgobj_make_gobject_auto_type(gobj);    
}

static VALUE
gobj_new(self, gtype, params_hash)
    VALUE self, gtype, params_hash;
{
    return rbgobj_gobject_new(NUM2INT(gtype), params_hash);
}

static VALUE
gobj_initialize(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    rb_raise(rb_eRuntimeError, "can't instantiate class %s", rb_class2name(self));
}

static VALUE
gobj_equal(self, other)
    VALUE self, other;
{
    if (self == other)
        return Qtrue;
    if (rbgobj_get_gobject(self) == rbgobj_force_get_gobject(other))
        return Qtrue;
    return Qfalse;
}

static VALUE
gobj_inspect(self)
    VALUE self;
{
    VALUE iv = rb_ivar_get(self, id_gobject_data);
    char *cname = rb_class2name(CLASS_OF(self));
    char *s;

    if (NIL_P(iv) || RDATA(iv)->data == 0) {
	s = ALLOCA_N(char, 2+strlen(cname)+2+9+1+1);
	sprintf(s, "#<%s: destroyed>", cname);
    }
    else {
	s = ALLOCA_N(char, 2+strlen(cname)+1+18+1+4+18+1+1);
	sprintf(s, "#<%s:%p ptr=%p>", cname, (void *)self,
                rbgobj_get_gobject(self));
    }
    return rb_str_new2(s);
}

static void
clear_gobject(obj)
    VALUE obj;
{
    rb_ivar_set(obj, id_relatives, Qnil);
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
    GValue return_value;
    int i;
    VALUE result;

    rb_scan_args(argc, argv, "1*", &sig_id, &rest);
    g_signal_query(NUM2INT(sig_id), &query);

    params = g_value_array_new(query.n_params + 1);
    rbgobj_rvalue_to_gvalue(self, &(params->values[0]));
    for (i = 0; i < query.n_params; i++)
        rbgobj_rvalue_to_gvalue(rb_ary_entry(rest, i),
                                   &(params->values[i+1]));
    g_value_init(&return_value, G_TYPE_NONE);

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
_gobject_to_ruby(GValue* from)
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
    static rbgobj_class_info cinfo;

    rbgobj_cGObject = rb_define_class_under(mGLib, "GObject", rb_cObject);
    cinfo.klass = rbgobj_cGObject;
    cinfo.gtype = G_TYPE_OBJECT;
    cinfo.mark = 0;
    cinfo.free = 0;
    rbgobj_register_class(&cinfo);
    rbgobj_register_r2g_func(rbgobj_cGObject, _gobject_from_ruby);
    rbgobj_register_g2r_func(G_TYPE_OBJECT, _gobject_to_ruby);

    rb_define_singleton_method(rbgobj_cGObject, "gobject_new", gobj_new, 2);

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
    rb_define_method(rbgobj_cGObject, "==", gobj_equal, 1);
    rb_define_method(rbgobj_cGObject, "inspect", gobj_inspect, 0);
    rb_define_method(rbgobj_cGObject, "clone", gobj_clone, 0);
}

/**********************************************************************/

void Init_gobject()
{
    /* IDs */
    id_gobject_data = rb_intern("__gobject_data__");
    id_relatives = rb_intern("__relatives__");
    id_relative_callbacks = rb_intern("__relative_callbacks__");
    id_delete = rb_intern("delete");

    Init_gobject_gtype();
    Init_gobject_gvalue();
    Init_gobject_gclosure();
    Init_gobject_gparam();

    gobject_object_list_v = Qnil;
    rb_global_variable(&gobject_object_list_v);
    gobject_object_list = st_init_numtable();
    gobject_object_list_v = Data_Wrap_Struct(rb_cObject,
                                             rb_mark_tbl, st_free_table,
                                             gobject_object_list);

    Init_gobject_gobj();
}
