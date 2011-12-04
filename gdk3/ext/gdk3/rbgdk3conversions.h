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
#define RVAL2GDKDRAGCONTEXT(o)             (GDK_DRAG_CONTEXT(RVAL2GOBJ(o)))
#define RVAL2GDKPANGORENDERER(o)           (GDK_PANGO_RENDERER(RVAL2GOBJ(o)))
#define RVAL2GDKVISUAL(o)                  (GDK_VISUAL(RVAL2GOBJ(o)))
#define RVAL2GDKDISPLAYMANAGER(o)          (GDK_DISPLAY_MANAGER(RVAL2GOBJ(o)))
#define RVAL2GDKDISPLAYOBJECT(o)           (GDK_DISPLAY_OBJECT(RVAL2GOBJ(o)))
#define RVAL2GDKDRAWABLE(o)                (GDK_DRAWABLE(RVAL2GOBJ(o)))
#define RVAL2GDKKEYMAP(o)                  (GDK_KEYMAP(RVAL2GOBJ(o)))
#define RVAL2GDKPIXBUF(o)                  (GDK_PIXBUF(RVAL2GOBJ(o)))
#define RVAL2GDKSCREEN(o)                  (GDK_SCREEN(RVAL2GOBJ(o)))
#define RVAL2GDKWINDOW(o)                  (GDK_WINDOW(RVAL2GOBJ(o)))

#define RVAL2GDKATOM(o)                    ((GdkAtomData*)RVAL2BOXED(o, GDK_TYPE_ATOM))
#define GDKATOM2RVAL(o)                    (BOXED2RVAL(o, GDK_TYPE_ATOM))
#define RVAL2GDKCOLOR(o)                   ((GdkColor*)RVAL2BOXED(o, GDK_TYPE_COLOR))
#define GDKCOLOR2RVAL(o)                   (BOXED2RVAL(o, GDK_TYPE_COLOR))
#define RVAL2GDKCURSOR(o)                  ((GdkCursor*)RVAL2BOXED(o, GDK_TYPE_CURSOR))
#define GDKCURSOR2RVAL(o)                  (BOXED2RVAL(o, GDK_TYPE_CURSOR))
#define RVAL2GDKEVENT(o)                   ((GdkEvent*)RVAL2BOXED(o, GDK_TYPE_EVENT))
#define GDKEVENT2RVAL(o)                   (BOXED2RVAL(o, GDK_TYPE_EVENT))
#define RVAL2GDKGEOMETRY(o)                ((GdkGeometry*)RVAL2BOXED(o, GDK_TYPE_GEOMETRY))
#define GDKGEOMETRY2RVAL(o)                (BOXED2RVAL(o, GDK_TYPE_GEOMETRY))
#define RVAL2GDKRECTANGLE(o)               ((GdkRectangle*)RVAL2BOXED(o, GDK_TYPE_RECTANGLE))
#define GDKRECTANGLE2RVAL(o)               (BOXED2RVAL(o, GDK_TYPE_RECTANGLE))
#define RVAL2GDKRGBA(o)                    ((GdkRGBA*)RVAL2BOXED(o, GDK_TYPE_RGBA))
#define GDKRGBA2RVAL(o)                    (BOXED2RVAL(o, GDK_TYPE_RGBA))
#define RVAL2GDKTIMECOORD(o)               ((GdkTimeCoord*)RVAL2BOXED(o, GDK_TYPE_TIME_COORD))
#define GDKTIMECOORD2RVAL(o)               (BOXED2RVAL(o, GDK_TYPE_TIME_COORD))
#define RVAL2GDKWINDOWATTR(o)              ((GdkWindowAttr*)RVAL2BOXED(o, GDK_TYPE_WINDOW_ATTR))
#define GDKWINDOWATTR2RVAL(o)              (BOXED2RVAL(o, GDK_TYPE_WINDOW_ATTR))

#endif /* __RBGDK3CONVERSIONS_H__ */
