/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtktable.c -

  $Author: mutoh $
  $Date: 2003/02/01 16:46:24 $

  Copyright (C) 2002,2003 Ruby-GNOME2 Project Team
  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

#define _SELF(self) (GTK_TABLE(RVAL2GOBJ(self)))

static VALUE
tbl_initialize(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE row, col, homogeneous;

    rb_scan_args(argc, argv, "21", &row, &col, &homogeneous);
    RBGTK_INITIALIZE(self, gtk_table_new(NUM2INT(row),
                                         NUM2INT(col),
                                         RTEST(homogeneous)));
    return Qnil;
}

static VALUE
tbl_resize(self, rows, columns)
    VALUE self, rows, columns;
{
    gtk_table_resize(_SELF(self), NUM2UINT(rows), NUM2UINT(columns));
    return self;
}

static VALUE
tbl_attach(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE child, left, right, top, bottom;
    VALUE arg0, arg1, arg2, arg3;
    int xopt, yopt, xspc, yspc;

    xopt = yopt = GTK_EXPAND | GTK_FILL;
    xspc = yspc = 0;
    rb_scan_args(argc, argv, "54",
                 &child, &left, &right, &top, &bottom,
                 &arg0, &arg1, &arg2, &arg3);
    if (!NIL_P(arg0)) xopt = NUM2INT(arg0);
    if (!NIL_P(arg1)) yopt = NUM2INT(arg1);
    if (!NIL_P(arg2)) xspc = NUM2INT(arg2);
    if (!NIL_P(arg3)) yspc = NUM2INT(arg3);

    gtk_table_attach(_SELF(self),
                     GTK_WIDGET(RVAL2GOBJ(child)),
                     NUM2INT(left),NUM2INT(right),
                     NUM2INT(top),NUM2INT(bottom),
                     xopt, yopt, xspc, yspc);

    return self;
}

static VALUE
tbl_attach_defaults(self, widget, left_attach, right_attach, top_attach, 
                    bottom_attach)
    VALUE self, widget, left_attach, right_attach, top_attach, bottom_attach;
{
    gtk_table_attach_defaults(_SELF(self), GTK_WIDGET(RVAL2GOBJ(widget)),
                              NUM2UINT(left_attach), NUM2UINT(right_attach),
                              NUM2UINT(top_attach), NUM2UINT(bottom_attach));
    return self;
}

static VALUE
tbl_set_row_spacings(self, spc)
    VALUE self, spc;
{
    gtk_table_set_row_spacings(_SELF(self), NUM2INT(spc));
    return self;
}

static VALUE
tbl_set_col_spacings(self, spc)
    VALUE self, spc;
{
    gtk_table_set_col_spacings(_SELF(self), NUM2INT(spc));
    return self;
}

static VALUE
tbl_get_default_row_spacing(self)
    VALUE self;
{
    return UINT2NUM(gtk_table_get_default_row_spacing(_SELF(self)));
}

static VALUE
tbl_get_default_col_spacing(self)
    VALUE self;
{
    return UINT2NUM(gtk_table_get_default_col_spacing(_SELF(self)));
}

void 
Init_gtk_table()
{
    VALUE gTable = G_DEF_CLASS(GTK_TYPE_TABLE, "Table", mGtk);

    rb_define_method(gTable, "initialize", tbl_initialize, -1);
    rb_define_method(gTable, "resize", tbl_resize, 2);
    rb_define_method(gTable, "attach", tbl_attach, -1);
    rb_define_method(gTable, "attach_defaults", tbl_attach_defaults, 5);
    rb_define_method(gTable, "set_row_spacings", tbl_set_row_spacings, 1);
    rb_define_method(gTable, "set_col_spacings", tbl_set_col_spacings, 1);
    rb_define_method(gTable, "default_row_spacing", tbl_get_default_row_spacing, 0);
    rb_define_method(gTable, "default_col_spacing", tbl_get_default_col_spacing, 0);

    G_DEF_SETTERS(gTable);
}
