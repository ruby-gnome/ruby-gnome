/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtkarrow.c -

  $Author: mutoh $
  $Date: 2002/05/19 12:29:36 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

static VALUE
arrow_initialize(self, arrow_t, shadow_t)
    VALUE self, arrow_t, shadow_t;
{
    set_widget(self, gtk_arrow_new((GtkArrowType)NUM2INT(arrow_t),
				   (GtkShadowType)NUM2INT(shadow_t)));
    return Qnil;
}

static VALUE
arrow_set(self, arrow_t, shadow_t)
    VALUE self, arrow_t, shadow_t;
{
    gtk_arrow_set(GTK_ARROW(get_widget(self)),
		  (GtkArrowType)NUM2INT(arrow_t),
		  (GtkShadowType)NUM2INT(shadow_t));
    return self;
}

void Init_gtk_arrow()
{
    gArrow = rb_define_class_under(mGtk, "Arrow", gMisc);

    rb_define_method(gArrow, "initialize", arrow_initialize, 2);
    rb_define_method(gArrow, "set", arrow_set, 2);
}
