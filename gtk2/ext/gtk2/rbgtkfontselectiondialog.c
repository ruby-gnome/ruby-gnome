/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2002,2003 Ruby-GNOME2 Project Team
 *  Copyright (C) 2001  Neil Conway <neilconway@rogers.com>
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

#define RG_TARGET_NAMESPACE cFontSelectionDialog
#define _SELF(self) (GTK_FONT_SELECTION_DIALOG(RVAL2GOBJ(self)))

static VALUE
fsd_initialize(int argc, VALUE *argv, VALUE self)
{
    VALUE title;
    rb_scan_args(argc, argv, "01", &title);
    RBGTK_INITIALIZE(self, gtk_font_selection_dialog_new(NIL_P(title) ? NULL : RVAL2CSTR(title)));
    return Qnil;
}

static VALUE
fsd_get_font_name(VALUE self)
{
    gchar* name = gtk_font_selection_dialog_get_font_name(_SELF(self));
    return name ? CSTR2RVAL(name) : Qnil;
}

static VALUE
fsd_set_font_name(VALUE self, VALUE fontname)
{
    return CBOOL2RVAL(gtk_font_selection_dialog_set_font_name(_SELF(self),
                                                              RVAL2CSTR(fontname)));
}

static VALUE
fsd_get_preview_text(VALUE self)
{
    const gchar* text = gtk_font_selection_dialog_get_preview_text(_SELF(self));
    return text ? CSTR2RVAL(text) : Qnil;
}

static VALUE
fsd_set_preview_text(VALUE self, VALUE text)
{
    gtk_font_selection_dialog_set_preview_text(_SELF(self), RVAL2CSTR(text));
    return self;
}

static VALUE
fsd_get_font_selection(VALUE self)
{
    return GOBJ2RVAL(_SELF(self)->fontsel);
}

static VALUE
fsd_get_ok_button(VALUE self)
{
    return GOBJ2RVAL(_SELF(self)->ok_button);
}

static VALUE
fsd_get_cancel_button(VALUE self)
{
    return GOBJ2RVAL(_SELF(self)->cancel_button);
}

static VALUE
fsd_get_apply_button(VALUE self)
{
    return GOBJ2RVAL(_SELF(self)->apply_button);
}

void 
Init_gtk_font_selection_dialog(void)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_FONT_SELECTION_DIALOG, 
                                             "FontSelectionDialog", mGtk);

    rb_define_method(RG_TARGET_NAMESPACE, "initialize", fsd_initialize, -1);
    rb_define_method(RG_TARGET_NAMESPACE, "font_name", fsd_get_font_name, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "set_font_name", fsd_set_font_name, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "preview_text", fsd_get_preview_text, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "set_preview_text", fsd_set_preview_text, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "font_selection", fsd_get_font_selection, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "ok_button", fsd_get_ok_button, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "cancel_button", fsd_get_cancel_button, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "apply_button", fsd_get_apply_button, 0);

    G_DEF_SETTERS(RG_TARGET_NAMESPACE);
}
