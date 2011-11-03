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

#define RG_TARGET_NAMESPACE cTable
#define _SELF(self) (GTK_TABLE(RVAL2GOBJ(self)))

static VALUE
rg_initialize(int argc, VALUE *argv, VALUE self)
{
    VALUE row, col, homogeneous;

    rb_scan_args(argc, argv, "21", &row, &col, &homogeneous);
    RBGTK_INITIALIZE(self, gtk_table_new(NUM2INT(row),
                                         NUM2INT(col),
                                         RVAL2CBOOL(homogeneous)));
    return Qnil;
}

static VALUE
rg_resize(VALUE self, VALUE rows, VALUE columns)
{
    gtk_table_resize(_SELF(self), NUM2UINT(rows), NUM2UINT(columns));
    return self;
}

static VALUE
rg_attach(int argc, VALUE *argv, VALUE self)
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
rg_attach_defaults(VALUE self, VALUE widget, VALUE left_attach, VALUE right_attach, VALUE top_attach, VALUE bottom_attach)
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
rg_get_row_spacing(VALUE self, VALUE row)
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
rg_get_column_spacing(VALUE self, VALUE col)
{
    return UINT2NUM(gtk_table_get_col_spacing(_SELF(self), NUM2UINT(col)));
}

static VALUE
rg_set_row_spacings(VALUE self, VALUE spc)
{
    gtk_table_set_row_spacings(_SELF(self), NUM2UINT(spc));
    return self;
}

static VALUE
rg_set_column_spacings(VALUE self, VALUE spc)
{
    gtk_table_set_col_spacings(_SELF(self), NUM2UINT(spc));
    return self;
}

static VALUE
rg_row_spacings(VALUE self)
{
    return UINT2NUM(_SELF(self)->row_spacing);
}

static VALUE
rg_column_spacings(VALUE self)
{
    return UINT2NUM(_SELF(self)->column_spacing);
}

static VALUE
rg_default_row_spacing(VALUE self)
{
    return UINT2NUM(gtk_table_get_default_row_spacing(_SELF(self)));
}

static VALUE
rg_default_column_spacing(VALUE self)
{
    return UINT2NUM(gtk_table_get_default_col_spacing(_SELF(self)));
}

void 
Init_gtk_table(VALUE mGtk)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_TABLE, "Table", mGtk);

    RG_DEF_METHOD(initialize, -1);
    RG_DEF_METHOD(resize, 2);
    RG_DEF_METHOD(attach, -1);
    RG_DEF_METHOD(attach_defaults, 5);
    G_REPLACE_SET_PROPERTY(RG_TARGET_NAMESPACE, "row_spacing", tbl_set_row_spacing, 2);
    rb_undef_method(RG_TARGET_NAMESPACE, "row_spacing=");
    G_REPLACE_SET_PROPERTY(RG_TARGET_NAMESPACE, "column_spacing", tbl_set_col_spacing, 2);
    rb_undef_method(RG_TARGET_NAMESPACE, "column_spacing=");
    RG_DEF_METHOD(get_row_spacing, 1);
    rb_undef_method(RG_TARGET_NAMESPACE, "row_spacing");
    RG_DEF_METHOD(get_column_spacing, 1);
    rb_undef_method(RG_TARGET_NAMESPACE, "column_spacing");
    RG_DEF_METHOD(set_row_spacings, 1);
    G_DEF_SETTER(RG_TARGET_NAMESPACE, "row_spacings");
    RG_DEF_METHOD(set_column_spacings, 1);
    G_DEF_SETTER(RG_TARGET_NAMESPACE, "column_spacings");
    RG_DEF_METHOD(row_spacings, 0);
    RG_DEF_METHOD(column_spacings, 0);
    RG_DEF_METHOD(default_row_spacing, 0);
    RG_DEF_METHOD(default_column_spacing, 0);
}
