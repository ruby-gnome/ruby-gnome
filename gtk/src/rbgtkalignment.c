/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtkalignment.c -

  $Author: mutoh $
  $Date: 2002/07/31 17:23:54 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

static VALUE
align_initialize(self, xalign, yalign, xscale, yscale)
    VALUE self, xalign, yalign, xscale, yscale;
{
    RBGTK_INITIALIZE(self, gtk_alignment_new(NUM2DBL(xalign),
				       NUM2DBL(yalign),
				       NUM2DBL(xscale),
				       NUM2DBL(yscale)));
    return Qnil;
}

static VALUE
align_set(self, xalign, yalign, xscale, yscale)
    VALUE self, xalign, yalign, xscale, yscale;
{
    gtk_alignment_set(GTK_ALIGNMENT(RVAL2GOBJ(self)),
		      NUM2DBL(xalign), NUM2DBL(yalign),
		      NUM2DBL(xscale), NUM2DBL(yscale));
    return self;
}

void 
Init_gtk_alignment()
{
    VALUE gAlignment = G_DEF_CLASS(GTK_TYPE_ALIGNMENT, "Alignment", mGtk);

    rb_define_method(gAlignment, "initialize", align_initialize, 4);
    rb_define_method(gAlignment, "set", align_set, 4);
}
