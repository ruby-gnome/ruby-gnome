/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgdk-pixbuf-loader.c -

  $Author: mutoh $
  $Date: 2004/08/29 12:28:57 $

  Copyright (C) 2003 Geoff Youngs
************************************************/
#include "rbgdk-pixbuf.h"

#define _SELF(s) GDK_PIXBUF_LOADER(RVAL2GOBJ(s))

/****************************************************/
/* File opening */
/* Image Data in Memory */
static VALUE
initialize_loader(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    GdkPixbufLoader* loader;
    GError* error = NULL;
    VALUE arg1, is_mime_type;

    rb_scan_args(argc, argv, "02", &arg1, &is_mime_type);

    if (NIL_P(arg1)) {
        loader = gdk_pixbuf_loader_new();
    } else {
        if (is_mime_type == Qtrue) {
#if RBGDK_PIXBUF_CHECK_VERSION(2,4,0)
            loader = gdk_pixbuf_loader_new_with_mime_type(RVAL2CSTR(arg1), &error);
#else
            rb_warning("Not supported in GTK+-2.0.x.");
            loader = gdk_pixbuf_loader_new();
#endif
        } else {
            /* Default behavior */
            loader = gdk_pixbuf_loader_new_with_type(RVAL2CSTR(arg1), &error);
        }
        if(error) RAISE_GERROR(error);
    }
    
    G_INITIALIZE(self, loader);
    return Qnil;
}

static VALUE
copy(self)
    VALUE self;
{
    /* You can't */
    rb_raise(rb_eNotImpError, "GdkPixbufLoader objects cannot be copied");
    return Qnil;
}

#if RBGDK_PIXBUF_CHECK_VERSION(2,2,0)
static VALUE
loader_get_format(self)
    VALUE self;
{
    GdkPixbufFormat* format = gdk_pixbuf_loader_get_format(_SELF(self));
    return format ? BOXED2RVAL(format, GDK_TYPE_PIXBUF_FORMAT) : Qnil;
}
#endif

static VALUE
loader_write(self, data)
    VALUE self;
    VALUE data;
{
    GError *error = NULL;
    gboolean res;
  
    res = gdk_pixbuf_loader_write(_SELF(self), RVAL2CSTR(data), RSTRING(data)->len, &error);
    if(error)
        RAISE_GERROR(error);
    return res ? Qtrue : Qfalse;
}

static VALUE
last_write(self, data)
    VALUE self;
    VALUE data;
{
    GError *error = NULL;
    gboolean res;
	
    res = gdk_pixbuf_loader_write(_SELF(self), RVAL2CSTR(data), RSTRING(data)->len, &error);
    if(error)
    {
        RAISE_GERROR(error);
        return Qnil;
    }
	
    res = gdk_pixbuf_loader_close(_SELF(self), &error);
    if(error)
    {
        RAISE_GERROR(error);
        return Qnil;
    }
	
    return res ? Qtrue : Qfalse;
}

#if RBGDK_PIXBUF_CHECK_VERSION(2,2,0)
static VALUE
loader_set_size(self, width, height)
    VALUE self, width, height;
{
    gdk_pixbuf_loader_set_size(_SELF(self), 
                               NUM2INT(width), NUM2INT(height));
    return self;
}
#endif

static VALUE
loader_close(self)
    VALUE self;
{
    GError *error = NULL;
    gboolean res;
	
    res = gdk_pixbuf_loader_close(_SELF(self), &error);
    if(error)
        RAISE_GERROR(error);
	
    return res ? Qtrue : Qfalse;
}

/****************************************************/
/* Creating image */
static VALUE
get_pixbuf(self)
    VALUE self;
{
    return GOBJ2RVAL(gdk_pixbuf_loader_get_pixbuf(_SELF(self)));
}

/* Creating animation */
static VALUE
get_animation(self)
    VALUE self;
{
    return GOBJ2RVAL(gdk_pixbuf_loader_get_animation(_SELF(self)));
}


void 
Init_gdk_pixbuf_loader(VALUE mGdk)
{
    VALUE gdkPixbufLoader;
    /* initialize it */
    gdkPixbufLoader = G_DEF_CLASS(GDK_TYPE_PIXBUF_LOADER, "PixbufLoader", mGdk);    

    /* 
     * File Loading, Image Data in Memory
     */
    rb_define_method(gdkPixbufLoader, "initialize", initialize_loader, -1);
    rb_define_method(gdkPixbufLoader, "dup", copy, 0);
#if RBGDK_PIXBUF_CHECK_VERSION(2,2,0)
    rb_define_method(gdkPixbufLoader, "format", loader_get_format, 0);
#endif
    rb_define_method(gdkPixbufLoader, "write", loader_write, 1);
    rb_define_method(gdkPixbufLoader, "last_write", last_write, 1);
#if RBGDK_PIXBUF_CHECK_VERSION(2,2,0)
    rb_define_method(gdkPixbufLoader, "set_size", loader_set_size, 2);
#endif
    rb_define_method(gdkPixbufLoader, "close", loader_close, 0);
    rb_define_method(gdkPixbufLoader, "pixbuf", get_pixbuf, 0);
    rb_define_method(gdkPixbufLoader, "animation", get_animation, 0);
}
