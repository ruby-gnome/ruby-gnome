/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkaspectframe.c -

  $Author: ggc $
  $Date: 2007/07/13 16:07:31 $

  Copyright (C) 2002,2003 Ruby-GNOME2 Project Team
  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

static VALUE
aframe_initialize(VALUE self, VALUE label, VALUE xalign, VALUE yalign, VALUE ratio, VALUE obey_child)
{
    RBGTK_INITIALIZE(self, gtk_aspect_frame_new(NIL_P(label)?NULL:RVAL2CSTR(label),
                                                NUM2DBL(xalign),
                                                NUM2DBL(yalign),
                                                NUM2DBL(ratio),
                                                RVAL2CBOOL(obey_child)));
    return Qnil;
}

static VALUE
aframe_set(VALUE self, VALUE xalign, VALUE yalign, VALUE ratio, VALUE obey_child)
{
    gtk_aspect_frame_set(GTK_ASPECT_FRAME(RVAL2GOBJ(self)),
			 NUM2DBL(xalign), NUM2DBL(yalign),
			 NUM2DBL(ratio), RVAL2CBOOL(obey_child));
    return self;
}

void 
Init_gtk_aspect_frame()
{
    VALUE gAspectFrame = G_DEF_CLASS(GTK_TYPE_ASPECT_FRAME, "AspectFrame", mGtk);

    rb_define_method(gAspectFrame, "initialize", aframe_initialize, 5);
    rb_define_method(gAspectFrame, "set", aframe_set, 4);
}
