/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkviewport.c -

  $Author: mutoh $
  $Date: 2002/12/25 13:18:07 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

static VALUE
vport_initialize(self, hadj, vadj)
    VALUE self, hadj, vadj;
{
    GtkAdjustment *h_adj = GTK_ADJUSTMENT(RVAL2GOBJ(hadj));
    GtkAdjustment *v_adj = GTK_ADJUSTMENT(RVAL2GOBJ(vadj));

    RBGTK_INITIALIZE(self, gtk_viewport_new(h_adj, v_adj));
    return Qnil;
}

void 
Init_gtk_viewport()
{
    VALUE gViewport = G_DEF_CLASS(GTK_TYPE_VIEWPORT, "Viewport", mGtk);
    rb_define_method(gViewport, "initialize", vport_initialize, 2);
}
