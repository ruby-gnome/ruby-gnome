/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2002,2003 Ruby-GNOME2 Project Team
 *  Copyright (C) 1998-2000 Yukihiro Matsumoto,
 *                          Daisuke Kanda,
 *                          Hiroshi Igarashi
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

#define RG_TARGET_NAMESPACE cScale
#define _SELF(self) (RVAL2GTKSCALE(self))

static VALUE
rg_initialize(int argc, VALUE *argv, VALUE self)
{
    VALUE orientation, arg1, arg2, arg3;
    GtkWidget *widget = NULL;

    rb_scan_args(argc, argv, "13", &orientation, &arg1, &arg2, &arg3);

   if (NIL_P(arg1) || RVAL2GTYPE(arg1) == GTK_TYPE_ADJUSTMENT) {
        widget = gtk_scale_new(RVAL2GTKORIENTATION(orientation),
                               NIL_P(arg1) ? NULL : RVAL2GTKADJUSTMENT(arg1));
    } else {
        widget = gtk_scale_new_with_range(RVAL2GTKORIENTATION(orientation),
                                          NUM2DBL(arg1), NUM2DBL(arg2), NUM2DBL(arg3));
    }
    RBGTK_INITIALIZE(self, widget);

    return Qnil;
}

static VALUE
rg_layout(VALUE self)
{
    return GOBJ2RVAL(gtk_scale_get_layout(_SELF(self)));
}

static VALUE
rg_layout_offsets(VALUE self)
{
    gint x, y;
    gtk_scale_get_layout_offsets(_SELF(self), &x, &y);
    return rb_assoc_new(INT2NUM(x), INT2NUM(y));
}

static VALUE
rg_add_mark(int argc, VALUE *argv, VALUE self)
{
    VALUE value, position, markup;

    rb_scan_args(argc, argv, "21", &value, &position, &markup);

    gtk_scale_add_mark(_SELF(self),
                       NUM2DBL(value),
                       RVAL2GTKPOSITIONTYPE(position),
                       RVAL2CSTR_ACCEPT_NIL(markup));

    return self;
}

static VALUE
rg_clear_marks(VALUE self)
{
    gtk_scale_clear_marks(_SELF(self));

    return self;
}

void 
Init_gtk_scale(VALUE mGtk)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_SCALE, "Scale", mGtk);

    RG_DEF_METHOD(initialize, -1);
    RG_DEF_METHOD(layout, 0);
    RG_DEF_METHOD(layout_offsets, 0);
    RG_DEF_METHOD(add_mark, -1);
    RG_DEF_METHOD(clear_marks, 0);
}
