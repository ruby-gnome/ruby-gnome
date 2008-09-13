/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/* $Id$ */
/* GooCanvasItem
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

#define SELF(self) RVAL2GCI(self)

static VALUE
rb_goo_canvas_item_rotate(VALUE self, VALUE degrees, VALUE cx, VALUE cy)
{
    goo_canvas_item_rotate(SELF(self), NUM2DBL(degrees),
                           NUM2DBL(cx), NUM2DBL(cy));
    return self;
}

static VALUE
rb_goo_canvas_item_translate(VALUE self, VALUE tx, VALUE ty)
{
    goo_canvas_item_translate(SELF(self), NUM2DBL(tx), NUM2DBL(ty));
    return self;
}

static VALUE
rb_goo_canvas_item_lower(VALUE self, VALUE below)
{
    goo_canvas_item_lower(SELF(self), SELF(below));
    return self;
}

static VALUE
rb_goo_canvas_item_raise(VALUE self, VALUE above)
{
    goo_canvas_item_raise(SELF(self), SELF(above));
    return self;
}

static VALUE
rb_goo_canvas_item_scale(VALUE self, VALUE sx, VALUE sy)
{
    goo_canvas_item_scale(SELF(self), NUM2DBL(sx), NUM2DBL(sy));
    return self;
}

static VALUE
rb_goo_canvas_bounds_x1(VALUE self)
{
    GooCanvasBounds bounds;
    goo_canvas_item_get_bounds(SELF(self), &bounds);
    return rb_float_new(bounds.x1);
}

static VALUE
rb_goo_canvas_bounds_x2(VALUE self)
{
    GooCanvasBounds bounds;
    goo_canvas_item_get_bounds(SELF(self), &bounds);
    return rb_float_new(bounds.x2);
}

static VALUE
rb_goo_canvas_bounds_y1(VALUE self)
{
    GooCanvasBounds bounds;
    goo_canvas_item_get_bounds(SELF(self), &bounds);
    return rb_float_new(bounds.y1);
}

static VALUE
rb_goo_canvas_bounds_y2(VALUE self)
{
    GooCanvasBounds bounds;
    goo_canvas_item_get_bounds(SELF(self), &bounds);
    return rb_float_new(bounds.y2);
}

static VALUE
rb_goo_canvas_item_skew_x(VALUE self, VALUE degrees, VALUE cx, VALUE cy)
{
    goo_canvas_item_skew_x(SELF(self), NUM2DBL(degrees),
                           NUM2DBL(cx), NUM2DBL(cy));
    return self;
}

static VALUE
rb_goo_canvas_item_skew_y(VALUE self, VALUE degrees, VALUE cx, VALUE cy)
{
    goo_canvas_item_skew_y(SELF(self), NUM2DBL(degrees),
                           NUM2DBL(cx), NUM2DBL(cy));
    return self;
}

static VALUE
rb_goo_canvas_item_set_simple_transform(VALUE self, VALUE x, VALUE y,
                                        VALUE scale, VALUE rotation)
{
    goo_canvas_item_set_simple_transform(SELF(self), NUM2DBL(x), NUM2DBL(y),
                                         NUM2DBL(scale), NUM2DBL(rotation));
    return self;
}

static VALUE
rb_goo_canvas_item_animate(VALUE self, VALUE x, VALUE y, VALUE scale,
                           VALUE degrees, VALUE absolute, VALUE duration,
                           VALUE step_time, VALUE type)
{
    goo_canvas_item_animate(SELF(self),
                            NUM2DBL(x),
                            NUM2DBL(y),
                            NUM2DBL(scale),
                            NUM2DBL(degrees),
                            RTEST(absolute),
                            NUM2INT(duration),
                            NUM2INT(step_time),
                            rbgobj_get_enum(type, GOO_TYPE_CANVAS_ANIMATE_TYPE));
    return self;
}

static VALUE
rb_goo_canvas_item_stop_animation(VALUE self)
{
    goo_canvas_item_stop_animation(SELF(self));
    return self;
}

static VALUE
rb_goo_canvas_item_set_style(VALUE self, VALUE style)
{
    goo_canvas_item_set_style(SELF(self), RVAL2GCS(style));
    return self;
}

static VALUE
rb_goo_canvas_item_set_child_property(VALUE self, VALUE child,
                                      VALUE prop_name, VALUE val)
{
    GParamSpec* pspec;
    const char* name;

    if (SYMBOL_P(prop_name)) {
        name = rb_id2name(SYM2ID(prop_name));
    } else {
        StringValue(prop_name);
        name = StringValuePtr(prop_name);
    }
    pspec = goo_canvas_item_class_find_child_property(G_OBJECT_GET_CLASS(RVAL2GOBJ(self)), name);

    if (!pspec)
        rb_raise(rb_eRuntimeError, "No such child property: %s", name);

#define _SET_PROP_FOR_TYPE(gtype, ctype, val) \
    case gtype: \
        goo_canvas_item_set_child_properties(SELF(self), SELF(child), name, (ctype)(val), NULL); \
        break;

    switch (G_TYPE_FUNDAMENTAL(G_PARAM_SPEC_VALUE_TYPE(pspec))) {
        _SET_PROP_FOR_TYPE(G_TYPE_CHAR, gchar, NUM2INT(val));
        _SET_PROP_FOR_TYPE(G_TYPE_UCHAR, guchar, NUM2INT(val));
        _SET_PROP_FOR_TYPE(G_TYPE_BOOLEAN, gboolean, RTEST(val));
        _SET_PROP_FOR_TYPE(G_TYPE_INT, gint, NUM2INT(val));
        _SET_PROP_FOR_TYPE(G_TYPE_UINT, guint, NUM2UINT(val));
        _SET_PROP_FOR_TYPE(G_TYPE_LONG, glong, NUM2LONG(val));
        _SET_PROP_FOR_TYPE(G_TYPE_ULONG, gulong, NUM2ULONG(val));
        _SET_PROP_FOR_TYPE(G_TYPE_INT64, gint64, rbglib_num_to_int64(val));
        _SET_PROP_FOR_TYPE(G_TYPE_UINT64, guint64, rbglib_num_to_uint64(val));
        _SET_PROP_FOR_TYPE(G_TYPE_ENUM, gint, rbgobj_get_enum(val, G_PARAM_SPEC_VALUE_TYPE(pspec)));
        _SET_PROP_FOR_TYPE(G_TYPE_FLAGS, guint, rbgobj_get_flags(val, G_PARAM_SPEC_VALUE_TYPE(pspec)));
        _SET_PROP_FOR_TYPE(G_TYPE_FLOAT, gfloat, NUM2DBL(val));
        _SET_PROP_FOR_TYPE(G_TYPE_DOUBLE, gdouble, NUM2DBL(val));
      case G_TYPE_STRING:
        {
            if (SYMBOL_P(val))
                val = rb_funcall(val, rb_intern("to_s"), 0);
            goo_canvas_item_set_child_properties(SELF(self), SELF(child), name, StringValuePtr(val), NULL);
            break;
        }

      default:
        g_warning("rb_goo_canvas_item_set_child_property: unsupported type: %s\n", g_type_name(G_PARAM_SPEC_VALUE_TYPE(pspec)));
        return Qnil;
    }

#undef _SET_PROP_FOR_TYPE

    G_CHILD_ADD(child, val);

    return self;
}

static VALUE
rb_goo_canvas_item_get_canvas(VALUE self, VALUE tx, VALUE ty)
{
    return GOBJ2RVAL(goo_canvas_item_get_canvas(SELF(self)));
}

static VALUE
rb_goo_canvas_item_remove_child(VALUE self, VALUE child)
{
    gint child_num;

    child_num = goo_canvas_item_find_child(SELF(self), SELF(child));
    goo_canvas_item_remove_child(SELF(self), child_num);
    G_CHILD_REMOVE(self, child);

    return self;
}

void
Init_goocanvasitem(void)
{
    VALUE GooCanvasItem;

    GooCanvasItem = G_DEF_CLASS(GOO_TYPE_CANVAS_ITEM, "CanvasItem", mGoo);

    rb_define_method(GooCanvasItem, "rotate", rb_goo_canvas_item_rotate, 3);
    rb_define_method(GooCanvasItem, "translate", rb_goo_canvas_item_translate, 2);
    rb_define_method(GooCanvasItem, "lower", rb_goo_canvas_item_lower, 1);
    rb_define_method(GooCanvasItem, "raise", rb_goo_canvas_item_raise, 1);
    rb_define_method(GooCanvasItem, "scale", rb_goo_canvas_item_scale, 2);
    rb_define_method(GooCanvasItem, "skew_x", rb_goo_canvas_item_skew_x, 3);
    rb_define_method(GooCanvasItem, "skew_y", rb_goo_canvas_item_skew_y, 3);
    rb_define_method(GooCanvasItem, "set_simple_transform", rb_goo_canvas_item_set_simple_transform, 4);
    rb_define_method(GooCanvasItem, "animate", rb_goo_canvas_item_animate, 8);
    rb_define_method(GooCanvasItem, "stop_animation", rb_goo_canvas_item_stop_animation, 0);

    rb_define_method(GooCanvasItem, "set_style", rb_goo_canvas_item_set_style, 1);

    rb_define_method(GooCanvasItem, "set_child_property", rb_goo_canvas_item_set_child_property, 3);

    rb_define_method(GooCanvasItem, "canvas", rb_goo_canvas_item_get_canvas, 0);

    rb_define_method(GooCanvasItem, "remove_child", rb_goo_canvas_item_remove_child, 1);

    rb_define_method(GooCanvasItem, "x1", rb_goo_canvas_bounds_x1, 0);
    rb_define_method(GooCanvasItem, "x2", rb_goo_canvas_bounds_x2, 0);
    rb_define_method(GooCanvasItem, "y1", rb_goo_canvas_bounds_y1, 0);
    rb_define_method(GooCanvasItem, "y2", rb_goo_canvas_bounds_y2, 0);

    /* Enums */
    G_DEF_CLASS(GOO_TYPE_CANVAS_ITEM_VISIBILITY, "Visibility", GooCanvasItem);
    G_DEF_CONSTANTS(GooCanvasItem, GOO_TYPE_CANVAS_ITEM_VISIBILITY, "GOO_CANVAS_ITEM_");
    G_DEF_CLASS(GOO_TYPE_CANVAS_ANIMATE_TYPE, "AnimateType", GooCanvasItem);
    G_DEF_CONSTANTS(GooCanvasItem, GOO_TYPE_CANVAS_ANIMATE_TYPE, "GOO_CANVAS_");

    
    G_DEF_CLASS(GOO_TYPE_CANVAS_POINTER_EVENTS, "PointerEvents",GooCanvasItem);
    G_DEF_CONSTANTS(GooCanvasItem, GOO_TYPE_CANVAS_POINTER_EVENTS,"GOO_CANVAS_");
   
