/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtkaspectframe.c -

  $Author: igapy $
  $Date: 2002/05/30 00:46:41 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

static VALUE
aframe_initialize(self, label, xalign, yalign, ratio, obey_child)
    VALUE self, label, xalign, yalign, ratio, obey_child;
{
    set_widget(self, gtk_aspect_frame_new(NIL_P(label)?NULL:STR2CSTR(label),
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
    gtk_aspect_frame_set(GTK_ASPECT_FRAME(get_widget(self)),
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
