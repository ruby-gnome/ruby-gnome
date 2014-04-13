/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2008  Ruby-GNOME2 Project Team
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

#ifndef __RBGDK3CONVERSIONS_H__
#define __RBGDK3CONVERSIONS_H__

#define RVAL2GDKDEVICE(o)                  (GDK_DEVICE(RVAL2GOBJ(o)))
#define RVAL2GDKDISPLAYOBJECT(o)           (GDK_DISPLAY_OBJECT(RVAL2GOBJ(o)))
#define RVAL2GDKVISUAL(o)                  (GDK_VISUAL(RVAL2GOBJ(o)))
#define RVAL2GDKATOM(o)                    ((GdkAtomData*)RVAL2BOXED(o, GDK_TYPE_ATOM))
#define GDKATOM2RVAL(o)                    (BOXED2RVAL(o, GDK_TYPE_ATOM))
#define RVAL2GDKCOLOR(o)                   ((GdkColor*)RVAL2BOXED(o, GDK_TYPE_COLOR))
#define GDKCOLOR2RVAL(o)                   (BOXED2RVAL(o, GDK_TYPE_COLOR))
#define RVAL2GDKEVENT(o)                   ((GdkEvent *)RVAL2BOXED(o, GDK_TYPE_EVENT))
#define GDKEVENT2RVAL(o)                   (BOXED2RVAL(o, GDK_TYPE_EVENT))
#define RVAL2GDKRECTANGLE(o)               ((GdkRectangle*)RVAL2BOXED(o, CAIRO_GOBJECT_TYPE_RECTANGLE_INT))
#define GDKRECTANGLE2RVAL(o)               (BOXED2RVAL(o, CAIRO_GOBJECT_TYPE_RECTANGLE_INT))
#define RVAL2GDKMODIFIERTYPE(o)            (RVAL2GFLAGS(o, GDK_TYPE_MODIFIER_TYPE))
#define GDKMODIFIERTYPE2RVAL(o)            (GFLAGS2RVAL(o, GDK_TYPE_MODIFIER_TYPE))
#define RVAL2GDKDRAGCONTEXT(o)             (GDK_DRAG_CONTEXT(RVAL2GOBJ(o)))
#define RVAL2GDKEVENTKEY(o)                ((GdkEventKey*)RVAL2GDKEVENT(o))
#define RVAL2GDKGEOMETRY(o)                ((GdkGeometry*)RVAL2BOXED(o, GDK_TYPE_GEOMETRY))
#define GDKGEOMETRY2RVAL(o)                (BOXED2RVAL(o, GDK_TYPE_GEOMETRY))
#define RVAL2GDKRGBA(o)                    ((GdkRGBA*)RVAL2BOXED(o, GDK_TYPE_RGBA))
#define GDKRGBA2RVAL(o)                    (BOXED2RVAL(o, GDK_TYPE_RGBA))
#define RVAL2GDKWINDOW(o)                  (GDK_WINDOW(RVAL2GOBJ(o)))
#define RVAL2GDKDRAGACTION(o)              (RVAL2GFLAGS(o, GDK_TYPE_DRAG_ACTION))
#define GDKDRAGACTION2RVAL(o)              (GFLAGS2RVAL(o, GDK_TYPE_DRAG_ACTION))
#define RVAL2GDKEVENTMASK(o)               (RVAL2GFLAGS(o, GDK_TYPE_EVENT_MASK))
#define GDKEVENTMASK2RVAL(o)               (GFLAGS2RVAL(o, GDK_TYPE_EVENT_MASK))
#define RVAL2GDKSCREEN(o)                  (GDK_SCREEN(RVAL2GOBJ(o)))
#define RVAL2GDKDRAGPROTOCOL(o)            (RVAL2GENUM(o, GDK_TYPE_DRAG_PROTOCOL))
#define GDKDRAGPROTOCOL2RVAL(o)            (GENUM2RVAL(o, GDK_TYPE_DRAG_PROTOCOL))
#define RVAL2GDKWINDOWEDGE(o)              (RVAL2GENUM(o, GDK_TYPE_WINDOW_EDGE))
#define GDKWINDOWEDGE2RVAL(o)              (GENUM2RVAL(o, GDK_TYPE_WINDOW_EDGE))
#define RVAL2GDKWINDOWHINTS(o)             (RVAL2GFLAGS(o, GDK_TYPE_WINDOW_HINTS))
#define GDKWINDOWHINTS2RVAL(o)             (GFLAGS2RVAL(o, GDK_TYPE_WINDOW_HINTS))

#endif /* __RBGDK3CONVERSIONS_H__ */
