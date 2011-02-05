/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkbbox.c -

  $Author: ggc $
  $Date: 2007/07/13 16:07:31 $

  Copyright (C) 2002,2003 Ruby-GNOME2 Project Team
  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

static VALUE
bbox_set_child_secondary(VALUE self, VALUE child, VALUE is_secondary)
{
    gtk_button_box_set_child_secondary(GTK_BUTTON_BOX(RVAL2GOBJ(self)), 
                                       GTK_WIDGET(RVAL2GOBJ(child)),
                                       RVAL2CBOOL(is_secondary));
    return self;
}
#if GTK_CHECK_VERSION(2,4,0)
static VALUE
bbox_get_child_secondary(VALUE self, VALUE child)
{
    return CBOOL2RVAL(gtk_button_box_get_child_secondary(GTK_BUTTON_BOX(RVAL2GOBJ(self)), 
                                                         GTK_WIDGET(RVAL2GOBJ(child))));
}
#endif

void 
Init_gtk_button_box()
{
    VALUE gBBox = G_DEF_CLASS(GTK_TYPE_BUTTON_BOX, "ButtonBox", mGtk);
    rb_define_method(gBBox, "set_child_secondary", bbox_set_child_secondary, 2);
#if GTK_CHECK_VERSION(2,4,0)
    rb_define_method(gBBox, "get_child_secondary", bbox_get_child_secondary, 1);
#endif
    /* GtkButtonBoxStyle(General constants) */
    G_DEF_CLASS(GTK_TYPE_BUTTON_BOX_STYLE, "Style", gBBox);
    G_DEF_CONSTANTS(gBBox, GTK_TYPE_BUTTON_BOX_STYLE, "GTK_BUTTONBOX_");
}
