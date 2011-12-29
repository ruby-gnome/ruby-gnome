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

#define RG_TARGET_NAMESPACE cCanvasItem
#define SELF(self) RVAL2GCI(self)

static VALUE
rg_rotate(VALUE self, VALUE degrees, VALUE cx, VALUE cy)
{
    goo_canvas_item_rotate(SELF(self), NUM2DBL(degrees),
                           NUM2DBL(cx), NUM2DBL(cy));
    return self;
}

static VALUE
rg_translate(VALUE self, VALUE tx, VALUE ty)
{
    goo_canvas_item_translate(SELF(self), NUM2DBL(tx), NUM2DBL(ty));
    return self;
}

static VALUE
rg_lower(VALUE self, VALUE below)
{
    goo_canvas_item_lower(SELF(self), SELF(below));
    return self;
}

static VALUE
rg_raise(VALUE self, VALUE above)
{
    goo_canvas_item_raise(SELF(self), SELF(above));
    return self;
}

static VALUE
rg_scale(VALUE self, VALUE sx, VALUE sy)
{
    goo_canvas_item_scale(SELF(self), NUM2DBL(sx), NUM2DBL(sy));
    return self;
}

static VALUE
rg_x1(VALUE self)
{
    GooCanvasBounds bounds;
    goo_canvas_item_get_bounds(SELF(self), &bounds);
    return rb_float_new(bounds.x1);
}

static VALUE
rg_x2(VALUE self)
{
    GooCanvasBounds bounds;
    goo_canvas_item_get_bounds(SELF(self), &bounds);
    return rb_float_new(bounds.x2);
}

static VALUE
rg_y1(VALUE self)
{
    GooCanvasBounds bounds;
    goo_canvas_item_get_bounds(SELF(self), &bounds);
    return rb_float_new(bounds.y1);
}

static VALUE
rg_y2(VALUE self)
{
    GooCanvasBounds bounds;
    goo_canvas_item_get_bounds(SELF(self), &bounds);
    return rb_float_new(bounds.y2);
}

static VALUE
rg_skew_x(VALUE self, VALUE degrees, VALUE cx, VALUE cy)
{
    goo_canvas_item_skew_x(SELF(self), NUM2DBL(degrees),
                           NUM2DBL(cx), NUM2DBL(cy));
    return self;
}

static VALUE
rg_skew_y(VALUE self, VALUE degrees, VALUE cx, VALUE cy)
{
    goo_canvas_item_skew_y(SELF(self), NUM2DBL(degrees),
                           NUM2DBL(cx), NUM2DBL(cy));
    return self;
}

static VALUE
rg_set_simple_transform(VALUE self, VALUE x, VALUE y,
                                        VALUE scale, VALUE rotation)
{
    goo_canvas_item_set_simple_transform(SELF(self), NUM2DBL(x), NUM2DBL(y),
                                         NUM2DBL(scale), NUM2DBL(rotation));
    return self;
}

static VALUE
rg_animate(VALUE self, VALUE x, VALUE y, VALUE scale,
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
rg_stop_animation(VALUE self)
{
    goo_canvas_item_stop_animation(SELF(self));
    return self;
}

static VALUE
rg_set_style(VALUE self, VALUE style)
{
    goo_canvas_item_set_style(SELF(self), RVAL2GCS(style));
    return self;
}

static VALUE
rg_set_child_property(VALUE self, VALUE child,
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
rg_canvas(VALUE self, VALUE tx, VALUE ty)
{
    return GOBJ2RVAL(goo_canvas_item_get_canvas(SELF(self)));
}

static VALUE
rg_remove_child(VALUE self, VALUE child)
{
    gint child_num;

    child_num = goo_canvas_item_find_child(SELF(self), SELF(child));
    goo_canvas_item_remove_child(SELF(self), child_num);
    G_CHILD_REMOVE(self, child);

    return self;
}

void
Init_goocanvasitem(VALUE mGoo)
{
    VALUE RG_TARGET_NAMESPACE;

    RG_TARGET_NAMESPACE = G_DEF_CLASS(GOO_TYPE_CANVAS_ITEM, "CanvasItem", mGoo);

    RG_DEF_METHOD(rotate, 3);
    RG_DEF_METHOD(translate, 2);
    RG_DEF_METHOD(lower, 1);
    RG_DEF_METHOD(raise, 1);
    RG_DEF_METHOD(scale, 2);
    RG_DEF_METHOD(skew_x, 3);
    RG_DEF_METHOD(skew_y, 3);
    RG_DEF_METHOD(set_simple_transform, 4);
    RG_DEF_METHOD(animate, 8);
    RG_DEF_METHOD(stop_animation, 0);

    RG_DEF_METHOD(set_style, 1);

    RG_DEF_METHOD(set_child_property, 3);

    RG_DEF_METHOD(canvas, 0);

    RG_DEF_METHOD(remove_child, 1);

    RG_DEF_METHOD(x1, 0);
    RG_DEF_METHOD(x2, 0);
    RG_DEF_METHOD(y1, 0);
    RG_DEF_METHOD(y2, 0);

    /* Enums */
    G_DEF_CLASS(GOO_TYPE_CANVAS_ITEM_VISIBILITY, "Visibility", RG_TARGET_NAMESPACE);
    G_DEF_CONSTANTS(RG_TARGET_NAMESPACE, GOO_TYPE_CANVAS_ITEM_VISIBILITY, "GOO_CANVAS_ITEM_");
    G_DEF_CLASS(GOO_TYPE_CANVAS_ANIMATE_TYPE, "AnimateType", RG_TARGET_NAMESPACE);
    G_DEF_CONSTANTS(RG_TARGET_NAMESPACE, GOO_TYPE_CANVAS_ANIMATE_TYPE, "GOO_CANVAS_");

    G_DEF_CLASS(GOO_TYPE_CANVAS_POINTER_EVENTS, "PointerEvents",RG_TARGET_NAMESPACE);
    G_DEF_CONSTANTS(RG_TARGET_NAMESPACE, GOO_TYPE_CANVAS_POINTER_EVENTS,"GOO_CANVAS_");

    G_DEF_SETTERS(RG_TARGET_NAMESPACE);
}
