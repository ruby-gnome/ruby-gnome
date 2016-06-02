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

#include "rbgdk-pixbuf2private.h"

#define RG_TARGET_NAMESPACE cPixbufLoader
#define _SELF(s) RVAL2GDKPIXBUFLOADER(s)

/****************************************************/
/* File opening */
/* Image Data in Memory */
static VALUE
rg_initialize(int argc, VALUE *argv, VALUE self)
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
rg_format(VALUE self)
{
    GdkPixbufFormat* format = gdk_pixbuf_loader_get_format(_SELF(self));
    return GDKPIXBUFFORMAT2RVAL(format);
}
#endif

static VALUE
rg_write(VALUE self, VALUE data)
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
rg_last_write(VALUE self, VALUE data)
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
rg_set_size(VALUE self, VALUE width, VALUE height)
{
    gdk_pixbuf_loader_set_size(_SELF(self), NUM2INT(width), NUM2INT(height));
    return self;
}
#endif

static VALUE
rg_close(VALUE self)
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
rg_pixbuf(VALUE self)
{
    return GOBJ2RVAL(gdk_pixbuf_loader_get_pixbuf(_SELF(self)));
}

/* Creating animation */
static VALUE
rg_animation(VALUE self)
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
    RG_DEF_METHOD(initialize, -1);

    rb_undef_method(RG_TARGET_NAMESPACE, "dup");
#if RBGDK_PIXBUF_CHECK_VERSION(2,2,0)
    RG_DEF_METHOD(format, 0);
#endif
    RG_DEF_METHOD(write, 1);
    RG_DEF_METHOD(last_write, 1);
#if RBGDK_PIXBUF_CHECK_VERSION(2,2,0)
    RG_DEF_METHOD(set_size, 2);
#endif
    RG_DEF_METHOD(close, 0);
    RG_DEF_METHOD(pixbuf, 0);
    RG_DEF_METHOD(animation, 0);
}