/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/* $Id: rbgnome-canvas-group.c,v 1.2 2002/09/20 16:22:16 tkubo Exp $ */

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
static VALUE item_type_hash;

void
rbgnome_register_citem_type(klass, type)
    VALUE klass;
    GtkType type;
{
    rb_hash_aset(item_type_hash, klass, INT2NUM(type));
}

static VALUE
group_item_new(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE klass, citem, type = Qnil;
    GnomeCanvasItem* ci;

    if (argc == 0) {
        rb_raise(rb_eArgError, "wrong # of argument.");
    }
    Check_Type(argv[0], T_CLASS);
    for (klass = argv[0]; klass != rb_cObject; klass = RCLASS(klass)->super) {
        type = rb_hash_aref(item_type_hash, klass);
        if (!NIL_P(type))
            break;
    }
    if (NIL_P(type)) {
        rb_raise(rb_eTypeError, "wrong argument type (expect a sub-class of Gnome::CanvasItem");
    }
    ci = gnome_canvas_item_new(GNOME_CANVAS_GROUP(get_gobject(self)), NUM2INT(type), NULL);
    citem = make_gobject(argv[0], GTK_OBJECT(ci));
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
    item_type_hash = rb_hash_new();
    rb_global_variable(&item_type_hash);

    gnoCanvasGroup = rb_define_class_under(mGnome, "CanvasGroup", gnoCanvasItem);
    rb_define_method(gnoCanvasGroup, "item_new", group_item_new, -1);
    rb_define_method(gnoCanvasGroup, "child_bounds", group_child_bounds, 1);
}
