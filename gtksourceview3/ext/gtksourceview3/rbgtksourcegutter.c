/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
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

#include "rbgtksourceview3private.h"

#define RG_TARGET_NAMESPACE cGutter
#define _SELF(self) (RVAL2GTKSOURCEGUTTER(self))

static VALUE
rg_window(VALUE self)
{
    return GOBJ2RVAL(gtk_source_gutter_get_window(_SELF(self)));
}

static VALUE
rg_insert(VALUE self, VALUE renderer, VALUE position)
{
    gboolean result;

    result = gtk_source_gutter_insert(_SELF(self),
                                      RVAL2GTKSOURCEGUTTERRENDERER(renderer),
                                      NUM2INT(position));
    G_CHILD_ADD(self, renderer);

    return CBOOL2RVAL(result);
}

static VALUE
rg_queue_draw(VALUE self)
{
    gtk_source_gutter_queue_draw(_SELF(self));

    return self;
}

static VALUE
rg_remove(VALUE self, VALUE renderer)
{
    gtk_source_gutter_remove(_SELF(self), RVAL2GTKSOURCEGUTTERRENDERER(renderer));

    return self;
}

static VALUE
rg_reorder(VALUE self, VALUE renderer, VALUE position)
{
    gtk_source_gutter_reorder(_SELF(self),
                              RVAL2GTKSOURCEGUTTERRENDERER(renderer),
                              NUM2INT(position));

    return self;
}

void
Init_gtksource_gutter(VALUE mGtkSource)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_SOURCE_TYPE_GUTTER, "Gutter", mGtkSource);

    RG_DEF_METHOD(window, 0);
    RG_DEF_METHOD(insert, 2);
    RG_DEF_METHOD(queue_draw, 0);
    RG_DEF_METHOD(remove, 1);
    RG_DEF_METHOD(reorder, 2);
}
