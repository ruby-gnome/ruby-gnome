/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkmisc.c -

  $Author: mutoh $
  $Date: 2003/02/01 16:46:23 $

  Copyright (C) 2002,2003 Ruby-GNOME2 Project Team
  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

#define _SELF(s) (GTK_MISC(RVAL2GOBJ(s)))

static VALUE
misc_set_align(VALUE self, VALUE xalign, VALUE yalign)
{
    gtk_misc_set_alignment(_SELF(self), NUM2DBL(xalign), NUM2DBL(yalign));
    return self;
}

static VALUE
misc_set_padding(VALUE self, VALUE xpad, VALUE ypad)
{
    gtk_misc_set_padding(_SELF(self), NUM2DBL(xpad), NUM2DBL(ypad));
    return self;
}

static VALUE
misc_get_align(VALUE self)
{
    gfloat xalign, yalign;
    gtk_misc_get_alignment(_SELF(self), &xalign, &yalign);

    return rb_ary_new3(2, rb_float_new(xalign), rb_float_new(yalign));
}

static VALUE
misc_get_padding(VALUE self)
{
    gint xpad, ypad;
    gtk_misc_get_padding(_SELF(self), &xpad, &ypad);

    return rb_ary_new3(2, INT2NUM(xpad), INT2NUM(ypad));
}
void 
Init_gtk_misc()
{
    VALUE gMisc = G_DEF_CLASS(GTK_TYPE_MISC, "Misc", mGtk);

    rb_define_method(gMisc, "set_alignment", misc_set_align, 2);
    rb_define_method(gMisc, "set_padding", misc_set_padding, 2);
    rb_define_method(gMisc, "alignment", misc_get_align, 0);
    rb_define_method(gMisc, "padding", misc_get_padding, 0);
}
