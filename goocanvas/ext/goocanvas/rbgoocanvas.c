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

#define RG_TARGET_NAMESPACE cCanvas
#define SELF(self) RVAL2GC(self)

void
rb_goo_canvas_initialize_item_object(VALUE obj, GooCanvasItem *item)
{
    g_object_ref_sink(item);
    G_INITIALIZE(obj, item);
}

static VALUE
rg_initialize(VALUE self)
{
    RBGTK_INITIALIZE(self, goo_canvas_new());
    return Qnil;
}

static VALUE
rg_set_bounds(VALUE self, VALUE left, VALUE top,
                         VALUE right, VALUE bottom)
{
    goo_canvas_set_bounds(SELF(self), NUM2DBL(left), NUM2DBL(top),
                          NUM2DBL(right), NUM2DBL(bottom));
    return self;
}

static VALUE
rg_root_item(VALUE self)
{
    VALUE root;

    root = GOBJ2RVAL(goo_canvas_get_root_item(SELF(self)));
    G_CHILD_ADD(self, root);

    return root;
}

static VALUE
rg_grab_focus(int argc, VALUE *argv, VALUE self)
{
    VALUE item;

    if (rb_scan_args(argc, argv, "01", &item) == 1) {
        goo_canvas_grab_focus(SELF(self), RVAL2GCI(item));
    } else {
        rb_call_super(0, 0);
    }

    return self;
}

static VALUE
rg_pointer_grab(VALUE self, VALUE item, VALUE event_mask, VALUE cursor, VALUE etime)
{
    return GENUM2RVAL(
        goo_canvas_pointer_grab(SELF(self), RVAL2GCI(item),
                                NUM2INT(event_mask),
                                (GdkCursor *)RVAL2BOXED(cursor, GDK_TYPE_CURSOR),
                                NIL_P(etime) ? 0 : NUM2UINT(etime)),
        GDK_TYPE_GRAB_STATUS);
}

static VALUE
rg_pointer_ungrab(VALUE self, VALUE item, VALUE etime)
{
    goo_canvas_pointer_ungrab(SELF(self), RVAL2GCI(item),
                              NIL_P(etime) ? 0 : NUM2UINT(etime));
    return self;
}

static VALUE
rg_render(VALUE self, VALUE cr, VALUE rb_bounds, VALUE scale)
{
    GooCanvasBounds bounds;

    goo_canvas_render(SELF(self), RVAL2CRCONTEXT(cr),
                      RVAL2GCBOUNDS(rb_bounds, &bounds),
                      NUM2DBL(scale));
    return self;
}

static VALUE
rg_scroll_to(VALUE self, VALUE left, VALUE top)
{
    goo_canvas_scroll_to(SELF(self), NUM2DBL(left), NUM2DBL(top));
    return self;
}

void
Init_goocanvas(void)
{
    Init_goo();

    VALUE RG_TARGET_NAMESPACE;

    RG_TARGET_NAMESPACE = G_DEF_CLASS(GOO_TYPE_CANVAS, "Canvas", mGoo);
    VALUE mCairo = rb_const_get(rb_mKernel, rb_intern("Cairo"));

    RG_DEF_METHOD(initialize, 0);
    RG_DEF_METHOD(set_bounds, 4);
    RG_DEF_METHOD(root_item, 0);
    RG_DEF_METHOD(grab_focus, -1);
    RG_DEF_METHOD(pointer_grab, 4);
    RG_DEF_METHOD(pointer_ungrab, 2);
    RG_DEF_METHOD(render, 3);
    RG_DEF_METHOD(scroll_to, 2);

    Init_goocanvasitem(mGoo); /* Goo::CanvasItem */
    Init_goocanvastext(mGoo); /* Goo::CanvasText */
    Init_goocanvasrect(mGoo); /* Goo::CanvasRect */
    Init_goocanvasellipse(mGoo); /* Goo::CanvasEllipse */
    Init_goocanvaspolyline(mGoo); /* Goo::CanvasPolyline */
    Init_goocanvaspoints(mGoo); /* Goo::CanvasPoints */
    Init_goocanvasimage(mGoo); /* Goo::CanvasImage */
    Init_goocanvastable(mGoo); /* Goo::CanvasTable */
    Init_goocanvaswidget(mGoo); /* Goo::CanvasWidget */
    Init_goocanvasstyle(mGoo); /* Goo::CanvasStyle */
    Init_goocanvasgroup(mGoo); /* Goo::CanvasGroup */
    Init_goocairopattern(mCairo);
    Init_goocairomatrix(mCairo);
}
