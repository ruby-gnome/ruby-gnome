/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/* $Id: rbgnome-icon-list.c,v 1.8 2003/11/08 18:49:45 mutoh Exp $ */
/* based on libgnomeui/gnome-icon-list.h */

/* Gnome::IconList widget for Ruby/GNOME2
 * Copyright (C) 2002-2003 Ruby-GNOME2 Project Team
 * Copyright (C) 2002      KUBO Takehiro <kubo@jiubao.org>
 * Copyright (C) 2001      Neil Conway <neilconway@rogers.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "rbgnome.h"

static ID id_icon_data;

#define _SELF(self) GNOME_ICON_LIST(RVAL2GOBJ(self))

static VALUE
icon_list_initialize(self, icon_width, adj, flags)
    VALUE self, icon_width, adj, flags;
{
    RBGTK_INITIALIZE(self, gnome_icon_list_new(NUM2UINT(icon_width),
                                               NIL_P(adj) ? NULL : GTK_ADJUSTMENT(RVAL2GOBJ(adj)),
                                               NUM2INT(flags)));
    return Qnil;
}

static VALUE
icon_list_set_hadjustment(self, hadj)
    VALUE self, hadj;
{
    gnome_icon_list_set_hadjustment(_SELF(self), GTK_ADJUSTMENT(RVAL2GOBJ(hadj)));
    return self;
}

static VALUE
icon_list_set_vadjustment(self, vadj)
    VALUE self, vadj;
{
    gnome_icon_list_set_vadjustment(_SELF(self), GTK_ADJUSTMENT(RVAL2GOBJ(vadj)));
    return self;
}

/* To avoid excesive recomputes during insertion/deletion */
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
    return INT2NUM(gnome_icon_list_append(_SELF(self),
                                          RVAL2CSTR(icon_filename),
                                          RVAL2CSTR(text)));
}

