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

#include "rbgtk3private.h"

#define RG_TARGET_NAMESPACE cGrid
#define _SELF(self) (RVAL2GTKGRID(self))

static VALUE
rg_initialize(VALUE self)
{
    RBGTK_INITIALIZE(self, gtk_grid_new());

    return Qnil;
}

static VALUE
rg_attach(VALUE self, VALUE child, VALUE left, VALUE top, VALUE width, VALUE height)
{
    gtk_grid_attach(_SELF(self),
                    RVAL2GTKWIDGET(child),
                    NUM2INT(left), NUM2INT(top),
                    NUM2INT(width), NUM2INT(height));

    return self;
}

static VALUE
rg_attach_next_to(VALUE self, VALUE child, VALUE sibling, VALUE side, VALUE width, VALUE height)
{
    gtk_grid_attach_next_to(_SELF(self),
                            RVAL2GTKWIDGET(child),
                            NIL_P(sibling) ? NULL : RVAL2GTKWIDGET(sibling),
                            RVAL2GTKPOSITIONTYPE(side),
                            NUM2INT(width), NUM2INT(height));

    return self;
}

#if GTK_CHECK_VERSION(3, 2, 0)
static VALUE
rg_get_child_at(VALUE self, VALUE left, VALUE top)
{
    return GOBJ2RVAL(gtk_grid_get_child_at(_SELF(self), NUM2INT(left), NUM2INT(top)));
}

static VALUE
rg_insert_column(VALUE self, VALUE position)
{
    gtk_grid_insert_column(_SELF(self), NUM2INT(position));

    return self;
}

static VALUE
rg_insert_next_to(VALUE self, VALUE sibling, VALUE side)
{
    gtk_grid_insert_next_to(_SELF(self),
                            RVAL2GTKWIDGET(sibling),
                            RVAL2GTKPOSITIONTYPE(side));

    return self;
}

static VALUE
rg_insert_row(VALUE self, VALUE position)
{
    gtk_grid_insert_row(_SELF(self), NUM2INT(position));

    return self;
}
#endif

void
Init_gtk_grid(VALUE mGtk)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_GRID, "Grid", mGtk);

    RG_DEF_METHOD(initialize, 0);
    RG_DEF_METHOD(attach, 5);
    RG_DEF_METHOD(attach_next_to, 5);
#if GTK_CHECK_VERSION(3, 2, 0)
    RG_DEF_METHOD(get_child_at, 2);
    RG_DEF_METHOD(insert_column, 1);
    RG_DEF_METHOD(insert_next_to, 2);
    RG_DEF_METHOD(insert_row, 1);
#endif
}
