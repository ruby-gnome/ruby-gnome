/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtkhseparator.c -

  $Author: mutoh $
  $Date: 2002/07/31 17:23:54 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

static VALUE
hsep_initialize(self)
    VALUE self;
{
    RBGTK_INITIALIZE(self, gtk_hseparator_new());
    return Qnil;
}

void 
Init_gtk_hseparator()
{
    VALUE gHSeparator = G_DEF_CLASS(GTK_TYPE_HSEPARATOR, "HSeparator", mGtk);

    rb_define_method(gHSeparator, "initialize", hsep_initialize, 0);
}
