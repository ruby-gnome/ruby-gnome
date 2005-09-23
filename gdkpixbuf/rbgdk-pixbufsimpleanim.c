/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgdk-pixbufanimation.c -

  $Author: ggc $
  $Date: 2005/09/23 22:02:07 $

  Copyright (C) 2002,2003 the ruby-gnome2 project
************************************************/
#include "rbgdk-pixbuf.h"

#if RBGDK_PIXBUF_CHECK_VERSION(2,8,0)
#define _SELF(s) (GDK_PIXBUF_SIMPLE_ANIM(RVAL2GOBJ(s)))

static VALUE
simpleanim_initialize(self, width, height, rate)
    VALUE self, width, height, rate;
{
    GdkPixbufSimpleAnim* ret = gdk_pixbuf_simple_anim_new(NUM2INT(width), NUM2INT(height), NUM2DBL(rate));
    G_INITIALIZE(self, ret);
    return Qnil;
}

static VALUE
simpleanim_add_frame(self, pixbuf)
    VALUE self, pixbuf;
{
    gdk_pixbuf_simple_anim_add_frame(_SELF(self), RVAL2GOBJ(pixbuf));
    return self;
}
#endif

void
Init_gdk_pixbuf_simpleanim(VALUE mGdk)
{
#if RBGDK_PIXBUF_CHECK_VERSION(2,8,0)
    VALUE anim = G_DEF_CLASS(GDK_TYPE_PIXBUF_SIMPLE_ANIM, "PixbufSimpleAnim", mGdk);    

    rb_define_method(anim, "initialize", simpleanim_initialize, 3);
    rb_define_method(anim, "add_frame", simpleanim_add_frame, 1);
#endif
}
