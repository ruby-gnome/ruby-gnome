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

#ifndef __RB_GDK_CONVERSIONS_H__
#define __RB_GDK_CONVERSIONS_H__

#include <rbgobject.h>
#include <gdk/gdk.h>

#define RVAL2GDKCOLOR(color) ((GdkColor *)RVAL2BOXED(color, GDK_TYPE_COLOR))
#define GDKCOLOR2RVAL(color) (BOXED2RVAL(color, GDK_TYPE_COLOR))

#endif /* __RB_GDK_CONVERSIONS_H__ */
