/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkvpaned.c -

  $Author: mutoh $
  $Date: 2002/09/12 19:06:02 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

static VALUE
vpaned_initialize(self)
    VALUE self;
{
    RBGTK_INITIALIZE(self, gtk_vpaned_new());
    return Qnil;
}

void 
Init_gtk_vpaned()
{
    VALUE gVPaned = G_DEF_CLASS(GTK_TYPE_VPANED, "VPaned", mGtk);

    rb_define_method(gVPaned, "initialize", vpaned_initialize, 0);
}
