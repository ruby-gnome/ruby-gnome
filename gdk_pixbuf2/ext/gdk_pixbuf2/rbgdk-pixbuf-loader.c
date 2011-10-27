/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2004  Masao Mutoh
 *  Copyright (C) 2003  Geoff Youngs
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 *  MA  02110-1301  USA
 */

#include "rbgdk-pixbuf.h"

#define RG_TARGET_NAMESPACE cPixbufLoader
#define _SELF(s) GDK_PIXBUF_LOADER(RVAL2GOBJ(s))

/****************************************************/
/* File opening */
/* Image Data in Memory */
static VALUE
initialize_loader(int argc, VALUE *argv, VALUE self)
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
            rb_warning("Not supported GTK+-2.0/2.2.");
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

#if RBGDK_PIXBUF_CHECK_VERSION(2,2,0)
static VALUE
loader_get_format(VALUE self)
{
    GdkPixbufFormat* format = gdk_pixbuf_loader_get_format(_SELF(self));
    return BOXED2RVAL(format, GDK_TYPE_PIXBUF_FORMAT);
}
#endif

static VALUE
loader_write(VALUE self, VALUE data)
{
    GError *error = NULL;
  
    StringValue(data);
    if (!gdk_pixbuf_loader_write(_SELF(self),
                                 (const guchar *)RSTRING_PTR(data),
                                 RSTRING_LEN(data),
                                 &error))
        RAISE_GERROR(error);

    return Qtrue;
}

static VALUE
last_write(VALUE self, VALUE data)
{
    GError *error = NULL;

    StringValue(data);
    if (!gdk_pixbuf_loader_write(_SELF(self),
                                 (const guchar *)RSTRING_PTR(data),
                                 RSTRING_LEN(data),
                                 &error))
        RAISE_GERROR(error);

    if (!gdk_pixbuf_loader_close(_SELF(self), &error))
        RAISE_GERROR(error);

    return Qtrue;
}

#if RBGDK_PIXBUF_CHECK_VERSION(2,2,0)
static VALUE
loader_set_size(VALUE self, VALUE width, VALUE height)
{
    gdk_pixbuf_loader_set_size(_SELF(self), NUM2INT(width), NUM2INT(height));
    return self;
}
#endif

static VALUE
loader_close(VALUE self)
{
    GError *error = NULL;
    gboolean res;
	
    res = gdk_pixbuf_loader_close(_SELF(self), &error);
    if(error)
        RAISE_GERROR(error);
	
    return CBOOL2RVAL(res);
}

/****************************************************/
/* Creating image */
static VALUE
get_pixbuf(VALUE self)
{
    return GOBJ2RVAL(gdk_pixbuf_loader_get_pixbuf(_SELF(self)));
}

/* Creating animation */
static VALUE
get_animation(VALUE self)
{
    return GOBJ2RVAL(gdk_pixbuf_loader_get_animation(_SELF(self)));
}


void 
Init_gdk_pixbuf_loader(VALUE mGdk)
{
    VALUE RG_TARGET_NAMESPACE;
    /* initialize it */
    RG_TARGET_NAMESPACE = G_DEF_CLASS(GDK_TYPE_PIXBUF_LOADER, "PixbufLoader", mGdk);    

    /* 
     * File Loading, Image Data in Memory
     */
    rb_define_method(RG_TARGET_NAMESPACE, "initialize", initialize_loader, -1);

    rb_undef_method(RG_TARGET_NAMESPACE, "dup");
#if RBGDK_PIXBUF_CHECK_VERSION(2,2,0)
    rb_define_method(RG_TARGET_NAMESPACE, "format", loader_get_format, 0);
#endif
    rb_define_method(RG_TARGET_NAMESPACE, "write", loader_write, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "last_write", last_write, 1);
#if RBGDK_PIXBUF_CHECK_VERSION(2,2,0)
    rb_define_method(RG_TARGET_NAMESPACE, "set_size", loader_set_size, 2);
#endif
    rb_define_method(RG_TARGET_NAMESPACE, "close", loader_close, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "pixbuf", get_pixbuf, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "animation", get_animation, 0);
}
