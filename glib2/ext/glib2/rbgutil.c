/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgutil.c -

  $Author: sakai $
  $Date: 2007/07/07 08:15:26 $

  Copyright (C) 2002-2004 Masao Mutoh
************************************************/

#include "rbgprivate.h"

ID rbgutil_id_module_eval;

static ID id_add_one_arg_setter;
static ID id_set_property;
static ID id_to_a;
static ID id_allocate;

void
rbgutil_set_properties(VALUE self, VALUE hash)
{
    int i;
    VALUE ary;
    GObject* obj;

    Check_Type(hash, T_HASH);
    ary = rb_funcall(hash, id_to_a, 0);
    obj = RVAL2GOBJ(self);

    g_object_freeze_notify(obj);
    for (i = 0; i < RARRAY_LEN(ary); i++) {
      rb_funcall(self, id_set_property, 2,
                 RARRAY_PTR(RARRAY_PTR(ary)[i])[0],
                 RARRAY_PTR(RARRAY_PTR(ary)[i])[1]);
    }
    g_object_thaw_notify(obj);
}

VALUE
rbgutil_glist2ary(const GList *const list)
{
    VALUE ary;
    const GList *i;

    ary = rb_ary_new();
    for (i = list; i != NULL; i = i->next)
        rb_ary_push(ary, GOBJ2RVAL(i->data));

    return ary;
}

VALUE
rbgutil_glist2ary_and_free_body(VALUE data)
{
    VALUE ary;
    GList *i;

    ary = rb_ary_new();
    for (i = (GList *)data; i != NULL; i = i->next)
        rb_ary_push(ary, GOBJ2RVAL(i->data));

    return ary;
}

VALUE
rbgutil_glist2ary_and_free_ensure(VALUE data)
{
    g_list_free((GList *)data);

    return Qnil;
}

VALUE
rbgutil_glist2ary_and_free(GList *const list)
{
    return rb_ensure(rbgutil_glist2ary_and_free_body, (VALUE)list,
                     rbgutil_glist2ary_and_free_ensure, (VALUE)list);
}

VALUE
rbgutil_glist2ary_boxed(const GList *const list, GType gtype)
{
    VALUE ary;
    const GList *i;

    ary = rb_ary_new();
    for (i = list; i != NULL; i = i->next)
        rb_ary_push(ary, BOXED2RVAL(i->data, gtype));

    return ary;
}

struct rbgutil_glist2ary_boxed_and_free_data
{
    GList *const list;
    GType gtype;
};

VALUE
rbgutil_glist2ary_boxed_and_free_body(VALUE data)
{
    struct rbgutil_glist2ary_boxed_and_free_data *real;
    VALUE ary;
    GList *i;

    real = (struct rbgutil_glist2ary_boxed_and_free_data *)data;
    ary = rb_ary_new();
    for (i = real->list; i != NULL; i = i->next)
        rb_ary_push(ary, BOXED2RVAL(i->data, real->gtype));

    return ary;
}

VALUE
rbgutil_glist2ary_boxed_and_free_ensure(VALUE data)
{
    g_list_free(((struct rbgutil_glist2ary_boxed_and_free_data *)data)->list);

    return Qnil;
}

VALUE
rbgutil_glist2ary_boxed_and_free(GList *const list, GType gtype)
{
    struct rbgutil_glist2ary_boxed_and_free_data data = { list, gtype };

    return rb_ensure(rbgutil_glist2ary_boxed_and_free_body, (VALUE)&data,
                     rbgutil_glist2ary_boxed_and_free_ensure, (VALUE)&data);
}

VALUE
rbgutil_glist2ary_string(const GList *const list)
{
    VALUE ary;
    const GList *i;

    ary = rb_ary_new();
    for (i = list; i != NULL; i = i->next)
        rb_ary_push(ary, CSTR2RVAL(i->data));

    return ary;
}

