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
#define RVAL2GDKDISPLAY(o)                 (GDK_DISPLAY(RVAL2GOBJ(o)))
#define RVAL2GDKDISPLAYMANAGER(o)          (GDK_DISPLAY_MANAGER(RVAL2GOBJ(o)))
#define RVAL2GDKDEVICEMANAGER(o)           (GDK_DEVICE_MANAGER(RVAL2GOBJ(o)))
#define RVAL2GDKDISPLAYOBJECT(o)           (GDK_DISPLAY_OBJECT(RVAL2GOBJ(o)))
#define RVAL2GDKDRAGCONTEXT(o)             (GDK_DRAG_CONTEXT(RVAL2GOBJ(o)))
#define RVAL2GDKDRAWABLE(o)                (GDK_DRAWABLE(RVAL2GOBJ(o)))
#define RVAL2GDKKEYMAP(o)                  (GDK_KEYMAP(RVAL2GOBJ(o)))
#define RVAL2GDKPANGORENDERER(o)           (GDK_PANGO_RENDERER(RVAL2GOBJ(o)))
#define RVAL2GDKPIXBUF(o)                  (GDK_PIXBUF(RVAL2GOBJ(o)))
#define RVAL2GDKSCREEN(o)                  (GDK_SCREEN(RVAL2GOBJ(o)))
#define RVAL2GDKVISUAL(o)                  (GDK_VISUAL(RVAL2GOBJ(o)))
#define RVAL2GDKWINDOW(o)                  (GDK_WINDOW(RVAL2GOBJ(o)))
#define RVAL2GDKX11WINDOW(o)               (GDK_X11_WINDOW(RVAL2GOBJ(o)))

#define RVAL2GDKATOM(o)                    ((GdkAtomData*)RVAL2BOXED(o, GDK_TYPE_ATOM))
#define GDKATOM2RVAL(o)                    (BOXED2RVAL(o, GDK_TYPE_ATOM))
#define RVAL2GDKCOLOR(o)                   ((GdkColor*)RVAL2BOXED(o, GDK_TYPE_COLOR))
#define GDKCOLOR2RVAL(o)                   (BOXED2RVAL(o, GDK_TYPE_COLOR))
#define RVAL2GDKCURSOR(o)                  ((GdkCursor*)RVAL2BOXED(o, GDK_TYPE_CURSOR))
#define GDKCURSOR2RVAL(o)                  (BOXED2RVAL(o, GDK_TYPE_CURSOR))
#define RVAL2GDKEVENT(o)                   (rbgdk_rval2gdkevent(o))
#define GDKEVENT2RVAL(o)                   (rbgdk_gdkevent2rval(o))
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

#define RVAL2GDKEVENTKEY(o)                ((GdkEventKey*)RVAL2GDKEVENT(o))

#define RVAL2GDKAXISUSE(o)                 (RVAL2GENUM(o, GDK_TYPE_AXIS_USE))
#define GDKAXISUSE2RVAL(o)                 (GENUM2RVAL(o, GDK_TYPE_AXIS_USE))
#define RVAL2GDKBYTEORDER(o)               (RVAL2GENUM(o, GDK_TYPE_BYTE_ORDER))
#define GDKBYTEORDER2RVAL(o)               (GENUM2RVAL(o, GDK_TYPE_BYTE_ORDER))
#define RVAL2GDKCURSORTYPE(o)              (RVAL2GENUM(o, GDK_TYPE_CURSOR_TYPE))
#define GDKCURSORTYPE2RVAL(o)              (GENUM2RVAL(o, GDK_TYPE_CURSOR_TYPE))
#define RVAL2GDKDRAGPROTOCOL(o)            (RVAL2GENUM(o, GDK_TYPE_DRAG_PROTOCOL))
#define GDKDRAGPROTOCOL2RVAL(o)            (GENUM2RVAL(o, GDK_TYPE_DRAG_PROTOCOL))
#define RVAL2GDKEVENTTYPE(o)               (RVAL2GENUM(o, GDK_TYPE_EVENT_TYPE))
#define GDKEVENTTYPE2RVAL(o)               (GENUM2RVAL(o, GDK_TYPE_EVENT_TYPE))
#define RVAL2GDKFILTERRETURN(o)            (RVAL2GENUM(o, GDK_TYPE_FILTER_RETURN))
#define GDKFILTERRETURN2RVAL(o)            (GENUM2RVAL(o, GDK_TYPE_FILTER_RETURN))
#define RVAL2GDKGRABSTATUS(o)              (RVAL2GENUM(o, GDK_TYPE_GRAB_STATUS))
#define GDKGRABSTATUS2RVAL(o)              (GENUM2RVAL(o, GDK_TYPE_GRAB_STATUS))
#define RVAL2GDKGRAVITY(o)                 (RVAL2GENUM(o, GDK_TYPE_GRAVITY))
#define GDKGRAVITY2RVAL(o)                 (GENUM2RVAL(o, GDK_TYPE_GRAVITY))
#define RVAL2GDKINPUTMODE(o)               (RVAL2GENUM(o, GDK_TYPE_INPUT_MODE))
#define GDKINPUTMODE2RVAL(o)               (GENUM2RVAL(o, GDK_TYPE_INPUT_MODE))
#define RVAL2GDKINPUTSOURCE(o)             (RVAL2GENUM(o, GDK_TYPE_INPUT_SOURCE))
#define GDKINPUTSOURCE2RVAL(o)             (GENUM2RVAL(o, GDK_TYPE_INPUT_SOURCE))
#define RVAL2GDKGRABOWNERSHIP(o)           (RVAL2GENUM(o, GDK_TYPE_GRAB_OWNERSHIP))
#define GDKGRABOWNERSHIP2RVAL(o)           (GENUM2RVAL(o, GDK_TYPE_GRAB_OWNERSHIP))
#define RVAL2GDKPROPMODE(o)                (RVAL2GENUM(o, GDK_TYPE_PROP_MODE))
#define GDKPROPMODE2RVAL(o)                (GENUM2RVAL(o, GDK_TYPE_PROP_MODE))
#define RVAL2GDKRGBDITHER(o)               (RVAL2GENUM(o, GDK_TYPE_RGB_DITHER))
#define GDKRGBDITHER2RVAL(o)               (GENUM2RVAL(o, GDK_TYPE_RGB_DITHER))
#define RVAL2GDKVISUALTYPE(o)              (RVAL2GENUM(o, GDK_TYPE_VISUAL_TYPE))
#define GDKVISUALTYPE2RVAL(o)              (GENUM2RVAL(o, GDK_TYPE_VISUAL_TYPE))
#define RVAL2GDKWINDOWEDGE(o)              (RVAL2GENUM(o, GDK_TYPE_WINDOW_EDGE))
#define GDKWINDOWEDGE2RVAL(o)              (GENUM2RVAL(o, GDK_TYPE_WINDOW_EDGE))
#define RVAL2GDKWINDOWSTATE(o)             (RVAL2GENUM(o, GDK_TYPE_WINDOW_STATE))
#define GDKWINDOWSTATE2RVAL(o)             (GENUM2RVAL(o, GDK_TYPE_WINDOW_STATE))
#define RVAL2GDKWINDOWTYPE(o)              (RVAL2GENUM(o, GDK_TYPE_WINDOW_TYPE))
#define GDKWINDOWTYPE2RVAL(o)              (GENUM2RVAL(o, GDK_TYPE_WINDOW_TYPE))
#define RVAL2GDKWINDOWHINT(o)              (RVAL2GENUM(o, GDK_TYPE_WINDOW_TYPE_HINT))
#define GDKWINDOWHINT2RVAL(o)              (GENUM2RVAL(o, GDK_TYPE_WINDOW_TYPE_HINT))
#define RVAL2GDKWINDOWWINDOWCLASS(o)       (RVAL2GENUM(o, GDK_TYPE_WINDOW_WINDOW_CLASS))
#define GDKWINDOWWINDOWCLASS2RVAL(o)       (GENUM2RVAL(o, GDK_TYPE_WINDOW_WINDOW_CLASS))

