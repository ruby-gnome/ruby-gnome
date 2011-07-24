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

#include "rbvte.h"

static ID id_row, id_column, id_fore, id_back, id_underline, id_strikethrough;

static VALUE
ca_initialize(VALUE self, VALUE row, VALUE column, VALUE fore, VALUE back,
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
ca_get_underline(VALUE self)
{
    return rb_ivar_get(self, id_underline);
}

static VALUE
ca_get_strikethrough(VALUE self)
{
    return rb_ivar_get(self, id_strikethrough);
}

void
Init_vte_charattributes(VALUE mVte)
{
    VALUE cCharAttributes;

    id_row = rb_intern("@row");
    id_column = rb_intern("@column");
    id_fore = rb_intern("@fore");
    id_back = rb_intern("@back");
    id_underline = rb_intern("@underline");
    id_strikethrough = rb_intern("@strikethrough");

    cCharAttributes = rb_define_class_under(mVte, "CharAttributes", rb_cObject);

    rb_define_method(cCharAttributes, "initialize", ca_initialize, 6);
    rb_attr(cCharAttributes, rb_intern("row"), TRUE, FALSE, TRUE);
    rb_attr(cCharAttributes, rb_intern("column"), TRUE, FALSE, TRUE);
    rb_attr(cCharAttributes, rb_intern("fore"), TRUE, FALSE, TRUE);
    rb_attr(cCharAttributes, rb_intern("back"), TRUE, FALSE, TRUE);
    rb_define_alias(cCharAttributes, "foreground", "fore");
    rb_define_alias(cCharAttributes, "background", "back");
    rb_define_method(cCharAttributes, "underline?", ca_get_underline, 0);
    rb_define_method(cCharAttributes, "strikethrough?",
                     ca_get_strikethrough, 0);
}

