/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkvseparator.c -

  $Author: mutoh $
  $Date: 2002/09/12 19:06:02 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

static VALUE
vsep_initialize(self)
    VALUE self;
{
    RBGTK_INITIALIZE(self, gtk_vseparator_new());
    return Qnil;
}

void 
Init_gtk_vseparator()
{
    VALUE gVSeparator = G_DEF_CLASS(GTK_TYPE_VSEPARATOR, "VSeparator", mGtk);
    rb_define_method(gVSeparator, "initialize", vsep_initialize, 0);
}
