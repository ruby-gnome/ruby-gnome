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
rg_initialize(int argc, VALUE *argv, VALUE self)
{
    VALUE title;
    rb_scan_args(argc, argv, "01", &title);
    RBGTK_INITIALIZE(self, gtk_font_selection_dialog_new(NIL_P(title) ? NULL : RVAL2CSTR(title)));
    return Qnil;
}

static VALUE
rg_font_name(VALUE self)
{
    gchar* name = gtk_font_selection_dialog_get_font_name(_SELF(self));
    return name ? CSTR2RVAL(name) : Qnil;
}

static VALUE
rg_set_font_name(VALUE self, VALUE fontname)
{
    return CBOOL2RVAL(gtk_font_selection_dialog_set_font_name(_SELF(self),
                                                              RVAL2CSTR(fontname)));
}

static VALUE
rg_preview_text(VALUE self)
{
    const gchar* text = gtk_font_selection_dialog_get_preview_text(_SELF(self));
    return text ? CSTR2RVAL(text) : Qnil;
}

static VALUE
rg_set_preview_text(VALUE self, VALUE text)
{
    gtk_font_selection_dialog_set_preview_text(_SELF(self), RVAL2CSTR(text));
    return self;
}

static VALUE
rg_font_selection(VALUE self)
{
    return GOBJ2RVAL(_SELF(self)->fontsel);
}

static VALUE
rg_ok_button(VALUE self)
{
    return GOBJ2RVAL(_SELF(self)->ok_button);
}

static VALUE
rg_cancel_button(VALUE self)
{
    return GOBJ2RVAL(_SELF(self)->cancel_button);
}

static VALUE
rg_apply_button(VALUE self)
{
    return GOBJ2RVAL(_SELF(self)->apply_button);
}

void 
Init_gtk_font_selection_dialog(VALUE mGtk)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_FONT_SELECTION_DIALOG, 
                                             "FontSelectionDialog", mGtk);

    RG_DEF_METHOD(initialize, -1);
    RG_DEF_METHOD(font_name, 0);
    RG_DEF_METHOD(set_font_name, 1);
    RG_DEF_METHOD(preview_text, 0);
    RG_DEF_METHOD(set_preview_text, 1);
    RG_DEF_METHOD(font_selection, 0);
    RG_DEF_METHOD(ok_button, 0);
    RG_DEF_METHOD(cancel_button, 0);
    RG_DEF_METHOD(apply_button, 0);

    G_DEF_SETTERS(RG_TARGET_NAMESPACE);
}
