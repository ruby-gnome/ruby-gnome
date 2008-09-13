/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/* $Id$ */
/* GooCanvasRect
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
rb_goo_canvas_rect_new(VALUE self, VALUE parent, VALUE x, VALUE y,
                       VALUE width, VALUE height)
{
    GooCanvasItem *item;
    item = goo_canvas_rect_new(RVAL2GCI(parent),
                               NUM2DBL(x),
                               NUM2DBL(y),
                               NUM2DBL(width),
                               NUM2DBL(height),
                               NULL);
    RB_GOO_CANVAS_ITEM_INITIALIZE(self, item);
    G_CHILD_ADD(parent, self);
    return Qnil;
}

void
Init_goocanvasrect(void)
{
    VALUE GooCanvasRect;

    GooCanvasRect = G_DEF_CLASS(GOO_TYPE_CANVAS_RECT, "CanvasRect", mGoo);

    rb_define_method(GooCanvasRect, "initialize", rb_goo_canvas_rect_new, 5);
}
