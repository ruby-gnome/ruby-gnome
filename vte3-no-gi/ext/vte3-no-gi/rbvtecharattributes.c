/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2006-2011  Ruby-GNOME2 Project Team
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

#include "rbvte3private.h"

#define RG_TARGET_NAMESPACE cCharAttributes

static ID id_row, id_column, id_fore, id_back, id_underline, id_strikethrough;

static VALUE
rg_initialize(VALUE self, VALUE row, VALUE column, VALUE fore, VALUE back,
              VALUE underline, VALUE strikethrough)
{
    rb_ivar_set(self, id_row, row);
    rb_ivar_set(self, id_column, column);
    rb_ivar_set(self, id_fore, fore);
    rb_ivar_set(self, id_back, back);
    rb_ivar_set(self, id_underline, underline);
    rb_ivar_set(self, id_strikethrough, strikethrough);
    return Qnil;
}

static VALUE
rg_underline_p(VALUE self)
{
    return rb_ivar_get(self, id_underline);
}

static VALUE
rg_strikethrough_p(VALUE self)
{
    return rb_ivar_get(self, id_strikethrough);
}

void
Init_vte_charattributes(VALUE mVte)
{
    VALUE RG_TARGET_NAMESPACE;

    id_row = rb_intern("@row");
    id_column = rb_intern("@column");
    id_fore = rb_intern("@fore");
    id_back = rb_intern("@back");
    id_underline = rb_intern("@underline");
    id_strikethrough = rb_intern("@strikethrough");

    RG_TARGET_NAMESPACE = rb_define_class_under(mVte, "CharAttributes", rb_cObject);

    RG_DEF_METHOD(initialize, 6);
    RG_DEF_ATTR("row", TRUE, FALSE, TRUE);
    RG_DEF_ATTR("column", TRUE, FALSE, TRUE);
    RG_DEF_ATTR("fore", TRUE, FALSE, TRUE);
    RG_DEF_ATTR("back", TRUE, FALSE, TRUE);
    RG_DEF_ALIAS("foreground", "fore");
    RG_DEF_ALIAS("background", "back");
    RG_DEF_METHOD_P(underline, 0);
    RG_DEF_METHOD_P(strikethrough, 0);
}

