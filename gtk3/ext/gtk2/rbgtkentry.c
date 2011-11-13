/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2002-2004 Ruby-GNOME2 Project Team
 *  Copyright (C) 1998-2000 Yukihiro Matsumoto,
 *                          Daisuke Kanda,
 *                          Hiroshi Igarashi
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

#define RG_TARGET_NAMESPACE cEntry
#define _SELF(self) (GTK_ENTRY(RVAL2GOBJ(self)))

static VALUE
rg_initialize(VALUE self)
{
    RBGTK_INITIALIZE(self, gtk_entry_new());
    return Qnil;
}

static VALUE
rg_layout(VALUE self)
{
    return GOBJ2RVAL(gtk_entry_get_layout(_SELF(self)));
}

static VALUE
rg_layout_offsets(VALUE self)
{
    int x, y;
    gtk_entry_get_layout_offsets(_SELF(self), &x, &y);
    return rb_ary_new3(2, INT2NUM(x), INT2NUM(y));
}

#if GTK_CHECK_VERSION(2,4,0)
static VALUE
rg_set_completion(VALUE self, VALUE completion)
{
    gtk_entry_set_completion(_SELF(self), GTK_ENTRY_COMPLETION(RVAL2GOBJ(completion)));

    G_CHILD_SET(self, rb_intern("completion"), completion);

    return self;
}

static VALUE
rg_completion(VALUE self)
{
    return GOBJ2RVAL(gtk_entry_get_completion(_SELF(self)));
}
#endif

static VALUE
rg_layout_index_to_text_index(VALUE self, VALUE layout_index)
{
    return INT2NUM(gtk_entry_layout_index_to_text_index(_SELF(self), NUM2INT(layout_index)));
}

static VALUE
rg_text_index_to_layout_index(VALUE self, VALUE text_index)
{
    return INT2NUM(gtk_entry_text_index_to_layout_index(_SELF(self), NUM2INT(text_index)));
}

#if GTK_CHECK_VERSION(2, 12, 0)
static VALUE
rg_cursor_hadjustment(VALUE self)
{
    return GOBJ2RVAL(gtk_entry_get_cursor_hadjustment(_SELF(self)));
}

static VALUE
rg_set_cursor_hadjustment(VALUE self, VALUE adjustment)
{
    gtk_entry_set_cursor_hadjustment(_SELF(self), RVAL2GOBJ(adjustment));
    return self;
}
#endif

void 
Init_gtk_entry(VALUE mGtk)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_ENTRY, "Entry", mGtk);

    RG_DEF_METHOD(initialize, 0);
    RG_DEF_METHOD(layout, 0);
    RG_DEF_METHOD(layout_offsets, 0);
#if GTK_CHECK_VERSION(2,4,0)
    RG_DEF_METHOD(set_completion, 1);
    G_DEF_SETTER(RG_TARGET_NAMESPACE, "completion");
    RG_DEF_METHOD(completion, 0);
#endif
    RG_DEF_METHOD(layout_index_to_text_index, 1);
    RG_DEF_METHOD(text_index_to_layout_index, 1);

#if GTK_CHECK_VERSION(2, 12, 0)
    RG_DEF_METHOD(cursor_hadjustment, 0);
    RG_DEF_METHOD(set_cursor_hadjustment, 1);
    G_DEF_SETTER(RG_TARGET_NAMESPACE, "cursor_hadjustment");
#endif
}
