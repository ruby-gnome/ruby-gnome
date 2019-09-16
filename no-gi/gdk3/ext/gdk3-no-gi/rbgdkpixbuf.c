/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011-2014  Ruby-GNOME2 Project Team
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

#include "rbgdk3private.h"

#define RG_TARGET_NAMESPACE cPixbuf
#define _SELF(s) RVAL2GDKPIXBUF(s)

static VALUE
rg_s_from_window(int argc, VALUE *argv, G_GNUC_UNUSED VALUE self)
{
    VALUE window, src_x, src_y, width, height;
    GdkPixbuf *pixbuf;

    rb_scan_args(argc, argv, "50", &window, &src_x, &src_y, &width, &height);

    pixbuf = gdk_pixbuf_get_from_window(RVAL2GDKWINDOW(window),
                                        NUM2INT(src_x), NUM2INT(src_y),
                                        NUM2INT(width), NUM2INT(height));
    return GOBJ2RVAL_UNREF(pixbuf);
}

static VALUE
rg_s_from_surface(int argc, VALUE *argv, G_GNUC_UNUSED VALUE self)
{
    VALUE surface, src_x, src_y, width, height;
    GdkPixbuf *pixbuf;

    rb_scan_args(argc, argv, "50", &surface, &src_x, &src_y, &width, &height);

    pixbuf = gdk_pixbuf_get_from_surface(RVAL2CRSURFACE(surface),
                                         NUM2INT(src_x), NUM2INT(src_y),
                                         NUM2INT(width), NUM2INT(height));
    return GOBJ2RVAL_UNREF(pixbuf);
}

void
Init_gdk_pixbuf(VALUE mGdk)
{
    /*
     *  This defines Gdk::Pixbuf methods of GDK side.
     */
    VALUE RG_TARGET_NAMESPACE = GTYPE2CLASS(GDK_TYPE_PIXBUF);

    RG_DEF_SMETHOD(from_window, -1);
    RG_DEF_SMETHOD(from_surface, -1);
}