#if 0
   gint               goo_canvas_item_get_n_children (GooCanvasItem   *item);
   GooCanvasItem*     goo_canvas_item_get_child      (GooCanvasItem   *item,
                              gint             child_num);
   gint               goo_canvas_item_find_child     (GooCanvasItem   *item,
                              GooCanvasItem   *child);
   void               goo_canvas_item_add_child      (GooCanvasItem   *item,
                              GooCanvasItem   *child,
                              gint             position);
   void               goo_canvas_item_move_child     (GooCanvasItem   *item,
                              gint             old_position,
                              gint             new_position);
   void               goo_canvas_item_remove_child   (GooCanvasItem   *item,
                              gint             child_num);

   void  goo_canvas_item_get_child_properties        (GooCanvasItem   *item,
                              GooCanvasItem   *child,
                              ...) G_GNUC_NULL_TERMINATED;
   void  goo_canvas_item_set_child_properties        (GooCanvasItem   *item,
                              GooCanvasItem   *child,
                              ...) G_GNUC_NULL_TERMINATED;
   void  goo_canvas_item_get_child_properties_valist (GooCanvasItem   *item,
                              GooCanvasItem   *child,
                              va_list        var_args);
   void  goo_canvas_item_set_child_properties_valist (GooCanvasItem   *item,
                              GooCanvasItem   *child,
                              va_list        var_args);

   gboolean goo_canvas_item_get_transform_for_child  (GooCanvasItem   *item,
                              GooCanvasItem   *child,
                              cairo_matrix_t  *transform);


   /*
    * Item functions - these are safe to call on all items.
    */
   GooCanvas*         goo_canvas_item_get_canvas     (GooCanvasItem   *item);
   void               goo_canvas_item_set_canvas     (GooCanvasItem   *item,
                              GooCanvas       *canvas);
   GooCanvasItem*     goo_canvas_item_get_parent     (GooCanvasItem   *item);
   void               goo_canvas_item_set_parent      (GooCanvasItem   *item,
                              GooCanvasItem   *parent);
   gboolean           goo_canvas_item_is_container   (GooCanvasItem   *item);

   cairo_matrix_t*    goo_canvas_item_get_transform  (GooCanvasItem   *item);
   void               goo_canvas_item_set_transform  (GooCanvasItem   *item,
                              cairo_matrix_t  *matrix);
   void               goo_canvas_item_set_simple_transform (GooCanvasItem   *item,
                                gdouble          x,
                                gdouble          y,
                                gdouble          scale,
                                gdouble          rotation);

   GooCanvasStyle*    goo_canvas_item_get_style      (GooCanvasItem   *item);


   void               goo_canvas_item_get_bounds      (GooCanvasItem   *item,
                              GooCanvasBounds *bounds);
   GooCanvasItem*       goo_canvas_item_get_item_at    (GooCanvasItem   *item,
                              gdouble          x,
                              gdouble          y,
                              cairo_t         *cr,
                              gboolean         is_pointer_event,
                              gboolean         parent_is_visible);
   gboolean           goo_canvas_item_is_visible     (GooCanvasItem   *item);

   GooCanvasItemModel* goo_canvas_item_get_model      (GooCanvasItem      *item);
   void                goo_canvas_item_set_model      (GooCanvasItem      *item,
                              GooCanvasItemModel *model);

   void               goo_canvas_item_request_update (GooCanvasItem   *item);
   void           goo_canvas_item_ensure_updated (GooCanvasItem   *item);
   void               goo_canvas_item_update         (GooCanvasItem   *item,
                              gboolean         entire_tree,
                              cairo_t         *cr,
                              GooCanvasBounds *bounds);
   void               goo_canvas_item_paint          (GooCanvasItem   *item,
                              cairo_t         *cr,
                              GooCanvasBounds *bounds,
                              gdouble          scale);

   gboolean       goo_canvas_item_get_requested_area (GooCanvasItem    *item,
                                  cairo_t          *cr,
                                  GooCanvasBounds  *requested_area);
   void           goo_canvas_item_allocate_area      (GooCanvasItem    *item,
                                  cairo_t          *cr,
                                  GooCanvasBounds  *requested_area,
                                  GooCanvasBounds  *allocated_area,
                                  gdouble           x_offset,
                                  gdouble           y_offset);
#endif

    G_DEF_SETTERS(GooCanvasItem);
}
