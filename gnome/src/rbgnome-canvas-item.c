/* $Id: rbgnome-canvas-item.c,v 1.4 2002/08/06 12:45:23 mutoh Exp $ */

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

static void
set_gtkarg(type, name, value, arg)
    GtkType type;
    char *name;
    VALUE value;
    GtkArg *arg;
{
    GtkArgInfo *info;
    char *error;
    VALUE exc;

    error = gtk_object_arg_get_info(type, name, &info);
    if (error != NULL) {
	exc = rb_exc_new2(rb_eArgError, error);
	g_free(error);
	rb_exc_raise(exc);
    }
    arg->name = name;
    arg->type = info->type;
    switch (arg->type) {
    case GTK_TYPE_UINT:
	GTK_VALUE_UINT(*arg) = NUM2UINT(value);
	return;
    case GTK_TYPE_DOUBLE:
	GTK_VALUE_DOUBLE(*arg) = NUM2DBL(value);
	return;
    case GTK_TYPE_STRING:
	if (NIL_P(value)) {
	    GTK_VALUE_STRING(*arg) = NULL;
	} else {
	    Check_Type(value, T_STRING);
	    GTK_VALUE_STRING(*arg) = STR2CSTR(value);
	}
	return;
    case GTK_TYPE_BOOL:
	switch (value) {
	case Qtrue:
	    GTK_VALUE_BOOL(*arg) = TRUE;
	    break;
	case Qfalse:
	    GTK_VALUE_BOOL(*arg) = FALSE;
	    break;
	default:
	    rb_raise(rb_eTypeError, "wrong argument type for %s (except a boolean)", arg->name);
	}
	return;
    case GTK_TYPE_BOXED:
	rb_notimplement(); /* FIXME */
	return;
    case GTK_TYPE_OBJECT:
	GTK_VALUE_OBJECT (*arg) = (gpointer)get_gobject(value);
	return;
    }
    if (arg->type == GTK_TYPE_JUSTIFICATION) {
	GTK_VALUE_ENUM(*arg) = NUM2UINT(value);
    } else if (arg->type == GTK_TYPE_ANCHOR_TYPE) {
	GTK_VALUE_ENUM(*arg) = NUM2UINT(value);
    } else if (arg->type == GTK_TYPE_GDK_CAP_STYLE) {
	GTK_VALUE_ENUM(*arg) = NUM2UINT(value);
    } else if (arg->type == GTK_TYPE_GDK_JOIN_STYLE) {
	GTK_VALUE_ENUM(*arg) = NUM2UINT(value);
    } else if (arg->type == GTK_TYPE_GDK_LINE_STYLE) {
	GTK_VALUE_ENUM(*arg) = NUM2UINT(value);
    } else if (arg->type == GTK_TYPE_GDK_COLOR) {
	GTK_VALUE_BOXED(*arg) = get_gdkcolor(value);
    } else if (arg->type == GTK_TYPE_GDK_WINDOW) {
	GTK_VALUE_BOXED(*arg) = get_gdkbitmap(value);
    } else if (arg->type == GTK_TYPE_GDK_FONT) {
	GTK_VALUE_BOXED(*arg) = get_gdkfont(value);
    } else if (arg->type == GTK_TYPE_GNOME_CANVAS_POINTS) {
	GnomeCanvasPoints *gcp;

	if (!rb_obj_is_kind_of(value, gnoCanvasPoints)) {
	    rb_raise(rb_eTypeError, "not a GnomeCanvasPoints");
	}
	Data_Get_Struct(value, GnomeCanvasPoints, gcp);
	GTK_VALUE_POINTER(*arg) = gcp;
#ifdef HAVE_GDKIMLIB
    } else if (arg->type == GTK_TYPE_GDK_IMLIB_IMAGE) {
	GdkImlibImage *im;

	if (rb_obj_is_kind_of(value, cImlibImage) != Qtrue) {
	    rb_raise(rb_eTypeError, "not a GdkImlibImage");
	}
	Data_Get_Struct(value, GdkImlibImage, im);
	GTK_VALUE_POINTER(*arg) = im;
#endif
    } else {
	rb_raise(rb_eTypeError, "wrong argument name %s", arg->name);
    }
    
}

static VALUE
citem_set(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    GnomeCanvasItem* ci;
    GtkArg *arg;
    VALUE *ary; /* push temporary string to stack to prevent freed. */
    GtkType type;
    char *type_name;
    char *arg_name_prefix;
    int i, narg;

    if (argc & 1) {
	rb_raise(rb_eArgError, "wrong # of argument. The pairs of 'name' and 'value' are borken.");
    }
    ci = GNOME_CANVAS_ITEM(get_gobject(self));

    /* setup arg_name_prefix */
    type = GTK_OBJECT_TYPE(ci);
    if (gtk_type_parent(type) != GNOME_TYPE_CANVAS_ITEM) {
	type = gtk_type_parent(type);
    }
    type_name = gtk_type_name(type);
    arg_name_prefix = ALLOCA_N(char, strlen(type_name) + 3);
    strcpy(arg_name_prefix, type_name);
    strcat(arg_name_prefix, "::");

    narg = argc / 2;
    arg = ALLOCA_N(GtkArg, narg);
    ary = ALLOCA_N(VALUE, narg);

    for (i = 0;i < narg; i++) {
	VALUE arg_name;
	Check_Type(argv[i * 2], T_STRING);
	arg_name = rb_str_new2(arg_name_prefix);
	arg_name = rb_str_append(arg_name, argv[i * 2]);
	ary[i] = arg_name;
	set_gtkarg(GTK_OBJECT_TYPE(ci), STR2CSTR(arg_name), argv[i * 2 + 1], &(arg[i]));
    }
    gnome_canvas_item_setv(ci, narg, arg);
    return Qnil;
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
citem_scale(self, x, y, scale_x, scale_y)
    VALUE self, x, y, scale_x, scale_y;
{
    gnome_canvas_item_scale(GNOME_CANVAS_ITEM(get_gobject(self)),
                            NUM2DBL(x),
                            NUM2DBL(y),
                            NUM2DBL(scale_x),
                            NUM2DBL(scale_y));
    return Qnil;
}

static VALUE
citem_rotate(self, x, y, angle)
    VALUE self, x, y, angle;
{
    gnome_canvas_item_rotate(GNOME_CANVAS_ITEM(get_gobject(self)),
                             NUM2DBL(x),
                             NUM2DBL(y),
                             NUM2DBL(angle));
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
                                   NIL_P(cursor) ? get_gdkcursor(cursor) : NULL,
                                   NIL_P(etime)  ? NUM2INT(etime) : 0));
}

static VALUE
citem_ungrab(self, etime)
    VALUE self, etime;
{
    gnome_canvas_item_ungrab(GNOME_CANVAS_ITEM(get_gobject(self)),
			     NIL_P(etime) ? NUM2INT(etime) : 0);
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
    rb_define_method(gnoCanvasItem, "move",
                     citem_move, 2);
    rb_define_method(gnoCanvasItem, "affine_relative",
                     citem_affine_relative, 1);
    rb_define_method(gnoCanvasItem, "affine_absolute",
                     citem_affine_absolute, 1);
    rb_define_method(gnoCanvasItem, "scale",
                     citem_scale, 4);
    rb_define_method(gnoCanvasItem, "rotate",
                     citem_rotate, 3);
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
}
