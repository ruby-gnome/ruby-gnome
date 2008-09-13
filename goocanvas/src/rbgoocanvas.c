/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/* $Id$ */
/* GooCanvas initialization
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

VALUE mGoo;

#define SELF(self) RVAL2GC(self)

void
rb_goo_canvas_initialize_item_object(VALUE obj, GooCanvasItem *item)
{
    g_object_ref_sink(item);
    G_INITIALIZE(obj, item);
}

static VALUE
rb_goo_canvas_new(VALUE self)
{
    RBGTK_INITIALIZE(self, goo_canvas_new());
    return Qnil;
}

static VALUE
rb_goo_canvas_set_bounds(VALUE self, VALUE left, VALUE top,
                         VALUE right, VALUE bottom)
{
    goo_canvas_set_bounds(SELF(self), NUM2DBL(left), NUM2DBL(top),
                          NUM2DBL(right), NUM2DBL(bottom));
    return self;
}

static VALUE
rb_goo_canvas_get_root_item(VALUE self)
{
    VALUE root;

    root = GOBJ2RVAL(goo_canvas_get_root_item(SELF(self)));
    G_CHILD_ADD(self, root);

    return root;
}

static VALUE
rb_goo_canvas_grab_focus(int argc, VALUE *argv, VALUE self)
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
rb_goo_canvas_pointer_grab(VALUE self, VALUE item, VALUE event_mask, VALUE cursor, VALUE etime)
{
    return GENUM2RVAL(
        goo_canvas_pointer_grab(SELF(self), RVAL2GCI(item),
                                NUM2INT(event_mask),
                                (GdkCursor *)RVAL2BOXED(cursor, GDK_TYPE_CURSOR),
                                NIL_P(etime) ? 0 : NUM2UINT(etime)),
        GDK_TYPE_GRAB_STATUS);
}

static VALUE
rb_goo_canvas_pointer_ungrab(VALUE self, VALUE item, VALUE etime)
{
    goo_canvas_pointer_ungrab(SELF(self), RVAL2GCI(item),
                              NIL_P(etime) ? 0 : NUM2UINT(etime));
    return self;
}

static VALUE
rb_goo_canvas_render(VALUE self, VALUE cr, VALUE rb_bounds, VALUE scale)
{
    GooCanvasBounds bounds;

    goo_canvas_render(SELF(self), RVAL2CRCONTEXT(cr),
                      RVAL2GCBOUNDS(rb_bounds, &bounds),
                      NUM2DBL(scale));
    return self;
}

static VALUE
rb_goo_canvas_scroll_to(VALUE self, VALUE left, VALUE top)
{
    goo_canvas_scroll_to(SELF(self), NUM2DBL(left), NUM2DBL(top));
    return self;
}

void
Init_goocanvas(void)
{
    VALUE cGooCanvas;

    mGoo = rb_define_module("Goo");
    cGooCanvas = G_DEF_CLASS(GOO_TYPE_CANVAS, "Canvas", mGoo);

    rb_define_method(cGooCanvas, "initialize", rb_goo_canvas_new, 0);
    rb_define_method(cGooCanvas, "set_bounds", rb_goo_canvas_set_bounds, 4);
    rb_define_method(cGooCanvas, "root_item", rb_goo_canvas_get_root_item, 0);
    rb_define_method(cGooCanvas, "grab_focus", rb_goo_canvas_grab_focus, -1);
    rb_define_method(cGooCanvas, "pointer_grab", rb_goo_canvas_pointer_grab, 4);
    rb_define_method(cGooCanvas, "pointer_ungrab",
		     rb_goo_canvas_pointer_ungrab, 2);
    rb_define_method(cGooCanvas, "render", rb_goo_canvas_render, 3);
    rb_define_method(cGooCanvas, "scroll_to", rb_goo_canvas_scroll_to, 2);

    Init_goocanvasitem(); /* Goo::CanvasItem */
    Init_goocanvastext(); /* Goo::CanvasText */
    Init_goocanvasrect(); /* Goo::CanvasRect */
    Init_goocanvasellipse(); /* Goo::CanvasEllipse */
    Init_goocanvaspolyline(); /* Goo::CanvasPolyline */
    Init_goocanvasimage(); /* Goo::CanvasImage */
    Init_goocanvastable(); /* Goo::CanvasTable */
    Init_goocanvaswidget(); /* Goo::CanvasWidget */
    Init_goocanvasstyle(); /* Goo::CanvasStyle */
    Init_goocanvasgroup(); /* Goo::CanvasGroup */
    Init_goocairo(); /* conversion from Cairo types to GooCairo types */

