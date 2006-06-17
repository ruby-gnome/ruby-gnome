/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgutil.c -

  $Author: mutoh $
  $Date: 2006/06/17 11:05:46 $

  Copyright (C) 2002-2004 Masao Mutoh
************************************************/

#include "global.h"

ID rbgutil_id_module_eval;

static ID id_add_one_arg_setter;
static ID id_set_property;
static ID id_to_a;
static ID id_allocate;

void
rbgutil_set_properties(self, hash)
    VALUE self, hash;
{
    int i;
    VALUE ary;
    GObject* obj;

    Check_Type(hash, T_HASH);
    ary = rb_funcall(hash, id_to_a, 0);
    obj = RVAL2GOBJ(self);

    g_object_freeze_notify(obj);
    for (i = 0; i < RARRAY(ary)->len; i++) {
      rb_funcall(self, id_set_property, 2,
                 RARRAY(RARRAY(ary)->ptr[i])->ptr[0],
                 RARRAY(RARRAY(ary)->ptr[i])->ptr[1]);
    }
    g_object_thaw_notify(obj);
}

VALUE
rbgutil_glist2ary(list)
    GList *list;
{
    VALUE ary = rb_ary_new();
    while (list) {
        rb_ary_push(ary, GOBJ2RVAL(list->data));
        list = list->next;
    }
    return ary;
}

VALUE
rbgutil_glist2ary_and_free(list)
    GList *list;
{
    VALUE ary = rb_ary_new();
    while (list) {
        rb_ary_push(ary, GOBJ2RVAL(list->data));
        list = list->next;
    }
    g_list_free(list);
    return ary;
}

VALUE
rbgutil_glist2ary_boxed(list, gtype)
    GList *list;
    GType gtype;
{
    VALUE ary = rb_ary_new();
    while (list) {
        rb_ary_push(ary, BOXED2RVAL(list->data, gtype));
        list = list->next;
    }
    return ary;
}

VALUE
rbgutil_glist2ary_boxed_and_free(list, gtype)
    GList *list;
    GType gtype;
{
    VALUE ary = rb_ary_new();
    while (list) {
        rb_ary_push(ary, BOXED2RVAL(list->data, gtype));
        list = list->next;
    }
    g_list_free(list);
    return ary;
}

VALUE
rbgutil_gslist2ary(list)
    GSList *list;
{
    VALUE ary = rb_ary_new();
    while (list) {
        rb_ary_push(ary, GOBJ2RVAL(list->data));
        list = list->next;
    }
    return ary;
}

VALUE
rbgutil_gslist2ary_boxed(list, gtype)
    GSList *list;
    GType gtype;
{
    VALUE ary = rb_ary_new();
    while (list) {
        rb_ary_push(ary, BOXED2RVAL(list->data, gtype));
        list = list->next;
    }
    return ary;
}

VALUE
rbgutil_gslist2ary_and_free(list)
    GSList *list;
{
    VALUE ary = rb_ary_new();
    while (list) {
        rb_ary_push(ary, GOBJ2RVAL(list->data));
        list = list->next;
    }
    g_slist_free(list);
    return ary;
}

VALUE
rbgutil_gslist2ary_boxed_and_free(list, gtype)
    GSList *list;
    GType gtype;
{
    VALUE ary = rb_ary_new();
    while (list) {
        rb_ary_push(ary, BOXED2RVAL(list->data, gtype));
        list = list->next;
    }
    g_slist_free(list);
    return ary;
}

VALUE
rbgutil_def_setters(klass)
    VALUE klass;
{
    return rb_funcall(mGLib, id_add_one_arg_setter, 1, klass);
}

VALUE
rbgutil_sym_g2r_func(from)
    const GValue *from;
{
    const gchar *str = g_value_get_string(from);
    return str ? ID2SYM(rb_intern(str)) : Qnil;
}

#ifndef HAVE_OBJECT_ALLOCATE
VALUE
rbgutil_generic_s_new(int argc, VALUE* argv, VALUE self)
{
    VALUE obj = rb_funcall(self, id_allocate, 0);
    rb_obj_call_init(obj, argc, argv);
    return obj;
}
#endif /* HAVE_OBJECT_ALLOCATE */

VALUE
rbgutil_generic_s_gtype(VALUE klass)
{
    return rbgobj_gtype_new(CLASS2GTYPE(klass));
}

VALUE
rbgutil_generic_gtype(VALUE self)
{
    return generic_s_gtype(CLASS_OF(self));
}


VALUE
rbgutil_protect(VALUE (*func) (VALUE), VALUE data)
{
  int state = 0;
  VALUE ret = rb_protect(func, data, &state);
  if (state && !NIL_P(ruby_errinfo)) {
      rb_funcall(mGLib, rb_intern("exit_application"), 1, EXIT_FAILURE);
  }
  return ret;
}

void
Init_gutil()
{
    rbgutil_id_module_eval = rb_intern("module_eval");
    id_set_property = rb_intern("set_property");
    id_to_a = rb_intern("to_a");
    id_add_one_arg_setter = rb_intern("__add_one_arg_setter");
    id_allocate = rb_intern("allocate");
}
