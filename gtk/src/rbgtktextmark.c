/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtktextmark.c -

  $Author: mutoh $
  $Date: 2002/09/12 19:06:02 $

  Copyright (C) 2002 Masahiro Sakai
************************************************/

#include "global.h"

static VALUE
set_visible(self, setting)
    VALUE self, setting;
{
    gtk_text_mark_set_visible(GTK_TEXT_MARK(RVAL2GOBJ(self)), RTEST(setting));
    return setting;
}

static VALUE
get_visible(self)
    VALUE self;
{
    return gtk_text_mark_get_visible(GTK_TEXT_MARK(RVAL2GOBJ(self))) ? Qtrue : Qfalse;;
}

static VALUE
get_name(self)
    VALUE self;
{
    return rb_str_new2(gtk_text_mark_get_name(GTK_TEXT_MARK(RVAL2GOBJ(self))));
}

static VALUE
get_deleted(self)
    VALUE self;
{
    return gtk_text_mark_get_deleted(GTK_TEXT_MARK(RVAL2GOBJ(self))) ? Qtrue : Qfalse;
}

static VALUE
get_buffer(self)
    VALUE self;
{
    return GOBJ2RVAL(gtk_text_mark_get_buffer(GTK_TEXT_MARK(RVAL2GOBJ(self))));
}

static VALUE
get_left_gravity(self)
    VALUE self;
{
    return gtk_text_mark_get_left_gravity(GTK_TEXT_MARK(RVAL2GOBJ(self))) ? Qtrue : Qfalse;
}

void 
Init_gtk_textmark()
{
    VALUE gTextMark = G_DEF_CLASS(GTK_TYPE_TEXT_MARK, "TextMark", mGtk);
    rb_define_method(gTextMark, "visible=", set_visible, 1);
    rb_define_method(gTextMark, "visible", get_visible, 0);
    rb_define_method(gTextMark, "name", get_name, 0);
    rb_define_method(gTextMark, "deleted", get_deleted, 0);
    rb_define_method(gTextMark, "buffer", get_buffer, 0);
    rb_define_method(gTextMark, "left_gravity", get_left_gravity, 0);
}