#define RVAL2GDKDRAGACTION(o)              (RVAL2GFLAGS(o, GDK_TYPE_DRAG_ACTION))
#define GDKDRAGACTION2RVAL(o)              (GFLAGS2RVAL(o, GDK_TYPE_DRAG_ACTION))
#define RVAL2GDKEVENTMASK(o)               (RVAL2GFLAGS(o, GDK_TYPE_EVENT_MASK))
#define GDKEVENTMASK2RVAL(o)               (GFLAGS2RVAL(o, GDK_TYPE_EVENT_MASK))
#define RVAL2GDKMODIFIERTYPE(o)            (RVAL2GFLAGS(o, GDK_TYPE_MODIFIER_TYPE))
#define GDKMODIFIERTYPE2RVAL(o)            (GFLAGS2RVAL(o, GDK_TYPE_MODIFIER_TYPE))
#define RVAL2GDKWINDOWHINTS(o)             (RVAL2GFLAGS(o, GDK_TYPE_WINDOW_HINTS))
#define GDKWINDOWHINTS2RVAL(o)             (GFLAGS2RVAL(o, GDK_TYPE_WINDOW_HINTS))
#define RVAL2GDKWMDECORATION(o)            (RVAL2GFLAGS(o, GDK_TYPE_WM_DECORATION))
#define GDKWMDECORATION2RVAL(o)            (GFLAGS2RVAL(o, GDK_TYPE_WM_DECORATION))
#define RVAL2GDKWMFUNCTION(o)              (RVAL2GFLAGS(o, GDK_TYPE_WM_FUNCTION))
#define GDKWMFUNCTION2RVAL(o)              (GFLAGS2RVAL(o, GDK_TYPE_WM_FUNCTION))
#define RVAL2GDKDRAGACTION(o)              (RVAL2GFLAGS(o, GDK_TYPE_DRAG_ACTION))
#define GDKDRAGACTION2RVAL(o)              (GFLAGS2RVAL(o, GDK_TYPE_DRAG_ACTION))
#define RVAL2GDKMODIFIERTYPE(o)            (RVAL2GFLAGS(o, GDK_TYPE_MODIFIER_TYPE))
#define GDKMODIFIERTYPE2RVAL(o)            (GFLAGS2RVAL(o, GDK_TYPE_MODIFIER_TYPE))
#define RVAL2GDKWINDOWATTRIBUTESTYPE(o)    (RVAL2GFLAGS(o, GDK_TYPE_WINDOW_ATTRIBUTES_TYPE))
#define GDKWINDOWATTRIBUTESTYPE2RVAL(o)    (GFLAGS2RVAL(o, GDK_TYPE_WINDOW_ATTRIBUTES_TYPE))
#define RVAL2GDKWINDOWHINTS(o)             (RVAL2GFLAGS(o, GDK_TYPE_WINDOW_HINTS))
#define GDKWINDOWHINTS2RVAL(o)             (GFLAGS2RVAL(o, GDK_TYPE_WINDOW_HINTS))

#endif /* __RBGDK3CONVERSIONS_H__ */
