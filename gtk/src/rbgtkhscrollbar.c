/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkhscrollbar.c -

  $Author: mutoh $
  $Date: 2002/09/12 19:06:02 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

static VALUE
hscrollbar_initialize(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE arg1;
    GtkAdjustment *adj = NULL;

    rb_scan_args(argc, argv, "01", &arg1);
    if (!NIL_P(arg1)) adj = GTK_ADJUSTMENT(RVAL2GOBJ(arg1));

    RBGTK_INITIALIZE(self, gtk_hscrollbar_new(adj));
    return Qnil;
}

void
Init_gtk_hscrollbar()
{
  VALUE gHScrollbar = G_DEF_CLASS(GTK_TYPE_HSCROLLBAR, "HScrollbar", mGtk);

  rb_define_method(gHScrollbar, "initialize", hscrollbar_initialize, -1);
}
