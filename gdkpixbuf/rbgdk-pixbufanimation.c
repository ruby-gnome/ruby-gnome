/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgdk-pixbufanimation.c -

  $Author: sakai $
  $Date: 2003/02/17 15:31:42 $

  Copyright (C) 2002,2003 Masao Mutoh
************************************************/
#include "rbgdk-pixbuf.h"

#define _SELF(s) (GDK_PIXBUF_ANIMATION(RVAL2GOBJ(s)))
#define RVAL2ITR(i) (GDK_PIXBUF_ANIMATION_ITER(RVAL2GOBJ(i)))

static ID tvsec, tvusec;

static VALUE
animation_initialize(self, filename)
    VALUE self, filename;
{
    GdkPixbufAnimation* ret;
    GError* error = NULL;

    ret = gdk_pixbuf_animation_new_from_file(RVAL2CSTR(filename), &error);
    if (ret == NULL)
        RAISE_GERROR(error);

    G_INITIALIZE(self, ret);
    return Qnil;
}

static VALUE
animation_get_width(self)
    VALUE self;
{
    return INT2NUM(gdk_pixbuf_animation_get_width(_SELF(self)));
}

static VALUE
animation_get_height(self)
    VALUE self;
{
    return INT2NUM(gdk_pixbuf_animation_get_height(_SELF(self)));
}

static VALUE
animation_get_iter(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE start_time;
    GTimeVal* time = NULL;

    rb_scan_args(argc, argv, "01", &start_time);

    if (! NIL_P(start_time)){
        time = g_new(GTimeVal, 1);
        time->tv_sec = rb_funcall(start_time, tvsec, 0);
        time->tv_usec = rb_funcall(start_time,tvusec, 0);
    }
    return GOBJ2RVAL(gdk_pixbuf_animation_get_iter(_SELF(self), time)); 
}

static VALUE
animation_is_static_image(self)
    VALUE self;
{
    return gdk_pixbuf_animation_is_static_image(_SELF(self)) ? Qtrue : Qfalse;
}

static VALUE
animation_get_static_image(self)
    VALUE self;
{
    return GOBJ2RVAL(gdk_pixbuf_animation_get_static_image(_SELF(self)));
}

static VALUE
animation_iter_advance(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE current_time;
    GTimeVal* time = NULL;

    rb_scan_args(argc, argv, "01", &current_time);

    if (! NIL_P(current_time)){
        time = g_new(GTimeVal, 1);
        time->tv_sec = rb_funcall(current_time, tvsec, 0);
        time->tv_usec = rb_funcall(current_time, tvusec, 0);
    }

    return gdk_pixbuf_animation_iter_advance(RVAL2ITR(self), time) ? Qtrue : Qfalse;
}

static VALUE
animation_iter_get_delay_time(self)
    VALUE self;
{
    return INT2NUM(gdk_pixbuf_animation_iter_get_delay_time(RVAL2ITR(self)));
}

static VALUE
animation_iter_on_currently_loading_frame(self)
    VALUE self;
{
    return gdk_pixbuf_animation_iter_on_currently_loading_frame(RVAL2ITR(self)) ? Qtrue : Qfalse;
}

static VALUE
animation_iter_get_pixbuf(self)
    VALUE self;
{
    return GOBJ2RVAL(gdk_pixbuf_animation_iter_get_pixbuf(RVAL2ITR(self)));
}

void
Init_gdk_pixbuf_animation(VALUE mGdk)
{
    VALUE anim = G_DEF_CLASS(GDK_TYPE_PIXBUF_ANIMATION, "PixbufAnimation", mGdk);    
    VALUE animiter = G_DEF_CLASS(GDK_TYPE_PIXBUF_ANIMATION_ITER, "PixbufAnimationIter", mGdk);    

    tvsec = rb_intern("tv_sec");
    tvusec = rb_intern("tv_usec");
   
    rb_define_method(anim, "initialize", animation_initialize, 1);
    rb_define_method(anim, "width", animation_get_width, 0);
    rb_define_method(anim, "height", animation_get_height, 0);
    rb_define_method(anim, "get_iter", animation_get_iter, -1);
    rb_define_method(anim, "static_image?", animation_is_static_image, 0);
    rb_define_method(anim, "static_image", animation_get_static_image, 0);
    rb_define_method(animiter, "advance", animation_iter_advance, -1);
    rb_define_method(animiter, "delay_time", animation_iter_get_delay_time, 0);
    rb_define_method(animiter, "on_currently_loading_frame?", animation_iter_on_currently_loading_frame, 0);
    rb_define_method(animiter, "pixbuf", animation_iter_get_pixbuf, 0);

}
