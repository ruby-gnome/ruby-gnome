/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkviewport.c -

  $Author: mutoh $
  $Date: 2002/10/23 18:02:19 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

static VALUE
vport_initialize(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE arg1, arg2;
    GtkAdjustment *h_adj = NULL;
    GtkAdjustment *v_adj = NULL;

    rb_scan_args(argc, argv, "02", &arg1, &arg2);
    if (!NIL_P(arg1)) h_adj = GTK_ADJUSTMENT(RVAL2GOBJ(arg1));
    if (!NIL_P(arg2)) v_adj = GTK_ADJUSTMENT(RVAL2GOBJ(arg2));

    RBGTK_INITIALIZE(self, gtk_viewport_new(h_adj, v_adj));
    return Qnil;
}

void 
Init_gtk_viewport()
{
    VALUE gViewport = G_DEF_CLASS(GTK_TYPE_VIEWPORT, "Viewport", mGtk);
    rb_define_method(gViewport, "initialize", vport_initialize, -1);
}
