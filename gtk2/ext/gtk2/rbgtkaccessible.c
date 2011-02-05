/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkaccessible.c -

  $Author: mutoh $
  $Date: 2003/05/24 03:27:02 $

  Copyright (C) 2003 Masao Mutoh
************************************************/

#include "global.h"

static VALUE
access_connect_widget_destroyed(VALUE self)
{
    gtk_accessible_connect_widget_destroyed(GTK_ACCESSIBLE(RVAL2GOBJ(self)));
    return self;
}

void
Init_gtk_accessible()
{
    VALUE gAccess = G_DEF_CLASS(GTK_TYPE_ACCESSIBLE, "Accessible", mGtk);

    rb_define_method(gAccess, "connect_widget_destroyed", access_connect_widget_destroyed, 0);
}

