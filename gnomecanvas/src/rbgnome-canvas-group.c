/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/* $Id: rbgnome-canvas-group.c,v 1.4 2002/09/21 14:53:20 tkubo Exp $ */

/* Gnome::CanvasGroup widget for Ruby/Gnome
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

#include "rbgnomecanvas.h"

#ifdef HAVE_STDARG_PROTOTYPES
#include <stdarg.h>
#define va_init_list(a,b) va_start(a,b)
#else
#include <varargs.h>
#define va_init_list(a,b) va_start(a)
#endif

static void
#ifdef HAVE_STDARG_PROTOTYPES
group_do_item_construct(GnomeCanvasItem *item, GnomeCanvasGroup *parent, const gchar *first_arg_name, ...)
#else
group_do_item_construct(item, parent, first_arg_name, va_alist)
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
group_item_new(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    GnomeCanvasItem *item;
    GnomeCanvasGroup *parent;
    GType type;
    VALUE rval;

    if (argc == 0) 
        rb_raise(rb_eArgError, "wrong # of argument (%d)", argc);
    type = CLASS2GTYPE(argv[0]);

    if (!g_type_is_a(type, GNOME_TYPE_CANVAS_ITEM))
        rb_raise(rb_eArgError, "invalid class %s (expect the subclass of Gnome::CanvasItem", rb_class2name(argv[0]));

    item = GNOME_CANVAS_ITEM(g_object_new(type, NULL));
    rval = rbgobj_create_object(argv[0]);
    RBGTK_INITIALIZE(rval, item);
    parent = GNOME_CANVAS_GROUP(RVAL2GOBJ(self));

	item->parent = GNOME_CANVAS_ITEM(parent);
	item->canvas = item->parent->canvas;
    rbgnomecanvas_do_item_set(argc - 1, argv + 1, rval);
    group_do_item_construct(item, parent, NULL);
    return rval;
}

void
Init_gnome_canvas_group(mGnome)
    VALUE mGnome;
{
    VALUE gnoCanvasGroup = G_DEF_CLASS(GNOME_TYPE_CANVAS_GROUP, "CanvasGroup", mGnome);
    rb_define_method(gnoCanvasGroup, "item_new", group_item_new, -1);
}
