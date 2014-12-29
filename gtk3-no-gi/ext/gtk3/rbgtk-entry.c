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

#include "rbgtk3private.h"

#define RG_TARGET_NAMESPACE cEntry
#define _SELF(self) (RVAL2GTKENTRY(self))

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

static VALUE
rg_cursor_hadjustment(VALUE self)
{
    return GOBJ2RVAL(gtk_entry_get_cursor_hadjustment(_SELF(self)));
}

static VALUE
rg_set_cursor_hadjustment(VALUE self, VALUE adjustment)
{
    gtk_entry_set_cursor_hadjustment(_SELF(self), RVAL2GTKADJUSTMENT(adjustment));
    return self;
}

static VALUE
rg_current_icon_drag_source(VALUE self)
{
    return INT2NUM(gtk_entry_get_current_icon_drag_source(_SELF(self)));
}

static VALUE
rg_get_icon_area(VALUE self, VALUE icon_pos)
{
    GdkRectangle icon_area;

    gtk_entry_get_icon_area(_SELF(self),
                            RVAL2GTKENTRYICONPOSITION(icon_pos),
                            &icon_area);

    return GOBJ2RVAL(&icon_area);
}

static VALUE
rg_get_icon_at_pos(VALUE self, VALUE x, VALUE y)
{
    return INT2NUM(gtk_entry_get_icon_at_pos(_SELF(self), NUM2INT(x), NUM2INT(y)));
}

static VALUE
rg_text_area(VALUE self)
{
    GdkRectangle text_area;

    gtk_entry_get_text_area(_SELF(self), &text_area);

    return GOBJ2RVAL(&text_area);
}

static VALUE
rg_im_context_filter_keypress(VALUE self, VALUE event)
{
    gboolean result;

    result = gtk_entry_im_context_filter_keypress(_SELF(self),
                                                  RVAL2GDKEVENTKEY(event));

    return CBOOL2RVAL(result);
}

static VALUE
rg_progress_pulse(VALUE self)
{
    gtk_entry_progress_pulse(_SELF(self));

    return self;
}

static VALUE
rg_reset_im_context(VALUE self)
{
    gtk_entry_reset_im_context(_SELF(self));

    return self;
}

static VALUE
rg_set_icon_drag_source(VALUE self, VALUE icon_pos, VALUE target_list, VALUE actions)
{
    gtk_entry_set_icon_drag_source(_SELF(self),
                                   RVAL2GTKENTRYICONPOSITION(icon_pos),
                                   RVAL2GTKTARGETLIST(target_list),
                                   RVAL2GDKDRAGACTION(actions));

    return self;
}

static VALUE
rg_unset_invisible_char(VALUE self)
{
    gtk_entry_unset_invisible_char(_SELF(self));

    return self;
}

void 
Init_gtk_entry(VALUE mGtk)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_ENTRY, "Entry", mGtk);

#if GTK_CHECK_VERSION(3, 6, 0)
    G_DEF_CLASS(GTK_TYPE_INPUT_PURPOSE, "InputPurpose", mGtk);
    G_DEF_CLASS(GTK_TYPE_INPUT_HINTS, "InputHints", mGtk);
#endif
    RG_DEF_METHOD(initialize, 0);
    RG_DEF_METHOD(layout, 0);
    RG_DEF_METHOD(layout_offsets, 0);
    RG_DEF_METHOD(layout_index_to_text_index, 1);
    RG_DEF_METHOD(text_index_to_layout_index, 1);
    RG_DEF_METHOD(cursor_hadjustment, 0);
    RG_DEF_METHOD(set_cursor_hadjustment, 1);
    RG_DEF_METHOD(current_icon_drag_source, 0);
    RG_DEF_METHOD(get_icon_area, 1);
    RG_DEF_METHOD(get_icon_at_pos, 2);
    RG_DEF_METHOD(text_area, 0);
    RG_DEF_METHOD(im_context_filter_keypress, 1);
    RG_DEF_METHOD(progress_pulse, 0);
    RG_DEF_METHOD(reset_im_context, 0);
    RG_DEF_METHOD(set_icon_drag_source, 3);
    RG_DEF_METHOD(unset_invisible_char, 0);

    RG_REG_GLIBID_SETTER("primary-icon-stock");
    RG_REG_GLIBID_SETTER("secondary-icon-stock");
}
