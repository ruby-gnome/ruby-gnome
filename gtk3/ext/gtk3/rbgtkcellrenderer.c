/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2002-2005 Masao Mutoh
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

#include "global.h"

#define RG_TARGET_NAMESPACE cCellRenderer
#define _SELF(s) (GTK_CELL_RENDERER(RVAL2GOBJ(s)))
#define RVAL2RECT(r) ((GdkRectangle*)RVAL2BOXED(r, GDK_TYPE_RECTANGLE))
#define RECT2RVAL(r) (BOXED2RVAL(r, GDK_TYPE_RECTANGLE))

static VALUE
rg_get_size(VALUE self, VALUE widget, VALUE cell_area)
{
    GdkRectangle ret;

    gtk_cell_renderer_get_size(_SELF(self), GTK_WIDGET(RVAL2GOBJ(widget)),
                               RVAL2RECT(cell_area),
                               &ret.x, &ret.y, &ret.width, &ret.height);
    return RECT2RVAL(&ret);
}

static VALUE
rg_render(VALUE self, VALUE window, VALUE widget, VALUE background_area, VALUE cell_area, VALUE expose_area, VALUE flags)
{
    gtk_cell_renderer_render(_SELF(self), GDK_WINDOW(RVAL2GOBJ(window)),
                             GTK_WIDGET(RVAL2GOBJ(widget)),
                             RVAL2RECT(background_area),
                             RVAL2RECT(cell_area),
                             RVAL2RECT(expose_area),
                             RVAL2GFLAGS(flags, GTK_TYPE_CELL_RENDERER_STATE));
    return self;
}

static VALUE
rg_activate(VALUE self, VALUE event, VALUE widget, VALUE path, VALUE background_area, VALUE cell_area, VALUE flags)
{
    gboolean ret =
    gtk_cell_renderer_activate(_SELF(self), (GdkEvent*)RVAL2GEV(event),
                               GTK_WIDGET(RVAL2GOBJ(widget)),
                               RVAL2CSTR(path), RVAL2RECT(background_area),
                               RVAL2RECT(cell_area), 
                               RVAL2GFLAGS(flags, GTK_TYPE_CELL_RENDERER_STATE));
    return CBOOL2RVAL(ret);
}

static VALUE
rg_start_editing(VALUE self, VALUE event, VALUE widget, VALUE path, VALUE background_area, VALUE cell_area, VALUE flags)
{
    GtkCellEditable* edit =
    gtk_cell_renderer_start_editing(_SELF(self), (GdkEvent*)RVAL2GEV(event),
                               GTK_WIDGET(RVAL2GOBJ(widget)),
                               RVAL2CSTR(path), RVAL2RECT(background_area),
                               RVAL2RECT(cell_area), 
                               RVAL2GFLAGS(flags, GTK_TYPE_CELL_RENDERER_STATE));
    return edit ? GOBJ2RVAL(edit) : Qnil;
}

#if GTK_CHECK_VERSION(2,4,0)
#ifndef GTK_DISABLE_DEPRECATED
static VALUE
rg_editing_canceled(VALUE self)
{
    gtk_cell_renderer_editing_canceled(_SELF(self));
    return self;
}
#endif
#endif

#if GTK_CHECK_VERSION(2,6,0)
static VALUE
rg_stop_editing(VALUE self, VALUE canceled)
{
    gtk_cell_renderer_stop_editing(_SELF(self), RVAL2CBOOL(canceled));
    return self;
}
#endif

static VALUE
rg_fixed_size(VALUE self)
{
    int width, height;
    gtk_cell_renderer_get_fixed_size(_SELF(self), &width, &height);
    return rb_ary_new3(2, INT2NUM(width), INT2NUM(height));
}

static VALUE
rg_set_fixed_size(VALUE self, VALUE width, VALUE height)
{
    gtk_cell_renderer_set_fixed_size(_SELF(self), NUM2INT(width), 
                                     NUM2INT(height));
    return self;
}

void
Init_gtk_cellrenderer(VALUE mGtk)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_CELL_RENDERER, "CellRenderer", mGtk);

    RG_DEF_METHOD(get_size, 2);
    RG_DEF_METHOD(render, 6);
    RG_DEF_METHOD(activate, 6);
    RG_DEF_METHOD(start_editing, 6);
#if GTK_CHECK_VERSION(2,4,0)
#ifndef GTK_DISABLE_DEPRECATED
    RG_DEF_METHOD(editing_canceled, 0);
#endif
#endif
#if GTK_CHECK_VERSION(2,6,0)
    RG_DEF_METHOD(stop_editing, 1);
#endif
    RG_DEF_METHOD(fixed_size, 0);
    RG_DEF_METHOD(set_fixed_size, 2);

    /* GtkCellRendererState */
    G_DEF_CLASS(GTK_TYPE_CELL_RENDERER_STATE, "State", RG_TARGET_NAMESPACE);
    G_DEF_CONSTANTS(RG_TARGET_NAMESPACE, GTK_TYPE_CELL_RENDERER_STATE, "GTK_CELL_RENDERER_");

    /* GtkCellRendererMode */
    G_DEF_CLASS(GTK_TYPE_CELL_RENDERER_MODE, "Mode", RG_TARGET_NAMESPACE);
    G_DEF_CONSTANTS(RG_TARGET_NAMESPACE, GTK_TYPE_CELL_RENDERER_MODE, "GTK_CELL_RENDERER_");
}
