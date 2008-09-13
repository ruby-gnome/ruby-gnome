/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/* $Id$ */
/* GooCanvasEllipse
 * Copyright (C) 2007 Vincent Isambart <vincent.isambart@gmail.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "rbgoocanvas.h"

static VALUE
rb_goo_canvas_ellipse_new(VALUE self, VALUE parent,
                          VALUE center_x, VALUE center_y,
                          VALUE radius_x, VALUE radius_y)
{
    GooCanvasItem *item;
    item = goo_canvas_ellipse_new(RVAL2GCI(parent),
                                  NUM2DBL(center_x),
                                  NUM2DBL(center_y),
                                  NUM2DBL(radius_x),
                                  NUM2DBL(radius_y),
                                  NULL);
    RB_GOO_CANVAS_ITEM_INITIALIZE(self, item);
    G_CHILD_ADD(parent, self);
    return Qnil;
}

void
Init_goocanvasellipse(void)
{
    VALUE GooCanvasEllipse;

    GooCanvasEllipse = G_DEF_CLASS(GOO_TYPE_CANVAS_ELLIPSE, "CanvasEllipse",
                                   mGoo);

    rb_define_method(GooCanvasEllipse, "initialize",
                     rb_goo_canvas_ellipse_new, 5);
}
