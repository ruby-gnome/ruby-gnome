/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkbbox.c -

  $Author: sakai $
  $Date: 2003/07/20 05:05:08 $

  Copyright (C) 2002,2003 Ruby-GNOME2 Project Team
  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

static VALUE
bbox_set_child_secondary(self, child, is_secondary)
    VALUE self, child, is_secondary;
{
    gtk_button_box_set_child_secondary(GTK_BUTTON_BOX(RVAL2GOBJ(self)), 
                                       GTK_WIDGET(RVAL2GOBJ(child)),
                                       RTEST(is_secondary));
    return self;
}

void 
Init_gtk_button_box()
{
    VALUE gBBox = G_DEF_CLASS(GTK_TYPE_BUTTON_BOX, "ButtonBox", mGtk);
    rb_define_method(gBBox, "set_child_secondary", bbox_set_child_secondary, 2);

    /* GtkButtonBoxStyle(General constants) */
    G_DEF_CONSTANTS(gBBox, GTK_TYPE_BUTTON_BOX_STYLE, "GTK_BUTTONBOX_");
}
