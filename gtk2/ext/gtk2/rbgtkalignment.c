/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkalignment.c -

  $Author: mutoh $
  $Date: 2004/07/31 05:44:45 $

  Copyright (C) 2002-2004 Ruby-GNOME2 Project Team
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
#if GTK_CHECK_VERSION(2,4,0)
static VALUE
align_get_padding(self)
    VALUE self;
{
    guint top, bottom, left, right;
    gtk_alignment_get_padding(GTK_ALIGNMENT(RVAL2GOBJ(self)),
                              &top, &bottom, &left, &right);
    return rb_ary_new3(4, UINT2NUM(top), UINT2NUM(bottom), 
                       UINT2NUM(left), UINT2NUM(right));
}
static VALUE
align_set_padding(self, top, bottom, left, right)
    VALUE self, top, bottom, left, right;
{
    gtk_alignment_set_padding(GTK_ALIGNMENT(RVAL2GOBJ(self)),
                              NUM2UINT(top), NUM2UINT(bottom),
                              NUM2UINT(left), NUM2UINT(right));
    return self;
}
#endif

void 
Init_gtk_alignment()
{
    VALUE gAlignment = G_DEF_CLASS(GTK_TYPE_ALIGNMENT, "Alignment", mGtk);

    rb_define_method(gAlignment, "initialize", align_initialize, 4);
    rb_define_method(gAlignment, "set", align_set, 4);
#if GTK_CHECK_VERSION(2,4,0)
    rb_define_method(gAlignment, "padding", align_get_padding, 0);
    rb_define_method(gAlignment, "set_padding", align_set_padding, 4);
#endif

}
