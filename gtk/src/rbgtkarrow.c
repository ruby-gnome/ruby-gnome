/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkarrow.c -

  $Author: mutoh $
  $Date: 2002/10/26 16:04:03 $

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
    rb_define_const(gArrow, "UP", INT2FIX(GTK_ARROW_UP));
    rb_define_const(gArrow, "DOWN", INT2FIX(GTK_ARROW_DOWN));
    rb_define_const(gArrow, "LEFT", INT2FIX(GTK_ARROW_LEFT));
    rb_define_const(gArrow, "RIGHT", INT2FIX(GTK_ARROW_RIGHT));
}
