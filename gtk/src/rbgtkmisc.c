/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtkmisc.c -

  $Author: mutoh $
  $Date: 2002/07/31 17:23:54 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

static VALUE
misc_set_align(self, xalign, yalign)
    VALUE self, xalign, yalign;
{
    gtk_misc_set_alignment(GTK_MISC(RVAL2GOBJ(self)),
			   NUM2DBL(xalign), NUM2DBL(yalign));
    return self;
}

static VALUE
misc_set_padding(self, xpad, ypad)
    VALUE self, xpad, ypad;
{
    gtk_misc_set_padding(GTK_MISC(RVAL2GOBJ(self)),
			 NUM2DBL(xpad), NUM2DBL(ypad));
    return self;
}

static VALUE
misc_get_xalign(self)
    VALUE self;
{
    return rb_float_new(GTK_MISC(RVAL2GOBJ(self))->xalign);
}

static VALUE
misc_get_yalign(self)
    VALUE self;
{
    return rb_float_new(GTK_MISC(RVAL2GOBJ(self))->yalign);
}

static VALUE
misc_get_xpad(self)
    VALUE self;
{
    return INT2NUM(GTK_MISC(RVAL2GOBJ(self))->xpad);
}

static VALUE
misc_get_ypad(self)
    VALUE self;
{
    return INT2NUM(GTK_MISC(RVAL2GOBJ(self))->ypad);
}

void 
Init_gtk_misc()
{
    VALUE gMisc = G_DEF_CLASS(GTK_TYPE_MISC, "Misc", mGtk);

    rb_define_method(gMisc, "set_alignment", misc_set_align, 2);
    rb_define_method(gMisc, "set_padding", misc_set_padding, 2);
    rb_define_method(gMisc, "xalign", misc_get_xalign, 0);
    rb_define_method(gMisc, "yalign", misc_get_yalign, 0);
    rb_define_method(gMisc, "xpad", misc_get_xpad, 0);
    rb_define_method(gMisc, "ypad", misc_get_ypad, 0);

    /* child initialize */
    Init_gtk_arrow();
    Init_gtk_image();
    Init_gtk_label();
    Init_gtk_pixmap();
}
