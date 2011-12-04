/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2002-2004 Masao Mutoh
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

#define GDK_PIXBUF_ENABLE_BACKEND
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <gdk-pixbuf/gdk-pixdata.h>
#include "rbgobject.h"
#include "gdk_pixbuf2conversions.h"

G_GNUC_INTERNAL void Init_gdk_pixbuf_animation(VALUE mGLib);
G_GNUC_INTERNAL void Init_gdk_pixbuf_animation_iter(VALUE mGLib);
G_GNUC_INTERNAL void Init_gdk_pixdata(VALUE mGLib);
G_GNUC_INTERNAL void Init_gdk_pixbuf_loader(VALUE mGLib);
G_GNUC_INTERNAL void Init_gdk_pixbuf_format(VALUE mGLib);
extern GType gdk_pixbuf_format_get_type(void);

#define RBGDK_PIXBUF_CHECK_VERSION(major,minor,micro)    \
    (GDK_PIXBUF_MAJOR > (major) || \
     (GDK_PIXBUF_MAJOR == (major) && GDK_PIXBUF_MINOR > (minor)) || \
     (GDK_PIXBUF_MAJOR == (major) && GDK_PIXBUF_MINOR == (minor) && \
      GDK_PIXBUF_MICRO >= (micro)))

#if RBGDK_PIXBUF_CHECK_VERSION(2,8,0)
G_GNUC_INTERNAL void Init_gdk_pixbuf_simpleanim(VALUE mGLib);
#endif

#define GDK_TYPE_PIXBUF_FORMAT (gdk_pixbuf_format_get_type())
