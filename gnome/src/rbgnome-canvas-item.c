/* $Id: rbgnome-canvas-item.c,v 1.7 2002/08/27 11:54:51 mutoh Exp $ */

/* Gnome::CanvasItem widget for Ruby/Gnome
 * Copyright (C) 2001 Neil Conway <neilconway@rogers.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include "rbgnome.h"
#include "rbart.h"
extern VALUE cImlibImage;

VALUE gnoCanvasItem;
VALUE gnoCanvasLine;
VALUE gnoCanvasPolygon;
VALUE gnoCanvasRect;
VALUE gnoCanvasEllipse;
VALUE gnoCanvasText;
VALUE gnoCanvasImage;
VALUE gnoCanvasWidget;

#ifdef HAVE_GDKIMLIB
GdkImlibImage *get_gdkimlib_image(obj)
    VALUE obj;
{
    GdkImlibImage *im;

    if (!rb_obj_is_kind_of(obj, cImlibImage)) {
	rb_raise(rb_eTypeError, "not a GdkImlibImage");
    }
    Data_Get_Struct(obj, GdkImlibImage, im);
    return im;
}
#endif

static VALUE
citem_set(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    GnomeCanvasItem *ci = GNOME_CANVAS_ITEM(get_gobject(self));
    VALUE ary, key, val;
    GtkArg *arg;
    int i;

    if (argc > 0 && TYPE(argv[0]) == T_HASH) {
	if (argc != 1)
	    rb_raise(rb_eArgError, "wrong # of argument.");
	ary = rb_funcall(argv[0], rb_intern("to_a"), 0, NULL);
	argc = RARRAY(ary)->len;
	arg = ALLOCA_N(GtkArg, argc);
	for (i = 0;i < argc;i++) {
	    key = RARRAY(RARRAY(ary)->ptr[i])->ptr[0];
	    val = RARRAY(RARRAY(ary)->ptr[i])->ptr[1];
	    rbgtk_arg_init(&(arg[i]), GTK_OBJECT_TYPE(ci), STR2CSTR(key));
	    rbgtk_arg_set(&(arg[i]), val);
	}
    } else {
	if (argc & 1) {
	    rb_raise(rb_eArgError, "wrong # of argument. The pairs of 'name' and 'value' are borken.");
	}
	argc /= 2;
	arg = ALLOCA_N(GtkArg, argc);
	for (i = 0;i < argc; i++) {
	    key = argv[i * 2];
	    val = argv[i * 2 + 1];
	    rbgtk_arg_init(&(arg[i]), GTK_OBJECT_TYPE(ci), STR2CSTR(key));
	    rbgtk_arg_set(&(arg[i]), val);
	}
    }
    gnome_canvas_item_setv(ci, argc, arg);
    return Qnil;
}

static VALUE
citem_get(self, name)
    VALUE self, name;
{
    GtkObject *gobj;
    GtkArg arg;
    VALUE obj;

    gobj = get_gobject(self);
    rbgtk_arg_init(&arg, GTK_OBJECT_TYPE(gobj), STR2CSTR(name));
    gtk_object_getv(gobj, 1, &arg);
    if (arg.type == GTK_TYPE_INVALID) {
	rb_raise(rb_eTypeError, "wrong argument name %s", arg.name);
    }
    obj = rbgtk_arg_get(&arg);
    if (arg.type == GTK_TYPE_STRING) {
	g_free(GTK_VALUE_STRING(arg));
    }
    return obj;
}

static VALUE
citem_move(self, dx, dy)
    VALUE self, dx, dy;
{
    gnome_canvas_item_move(GNOME_CANVAS_ITEM(get_gobject(self)),
                           NUM2DBL(dx),
                           NUM2DBL(dy));
    return Qnil;
}

static VALUE
citem_affine_relative(self, affine)
    VALUE self, affine;
{
    gnome_canvas_item_affine_relative(GNOME_CANVAS_ITEM(get_gobject(self)),
                                       get_art_affine(affine));
    return Qnil;
}

static VALUE
citem_affine_absolute(self, affine)
    VALUE self, affine;
{
    gnome_canvas_item_affine_absolute(GNOME_CANVAS_ITEM(get_gobject(self)),
                                      get_art_affine(affine));
    return Qnil;
}

static VALUE
citem_raise(self, positions)
    VALUE self, positions;
{
    gnome_canvas_item_raise(GNOME_CANVAS_ITEM(get_gobject(self)),
                            NUM2INT(positions));
    return Qnil;
}

static VALUE
citem_lower(self, positions)
    VALUE self, positions;
{
    gnome_canvas_item_lower(GNOME_CANVAS_ITEM(get_gobject(self)),
                            NUM2INT(positions));
    return Qnil;
}

static VALUE
citem_raise_to_top(self)
    VALUE self;
{
    gnome_canvas_item_raise_to_top(GNOME_CANVAS_ITEM(get_gobject(self)));
    return Qnil;
}

static VALUE
citem_lower_to_bottom(self)
    VALUE self;
{
    gnome_canvas_item_lower_to_bottom(
            GNOME_CANVAS_ITEM(get_gobject(self)));
    return Qnil;
}

static VALUE
citem_show(self)
    VALUE self;
{
    gnome_canvas_item_show(GNOME_CANVAS_ITEM(get_gobject(self)));
    return Qnil;
}

static VALUE
citem_hide(self)
    VALUE self;
{
    gnome_canvas_item_hide(GNOME_CANVAS_ITEM(get_gobject(self)));
    return Qnil;
}

static VALUE
citem_grab(self, event_mask, cursor, etime)
    VALUE self, event_mask, cursor, etime;
{
    return NUM2INT(
            gnome_canvas_item_grab(GNOME_CANVAS_ITEM(get_gobject(self)),
                                   NUM2INT(event_mask),
                                   get_gdkcursor(cursor),
                                   NIL_P(etime)  ? 0 : NUM2INT(etime)));
}

static VALUE
citem_ungrab(self, etime)
    VALUE self, etime;
{
    gnome_canvas_item_ungrab(GNOME_CANVAS_ITEM(get_gobject(self)),
			     NIL_P(etime) ? 0 : NUM2INT(etime));
    return Qnil;
}

static VALUE
citem_w2i(self, x, y)
    VALUE self, x, y;
{
    double dx = NUM2DBL(x), dy = NUM2DBL(y);
    gnome_canvas_item_w2i(GNOME_CANVAS_ITEM(get_gobject(self)), &dx, &dy);
    return rb_ary_new3(2, rb_float_new(dx), rb_float_new(dy));
}

static VALUE
citem_i2w(self, x, y)
    VALUE self, x, y;
{
    double dx = NUM2DBL(x), dy = NUM2DBL(y);
    gnome_canvas_item_i2w(GNOME_CANVAS_ITEM(get_gobject(self)), &dx, &dy);
    return rb_ary_new3(2, rb_float_new(dx), rb_float_new(dy));
}

static VALUE
citem_i2c_affine(self)
    VALUE self;
{
    double affine[6];
    gnome_canvas_item_i2c_affine(GNOME_CANVAS_ITEM(get_gobject(self)), affine);
    return make_art_affine(affine);
}

static VALUE
citem_i2w_affine(self)
    VALUE self;
{
    double affine[6];
    gnome_canvas_item_i2w_affine(GNOME_CANVAS_ITEM(get_gobject(self)), affine);
    return make_art_affine(affine);
}

static VALUE
citem_reparent(self, new_group)
    VALUE self, new_group;
{
    if (!rb_obj_is_kind_of(new_group, gnoCanvasGroup)) {
	rb_raise(rb_eTypeError, "not a GnomeCanvasGroup");
    }
    gnome_canvas_item_reparent(GNOME_CANVAS_ITEM(get_gobject(self)), GNOME_CANVAS_GROUP(get_gobject(new_group)));
    return Qnil;
}

static VALUE
citem_grab_focus(self)
    VALUE self;
{
    gnome_canvas_item_grab_focus(GNOME_CANVAS_ITEM(get_gobject(self)));
    return Qnil;
}

static VALUE
citem_get_bounds(self)
    VALUE self;
{
    double x1, y1, x2, y2;
    gnome_canvas_item_get_bounds(GNOME_CANVAS_ITEM(get_gobject(self)),
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
    gnome_canvas_item_request_update(GNOME_CANVAS_ITEM(get_gobject(self)));
    return Qnil;
}

static VALUE
citem_parent(self)
    VALUE self;
{
    return get_value_from_gobject(GTK_OBJECT(GNOME_CANVAS_ITEM(get_gobject(self))->parent));
}

static VALUE
citem_canvas(self)
    VALUE self;
{
    return get_value_from_gobject(GTK_OBJECT(GNOME_CANVAS_ITEM(get_gobject(self))->canvas));
}

void
Init_gnome_canvas_item()
{
    gnoCanvasItem = rb_define_class_under(mGnome, "CanvasItem", gObject);
    rb_define_method(gnoCanvasItem, "set",
                     citem_set, -1);
    rb_define_method(gnoCanvasItem, "get",
                     citem_get, 1);
    rb_define_method(gnoCanvasItem, "move",
                     citem_move, 2);
    rb_define_method(gnoCanvasItem, "affine_relative",
                     citem_affine_relative, 1);
    rb_define_method(gnoCanvasItem, "affine_absolute",
                     citem_affine_absolute, 1);
    rb_define_method(gnoCanvasItem, "raise",
                     citem_raise, 1);
    rb_define_method(gnoCanvasItem, "lower",
                     citem_lower, 1);
    rb_define_method(gnoCanvasItem, "raise_to_top",
                     citem_raise_to_top, 0);
    rb_define_method(gnoCanvasItem, "lower_to_bottom",
                     citem_lower_to_bottom, 0);
    rb_define_method(gnoCanvasItem, "show",
                     citem_show, 0);
    rb_define_method(gnoCanvasItem, "hide",
                     citem_hide, 0);
    rb_define_method(gnoCanvasItem, "grab",
                     citem_grab, 3);
    rb_define_method(gnoCanvasItem, "ungrab",
                     citem_ungrab, 1);
    rb_define_method(gnoCanvasItem, "w2i",
                     citem_w2i, 2);
    rb_define_method(gnoCanvasItem, "i2w",
                     citem_i2w, 2);
    rb_define_method(gnoCanvasItem, "i2w_affine",
                     citem_i2w_affine, 0);
    rb_define_method(gnoCanvasItem, "i2c_affine",
                     citem_i2c_affine, 0);
    rb_define_method(gnoCanvasItem, "reparent",
		     citem_reparent, 1);
    rb_define_method(gnoCanvasItem, "grab_focus",
                     citem_grab_focus, 0);
    rb_define_method(gnoCanvasItem, "get_bounds",
                     citem_get_bounds, 0);
    rb_define_method(gnoCanvasItem, "request_update",
                     citem_request_update, 0);
    rb_define_method(gnoCanvasItem, "parent",
                     citem_parent, 0);
    rb_define_method(gnoCanvasItem, "canvas",
                     citem_canvas, 0);

    gnoCanvasLine = rb_define_class_under(mGnome, "CanvasLine", gnoCanvasItem);
    gnoCanvasPolygon = rb_define_class_under(mGnome, "CanvasPolygon", gnoCanvasItem);
    gnoCanvasRect = rb_define_class_under(mGnome, "CanvasRect", gnoCanvasItem);
    gnoCanvasEllipse = rb_define_class_under(mGnome, "CanvasEllipse", gnoCanvasItem);
    gnoCanvasText = rb_define_class_under(mGnome, "CanvasText", gnoCanvasItem);
    gnoCanvasImage = rb_define_class_under(mGnome, "CanvasImage", gnoCanvasItem);
    gnoCanvasWidget = rb_define_class_under(mGnome, "CanvasWidget", gnoCanvasItem);

    /* child init */
    Init_gnome_canvas_group();

    rbgnome_register_citem_type(gnoCanvasLine, GNOME_TYPE_CANVAS_LINE);
    rbgnome_register_citem_type(gnoCanvasPolygon, GNOME_TYPE_CANVAS_POLYGON);
    rbgnome_register_citem_type(gnoCanvasRect, GNOME_TYPE_CANVAS_RECT);
    rbgnome_register_citem_type(gnoCanvasEllipse, GNOME_TYPE_CANVAS_ELLIPSE);
    rbgnome_register_citem_type(gnoCanvasText, GNOME_TYPE_CANVAS_TEXT);
    rbgnome_register_citem_type(gnoCanvasImage, GNOME_TYPE_CANVAS_IMAGE);
    rbgnome_register_citem_type(gnoCanvasWidget, GNOME_TYPE_CANVAS_WIDGET);
    rbgnome_register_citem_type(gnoCanvasGroup, GNOME_TYPE_CANVAS_GROUP);
}
