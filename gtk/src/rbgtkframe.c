/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtkframe.c -

  $Author: mutoh $
  $Date: 2002/06/23 16:13:32 $

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
    RBGTK_INITIALIZE(self, gtk_frame_new(NIL_P(label)?NULL:STR2CSTR(label)));
    return Qnil;
}

static VALUE
frame_set_label(self, label)
    VALUE self, label;
{
    gtk_frame_set_label(GTK_FRAME(RVAL2GOBJ(self)), STR2CSTR(label));
    return self;
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
frame_set_shadow_type(self, type)
    VALUE self, type;
{
    gtk_frame_set_shadow_type(GTK_FRAME(RVAL2GOBJ(self)),
			      (GtkShadowType)NUM2INT(type));
    return self;
}

void Init_gtk_frame()
{
    static RGObjClassInfo cinfo;

    gFrame = rb_define_class_under(mGtk, "Frame", gBin);
    cinfo.klass = gBin;
    cinfo.gtype = GTK_TYPE_FRAME;
    cinfo.mark = 0;
    cinfo.free = 0;
    rbgtk_register_class(&cinfo);

    rb_define_method(gFrame, "initialize", frame_initialize, -1);
    rb_define_method(gFrame, "set_label", frame_set_label, 1);
    rb_define_method(gFrame, "set_label_align", frame_set_label_align, 2);
    rb_define_method(gFrame, "set_shadow_type", frame_set_shadow_type, 1);

    /* child initialization */
    Init_gtk_aspect_frame();
}