static VALUE
rbgutil_glist2ary_string_and_free_body(VALUE data)
{
    VALUE ary;
    GList *i;

    ary = rb_ary_new();
    for (i = (GList *)data; i != NULL; i = i->next)
        rb_ary_push(ary, CSTR2RVAL(i->data));

    return ary;
}

static VALUE
rbgutil_glist2ary_string_and_free_ensure(VALUE data)
{
    GList *i;

    for (i = (GList *)data; i != NULL; i = i->next)
        g_free(i->data);
    g_list_free((GList *)data);

    return Qnil;
}

VALUE
rbgutil_glist2ary_string_and_free(GList *const list)
{
    return rb_ensure(rbgutil_glist2ary_string_and_free_body, (VALUE)list,
                     rbgutil_glist2ary_string_and_free_ensure, (VALUE)list);
}

VALUE
rbgutil_gslist2ary(const GSList *const list)
{
    VALUE ary;
    const GSList *i;
    
    ary = rb_ary_new();
    for (i = list; i != NULL; i = i->next)
        rb_ary_push(ary, GOBJ2RVAL(i->data));

    return ary;
}

static VALUE
rbgutil_gslist2ary_and_free_body(VALUE data)
{
    VALUE ary;
    GSList *i;

    ary = rb_ary_new();
    for (i = (GSList *)data; i != NULL; i = i->next)
        rb_ary_push(ary, GOBJ2RVAL(i->data));

    return ary;
}

static VALUE
rbgutil_gslist2ary_and_free_ensure(VALUE data)
{
    g_slist_free((GSList *)data);

    return Qnil;
}

VALUE
rbgutil_gslist2ary_and_free(GSList *const list)
{
    return rb_ensure(rbgutil_gslist2ary_and_free_body, (VALUE)list,
                     rbgutil_gslist2ary_and_free_ensure, (VALUE)list);
}

VALUE
rbgutil_gslist2ary_boxed(const GSList *const list, GType gtype)
{
    VALUE ary;
    const GSList *i;
    
    ary = rb_ary_new();
    for (i = list; i != NULL; i = i->next)
        rb_ary_push(ary, BOXED2RVAL(i->data, gtype));

    return ary;
}

struct rbgutil_gslist2ary_boxed_and_free_data
{
    GSList *const list;
    GType gtype;
};

static VALUE
rbgutil_gslist2ary_boxed_and_free_body(VALUE data)
{
    struct rbgutil_gslist2ary_boxed_and_free_data *real;
    VALUE ary;
    GSList *i;

    real = (struct rbgutil_gslist2ary_boxed_and_free_data *)data;
    ary = rb_ary_new();
    for (i = real->list; i != NULL; i = i->next)
        rb_ary_push(ary, BOXED2RVAL(i->data, real->gtype));

    return ary;
}

static VALUE
rbgutil_gslist2ary_boxed_and_free_ensure(VALUE data)
{
    g_slist_free(((struct rbgutil_gslist2ary_boxed_and_free_data *)data)->list);

    return Qnil;
}

VALUE
rbgutil_gslist2ary_boxed_and_free(GSList *const list, GType gtype)
{
    struct rbgutil_gslist2ary_boxed_and_free_data data = { list, gtype };

    return rb_ensure(rbgutil_gslist2ary_boxed_and_free_body, (VALUE)&data,
                     rbgutil_gslist2ary_boxed_and_free_ensure, (VALUE)&data);
}

VALUE
rbgutil_def_setters(VALUE klass)
{
    return rb_funcall(mGLib, id_add_one_arg_setter, 1, klass);
}

VALUE
rbgutil_sym_g2r_func(const GValue *from)
{
    const gchar *str = g_value_get_string(from);
    return str ? ID2SYM(rb_intern(str)) : Qnil;
}

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
rbgutil_string_set_utf8_encoding(VALUE string)
{
#ifdef HAVE_RUBY_ENCODING_H
    if (!NIL_P(string))
        rb_enc_associate(string, rb_utf8_encoding());
#endif
    return string;
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
