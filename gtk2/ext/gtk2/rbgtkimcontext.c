/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2003  Masao Mutoh
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 *  MA  02110-1301  USA
 */

#include "global.h"

#define RG_TARGET_NAMESPACE cIMContext
#define _SELF(self) (GTK_IM_CONTEXT(RVAL2GOBJ(self)))

static VALUE 
imcontext_set_client_window(VALUE self, VALUE gdkwindow)
{
    gtk_im_context_set_client_window(_SELF(self), GDK_WINDOW(RVAL2GOBJ(gdkwindow)));
    return self;
}

static VALUE
imcontext_get_preedit_string(VALUE self)
{
    gchar* str;
    PangoAttrList* attrs;
    gint cursor_pos;

    gtk_im_context_get_preedit_string(_SELF(self), &str, &attrs, &cursor_pos);
    return rb_ary_new3(3, CSTR2RVAL(str), BOXED2RVAL(attrs, PANGO_TYPE_ATTR_LIST), 
                       INT2NUM(cursor_pos));
}

static VALUE
imcontext_filter_keypress(VALUE self, VALUE event)
{
    return CBOOL2RVAL(gtk_im_context_filter_keypress(_SELF(self), (GdkEventKey*)RVAL2GEV(event)));
}

static VALUE
imcontext_focus_in(VALUE self)
{
    gtk_im_context_focus_in(_SELF(self));
    return self;
}

static VALUE
imcontext_focus_out(VALUE self)
{
    gtk_im_context_focus_out(_SELF(self));
    return self;
}

static VALUE
imcontext_reset(VALUE self)
{
    gtk_im_context_reset(_SELF(self));
    return self;
}

static VALUE
imcontext_set_cursor_location(VALUE self, VALUE area)
{
    gtk_im_context_set_cursor_location(
        _SELF(self), 
        ((GdkRectangle*)RVAL2BOXED(area, GDK_TYPE_RECTANGLE)));
    return self;
}

static VALUE
imcontext_set_use_preedit(VALUE self, VALUE use_preedit)
{
    gtk_im_context_set_use_preedit(_SELF(self), RVAL2CBOOL(use_preedit));
    return self;
}

static VALUE
imcontext_set_surrounding(VALUE self, VALUE text, VALUE cursor_index)
{
    StringValue(text);
    gtk_im_context_set_surrounding(_SELF(self),
                                   RSTRING_PTR(text), RSTRING_LEN(text),
                                   NUM2INT(cursor_index));
    return self;
}

static VALUE
imcontext_get_surrounding(VALUE self)
{
    gchar* text;
    gint cursor_index;
    gboolean result = gtk_im_context_get_surrounding(_SELF(self),
                                                     &text, &cursor_index);
    return result ? rb_ary_new3(2, CSTR2RVAL(text), INT2NUM(cursor_index)) : Qnil;
}

static VALUE
imcontext_delete_surrounding(VALUE self, VALUE offset, VALUE n_chars)
{
    return CBOOL2RVAL(gtk_im_context_delete_surrounding(_SELF(self), NUM2INT(offset),
                                                        NUM2INT(n_chars)));
}

void 
Init_gtk_imcontext(void)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_IM_CONTEXT, "IMContext", mGtk);

    rb_define_method(RG_TARGET_NAMESPACE, "set_client_window", imcontext_set_client_window, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "preedit_string", imcontext_get_preedit_string, 0);
    /* Is it better to name as "filter_keypress?" ?*/
    rb_define_method(RG_TARGET_NAMESPACE, "filter_keypress", imcontext_filter_keypress, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "focus_in", imcontext_focus_in, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "focus_out", imcontext_focus_out, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "reset", imcontext_reset, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "set_cursor_location", imcontext_set_cursor_location, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "set_use_preedit", imcontext_set_use_preedit, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "set_surrounding", imcontext_set_surrounding, 2);
    rb_define_method(RG_TARGET_NAMESPACE, "surrounding", imcontext_get_surrounding, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "delete_surrounding", imcontext_delete_surrounding, 2);

    G_DEF_SETTERS(RG_TARGET_NAMESPACE);
}
