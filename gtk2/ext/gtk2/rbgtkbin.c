/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkbin.c -

  $Author: mutoh $
  $Date: 2003/02/01 16:46:23 $

  Copyright (C) 2002,2003 Ruby-GNOME2 Project Team
  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

static VALUE
bin_child(self)
    VALUE self;
{
    GtkWidget *child = gtk_bin_get_child(RVAL2GOBJ(self));
    return child ? GOBJ2RVAL(child) : Qnil;
}

void 
Init_gtk_bin()
{
    VALUE gBin = G_DEF_CLASS(GTK_TYPE_BIN, "Bin", mGtk);

    rb_define_method(gBin, "child", bin_child, 0);
}
