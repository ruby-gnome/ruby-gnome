/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2002,2003 the ruby-gnome2 project
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

#if RBGDK_PIXBUF_CHECK_VERSION(2,8,0)
#define RG_TARGET_NAMESPACE cPixbufSimpleAnim
#define _SELF(s) (RVAL2GDKPIXBUFSIMPLEANIM(s))

static VALUE
rg_initialize(VALUE self, VALUE width, VALUE height, VALUE rate)
{
    GdkPixbufSimpleAnim* ret = gdk_pixbuf_simple_anim_new(NUM2INT(width), NUM2INT(height), NUM2DBL(rate));
    G_INITIALIZE(self, ret);
    return Qnil;
}

static VALUE
rg_add_frame(VALUE self, VALUE pixbuf)
{
    gdk_pixbuf_simple_anim_add_frame(_SELF(self), RVAL2GDKPIXBUF(pixbuf));
    return self;
}
#endif

void
Init_gdk_pixbuf_simpleanim(VALUE mGdk)
{
#if RBGDK_PIXBUF_CHECK_VERSION(2,8,0)
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GDK_TYPE_PIXBUF_SIMPLE_ANIM, "PixbufSimpleAnim", mGdk);    

    RG_DEF_METHOD(initialize, 3);
    RG_DEF_METHOD(add_frame, 1);
#endif
}
