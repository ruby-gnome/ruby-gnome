/* $Id: rbgnome-canvas-group.c,v 1.3 2002/07/29 15:50:00 mutoh Exp $ */

/* Gnome::Animator widget for Ruby/Gnome
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

VALUE gnoCanvasGroup;

static int
kind_of(klass, target)
    VALUE klass, target;
{
    while (klass) {
	if (klass == target)
	    return 1;
	klass = RCLASS(klass)->super;
    }
    return 0;
}

static VALUE
group_item_new(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE klass, citem;
    GtkType type;
    GnomeCanvasItem* ci;

    if (argc == 0) {
	rb_raise(rb_eArgError, "wrong # of argument.");
    }
    klass = argv[0];
    Check_Type(klass, T_CLASS);
    if (kind_of(klass, gnoCanvasLine)) {
	type = GNOME_TYPE_CANVAS_LINE;
    } else if (kind_of(klass, gnoCanvasPolygon)) {
	type = GNOME_TYPE_CANVAS_POLYGON;
    } else if (kind_of(klass, gnoCanvasRect)) {
	type = GNOME_TYPE_CANVAS_RECT;
    } else if (kind_of(klass, gnoCanvasEllipse)) {
	type = GNOME_TYPE_CANVAS_ELLIPSE;
    } else if (kind_of(klass, gnoCanvasText)) {
	type = GNOME_TYPE_CANVAS_TEXT;
    } else if (kind_of(klass, gnoCanvasImage)) {
	type = GNOME_TYPE_CANVAS_IMAGE;
    } else if (kind_of(klass, gnoCanvasWidget)) {
	type = GNOME_TYPE_CANVAS_WIDGET;
    } else if (kind_of(klass, gnoCanvasGroup)) {
	type = GNOME_TYPE_CANVAS_GROUP;
    } else {
	rb_raise(rb_eTypeError, "wrong argument type (expect a sub-class of Gnome::CanvasItem");
    }

    ci = gnome_canvas_item_new(GNOME_CANVAS_GROUP(get_gobject(self)), type, NULL);
    citem = make_gobject(klass, GTK_OBJECT(ci));
    add_relative(self, citem);
    if (argc > 1) {
	rb_funcall2(citem, rb_intern("set"), argc - 1, &(argv[1]));
    }
    return citem;
}

static VALUE
group_child_bounds(self, item)
    VALUE self, item;
{
    gnome_canvas_group_child_bounds(GNOME_CANVAS_GROUP(get_gobject(self)),
                                    GNOME_CANVAS_ITEM(get_gobject(item)));
    return Qnil;
}

void
Init_gnome_canvas_group()
{
    gnoCanvasGroup = rb_define_class_under(mGnome, "CanvasGroup", gnoCanvasItem);
    rb_define_method(gnoCanvasGroup, "item_new", group_item_new, -1);
    rb_define_method(gnoCanvasGroup, "child_bounds", group_child_bounds, 1);
}
