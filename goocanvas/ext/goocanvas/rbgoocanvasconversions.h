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

#ifndef __RBGOOCANVASCONVERSIONS_H__
#define __RBGOOCANVASCONVERSIONS_H__

#define RVAL2GOOCANVAS(o)                  (GOO_CANVAS(RVAL2GOBJ(o)))
#define RVAL2GOOCANVASITEM(o)              (GOO_CANVAS_ITEM(RVAL2GOBJ(o)))
#define RVAL2GOOCANVASSTYLE(o)             (GOO_CANVAS_STYLE(RVAL2GOBJ(o)))

#define RVAL2GOOCANVASPOINTS(o)            ((GooCanvasPoints*)RVAL2BOXED(o, GOO_TYPE_CANVAS_POINTS))
#define GOOCANVASPOINTS2RVAL(o)            (BOXED2RVAL(o, GOO_TYPE_CANVAS_POINTS))

#endif /* __RBGOOCANVASCONVERSIONS_H__ */
