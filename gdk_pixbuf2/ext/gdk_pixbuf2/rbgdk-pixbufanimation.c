/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2002,2003 Masao Mutoh
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

#define RG_TARGET_NAMESPACE cPixbufAnimation
#define _SELF(s) (RVAL2GDKPIXBUFANIMATION(s))

static VALUE
rg_initialize(VALUE self, VALUE filename)
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
rg_width(VALUE self)
{
    return INT2NUM(gdk_pixbuf_animation_get_width(_SELF(self)));
}

static VALUE
rg_height(VALUE self)
{
    return INT2NUM(gdk_pixbuf_animation_get_height(_SELF(self)));
}

static VALUE
rg_get_iter(int argc, VALUE *argv, VALUE self)
{
    VALUE start_time_sec, start_time_usec;
    GTimeVal start_time;

    rb_scan_args(argc, argv, "02", &start_time_sec, &start_time_usec);

    if (NIL_P(start_time_sec))
        return GOBJ2RVAL(gdk_pixbuf_animation_get_iter(_SELF(self), NULL));

    start_time.tv_sec = NUM2LONG(start_time_sec);
    start_time.tv_usec = NIL_P(start_time_usec) ? 0 : NUM2LONG(start_time_usec);

    return GOBJ2RVAL(gdk_pixbuf_animation_get_iter(_SELF(self), &start_time));
}

static VALUE
rg_static_image_p(VALUE self)
{
    return CBOOL2RVAL(gdk_pixbuf_animation_is_static_image(_SELF(self)));
}

static VALUE
rg_static_image(VALUE self)
{
    return GOBJ2RVAL(gdk_pixbuf_animation_get_static_image(_SELF(self)));
}

void
Init_gdk_pixbuf_animation(VALUE mGdk)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GDK_TYPE_PIXBUF_ANIMATION, "PixbufAnimation", mGdk);    

    RG_DEF_METHOD(initialize, 1);
    RG_DEF_METHOD(width, 0);
    RG_DEF_METHOD(height, 0);
    RG_DEF_METHOD(get_iter, -1);
    RG_DEF_METHOD_P(static_image, 0);
    RG_DEF_METHOD(static_image, 0);
}
