/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbatkeditabletext.c -

  $Author: mutoh $
  $Date: 2003/12/23 15:59:28 $

  Copyright (C) 2003 Masao Mutoh
************************************************/
#include "rbatk.h"

#define _SELF(s) (ATK_EDITABLE_TEXT(RVAL2GOBJ(s)))

static VALUE
rbatk_edit_set_run_attributes(self, attributes, start_offset, end_offset)
     VALUE self, attributes, start_offset, end_offset;
{
    long i;
    gboolean ret;
    AtkAttributeSet* list = NULL;
                                                                                
    if (NIL_P(attributes)) return Qfalse;

    Check_Type(attributes, T_ARRAY);
    for (i=0; i<RARRAY(attributes)->len; i++) {
        list = g_slist_append(list, RVAL2GOBJ(RARRAY(attributes)->ptr[i]));
    }

    ret = CBOOL2RVAL(atk_editable_text_set_run_attributes(
                         _SELF(self), list,
                         NUM2INT(start_offset), NUM2INT(end_offset)));
    g_slist_free(list);
    return ret;
}

static VALUE
rbatk_edit_set_text_contents(self, str)
    VALUE self, str;
{
    atk_editable_text_set_text_contents(_SELF(self), RVAL2CSTR(str));
    return self;
}

static VALUE
rbatk_edit_insert_text(self, str, position)
    VALUE self, str, position;
{
    gint pos = NUM2INT(position);

    StringValue(str);
    atk_editable_text_insert_text(_SELF(self), RVAL2CSTR(str), RSTRING(str)->len, &pos);
    return INT2NUM(pos);
}

static VALUE
rbatk_edit_copy_text(self, start_pos, end_pos)
    VALUE self, start_pos, end_pos;
{
    atk_editable_text_copy_text(_SELF(self), NUM2INT(start_pos), NUM2INT(end_pos));
    return self;
}

static VALUE
rbatk_edit_cut_text(self, start_pos, end_pos)
    VALUE self, start_pos, end_pos;
{
    atk_editable_text_cut_text(_SELF(self), NUM2INT(start_pos), NUM2INT(end_pos));
    return self;
}

static VALUE
rbatk_edit_delete_text(self, start_pos, end_pos)
    VALUE self, start_pos, end_pos;
{
    atk_editable_text_delete_text(_SELF(self), NUM2INT(start_pos), NUM2INT(end_pos));
    return self;
}

static VALUE
rbatk_edit_paste_text(self, position)
    VALUE self, position;
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
