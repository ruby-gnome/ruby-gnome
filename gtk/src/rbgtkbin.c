/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtkbin.c -

  $Author: sakai $
  $Date: 2002/08/01 17:46:18 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

static VALUE
bin_child(self)
    VALUE self;
{
    GtkWidget *child = GTK_BIN(RVAL2GOBJ(self))->child;
    return child ? GOBJ2RVAL(child) : Qnil;
}

void 
Init_gtk_bin()
{
    VALUE gBin = G_DEF_CLASS(GTK_TYPE_BIN, "Bin", mGtk);

    rb_define_method(gBin, "child", bin_child, 0);
}
