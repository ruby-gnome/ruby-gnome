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

#define RG_TARGET_NAMESPACE cPixbufAnimationIter
#define _SELF(i) (RVAL2GDKPIXBUFANIMATIONITER(i))

static VALUE
rg_advance(int argc, VALUE *argv, VALUE self)
{
    VALUE current_time_sec, current_time_usec;
    GTimeVal current_time;

    rb_scan_args(argc, argv, "02", &current_time_sec, &current_time_usec);

    if (NIL_P(current_time_sec))
        return CBOOL2RVAL(gdk_pixbuf_animation_iter_advance(_SELF(self), NULL));

    current_time.tv_sec = NUM2LONG(current_time_sec);
    current_time.tv_usec = NIL_P(current_time_usec) ? 0 : NUM2LONG(current_time_usec);

    return CBOOL2RVAL(gdk_pixbuf_animation_iter_advance(_SELF(self), &current_time));
}

static VALUE
rg_delay_time(VALUE self)
{
    return INT2NUM(gdk_pixbuf_animation_iter_get_delay_time(_SELF(self)));
}

static VALUE
rg_on_currently_loading_frame_p(VALUE self)
{
    return CBOOL2RVAL(gdk_pixbuf_animation_iter_on_currently_loading_frame(_SELF(self)));
}

static VALUE
rg_pixbuf(VALUE self)
{
    return GOBJ2RVAL(gdk_pixbuf_animation_iter_get_pixbuf(_SELF(self)));
}

void
Init_gdk_pixbuf_animation_iter(VALUE mGdk)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GDK_TYPE_PIXBUF_ANIMATION_ITER, "PixbufAnimationIter", mGdk);    

    RG_DEF_METHOD(advance, -1);
    RG_DEF_METHOD(delay_time, 0);
    RG_DEF_METHOD_P(on_currently_loading_frame, 0);
    RG_DEF_METHOD(pixbuf, 0);
}
