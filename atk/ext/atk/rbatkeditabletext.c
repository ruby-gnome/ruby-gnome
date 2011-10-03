/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbatkeditabletext.c -

  $Author: sakai $
  $Date: 2007/06/16 09:24:04 $

  Copyright (C) 2003 Masao Mutoh
************************************************/
#include "rbatk.h"

#define _SELF(s) (ATK_EDITABLE_TEXT(RVAL2GOBJ(s)))

struct rval2atkattributegslist_args {
    VALUE ary;
    long n;
    GSList *result;
};

static VALUE
rbatk_rval2atkattributegslist_body(VALUE value)
{
    long i;
    struct rval2atkattributegslist_args *args = (struct rval2atkattributegslist_args *)value;

    for (i = 0; i < args->n; i++) {
        VALUE ary = rb_ary_to_ary(RARRAY_PTR(args->ary)[i]);
        AtkAttribute *attribute;

        if (RARRAY_LEN(ary) != 2)
            rb_raise(rb_eArgError, "attribute %ld should be an array of length 2", i);

        attribute = g_new(AtkAttribute, 1);
        args->result = g_slist_append(args->result, attribute);
        attribute->name = g_strdup(RVAL2CSTR(RARRAY_PTR(ary)[0]));
        attribute->value = g_strdup(RVAL2CSTR(RARRAY_PTR(ary)[1]));
    }

    return Qnil;
}

static void
rbatk_atkattributegslist_free(GSList *list)
{
    GSList *p;

    for (p = list; p != NULL; p = g_slist_next(p)) {
        AtkAttribute *attribute = (AtkAttribute *)p->data;

        g_free(attribute->value);
        g_free(attribute->name);
        g_free(attribute);
    }

    g_slist_free(list);
}

static VALUE
rbatk_rval2atkattributegslist_rescue(VALUE value)
{
    rbatk_atkattributegslist_free(((struct rval2atkattributegslist_args *)value)->result);

    rb_exc_raise(rb_errinfo());
}

static GSList *
rbatk_rval2atkattributegslist(VALUE value)
{
    struct rval2atkattributegslist_args args;

    args.ary = rb_ary_to_ary(value);
    args.n = RARRAY_LEN(args.ary);
    args.result = NULL;

    rb_rescue(rbatk_rval2atkattributegslist_body, (VALUE)&args,
              rbatk_rval2atkattributegslist_rescue, (VALUE)&args);

    return args.result;
}

#define RVAL2ATKATTRIBUTEGSLIST(value) rbatk_rval2atkattributegslist(value)

static VALUE
rbatk_edit_set_run_attributes(VALUE self,
                              VALUE attributes,
                              VALUE rbstart_offset,
                              VALUE rbend_offset)
{
    AtkEditableText *editable;
    gint start_offset, end_offset;
    AtkAttributeSet *list;
    gboolean result;

    if (NIL_P(attributes))
        return Qfalse;

    editable = _SELF(self);
    start_offset = NUM2INT(rbstart_offset);
    end_offset = NUM2INT(rbend_offset);
    list = RVAL2ATKATTRIBUTEGSLIST(attributes);
    result = atk_editable_text_set_run_attributes(editable, list, start_offset, end_offset);
    rbatk_atkattributegslist_free(list);

    return CBOOL2RVAL(result);
}

static VALUE
rbatk_edit_set_text_contents(VALUE self, VALUE str)
{
    atk_editable_text_set_text_contents(_SELF(self), RVAL2CSTR(str));
    return self;
}

static VALUE
rbatk_edit_insert_text(VALUE self, VALUE str, VALUE position)
{
    gint pos = NUM2INT(position);

    StringValue(str);
    atk_editable_text_insert_text(_SELF(self), RVAL2CSTR(str), RSTRING_LEN(str), &pos);
    return INT2NUM(pos);
}

static VALUE
rbatk_edit_copy_text(VALUE self, VALUE start_pos, VALUE end_pos)
{
    atk_editable_text_copy_text(_SELF(self), NUM2INT(start_pos), NUM2INT(end_pos));
    return self;
}

static VALUE
rbatk_edit_cut_text(VALUE self, VALUE start_pos, VALUE end_pos)
{
    atk_editable_text_cut_text(_SELF(self), NUM2INT(start_pos), NUM2INT(end_pos));
    return self;
}

static VALUE
rbatk_edit_delete_text(VALUE self, VALUE start_pos, VALUE end_pos)
{
    atk_editable_text_delete_text(_SELF(self), NUM2INT(start_pos), NUM2INT(end_pos));
    return self;
}

static VALUE
rbatk_edit_paste_text(VALUE self, VALUE position)
{
    atk_editable_text_paste_text(_SELF(self), NUM2INT(position));
    return self;
}

void
Init_atk_editabletext()
{
    VALUE editable = G_DEF_INTERFACE(ATK_TYPE_EDITABLE_TEXT, "EditableText", mAtk);

   rb_define_method(editable, "set_run_attributes", rbatk_edit_set_run_attributes, 3);
   rb_define_method(editable, "set_text_contents", rbatk_edit_set_text_contents, 1);
   rb_define_method(editable, "insert_text", rbatk_edit_insert_text, 2);
   rb_define_method(editable, "copy_text", rbatk_edit_copy_text, 2);
   rb_define_method(editable, "cut_text", rbatk_edit_cut_text, 2);
   rb_define_method(editable, "delete_text", rbatk_edit_delete_text, 2);
   rb_define_method(editable, "paste_text", rbatk_edit_paste_text, 1);

   G_DEF_SETTERS(editable);
}
