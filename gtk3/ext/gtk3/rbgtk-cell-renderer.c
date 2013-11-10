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

#include "rbgtk3private.h"

#define RG_TARGET_NAMESPACE cCellRenderer
#define _SELF(s) (RVAL2GTKCELLRENDERER(s))

static VALUE
rg_get_preferred_size(VALUE self, VALUE widget)
{
    GtkRequisition minimum_size, natural_size;

    gtk_cell_renderer_get_preferred_size(_SELF(self),
                                         RVAL2GTKWIDGET(widget),
                                         &minimum_size, &natural_size);

    return rb_ary_new3(2,
                       NIL_P(&minimum_size) ? Qnil : GTKREQUISITION2RVAL(&minimum_size),
                       NIL_P(&natural_size) ? Qnil : GTKREQUISITION2RVAL(&natural_size));
}

/* TODO
static VALUE
rg_render(VALUE self, VALUE window, VALUE widget, VALUE background_area, VALUE cell_area, VALUE expose_area, VALUE flags)
{
    gtk_cell_renderer_render(_SELF(self), RVAL2GDKWINDOW(window),
                             RVAL2GTKWIDGET(widget),
                             RVAL2GDKRECTANGLE(background_area),
                             RVAL2GDKRECTANGLE(cell_area),
                             RVAL2GDKRECTANGLE(expose_area),
                             RVAL2GTKCELLRENDERERSTATE(flags));
    return self;
}
*/

static VALUE
rg_activate(VALUE self, VALUE event, VALUE widget, VALUE path, VALUE background_area, VALUE cell_area, VALUE flags)
{
    gboolean ret =
    gtk_cell_renderer_activate(_SELF(self), (GdkEvent*)RVAL2GEV(event),
                               RVAL2GTKWIDGET(widget),
                               RVAL2CSTR(path), RVAL2GDKRECTANGLE(background_area),
                               RVAL2GDKRECTANGLE(cell_area), 
                               RVAL2GTKCELLRENDERERSTATE(flags));
    return CBOOL2RVAL(ret);
}

static VALUE
rg_start_editing(VALUE self, VALUE event, VALUE widget, VALUE path, VALUE background_area, VALUE cell_area, VALUE flags)
{
    GtkCellEditable* edit =
    gtk_cell_renderer_start_editing(_SELF(self), (GdkEvent*)RVAL2GEV(event),
                               RVAL2GTKWIDGET(widget),
                               RVAL2CSTR(path), RVAL2GDKRECTANGLE(background_area),
                               RVAL2GDKRECTANGLE(cell_area), 
                               RVAL2GTKCELLRENDERERSTATE(flags));
    return edit ? GOBJ2RVAL(edit) : Qnil;
}

static VALUE
rg_stop_editing(VALUE self, VALUE canceled)
{
    gtk_cell_renderer_stop_editing(_SELF(self), RVAL2CBOOL(canceled));
    return self;
}

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

    RG_DEF_METHOD(get_preferred_size, 1);
/* TODO
    RG_DEF_METHOD(render, 6);
*/
    RG_DEF_METHOD(activate, 6);
    RG_DEF_METHOD(start_editing, 6);
    RG_DEF_METHOD(stop_editing, 1);
    RG_DEF_METHOD(fixed_size, 0);
    RG_DEF_METHOD(set_fixed_size, 2);

    G_DEF_CLASS(GTK_TYPE_CELL_RENDERER_STATE, "State", RG_TARGET_NAMESPACE);
    G_DEF_CLASS(GTK_TYPE_CELL_RENDERER_MODE, "Mode", RG_TARGET_NAMESPACE);
}