static VALUE
icon_list_append_pixbuf(self, im, icon_filename, text)
    VALUE self, im, icon_filename, text;
{
    return INT2NUM(gnome_icon_list_append_pixbuf(_SELF(self),
                                                 GDK_PIXBUF(RVAL2GOBJ(im)), 
                                                 RVAL2CSTR(icon_filename),
                                                 RVAL2CSTR(text)));
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
icon_list_get_num_icons(self)
    VALUE self;
{
    return UINT2NUM(gnome_icon_list_get_num_icons(_SELF(self)));
}

/* Managing the selection */
static VALUE
icon_list_get_selection_mode(self)
    VALUE self;
{
    return GENUM2RVAL(gnome_icon_list_get_selection_mode(_SELF(self)), GTK_TYPE_SELECTION_MODE);
}

static VALUE
icon_list_set_selection_mode(self, mode)
    VALUE self, mode;
{
    gnome_icon_list_set_selection_mode(_SELF(self), RVAL2GENUM(mode, GTK_TYPE_SELECTION_MODE));
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
icon_list_unselect_all(self)
    VALUE self;
{
    return INT2NUM(gnome_icon_list_unselect_all(_SELF(self)));
}

static VALUE
icon_list_get_selection(self)
    VALUE self;
{
    GList *list = gnome_icon_list_get_selection(_SELF(self));
    VALUE ary = rb_ary_new();
    while (list) {
        rb_ary_push(ary, INT2NUM(GPOINTER_TO_INT(list->data)));
        list = list->next;
    }
    return ary;
}

/* Managing focus */
static VALUE
icon_list_focus_icon(self, idx)
    VALUE self, idx;
{
    gnome_icon_list_focus_icon(_SELF(self), NUM2INT(idx));
    return self;
}

/* Setting the spacing values */
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

/* Icon filename. */
static VALUE
icon_list_get_icon_filename(self, idx)
    VALUE self, idx;
{
    gchar *result = gnome_icon_list_get_icon_filename(_SELF(self), NUM2INT(idx));
    return result ? rb_str_new2(result) : Qnil;
}

static VALUE
icon_list_find_icon_from_filename(self, filename)
    VALUE self, filename;
{
    return INT2NUM(gnome_icon_list_find_icon_from_filename(_SELF(self),
                                                           RVAL2CSTR(filename)));
}

/* Attaching information to the icons */
static VALUE
icon_list_set_icon_data(self, idx, data)
    VALUE self, idx, data;
{
    GnomeIconList *gil = _SELF(self);
    int gil_idx = NUM2INT(idx);
    VALUE ary;
    int ary_idx;
    int i;

    ary = rb_ivar_get(self, id_icon_data);
    if (NIL_P(ary)) {
        ary = rb_ary_new();
        rb_ivar_set(self, id_icon_data, ary);
    }
    ary_idx = GPOINTER_TO_INT(gnome_icon_list_get_icon_data(gil, gil_idx)) - 1;

    if (ary_idx == -1) {
        if (!NIL_P(data)) {
            /* search empty entry. */
            for (i = 0; i < RARRAY_LEN(ary); i++) {
                if (NIL_P(RARRAY_PTR(ary)[i]))
                    break;
            }
            rb_ary_store(ary, i, data);
            gnome_icon_list_set_icon_data(gil, gil_idx, GINT_TO_POINTER(i + 1));
        }
    } else {
        rb_ary_store(ary, ary_idx, data);
        if (NIL_P(data))
            gnome_icon_list_set_icon_data(gil, gil_idx, NULL);
    }
    return self;
}

static VALUE
icon_list_find_icon_from_data(self, data)
    VALUE self, data;
{
    VALUE ary;
    int i;

    ary = rb_ivar_get(self, id_icon_data);
    if (NIL_P(ary))
        return INT2FIX(-1);

    for (i = 0; i < RARRAY_LEN(ary); i++) {
        if (rb_equal(RARRAY_PTR(ary)[i], data))
            break;
    }
    if (i == RARRAY_LEN(ary))
        return INT2FIX(-1);
    return INT2NUM(gnome_icon_list_find_icon_from_data(_SELF(self), GINT_TO_POINTER(i + 1)));
}

static VALUE
icon_list_get_icon_data(self, pos)
    VALUE self, pos;
{
    int ary_idx;
    VALUE ary;

    ary_idx = GPOINTER_TO_INT(gnome_icon_list_get_icon_data(_SELF(self), NUM2INT(pos))) - 1;
    if (ary_idx == -1)
        return Qnil;
    ary = rb_ivar_get(self, id_icon_data);
    return rb_ary_entry(ary, ary_idx);
}

/* Visibility */
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
icon_list_icon_is_visible(self, pos)
    VALUE self, pos;
{
    return GENUM2RVAL(gnome_icon_list_icon_is_visible(_SELF(self),
                                                      NUM2INT(pos)),
                      GTK_TYPE_VISIBILITY);
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

/* Accessibility functions */
static VALUE
icon_list_get_icon_text_item(self, idx)
    VALUE self, idx;
{
    return GOBJ2RVAL(gnome_icon_list_get_icon_text_item(_SELF(self), NUM2INT(idx)));
}

static VALUE
icon_list_get_icon_pixbuf_item(self, idx)
    VALUE self, idx;
{
    return GOBJ2RVAL(gnome_icon_list_get_icon_pixbuf_item(_SELF(self), NUM2INT(idx)));
}

void
Init_gnome_icon_list(mGnome)
    VALUE mGnome;
{
    VALUE gnoIconList = G_DEF_CLASS(GNOME_TYPE_ICON_LIST, "IconList", mGnome);

    id_icon_data = rb_intern("___icon_data___");

    /* GnomeIconListMode */
    G_DEF_CLASS(GNOME_TYPE_ICON_LIST_MODE, "Mode", gnoIconList);
    G_DEF_CONSTANTS(gnoIconList, GNOME_TYPE_ICON_LIST_MODE, "GNOME_ICON_LIST_");

    rb_define_const(gnoIconList, "IS_EDITABLE", INT2FIX(GNOME_ICON_LIST_IS_EDITABLE));
#if 0 /* who needs this constant in ruby? */
    rb_define_const(gnoIconList, "STATIC_TEXT", INT2FIX(GNOME_ICON_LIST_STATIC_TEXT));
#endif

    rb_define_method(gnoIconList, "initialize", icon_list_initialize, 3);
    rb_define_method(gnoIconList, "set_hadjustment", icon_list_set_hadjustment, 1);
    rb_define_method(gnoIconList, "set_vadjustment", icon_list_set_vadjustment, 1);
    rb_define_method(gnoIconList, "freeze", icon_list_freeze, 0);
    rb_define_method(gnoIconList, "thaw", icon_list_thaw, 0);
    rb_define_method(gnoIconList, "insert", icon_list_insert, 3);
    rb_define_method(gnoIconList, "insert_pixbuf", icon_list_insert_pixbuf, 4);
    rb_define_method(gnoIconList, "append", icon_list_append, 2);
    rb_define_method(gnoIconList, "append_pixbuf", icon_list_append_pixbuf, 3);
    rb_define_method(gnoIconList, "clear", icon_list_clear, 0);
    rb_define_method(gnoIconList, "remove", icon_list_remove, 1);
    rb_define_method(gnoIconList, "num_icons", icon_list_get_num_icons, 0);
    rb_define_method(gnoIconList, "selection_mode", icon_list_get_selection_mode, 0);
    rb_define_method(gnoIconList, "set_selection_mode", icon_list_set_selection_mode, 1);
    rb_define_method(gnoIconList, "select_icon", icon_list_select_icon, 1);
    rb_define_method(gnoIconList, "unselect_icon", icon_list_unselect_icon, 1);
    rb_define_method(gnoIconList, "unselect_all", icon_list_unselect_all, 0);
    rb_define_method(gnoIconList, "selection", icon_list_get_selection, 0);
    rb_define_method(gnoIconList, "focus_icon", icon_list_focus_icon, 1);
    rb_define_method(gnoIconList, "set_icon_width", icon_list_set_icon_width, 1);
    rb_define_method(gnoIconList, "set_row_spacing", icon_list_set_row_spacing, 1);
    rb_define_method(gnoIconList, "set_col_spacing", icon_list_set_col_spacing, 1);
    rb_define_method(gnoIconList, "set_text_spacing", icon_list_set_text_spacing, 1);
    rb_define_method(gnoIconList, "set_icon_border", icon_list_set_icon_border, 1);
    rb_define_method(gnoIconList, "set_separators", icon_list_set_separators, 1);
    rb_define_method(gnoIconList, "get_icon_filename", icon_list_get_icon_filename, 1);
    rb_define_method(gnoIconList, "find_icon_from_filename", icon_list_find_icon_from_filename, 1);
    rb_define_method(gnoIconList, "set_icon_data", icon_list_set_icon_data, 2);
    rb_define_method(gnoIconList, "find_icon_from_data", icon_list_find_icon_from_data, 1);
    rb_define_method(gnoIconList, "get_icon_data", icon_list_get_icon_data, 1);
    rb_define_method(gnoIconList, "moveto", icon_list_moveto, 2);
    rb_define_method(gnoIconList, "icon_is_visible", icon_list_icon_is_visible, 1);
    rb_define_method(gnoIconList, "get_icon_at", icon_list_get_icon_at, 2);
    rb_define_method(gnoIconList, "items_per_line", icon_list_get_items_per_line, 0);
    rb_define_method(gnoIconList, "get_icon_text_item", icon_list_get_icon_text_item, 1);
    rb_define_method(gnoIconList, "get_icon_pixbuf_item", icon_list_get_icon_pixbuf_item, 1);

    G_DEF_SETTERS(gnoIconList);
}
