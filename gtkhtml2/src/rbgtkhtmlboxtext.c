/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkhtmlboxtext.c

  $Author: ggc $
  $Date: 2007/07/13 16:07:32 $

  Copyright (C) 2003 Masao Mutoh
************************************************/

#include <libgtkhtml/layout/htmlboxtext.h>
#include "rbgtkhtml.h"

#define _SELF(s) (HTML_BOX_TEXT(RVAL2GOBJ(s)))

static VALUE
text_initialize(self, master)
    VALUE self, master;
{
    G_INITIALIZE(self, html_box_text_new(RVAL2CBOOL(master)));
    return Qnil;
}

static VALUE
text_set_text(self, text)
    VALUE self, text;
{
    html_box_text_set_text(_SELF(self), RVAL2CSTR(text));
    return self;
}

static VALUE
text_free_relayout(self)
    VALUE self;
{
    html_box_text_free_relayout(_SELF(self));
    return self;
}

static VALUE
text_get_text(self)
    VALUE self;
{
    int text_len;
    return CSTR2RVAL(html_box_text_get_text(_SELF(self), &text_len));
}

static VALUE
text_is_master(self)
    VALUE self;
{
    return CBOOL2RVAL(html_box_text_is_master(_SELF(self)));
}

/* We don't need this
gint     html_box_text_get_len               (HtmlBoxText *box);
*/

static VALUE
text_set_generated_content(self, text)
    VALUE self, text;
{
    html_box_text_set_generated_content(_SELF(self), RVAL2CSTR(text));
    return self;
}

static VALUE
text_get_index(self, x_pos)
    VALUE self, x_pos;
{
    return INT2NUM(html_box_text_get_index(_SELF(self), NUM2INT(x_pos)));
}

static VALUE
text_get_character_extents(self, index)
    VALUE self, index;
{
    GdkRectangle rect;
    html_box_text_get_character_extents(_SELF(self), NUM2INT(index), &rect);

    return BOXED2RVAL(&rect, GDK_TYPE_RECTANGLE);
}

static VALUE
text_set_selection(self, selection, start_index, end_index)
    VALUE self, selection, start_index, end_index;
{
    html_box_text_set_selection(_SELF(self), NUM2INT(selection), 
                                NUM2INT(start_index), NUM2INT(end_index));
    return self;
}

void 
Init_html_box_text(mGtkHtml2)
    VALUE mGtkHtml2;
{
    VALUE text = G_DEF_CLASS(HTML_TYPE_BOX_TEXT, "HtmlBoxText", mGtkHtml2);

    rb_define_method(text, "initialize", text_initialize, 1);
    rb_define_method(text, "set_text", text_set_text, 1);
    rb_define_method(text, "free_relayout", text_free_relayout, 0);
    rb_define_method(text, "text", text_get_text, 0);
    rb_define_method(text, "master?", text_is_master, 0);
    rb_define_method(text, "set_generated_content", text_set_generated_content, 1);
    rb_define_method(text, "get_index", text_get_index, 1);
    rb_define_method(text, "get_character_extents", text_get_character_extents, 1);
    rb_define_method(text, "set_selection", text_set_selection, 3);

    G_DEF_SETTERS(text);

    /* HtmlBoxTextSelection */
    rb_define_const(text, "SELECTION_NONE", INT2NUM(HTML_BOX_TEXT_SELECTION_NONE));
    rb_define_const(text, "SELECTION_START", INT2NUM(HTML_BOX_TEXT_SELECTION_START));
    rb_define_const(text, "SELECTION_END", INT2NUM(HTML_BOX_TEXT_SELECTION_END));
    rb_define_const(text, "SELECTION_FULL", INT2NUM(HTML_BOX_TEXT_SELECTION_FULL));
    rb_define_const(text, "SELECTION_BOTH", INT2NUM(HTML_BOX_TEXT_SELECTION_BOTH));

}

