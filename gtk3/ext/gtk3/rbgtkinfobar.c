/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
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

#include "rbgtk3private.h"

#define RG_TARGET_NAMESPACE cInfoBar
#define _SELF(self) (RVAL2GTKINFOBAR(self))

static VALUE
rg_initialize(int argc, VALUE *argv, VALUE self)
{
    VALUE button_ary;

    rb_scan_args(argc, argv, "*", &button_ary);

    RBGTK_INITIALIZE(self, gtk_info_bar_new());
    if (!NIL_P(button_ary))
        rb_funcall2(self, rb_intern("add_buttons"),
                    RARRAY_LEN(button_ary), RARRAY_PTR(button_ary));

    return Qnil;
}

static VALUE
rg_add_action_widget(VALUE self, VALUE child, VALUE response_id)
{
    gtk_info_bar_add_action_widget(_SELF(self),
                                   RVAL2GTKWIDGET(child),
                                   NUM2INT(response_id));

    return self;
}

static VALUE
rg_add_button(VALUE self, VALUE button_text, VALUE response_id)
{
    VALUE buffer;
    return GOBJ2RVAL(gtk_info_bar_add_button(_SELF(self),
                                             SYMBOL_P(button_text) ? RVAL2GLIBID(button_text, buffer) : RVAL2CSTR(button_text),
                                             RVAL2GTKRESPONSETYPE(response_id)));
}

static VALUE
rg_add_buttons(int argc, VALUE *argv, VALUE self)
{
    VALUE button_ary;
    rb_scan_args(argc, argv, "*", &button_ary);
    rbgtk_add_buttons(self, button_ary, rg_add_button);
    return self;
}

static VALUE
rg_action_area(VALUE self)
{
    return GOBJ2RVAL(gtk_info_bar_get_action_area(_SELF(self)));
}

static VALUE
rg_content_area(VALUE self)
{
    return GOBJ2RVAL(gtk_info_bar_get_content_area(_SELF(self)));
}

static VALUE
rg_response(VALUE self, VALUE response_id)
{
    gtk_info_bar_response(_SELF(self), NUM2INT(response_id));

    return self;
}

static VALUE
rg_set_default_response(VALUE self, VALUE response_id)
{
    gtk_info_bar_set_default_response(_SELF(self), NUM2INT(response_id));

    return self;
}

static VALUE
rg_set_response_sensitive(VALUE self, VALUE response_id, VALUE setting)
{
    gtk_info_bar_set_response_sensitive(_SELF(self),
                                        NUM2INT(response_id),
                                        RVAL2CBOOL(setting));

    return self;
}

void
Init_gtk_infobar(VALUE mGtk)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_INFO_BAR, "InfoBar", mGtk);

    RG_DEF_METHOD(initialize, -1);
    RG_DEF_METHOD(add_action_widget, 2);
    RG_DEF_METHOD(add_button, 2);
    RG_DEF_METHOD(add_buttons, -1);
    RG_DEF_METHOD(action_area, 0);
    RG_DEF_METHOD(content_area, 0);
    RG_DEF_METHOD(response, 1);
    RG_DEF_METHOD(set_default_response, 1);
    RG_DEF_METHOD(set_response_sensitive, 2);
}
