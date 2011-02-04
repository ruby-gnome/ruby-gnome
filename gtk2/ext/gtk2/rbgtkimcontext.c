/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkimcontext.c -

  $Author: ggc $
  $Date: 2007/07/13 16:07:31 $

  Copyright (C) 2003 Masao Mutoh
************************************************/
#include "global.h"

#define _SELF(self) (GTK_IM_CONTEXT(RVAL2GOBJ(self)))

static VALUE 
imcontext_set_client_window(self, gdkwindow)
    VALUE self, gdkwindow;
{
    gtk_im_context_set_client_window(_SELF(self), GDK_WINDOW(RVAL2GOBJ(gdkwindow)));
    return self;
}

static VALUE
imcontext_get_preedit_string(self)
    VALUE self;
{
    gchar* str;
    PangoAttrList* attrs;
    gint cursor_pos;

    gtk_im_context_get_preedit_string(_SELF(self), &str, &attrs, &cursor_pos);
    return rb_ary_new3(3, CSTR2RVAL(str), BOXED2RVAL(attrs, PANGO_TYPE_ATTR_LIST), 
                       INT2NUM(cursor_pos));
}

static VALUE
imcontext_filter_keypress(self, event)
    VALUE self, event;
{
    return CBOOL2RVAL(gtk_im_context_filter_keypress(_SELF(self), (GdkEventKey*)RVAL2GEV(event)));
}

static VALUE
imcontext_focus_in(self)
    VALUE self;
{
    gtk_im_context_focus_in(_SELF(self));
    return self;
}

static VALUE
imcontext_focus_out(self)
    VALUE self;
{
    gtk_im_context_focus_out(_SELF(self));
    return self;
}

static VALUE
imcontext_reset(self)
    VALUE self;
{
    gtk_im_context_reset(_SELF(self));
    return self;
}

static VALUE
imcontext_set_cursor_location(self, area)
    VALUE self, area;
{
    gtk_im_context_set_cursor_location(
        _SELF(self), 
        ((GdkRectangle*)RVAL2BOXED(area, GDK_TYPE_RECTANGLE)));
    return self;
}

static VALUE
imcontext_set_use_preedit(self, use_preedit)
    VALUE self, use_preedit;
{
    gtk_im_context_set_use_preedit(_SELF(self), RVAL2CBOOL(use_preedit));
    return self;
}

static VALUE
imcontext_set_surrounding(self, text, cursor_index)
    VALUE self, text, cursor_index;
{
    StringValue(text);
    gtk_im_context_set_surrounding(_SELF(self),
                                   RVAL2CSTR(text), RSTRING_LEN(text),
                                   NUM2INT(cursor_index));
    return self;
}

static VALUE
imcontext_get_surrounding(self)
    VALUE self;
{
    gchar* text;
    gint cursor_index;
    gboolean result = gtk_im_context_get_surrounding(_SELF(self),
                                                     &text, &cursor_index);
    return result ? rb_ary_new3(2, CSTR2RVAL(text), INT2NUM(cursor_index)) : Qnil;
}

static VALUE
imcontext_delete_surrounding(self, offset, n_chars)
    VALUE self, offset, n_chars;
{
    return CBOOL2RVAL(gtk_im_context_delete_surrounding(_SELF(self), NUM2INT(offset),
                                                        NUM2INT(n_chars)));
}

void 
Init_gtk_imcontext()
{
    VALUE gIMContext = G_DEF_CLASS(GTK_TYPE_IM_CONTEXT, "IMContext", mGtk);

    rb_define_method(gIMContext, "set_client_window", imcontext_set_client_window, 1);
    rb_define_method(gIMContext, "preedit_string", imcontext_get_preedit_string, 0);
    /* Is it better to name as "filter_keypress?" ?*/
    rb_define_method(gIMContext, "filter_keypress", imcontext_filter_keypress, 1);
    rb_define_method(gIMContext, "focus_in", imcontext_focus_in, 0);
    rb_define_method(gIMContext, "focus_out", imcontext_focus_out, 0);
    rb_define_method(gIMContext, "reset", imcontext_reset, 0);
    rb_define_method(gIMContext, "set_cursor_location", imcontext_set_cursor_location, 1);
    rb_define_method(gIMContext, "set_use_preedit", imcontext_set_use_preedit, 1);
    rb_define_method(gIMContext, "set_surrounding", imcontext_set_surrounding, 2);
    rb_define_method(gIMContext, "surrounding", imcontext_get_surrounding, 0);
    rb_define_method(gIMContext, "delete_surrounding", imcontext_delete_surrounding, 2);

    G_DEF_SETTERS(gIMContext);
}
