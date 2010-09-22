/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkframe.c -

  $Author: mutoh $
  $Date: 2003/04/20 16:56:36 $

  Copyright (C) 2002,2003 Ruby-GNOME2 Project Team
  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

static VALUE
frame_initialize(argc, argv, self)
    int argc;
    VALUE* argv;
    VALUE self;
{
    VALUE label;
    rb_scan_args(argc, argv, "01", &label);
    RBGTK_INITIALIZE(self, gtk_frame_new(NIL_P(label)?NULL:RVAL2CSTR(label)));
    return Qnil;
}

static VALUE
frame_set_label_align(self, xalign, yalign)
    VALUE self, xalign, yalign;
{
    gtk_frame_set_label_align(GTK_FRAME(RVAL2GOBJ(self)),
			      NUM2DBL(xalign),
			      NUM2DBL(yalign));

    return self;
}

static VALUE
frame_get_label_align(self)
    VALUE self;
{
    gfloat xalign, yalign;
    gtk_frame_get_label_align(GTK_FRAME(RVAL2GOBJ(self)),
			      &xalign, &yalign);

    return rb_ary_new3(2, rb_float_new(xalign), rb_float_new(yalign));
}

void 
Init_gtk_frame()
{
    VALUE gFrame = G_DEF_CLASS(GTK_TYPE_FRAME, "Frame", mGtk);

    rb_define_method(gFrame, "initialize", frame_initialize, -1);
    rb_define_method(gFrame, "set_label_align", frame_set_label_align, 2);
    rb_define_method(gFrame, "label_align", frame_get_label_align, 0);

	 rb_undef_method(gFrame, "shadow");
	 rb_undef_method(gFrame, "shadow=");
	 rb_undef_method(gFrame, "set_shadow");
}

