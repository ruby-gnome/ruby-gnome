/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkvruler.c -

  $Author: mutoh $
  $Date: 2003/02/01 16:46:24 $

  Copyright (C) 2002,2003 Ruby-GNOME2 Project Team
  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

static VALUE
vruler_initialize(VALUE self)
{
    RBGTK_INITIALIZE(self, gtk_vruler_new());
    return Qnil;
}

void 
Init_gtk_vruler()
{
    VALUE gVRuler = G_DEF_CLASS(GTK_TYPE_VRULER, "VRuler", mGtk);
    rb_define_method(gVRuler, "initialize", vruler_initialize, 0);
}
