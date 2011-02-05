/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkhruler.c -

  $Author: mutoh $
  $Date: 2003/02/01 16:46:23 $

  Copyright (C) 2002,2003 Ruby-GNOME2 Project Team
  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

static VALUE
hruler_initialize(VALUE self)
{
    RBGTK_INITIALIZE(self, gtk_hruler_new());
    return Qnil;
}

void 
Init_gtk_hruler()
{
    VALUE gHRuler = G_DEF_CLASS(GTK_TYPE_HRULER, "HRuler", mGtk);

    rb_define_method(gHRuler, "initialize", hruler_initialize, 0);
}
