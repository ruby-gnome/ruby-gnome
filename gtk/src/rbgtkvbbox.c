/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkvbbox.c -

  $Author: mutoh $
  $Date: 2003/02/01 16:46:24 $

  Copyright (C) 2002,2003 Ruby-GNOME2 Project Team
  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

static VALUE
vbbox_initialize(self)
    VALUE self;
{
    RBGTK_INITIALIZE(self, gtk_vbutton_box_new());
    return Qnil;
}

void 
Init_gtk_vbutton_box()
{
    VALUE gVBBox = G_DEF_CLASS(GTK_TYPE_VBUTTON_BOX, "VButtonBox", mGtk);

    rb_define_method(gVBBox, "initialize", vbbox_initialize, 0);
}
