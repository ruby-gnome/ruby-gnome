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

#define RG_TARGET_NAMESPACE cComboBoxText
#define _SELF(self) (RVAL2GTKCOMBOBOXTEXT(self))

static VALUE
rg_initialize(int argc, VALUE *argv, VALUE self)
{
    VALUE options, entry;
    GtkWidget *widget = NULL;

    rb_scan_args(argc, argv, "01", &options);
    rbg_scan_options(options,
                     "entry", &entry,
                     NULL);

    if (RVAL2CBOOL(entry))
        widget = gtk_combo_box_text_new_with_entry();
    else
        widget = gtk_combo_box_text_new();
    RBGTK_INITIALIZE(self, widget);

    return Qnil;
}

static VALUE
rg_append(VALUE self, VALUE id, VALUE text)
{
    gtk_combo_box_text_append(_SELF(self),
                              RVAL2CSTR_ACCEPT_NIL(id),
                              RVAL2CSTR(text));

    return self;
}

static VALUE
rg_append_text(VALUE self, VALUE text)
{
    gtk_combo_box_text_append_text(_SELF(self), RVAL2CSTR(text));

    return self;
}

static VALUE
rg_active_text(VALUE self)
{
    return CSTR2RVAL_FREE(gtk_combo_box_text_get_active_text(_SELF(self)));
}

static VALUE
rg_insert(VALUE self, VALUE position, VALUE id, VALUE text)
{
    gtk_combo_box_text_insert(_SELF(self),
                              NUM2INT(position),
                              RVAL2CSTR_ACCEPT_NIL(id),
                              RVAL2CSTR(text));

    return self;
}

static VALUE
rg_insert_text(VALUE self, VALUE position, VALUE text)
{
    gtk_combo_box_text_insert_text(_SELF(self), NUM2INT(position), RVAL2CSTR(text));

    return self;
}

static VALUE
rg_prepend(VALUE self, VALUE id, VALUE text)
{
    gtk_combo_box_text_prepend(_SELF(self),
                               RVAL2CSTR_ACCEPT_NIL(id),
                               RVAL2CSTR(text));

    return self;
}

static VALUE
rg_prepend_text(VALUE self, VALUE text)
{
    gtk_combo_box_text_prepend_text(_SELF(self), RVAL2CSTR(text));

    return self;
}

static VALUE
rg_remove(VALUE self, VALUE position)
{
    gtk_combo_box_text_remove(_SELF(self), NUM2INT(position));

    return self;
}

static VALUE
rg_remove_all(VALUE self)
{
    gtk_combo_box_text_remove_all(_SELF(self));

    return self;
}

void
Init_gtk_comboboxtext(VALUE mGtk)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_COMBO_BOX_TEXT, "ComboBoxText", mGtk);

    RG_DEF_METHOD(initialize, -1);
    RG_DEF_METHOD(append, 2);
    RG_DEF_METHOD(append_text, 1);
    RG_DEF_METHOD(active_text, 0);
    RG_DEF_METHOD(insert, 3);
    RG_DEF_METHOD(insert_text, 2);
    RG_DEF_METHOD(prepend, 2);
    RG_DEF_METHOD(prepend_text, 1);
    RG_DEF_METHOD(remove, 1);
    RG_DEF_METHOD(remove_all, 0);
}