#if 0
   GooCanvasItem*  goo_canvas_get_root_item    (GooCanvas		*canvas);
   void            goo_canvas_set_root_item    (GooCanvas		*canvas,
   					     GooCanvasItem      *item);

   GooCanvasItemModel* goo_canvas_get_root_item_model (GooCanvas	       *canvas);
   void                goo_canvas_set_root_item_model (GooCanvas	       *canvas,
   						    GooCanvasItemModel *model);

   GooCanvasItem*  goo_canvas_get_item	    (GooCanvas		*canvas,
   					     GooCanvasItemModel *model);
   GooCanvasItem*  goo_canvas_get_item_at	    (GooCanvas		*canvas,
   					     gdouble             x,
   					     gdouble             y,
   					     gboolean            is_pointer_event);

   gdouble         goo_canvas_get_scale	    (GooCanvas		*canvas);
   void            goo_canvas_set_scale	    (GooCanvas		*canvas,
   					     gdouble             scale);

   void            goo_canvas_get_bounds	    (GooCanvas		*canvas,
   					     gdouble            *left,
   					     gdouble            *top,
   					     gdouble            *right,
   					     gdouble            *bottom);


   void            goo_canvas_grab_focus	    (GooCanvas		*canvas,
   					     GooCanvasItem	*item);

   void            goo_canvas_render	    (GooCanvas		*canvas,
   					     cairo_t            *cr,
   					     GooCanvasBounds    *bounds,
   					     gdouble             scale);

   /*
    * Coordinate conversion.
    */
   void		goo_canvas_convert_to_pixels	   (GooCanvas       *canvas,
   						    gdouble         *x,
   						    gdouble         *y);
   void		goo_canvas_convert_from_pixels	   (GooCanvas       *canvas,
   						    gdouble         *x,
   						    gdouble         *y);

   void		goo_canvas_convert_to_item_space   (GooCanvas	    *canvas,
   						    GooCanvasItem   *item,
   						    gdouble         *x,
   						    gdouble         *y);
   void		goo_canvas_convert_from_item_space (GooCanvas       *canvas,
   						    GooCanvasItem   *item,
   						    gdouble         *x,
   						    gdouble         *y);


   /*
    * Pointer/keyboard grabbing & ungrabbing.
    */
   GdkGrabStatus	goo_canvas_pointer_grab	    (GooCanvas		*canvas,
   					     GooCanvasItem	*item,
   					     GdkEventMask        event_mask,
   					     GdkCursor		*cursor,
   					     guint32             time);
   void		goo_canvas_pointer_ungrab   (GooCanvas		*canvas,
   					     GooCanvasItem	*item,
   					     guint32             time);
   GdkGrabStatus	goo_canvas_keyboard_grab    (GooCanvas		*canvas,
   					     GooCanvasItem	*item,
   					     gboolean            owner_events,
   					     guint32             time);
   void		goo_canvas_keyboard_ungrab  (GooCanvas		*canvas,
   					     GooCanvasItem	*item,
   					     guint32             time);


   /*
    * Internal functions, mainly for canvas subclasses and item implementations.
    */
   GooCanvasItem*	goo_canvas_create_item	    (GooCanvas          *canvas,
   					     GooCanvasItemModel *model);
   void		goo_canvas_unregister_item  (GooCanvas		*canvas,
   					     GooCanvasItemModel *model);
   void		goo_canvas_update	    (GooCanvas		*canvas);
   void		goo_canvas_request_update   (GooCanvas		*canvas);
   void		goo_canvas_request_redraw   (GooCanvas		*canvas,
   					     GooCanvasBounds	*bounds);
   gdouble         goo_canvas_get_default_line_width (GooCanvas    *canvas);


   void            goo_canvas_register_widget_item   (GooCanvas          *canvas,
   						   GooCanvasWidget    *witem);
   void            goo_canvas_unregister_widget_item (GooCanvas          *canvas,
   						   GooCanvasWidget    *witem);
#endif
}
