/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkarrow.c -

  $Author: sakai $
  $Date: 2003/08/20 17:07:03 $

  Copyright (C) 2002,2003 Ruby-GNOME2 Project Team
  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

static VALUE
arrow_initialize(self, arrow_t, shadow_t)
    VALUE self, arrow_t, shadow_t;
{
    RBGTK_INITIALIZE(self, gtk_arrow_new((GtkArrowType)NUM2INT(arrow_t),
				   (GtkShadowType)NUM2INT(shadow_t)));
    return Qnil;
}

static VALUE
arrow_set(self, arrow_t, shadow_t)
    VALUE self, arrow_t, shadow_t;
{
    gtk_arrow_set(GTK_ARROW(RVAL2GOBJ(self)),
		  (GtkArrowType)NUM2INT(arrow_t),
		  (GtkShadowType)NUM2INT(shadow_t));
    return self;
}

void 
Init_gtk_arrow()
{
    VALUE gArrow = G_DEF_CLASS(GTK_TYPE_ARROW, "Arrow", mGtk);

    rb_define_method(gArrow, "initialize", arrow_initialize, 2);
    rb_define_method(gArrow, "set", arrow_set, 2);

    /* GtkArrowType (from General constants) */
    G_DEF_CLASS(GTK_TYPE_ARROW_TYPE, "Type", gArrow);
    G_DEF_CONSTANTS(gArrow, GTK_TYPE_ARROW_TYPE, "GTK_ARROW_");
}
