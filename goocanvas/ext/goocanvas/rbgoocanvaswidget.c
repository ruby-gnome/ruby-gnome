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

#include "rbgoocanvasprivate.h"

#define RG_TARGET_NAMESPACE cCanvasWidget

static VALUE
rg_initialize(VALUE self, VALUE parent, VALUE widget,
                         VALUE x, VALUE y, VALUE width, VALUE height)
{
    GooCanvasItem *item;
    item = goo_canvas_widget_new(RVAL2GCI(parent),
                                 RVAL2GTK_WIDGET(widget),
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
Init_goocanvaswidget(VALUE mGoo)
{
    VALUE RG_TARGET_NAMESPACE;

    RG_TARGET_NAMESPACE = G_DEF_CLASS(GOO_TYPE_CANVAS_WIDGET, "CanvasWidget", mGoo);

    RG_DEF_METHOD(initialize, 6);
}
