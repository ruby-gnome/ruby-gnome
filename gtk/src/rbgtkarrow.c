/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtkarrow.c -

  $Author: mutoh $
  $Date: 2002/06/23 16:13:32 $

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

void Init_gtk_arrow()
{
    static RGObjClassInfo cinfo;

    gArrow = rb_define_class_under(mGtk, "Arrow", gMisc);
    cinfo.klass = gArrow;
    cinfo.gtype = GTK_TYPE_ARROW;
    cinfo.mark = 0;
    cinfo.free = 0;
    rbgtk_register_class(&cinfo);

    rb_define_method(gArrow, "initialize", arrow_initialize, 2);
    rb_define_method(gArrow, "set", arrow_set, 2);
}
