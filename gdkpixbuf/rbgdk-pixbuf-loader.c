/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgdk-pixbuf-loader.c -

  $Author: sakai $
  $Date: 2003/03/07 07:56:34 $

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
    VALUE arg1;

    rb_scan_args(argc, argv, "01", &arg1);

    if (argc == 0) {
        loader = gdk_pixbuf_loader_new();
    } else if (argc == 1 ) {
        loader = gdk_pixbuf_loader_new_with_type(RVAL2CSTR(arg1), &error);
        if(error)
            RAISE_GERROR(error);
    } else {
        rb_raise(rb_eArgError, "Wrong number of arguments");
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
    rb_define_method(gdkPixbufLoader, "write", loader_write, 1);
    rb_define_method(gdkPixbufLoader, "last_write", last_write, 1);
    rb_define_method(gdkPixbufLoader, "close", loader_close, 0);
    rb_define_method(gdkPixbufLoader, "pixbuf", get_pixbuf, 0);
    rb_define_method(gdkPixbufLoader, "animation", get_animation, 0);
}
