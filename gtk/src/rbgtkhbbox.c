/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkhbbox.c -

  $Author: mutoh $
  $Date: 2002/10/23 18:02:17 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

static VALUE
hbbox_initialize(self)
    VALUE self;
{
    RBGTK_INITIALIZE(self, gtk_hbutton_box_new());
    return Qnil;
}

void 
Init_gtk_hbutton_box()
{
    VALUE gHBBox = G_DEF_CLASS(GTK_TYPE_HBUTTON_BOX, "HButtonBox", mGtk);

    rb_define_method(gHBBox, "initialize", hbbox_initialize, 0);
}
