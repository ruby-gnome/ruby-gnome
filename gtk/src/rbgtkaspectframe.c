/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtkaspectframe.c -

  $Author: mutoh $
  $Date: 2002/06/22 19:50:57 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

static VALUE
aframe_initialize(self, label, xalign, yalign, ratio, obey_child)
    VALUE self, label, xalign, yalign, ratio, obey_child;
{
    RBGTK_INITIALIZE(self, gtk_aspect_frame_new(NIL_P(label)?NULL:STR2CSTR(label),
					  NUM2DBL(xalign),
					  NUM2DBL(yalign),
					  NUM2DBL(ratio),
					  RTEST(obey_child)));
    return Qnil;
}

static VALUE
aframe_set(self, xalign, yalign, ratio, obey_child)
    VALUE self, xalign, yalign, ratio, obey_child;
{
    gtk_aspect_frame_set(GTK_ASPECT_FRAME(RVAL2GOBJ(self)),
			 NUM2DBL(xalign), NUM2DBL(yalign),
			 NUM2DBL(ratio), RTEST(obey_child));
    return self;
}

void Init_gtk_aspect_frame()
{
    static rbgtk_class_info cinfo;

    gAspectFrame = rb_define_class_under(mGtk, "AspectFrame", gFrame);
    cinfo.klass = gAspectFrame;
    cinfo.gtype = GTK_TYPE_ASPECT_FRAME;
    cinfo.mark = 0;
    cinfo.free = 0;
    rbgtk_register_class(&cinfo);

    rb_define_method(gAspectFrame, "initialize", aframe_initialize, 5);
    rb_define_method(gAspectFrame, "set", aframe_set, 4);
}
