/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/* $Id: rbgnome-icon-list.c,v 1.3 2002/09/25 17:17:24 tkubo Exp $ */

/* Gnome::IconList widget for Ruby/Gnome
 * Copyright (C) 2001 Neil Conway <neilconway@rogers.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include "rbgnome.h"

#define _SELF(self) GNOME_ICON_LIST(RVAL2GOBJ(self))

static VALUE
icon_list_freeze(self)
    VALUE self;
{
    gnome_icon_list_freeze(_SELF(self));
    return self;
}

static VALUE
icon_list_thaw(self)
    VALUE self;
{
    gnome_icon_list_thaw(_SELF(self));
    return self;
}

static VALUE
icon_list_insert(self, pos, icon_filename, text)
    VALUE self, pos, icon_filename, text;
{
    gnome_icon_list_insert(_SELF(self),
                           NUM2INT(pos),
                           RVAL2CSTR(icon_filename),
                           RVAL2CSTR(text));
    return self;
}

static VALUE
icon_list_insert_pixbuf(self, pos, im, icon_filename, text)
    VALUE self, pos, im, icon_filename, text;
{
    gnome_icon_list_insert_pixbuf(_SELF(self),
                                  NUM2INT(pos),
                                  GDK_PIXBUF(RVAL2GOBJ(im)),
                                  RVAL2CSTR(icon_filename),
                                  RVAL2CSTR(text));
    return self;
}

static VALUE
icon_list_append(self, icon_filename, text)
    VALUE self, icon_filename, text;
{
    gnome_icon_list_append(_SELF(self),
                           RVAL2CSTR(icon_filename),
                           RVAL2CSTR(text));
    return Qnil;
}

static VALUE
icon_list_append_pixbuf(self, im, icon_filename, text)
    VALUE self, im, icon_filename, text;
{
    gnome_icon_list_append_pixbuf(_SELF(self),
                                  GDK_PIXBUF(RVAL2GOBJ(im)),                                  
                                  RVAL2CSTR(icon_filename),
                                  RVAL2CSTR(text));
    return Qnil;
}

static VALUE
icon_list_clear(self)
    VALUE self;
{
    gnome_icon_list_clear(_SELF(self));
    return self;
}

static VALUE
icon_list_remove(self, pos)
    VALUE self, pos;
{
    gnome_icon_list_remove(_SELF(self), NUM2INT(pos));
    return self;
}

static VALUE
icon_list_select_icon(self, idx)
    VALUE self, idx;
{
    gnome_icon_list_select_icon(_SELF(self), NUM2INT(idx));
    return self;
}

static VALUE
icon_list_unselect_icon(self, pos)
    VALUE self, pos;
{
    gnome_icon_list_unselect_icon(_SELF(self), NUM2INT(pos));
    return self;
}

static VALUE
icon_list_set_icon_width(self, w)
    VALUE self, w;
{
    gnome_icon_list_set_icon_width(_SELF(self), NUM2INT(w));
    return self;
}

static VALUE
icon_list_set_row_spacing(self, pixels)
    VALUE self, pixels;
{
    gnome_icon_list_set_row_spacing(_SELF(self), NUM2INT(pixels));
    return self;
}

static VALUE
icon_list_set_col_spacing(self, pixels)
    VALUE self, pixels;
{
    gnome_icon_list_set_col_spacing(_SELF(self), NUM2INT(pixels));
    return self;
}

static VALUE
icon_list_set_text_spacing(self, pixels)
    VALUE self, pixels;
{
    gnome_icon_list_set_text_spacing(_SELF(self), NUM2INT(pixels));
    return self;
}

static VALUE
icon_list_set_icon_border(self, pixels)
    VALUE self, pixels;
{
    gnome_icon_list_set_icon_border(_SELF(self), NUM2INT(pixels));
    return self;
}

static VALUE
icon_list_set_separators(self, sep)
    VALUE self, sep;
{
    gnome_icon_list_set_separators(_SELF(self), RVAL2CSTR(sep));
    return self;
}

static VALUE
icon_list_moveto(self, pos, yalign)
    VALUE self, pos, yalign;
{
    gnome_icon_list_moveto(_SELF(self),
                           NUM2INT(pos),
                           NUM2DBL(yalign));
    return self;
}

static VALUE
icon_list_get_icon_at(self, x, y)
    VALUE self, x, y;
{
    return INT2NUM(gnome_icon_list_get_icon_at(_SELF(self),
                                               NUM2INT(x),
                                               NUM2INT(y)));
}

static VALUE
icon_list_get_items_per_line(self)
    VALUE self;
{
    return INT2NUM(gnome_icon_list_get_items_per_line(_SELF(self)));
}

void
Init_gnome_icon_list(mGnome)
    VALUE mGnome;
{
    VALUE gnoIconList = G_DEF_CLASS(GNOME_TYPE_ICON_LIST, "IconList", mGnome);
    // TODO: new (calls new_flags)
    rb_define_method(gnoIconList, "freeze", icon_list_freeze, 0);
    rb_define_method(gnoIconList, "thaw", icon_list_thaw, 0);
    rb_define_method(gnoIconList, "insert", icon_list_insert, 3);
    rb_define_method(gnoIconList, "insert_pixbuf", icon_list_insert_pixbuf, 4);
    rb_define_method(gnoIconList, "append", icon_list_append, 2);
    rb_define_method(gnoIconList, "append_pixbuf", icon_list_append_pixbuf, 3);
    rb_define_method(gnoIconList, "clear", icon_list_clear, 0);
    rb_define_method(gnoIconList, "remove", icon_list_remove, 1);
    // TODO: set_selection_mode
    rb_define_method(gnoIconList, "select_icon", icon_list_select_icon, 1);
    rb_define_method(gnoIconList, "unselect_icon", icon_list_unselect_icon, 1);
    // TODO: unselect_all
    rb_define_method(gnoIconList, "set_icon_width", icon_list_set_icon_width, 1);
    rb_define_method(gnoIconList, "set_row_spacing", icon_list_set_row_spacing, 1);
    rb_define_method(gnoIconList, "set_col_spacing", icon_list_set_col_spacing, 1);
    rb_define_method(gnoIconList, "set_text_spacing", icon_list_set_text_spacing, 1);
    rb_define_method(gnoIconList, "set_icon_border", icon_list_set_icon_border, 1);
    rb_define_method(gnoIconList, "set_separators", icon_list_set_separators, 1);
    rb_define_method(gnoIconList, "moveto", icon_list_moveto, 2);
    rb_define_method(gnoIconList, "get_icon_at", icon_list_get_icon_at, 2);
    rb_define_method(gnoIconList, "get_items_per_line", icon_list_get_items_per_line, 0);
}
