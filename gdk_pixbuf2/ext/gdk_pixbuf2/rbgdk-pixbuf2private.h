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

#ifndef __RBGDK_PIXBUF2PRIVATE_H__
#define __RBGDK_PIXBUF2PRIVATE_H__

#include "rbgdk-pixbuf.h"

G_GNUC_INTERNAL void Init_gdk_pixbuf_animation(VALUE mGLib);
G_GNUC_INTERNAL void Init_gdk_pixbuf_animation_iter(VALUE mGLib);
G_GNUC_INTERNAL void Init_gdk_pixbuf_format(VALUE mGLib);
G_GNUC_INTERNAL void Init_gdk_pixbuf_loader(VALUE mGLib);
#if RBGDK_PIXBUF_CHECK_VERSION(2,8,0)
G_GNUC_INTERNAL void Init_gdk_pixbuf_simpleanim(VALUE mGLib);
#endif
G_GNUC_INTERNAL void Init_gdk_pixdata(VALUE mGLib);

#endif /* __RBGDK_PIXBUF2PRIVATE_H__ */
