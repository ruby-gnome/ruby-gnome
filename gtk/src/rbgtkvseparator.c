/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtkvseparator.c -

  $Author: mutoh $
  $Date: 2002/07/31 17:23:54 $

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
