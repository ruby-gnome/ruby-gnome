/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2002,2003 Ruby-GNOME2 Project Team
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

#define _SELF(self) (GTK_TABLE(RVAL2GOBJ(self)))

static VALUE
tbl_initialize(int argc, VALUE *argv, VALUE self)
{
    VALUE row, col, homogeneous;

    rb_scan_args(argc, argv, "21", &row, &col, &homogeneous);
    RBGTK_INITIALIZE(self, gtk_table_new(NUM2INT(row),
                                         NUM2INT(col),
                                         RVAL2CBOOL(homogeneous)));
    return Qnil;
}

static VALUE
tbl_resize(VALUE self, VALUE rows, VALUE columns)
{
    gtk_table_resize(_SELF(self), NUM2UINT(rows), NUM2UINT(columns));
    return self;
}

static VALUE
tbl_attach(int argc, VALUE *argv, VALUE self)
{
    VALUE child, left, right, top, bottom;
    VALUE arg0, arg1, arg2, arg3;
    gint xspc, yspc;
    GtkAttachOptions xopt, yopt;

    xopt = yopt = GTK_EXPAND | GTK_FILL;
    xspc = yspc = 0;
    rb_scan_args(argc, argv, "54",
                 &child, &left, &right, &top, &bottom,
                 &arg0, &arg1, &arg2, &arg3);
    if (!NIL_P(arg0)) xopt = RVAL2GFLAGS(arg0, GTK_TYPE_ATTACH_OPTIONS);
    if (!NIL_P(arg1)) yopt = RVAL2GFLAGS(arg1, GTK_TYPE_ATTACH_OPTIONS);
    if (!NIL_P(arg2)) xspc = NUM2INT(arg2);
    if (!NIL_P(arg3)) yspc = NUM2INT(arg3);

    gtk_table_attach(_SELF(self),
                     GTK_WIDGET(RVAL2GOBJ(child)),
                     NUM2INT(left),NUM2INT(right),
                     NUM2INT(top),NUM2INT(bottom),
                     xopt, yopt, xspc, yspc);

    G_CHILD_ADD(self, child);

    return self;
}

static VALUE
tbl_attach_defaults(VALUE self, VALUE widget, VALUE left_attach, VALUE right_attach, VALUE top_attach, VALUE bottom_attach)
{
    gtk_table_attach_defaults(_SELF(self), GTK_WIDGET(RVAL2GOBJ(widget)),
                              NUM2UINT(left_attach), NUM2UINT(right_attach),
                              NUM2UINT(top_attach), NUM2UINT(bottom_attach));
    G_CHILD_ADD(self, widget);
    return self;
}

static VALUE
tbl_set_row_spacing(VALUE self, VALUE row, VALUE spc)
{
    gtk_table_set_row_spacing(_SELF(self), NUM2UINT(row), NUM2UINT(spc));
    return self;
}

static VALUE
tbl_get_row_spacing(VALUE self, VALUE row)
{
    return UINT2NUM(gtk_table_get_row_spacing(_SELF(self), NUM2UINT(row)));
}

static VALUE
tbl_set_col_spacing(VALUE self, VALUE col, VALUE spc)
{
    gtk_table_set_col_spacing(_SELF(self), NUM2UINT(col), NUM2UINT(spc));
    return self;
}

static VALUE
tbl_get_col_spacing(VALUE self, VALUE col)
{
    return UINT2NUM(gtk_table_get_col_spacing(_SELF(self), NUM2UINT(col)));
}

static VALUE
tbl_set_row_spacings(VALUE self, VALUE spc)
{
    gtk_table_set_row_spacings(_SELF(self), NUM2UINT(spc));
    return self;
}

static VALUE
tbl_set_col_spacings(VALUE self, VALUE spc)
{
    gtk_table_set_col_spacings(_SELF(self), NUM2UINT(spc));
    return self;
}

static VALUE
tbl_get_row_spacings(VALUE self)
{
    return UINT2NUM(_SELF(self)->row_spacing);
}

static VALUE
tbl_get_col_spacings(VALUE self)
{
    return UINT2NUM(_SELF(self)->column_spacing);
}

static VALUE
tbl_get_default_row_spacing(VALUE self)
{
    return UINT2NUM(gtk_table_get_default_row_spacing(_SELF(self)));
}

static VALUE
tbl_get_default_col_spacing(VALUE self)
{
    return UINT2NUM(gtk_table_get_default_col_spacing(_SELF(self)));
}

void 
Init_gtk_table(void)
{
    VALUE gTable = G_DEF_CLASS(GTK_TYPE_TABLE, "Table", mGtk);

    rb_define_method(gTable, "initialize", tbl_initialize, -1);
    rb_define_method(gTable, "resize", tbl_resize, 2);
    rb_define_method(gTable, "attach", tbl_attach, -1);
    rb_define_method(gTable, "attach_defaults", tbl_attach_defaults, 5);
    G_REPLACE_SET_PROPERTY(gTable, "row_spacing", tbl_set_row_spacing, 2);
    rb_undef_method(gTable, "row_spacing=");
    G_REPLACE_SET_PROPERTY(gTable, "column_spacing", tbl_set_col_spacing, 2);
    rb_undef_method(gTable, "column_spacing=");
    rb_define_method(gTable, "get_row_spacing", tbl_get_row_spacing, 1);
    rb_undef_method(gTable, "row_spacing");
    rb_define_method(gTable, "get_column_spacing", tbl_get_col_spacing, 1);
    rb_undef_method(gTable, "column_spacing");
    rb_define_method(gTable, "set_row_spacings", tbl_set_row_spacings, 1);
    G_DEF_SETTER(gTable, "row_spacings");
    rb_define_method(gTable, "set_column_spacings", tbl_set_col_spacings, 1);
    G_DEF_SETTER(gTable, "column_spacings");
    rb_define_method(gTable, "row_spacings", tbl_get_row_spacings, 0);
    rb_define_method(gTable, "column_spacings", tbl_get_col_spacings, 0);
    rb_define_method(gTable, "default_row_spacing", tbl_get_default_row_spacing, 0);
    rb_define_method(gTable, "default_column_spacing", tbl_get_default_col_spacing, 0);
}
