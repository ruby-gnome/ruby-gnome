/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkhseparator.c -

  $Author: mutoh $
  $Date: 2003/02/01 16:46:23 $

  Copyright (C) 2002,2003 Ruby-GNOME2 Project Team
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
