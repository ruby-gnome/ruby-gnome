/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtktable.c -

  $Author: mutoh $
  $Date: 2002/05/19 12:29:41 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

static VALUE
tbl_initialize(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE row, col, homogeneous;

    rb_scan_args(argc, argv, "21", &row, &col, &homogeneous);
    set_widget(self, gtk_table_new(NUM2INT(row),
					   NUM2INT(col),
					   RTEST(homogeneous)));
    return Qnil;
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

    gtk_table_attach(GTK_TABLE(get_widget(self)),
		     get_widget(child),
		     NUM2INT(left),NUM2INT(right),
		     NUM2INT(top),NUM2INT(bottom),
		     xopt, yopt, xspc, yspc);

    return self;
}

static VALUE
tbl_set_row_spacing(self, row, spc)
    VALUE self, row, spc;
{
    gtk_table_set_row_spacing(GTK_TABLE(get_widget(self)),
			      NUM2INT(row), NUM2INT(spc));
    return self;
}

static VALUE
tbl_set_col_spacing(self, col, spc)
    VALUE self, col, spc;
{
    gtk_table_set_col_spacing(GTK_TABLE(get_widget(self)),
			      NUM2INT(col), NUM2INT(spc));
    return self;
}

static VALUE
tbl_set_row_spacings(self, spc)
    VALUE self, spc;
{
    gtk_table_set_row_spacings(GTK_TABLE(get_widget(self)), NUM2INT(spc));
    return self;
}

static VALUE
tbl_set_col_spacings(self, spc)
    VALUE self, spc;
{
    gtk_table_set_col_spacings(GTK_TABLE(get_widget(self)), NUM2INT(spc));
    return self;
}

void Init_gtk_table()
{
    gTable = rb_define_class_under(mGtk, "Table", gContainer);

    rb_define_method(gTable, "initialize", tbl_initialize, -1);
    rb_define_method(gTable, "attach", tbl_attach, -1);
    rb_define_method(gTable, "set_row_spacing", tbl_set_row_spacing, 2);
    rb_define_method(gTable, "set_col_spacing", tbl_set_col_spacing, 2);
    rb_define_method(gTable, "set_row_spacings", tbl_set_row_spacings, 1);
    rb_define_method(gTable, "set_col_spacings", tbl_set_col_spacings, 1);
}
