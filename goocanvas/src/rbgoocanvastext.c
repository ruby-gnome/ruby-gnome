/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/* $Id$ */
/* GooCanvasText
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
rb_goo_canvas_text_new(VALUE self, VALUE parent, VALUE string,
                       VALUE x, VALUE y, VALUE width, VALUE anchor)
{
    GooCanvasItem *item;
    item = goo_canvas_text_new(RVAL2GCI(parent),
                               NIL_P(string) ? NULL : StringValueCStr(string),
                               NUM2DBL(x),
                               NUM2DBL(y),
                               NUM2DBL(width),
                               RVAL2GENUM(anchor, GTK_TYPE_ANCHOR_TYPE),
                               NULL);
    RB_GOO_CANVAS_ITEM_INITIALIZE(self, item);
    G_CHILD_ADD(parent, self);
    return Qnil;
}

void
Init_goocanvastext(void)
{
    VALUE GooCanvasText;

    GooCanvasText = G_DEF_CLASS(GOO_TYPE_CANVAS_TEXT, "CanvasText", mGoo);

    rb_define_method(GooCanvasText, "initialize", rb_goo_canvas_text_new, 6);

#if 0
   GooCanvasItemModel* goo_canvas_text_model_new (GooCanvasItemModel *parent,
   					       const char         *string,
   					       gdouble             x,
   					       gdouble             y,
   					       gdouble             width,
   					       GtkAnchorType       anchor,
   					       ...);
#endif
}
