/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkarrow.c -

  $Author: mutoh $
  $Date: 2003/08/31 15:29:44 $

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
    RBGTK_INITIALIZE(self, gtk_arrow_new(RVAL2GENUM(arrow_t, GTK_TYPE_ARROW_TYPE),
                                         RVAL2GENUM(shadow_t, GTK_TYPE_SHADOW_TYPE)));
    return Qnil;
}

static VALUE
arrow_set(self, arrow_t, shadow_t)
    VALUE self, arrow_t, shadow_t;
{
    gtk_arrow_set(GTK_ARROW(RVAL2GOBJ(self)),
		  RVAL2GENUM(arrow_t, GTK_TYPE_ARROW_TYPE),
		  RVAL2GENUM(shadow_t, GTK_TYPE_SHADOW_TYPE));
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
