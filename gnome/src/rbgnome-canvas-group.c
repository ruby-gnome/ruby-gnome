/* $Id: rbgnome-canvas-group.c,v 1.1 2002/05/19 12:39:24 mutoh Exp $ */

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

static VALUE
group_child_bounds(self, item)
    VALUE self, item;
{
    gnome_canvas_group_child_bounds(GNOME_CANVAS_GROUP(get_widget(self)),
                                    GNOME_CANVAS_ITEM(get_widget(item)));
    return Qnil;
}

void
Init_gnome_canvas_group()
{
    gnoCanvasGroup = rb_define_class_under(mGnome, "CanvasGroup", gnoCanvasItem);
    rb_define_method(gnoCanvasGroup, "child_bounds", group_child_bounds, 1);
}
