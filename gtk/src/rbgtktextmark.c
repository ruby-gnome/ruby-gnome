/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtktextmark.c -

  $Author: ggc $
  $Date: 2007/07/13 16:30:53 $

  Copyright (C) 2002,2003 Masahiro Sakai
************************************************/

#include "global.h"

#define _SELF(s) (GTK_TEXT_MARK(RVAL2GOBJ(s)))

#if GTK_CHECK_VERSION(2,11,0)
static VALUE
initialize(self, name, left_gravity)
    VALUE self, name, left_gravity;
{
    if (NIL_P(name))
        G_INITIALIZE(self, gtk_text_mark_new(NULL, RVAL2CBOOL(left_gravity)));
    else
        G_INITIALIZE(self, gtk_text_mark_new(RVAL2CSTR(name), RVAL2CBOOL(left_gravity)));
    return Qnil;
}
#endif

static VALUE
set_visible(self, setting)
    VALUE self, setting;
{
    gtk_text_mark_set_visible(_SELF(self), RVAL2CBOOL(setting));
    return setting;
}

static VALUE
get_visible(self)
    VALUE self;
{
    return CBOOL2RVAL(gtk_text_mark_get_visible(_SELF(self)));
}

static VALUE
get_name(self)
    VALUE self;
{
    return CSTR2RVAL(gtk_text_mark_get_name(_SELF(self)));
}

static VALUE
get_deleted(self)
    VALUE self;
{
    return CBOOL2RVAL(gtk_text_mark_get_deleted(_SELF(self)));
}

static VALUE
get_buffer(self)
    VALUE self;
{
    return GOBJ2RVAL(gtk_text_mark_get_buffer(_SELF(self)));
}

static VALUE
get_left_gravity(self)
    VALUE self;
{
    return CBOOL2RVAL(gtk_text_mark_get_left_gravity(_SELF(self)));
}

void 
Init_gtk_textmark()
{
    VALUE gTextMark = G_DEF_CLASS(GTK_TYPE_TEXT_MARK, "TextMark", mGtk);
#if GTK_CHECK_VERSION(2,11,0)
    rb_define_method(gTextMark, "initialize", initialize, 2);
#endif
    rb_define_method(gTextMark, "set_visible", set_visible, 1);
    rb_define_method(gTextMark, "visible?", get_visible, 0);
    rb_define_method(gTextMark, "name", get_name, 0);
    rb_define_method(gTextMark, "deleted?", get_deleted, 0);
    rb_define_method(gTextMark, "buffer", get_buffer, 0);
    rb_define_method(gTextMark, "left_gravity?", get_left_gravity, 0);

    G_DEF_SETTERS(gTextMark);
}
