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

#define RG_TARGET_NAMESPACE cEntryBuffer
#define _SELF(self) (RVAL2GTKENTRYBUFFER(self))

static VALUE
rg_initialize(int argc, VALUE *argv, VALUE self)
{
    VALUE initial_chars;
    GtkEntryBuffer *buffer = NULL;

    rb_scan_args(argc, argv, "01", &initial_chars);

    if (NIL_P(initial_chars)) {
        buffer = gtk_entry_buffer_new(NULL, -1);
    } else {
        StringValue(initial_chars);
        buffer = gtk_entry_buffer_new(RSTRING_PTR(initial_chars),
                                      RSTRING_LEN(initial_chars));
    }
    G_INITIALIZE(self, buffer);

    return Qnil;
}

static VALUE
rg_delete_text(VALUE self, VALUE position, VALUE n_chars)
{
    guint result;

    result = gtk_entry_buffer_delete_text(_SELF(self),
                                          NUM2UINT(position),
                                          NUM2INT(n_chars));

    return UINT2NUM(result);
}

static VALUE
rg_bytes(VALUE self)
{
    return ULONG2NUM(gtk_entry_buffer_get_bytes(_SELF(self)));
}

static VALUE
rg_insert_text(VALUE self, VALUE position, VALUE chars)
{
    guint result;

    StringValue(chars);
    result = gtk_entry_buffer_insert_text(_SELF(self),
                                          NUM2UINT(position),
                                          RSTRING_PTR(chars),
                                          RSTRING_LEN(chars));

    return UINT2NUM(result);
}

void
Init_gtk_entrybuffer(VALUE mGtk)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_ENTRY_BUFFER, "EntryBuffer", mGtk);

    RG_DEF_METHOD(initialize, -1);
    RG_DEF_METHOD(delete_text, 2);
    RG_DEF_METHOD(bytes, 0);
    RG_DEF_METHOD(insert_text, 2);
}
