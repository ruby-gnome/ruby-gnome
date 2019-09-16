/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
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

#ifndef __RBGDK_PIXBUF2CONVERSIONS_H__
#define __RBGDK_PIXBUF2CONVERSIONS_H__

#define RVAL2GDKPIXBUF(o)                  (GDK_PIXBUF(RVAL2GOBJ(o)))
#define RVAL2GDKPIXBUFANIMATION(o)         (GDK_PIXBUF_ANIMATION(RVAL2GOBJ(o)))
#define RVAL2GDKPIXBUFANIMATIONITER(o)     (GDK_PIXBUF_ANIMATION_ITER(RVAL2GOBJ(o)))
#define RVAL2GDKPIXBUFLOADER(o)            (GDK_PIXBUF_LOADER(RVAL2GOBJ(o)))
#define RVAL2GDKPIXBUFSIMPLEANIM(o)        (GDK_PIXBUF_SIMPLE_ANIM(RVAL2GOBJ(o)))

#define RVAL2GDKPIXBUFFORMAT(o)            ((GdkPixbufFormat*)RVAL2BOXED(o, GDK_TYPE_PIXBUF_FORMAT))
#define GDKPIXBUFFORMAT2RVAL(o)            (BOXED2RVAL(o, GDK_TYPE_PIXBUF_FORMAT))
#define RVAL2GDKPIXDATA(o)                 ((GdkPixdata*)RVAL2BOXED(o, GDK_TYPE_PIXDATA))
#define GDKPIXDATA2RVAL(o)                 (BOXED2RVAL(o, GDK_TYPE_PIXDATA))

#define RVAL2GDKCOLORSPACE(o)              (RVAL2GENUM(o, GDK_TYPE_COLORSPACE))
#define GDKCOLORSPACE2RVAL(o)              (GENUM2RVAL(o, GDK_TYPE_COLORSPACE))
#define RVAL2GDKINTERPTYPE(o)              (RVAL2GENUM(o, GDK_TYPE_INTERP_TYPE))
#define GDKINTERPTYPE2RVAL(o)              (GENUM2RVAL(o, GDK_TYPE_INTERP_TYPE))
#define RVAL2GDKPIXBUFROTATION(o)          (RVAL2GENUM(o, GDK_TYPE_PIXBUF_ROTATION))
#define GDKPIXBUFROTATION2RVAL(o)          (GENUM2RVAL(o, GDK_TYPE_PIXBUF_ROTATION))

#endif /* __RBGDK_PIXBUF2CONVERSIONS_H__ */
