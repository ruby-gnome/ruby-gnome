/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/* $Id: rbgnome-canvas-item.c,v 1.23 2006/12/15 18:05:04 mutoh Exp $ */

/* Gnome::CanvasItem widget for Ruby/Gnome
 * Copyright (C) 2002-2005 Ruby-GNOME2 Project Team
 * Copyright (C) 2001 Neil Conway <neilconway@rogers.com>
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

#include "rbgnomecanvas.h"
#include "rbart.h"

#define _SELF(self) GNOME_CANVAS_ITEM(RVAL2GOBJ(self))

#ifdef HAVE_STDARG_PROTOTYPES
#include <stdarg.h>
#define va_init_list(a,b) va_start(a,b)
#else
#include <varargs.h>
#define va_init_list(a,b) va_start(a)
#endif

static VALUE citem_parent(VALUE self);

static void
#ifdef HAVE_STDARG_PROTOTYPES
citem_do_construct(GnomeCanvasItem *item, GnomeCanvasGroup *parent, const gchar *first_arg_name, ...)
#else
    citem_do_construct(item, parent, first_arg_name, va_alist)
    GnomeCanvasItem *item;
    GnomeCanvasGroup *parent;
    const gchar *first_arg_name;
    va_dcl
#endif
{
    va_list ap;
    va_init_list(ap, first_arg_name);
    gnome_canvas_item_construct(item, parent, first_arg_name, ap);
    va_end(ap);
}

static VALUE
citem_intialize(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE parent, hash;
    GnomeCanvasItem *item;
    GnomeCanvasGroup *group;

    rb_scan_args(argc, argv, "11", &parent, &hash);

    if (! rb_obj_is_kind_of(parent, GTYPE2CLASS(GNOME_TYPE_CANVAS_GROUP))){
        rb_raise(rb_eArgError, "the 1st parameter should be Gnome::CanvasGroup");
    }

    group = GNOME_CANVAS_GROUP(RVAL2GOBJ(parent));
    item = GNOME_CANVAS_ITEM(g_object_new(RVAL2GTYPE(self), NULL));
    RBGTK_INITIALIZE(self, item);
    g_object_ref(group);
    g_object_freeze_notify(G_OBJECT(item));

    citem_do_construct(item, group, NULL);

    if (! NIL_P(hash)){
        rbgutil_set_properties(self, hash);
    }
    g_object_thaw_notify(G_OBJECT(item));

    G_CHILD_ADD(parent, self);

    return Qnil;
}

static VALUE
citem_set(self, hash)
    VALUE self, hash;
{
    rbgutil_set_properties(self, hash);
    gnome_canvas_item_set(GNOME_CANVAS_ITEM(RVAL2GOBJ(self)), NULL);
    return self;
}

static VALUE
citem_move(self, dx, dy)
    VALUE self, dx, dy;
{
    gnome_canvas_item_move(_SELF(self),
                           NUM2DBL(dx),
                           NUM2DBL(dy));
    return self;
}

static VALUE
citem_affine_relative(self, affine)
    VALUE self, affine;
{
    gnome_canvas_item_affine_relative(_SELF(self),
                                      get_art_affine(affine));
    return self;
}

static VALUE
citem_affine_absolute(self, affine)
    VALUE self, affine;
{
    gnome_canvas_item_affine_absolute(_SELF(self),
                                      get_art_affine(affine));
    return self;
}

static VALUE
citem_raise(self, positions)
    VALUE self, positions;
{
    gnome_canvas_item_raise(_SELF(self),
                            NUM2INT(positions));
    return self;
}

static VALUE
citem_lower(self, positions)
    VALUE self, positions;
{
    gnome_canvas_item_lower(_SELF(self),
                            NUM2INT(positions));
    return self;
}

static VALUE
citem_raise_to_top(self)
    VALUE self;
{
    gnome_canvas_item_raise_to_top(_SELF(self));
    return self;
}

static VALUE
citem_lower_to_bottom(self)
    VALUE self;
{
    gnome_canvas_item_lower_to_bottom(_SELF(self));
    return self;
}

static VALUE
citem_show(self)
    VALUE self;
{
    gnome_canvas_item_show(_SELF(self));
    return self;
}

static VALUE
citem_hide(self)
    VALUE self;
{
    gnome_canvas_item_hide(_SELF(self));
    return self;
}

static VALUE
citem_grab(self, event_mask, cursor, etime)
    VALUE self, event_mask, cursor, etime;
{
    return GENUM2RVAL(
        gnome_canvas_item_grab(_SELF(self),
                               NUM2INT(event_mask),
                               (GdkCursor *)RVAL2BOXED(cursor, GDK_TYPE_CURSOR),
                               NIL_P(etime) ? 0 : NUM2UINT(etime)), GDK_TYPE_GRAB_STATUS);
}

static VALUE
citem_ungrab(self, etime)
    VALUE self, etime;
{
    gnome_canvas_item_ungrab(_SELF(self),
                             NIL_P(etime) ? 0 : NUM2UINT(etime));
    return self;
}

static VALUE
citem_w2i(self, x, y)
    VALUE self, x, y;
{
    double dx = NUM2DBL(x), dy = NUM2DBL(y);
    gnome_canvas_item_w2i(_SELF(self), &dx, &dy);
    return rb_ary_new3(2, rb_float_new(dx), rb_float_new(dy));
}

static VALUE
citem_i2w(self, x, y)
    VALUE self, x, y;
{
    double dx = NUM2DBL(x), dy = NUM2DBL(y);
    gnome_canvas_item_i2w(_SELF(self), &dx, &dy);
    return rb_ary_new3(2, rb_float_new(dx), rb_float_new(dy));
}

static VALUE
citem_i2c_affine(self)
    VALUE self;
{
    double affine[6];
    gnome_canvas_item_i2c_affine(_SELF(self), affine);
    return make_art_affine(affine);
}

static VALUE
citem_i2w_affine(self)
    VALUE self;
{
    double affine[6];
    gnome_canvas_item_i2w_affine(_SELF(self), affine);
    return make_art_affine(affine);
}

static VALUE
citem_reparent(self, new_group)
    VALUE self, new_group;
{
    if (!g_type_is_a(RVAL2GTYPE(new_group), GNOME_TYPE_CANVAS_GROUP)) {
        rb_raise(rb_eTypeError, "not a GnomeCanvasGroup");
    }
    G_CHILD_REMOVE(citem_parent(self), self);
    gnome_canvas_item_reparent(_SELF(self), GNOME_CANVAS_GROUP(RVAL2GOBJ(new_group)));
    G_CHILD_ADD(new_group, self);
    return self;
}

static VALUE
citem_grab_focus(self)
    VALUE self;
{
    gnome_canvas_item_grab_focus(_SELF(self));
    return self;
}

static VALUE
citem_get_bounds(self)
    VALUE self;
{
    double x1, y1, x2, y2;
    gnome_canvas_item_get_bounds(_SELF(self),
                                 &x1,
                                 &y1,
                                 &x2,
                                 &y2);
    return rb_ary_new3(4, rb_float_new(x1), rb_float_new(y1),
                       rb_float_new(x2), rb_float_new(y2));
}

static VALUE
citem_request_update(self)
    VALUE self;
{
    gnome_canvas_item_request_update(_SELF(self));
    return self;
}

static VALUE
citem_parent(self)
    VALUE self;
{
    return GOBJ2RVAL(GTK_OBJECT(_SELF(self)->parent));
}

static VALUE
citem_canvas(self)
    VALUE self;
{
    return GOBJ2RVAL(GTK_OBJECT(_SELF(self)->canvas));
}

static VALUE
citem_shape_set_path_def(self, def)
    VALUE self, def;
{
    gnome_canvas_shape_set_path_def(GNOME_CANVAS_SHAPE(RVAL2GOBJ(self)), 
                                    (GnomeCanvasPathDef*)RVAL2BOXED(def, GNOME_TYPE_CANVAS_PATH_DEF));
    return self;
}

static VALUE
citem_shape_get_path_def(self)
    VALUE self;
{
    return BOXED2RVAL(gnome_canvas_shape_get_path_def(GNOME_CANVAS_SHAPE(RVAL2GOBJ(self))), GNOME_TYPE_CANVAS_PATH_DEF);
}

static void
shapedash_r2g_func(from, to)
    VALUE from;
    GValue* to;
{
    g_value_set_pointer(to, (gpointer)get_art_vpath_dash(from));
}

static VALUE
shapedash_g2r_func(from)
    const GValue *from;
{
    ArtVpathDash* dash = g_value_get_pointer(from);
    
    ArtVpathDash* ret = g_new0(ArtVpathDash, 1);
    ret->offset = dash->offset;
    ret->n_dash = dash->n_dash;
    ret->dash = g_new(double, ret->n_dash * sizeof (double));
    memcpy(dash->dash,  ret->dash, dash->n_dash * sizeof (double));

    return make_art_vpath_dash(ret);
}

static void
clipgroup_r2g_func(from, to)
    VALUE from;
    GValue* to;
{
    g_value_set_pointer(to, (GnomeCanvasPathDef*)RVAL2BOXED(from, GNOME_TYPE_CANVAS_PATH_DEF));
}

static VALUE
clipgroup_g2r_func(from)
    const GValue *from;
{
    return BOXED2RVAL(g_value_get_pointer(from), GNOME_TYPE_CANVAS_PATH_DEF);
}

void
Init_gnome_canvas_item(mGnome)
    VALUE mGnome;
{
    VALUE gnoCanvasItem = G_DEF_CLASS(GNOME_TYPE_CANVAS_ITEM, "CanvasItem", mGnome);
    VALUE gnoCanvasShape = G_DEF_CLASS(GNOME_TYPE_CANVAS_SHAPE, "CanvasShape", mGnome);

    rb_define_method(gnoCanvasItem, "initialize", citem_intialize, -1);
    rb_define_method(gnoCanvasItem, "set", citem_set, 1);
    rb_define_method(gnoCanvasItem, "move", citem_move, 2);
    rb_define_method(gnoCanvasItem, "affine_relative", citem_affine_relative, 1);
    rb_define_method(gnoCanvasItem, "affine_absolute", citem_affine_absolute, 1);
    rb_define_method(gnoCanvasItem, "raise", citem_raise, 1);
    rb_define_method(gnoCanvasItem, "lower", citem_lower, 1);
    rb_define_method(gnoCanvasItem, "raise_to_top", citem_raise_to_top, 0);
    rb_define_method(gnoCanvasItem, "lower_to_bottom", citem_lower_to_bottom, 0);
    rb_define_method(gnoCanvasItem, "show", citem_show, 0);
    rb_define_method(gnoCanvasItem, "hide", citem_hide, 0);
    rb_define_method(gnoCanvasItem, "grab", citem_grab, 3);
    rb_define_method(gnoCanvasItem, "ungrab", citem_ungrab, 1);
    rb_define_method(gnoCanvasItem, "w2i", citem_w2i, 2);
    rb_define_method(gnoCanvasItem, "i2w", citem_i2w, 2);
    rb_define_method(gnoCanvasItem, "i2w_affine", citem_i2w_affine, 0);
    rb_define_method(gnoCanvasItem, "i2c_affine", citem_i2c_affine, 0);
    rb_define_method(gnoCanvasItem, "reparent", citem_reparent, 1);
    rb_define_method(gnoCanvasItem, "grab_focus", citem_grab_focus, 0);
    rb_define_method(gnoCanvasItem, "bounds", citem_get_bounds, 0);
    rb_define_method(gnoCanvasItem, "request_update", citem_request_update, 0);
    rb_define_method(gnoCanvasItem, "parent", citem_parent, 0);
    rb_define_method(gnoCanvasItem, "canvas", citem_canvas, 0);

    rb_define_method(gnoCanvasShape, "set_path_def", citem_shape_set_path_def, 1);
    rb_define_method(gnoCanvasShape, "path_def", citem_shape_get_path_def, 0);
    G_DEF_SETTERS(gnoCanvasShape);

    rbgobj_register_property_setter(GNOME_TYPE_CANVAS_SHAPE, "dash", shapedash_r2g_func);
    rbgobj_register_property_getter(GNOME_TYPE_CANVAS_SHAPE, "dash", shapedash_g2r_func);

    G_DEF_CLASS(GNOME_TYPE_CANVAS_RE, "CanvasRE", mGnome);
    G_DEF_CLASS(GNOME_TYPE_CANVAS_GROUP, "CanvasGroup", mGnome);
    G_DEF_CLASS(GNOME_TYPE_CANVAS_CLIPGROUP, "CanvasClipgroup", mGnome);

    rbgobj_register_property_setter(GNOME_TYPE_CANVAS_CLIPGROUP, "path", clipgroup_r2g_func);
    rbgobj_register_property_getter(GNOME_TYPE_CANVAS_CLIPGROUP, "path", clipgroup_g2r_func);

    G_DEF_CLASS(GNOME_TYPE_CANVAS_BPATH, "CanvasBpath", mGnome);
    G_DEF_CLASS(GNOME_TYPE_CANVAS_LINE, "CanvasLine", mGnome);
    G_DEF_CLASS(GNOME_TYPE_CANVAS_PIXBUF, "CanvasPixbuf", mGnome);
    G_DEF_CLASS(GNOME_TYPE_CANVAS_POLYGON, "CanvasPolygon", mGnome);
    G_DEF_CLASS(GNOME_TYPE_CANVAS_RECT, "CanvasRect", mGnome);
    G_DEF_CLASS(GNOME_TYPE_CANVAS_ELLIPSE, "CanvasEllipse", mGnome);
    G_DEF_CLASS(GNOME_TYPE_CANVAS_TEXT, "CanvasText", mGnome);
    G_DEF_CLASS(GNOME_TYPE_CANVAS_WIDGET, "CanvasWidget", mGnome);
}
