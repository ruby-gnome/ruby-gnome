/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2007  Vincent Isambart <vincent.isambart@gmail.com>
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

#ifndef __RBGOOCANVAS_H__
#define __RBGOOCANVAS_H__

#include <rb_cairo.h>
#include <rbgtk.h>
#include <goocanvas.h>

#include "rbgoo_canvasversion.h"
#include "rbgoocanvasconversions.h"

#define RVAL2GC(obj) GOO_CANVAS(RVAL2GOBJ(obj))
#define RVAL2GCI(obj) GOO_CANVAS_ITEM(RVAL2GOBJ(obj))
#define RVAL2GCS(obj) GOO_CANVAS_STYLE(RVAL2GOBJ(obj))

#define RVAL2GCPOINTS(obj) \
    ((GooCanvasPoints *)(RVAL2BOXED(obj, GOO_TYPE_CANVAS_POINTS)))

#define RVAL2GCBOUNDS(obj, bounds)              \
    (ruby_to_goo_canvas_bounds(obj, bounds))

#define RVAL2GTK_WIDGET(obj) GTK_WIDGET(RVAL2GOBJ(obj))

#define RVAL2GDK_PIXBUF(obj) GDK_PIXBUF(RVAL2GOBJ(obj))

#define RB_GOO_CANVAS_ITEM_INITIALIZE(obj, item) \
    (rb_goo_canvas_initialize_item_object(obj, GOO_CANVAS_ITEM(item)))

void rb_goo_canvas_initialize_item_object(VALUE obj, GooCanvasItem *item);
GooCanvasBounds *ruby_to_goo_canvas_bounds(VALUE rb_bounds, GooCanvasBounds *dest_bounds);

extern VALUE mGoo;

#endif /* __RBGOOCANVAS_H__ */
