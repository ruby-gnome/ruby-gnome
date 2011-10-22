/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2002,2004 Ruby-GNOME2 Project Team
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

#include "global.h"

#define _SELF(self) (GTK_SPIN_BUTTON(RVAL2GOBJ(self)))

static VALUE
sbtn_initialize(int argc, VALUE *argv, VALUE self)
{
    VALUE arg1, arg2, arg3;
    GtkAdjustment *adj = NULL;
    gfloat climb_rate;
    gint digits;
    GtkWidget *widget;

    rb_scan_args(argc, argv, "03", &arg1, &arg2, &arg3);

    if (NIL_P(arg1) || RVAL2GTYPE(arg1) == GTK_TYPE_ADJUSTMENT){
        if (!NIL_P(arg1)) adj = GTK_ADJUSTMENT(RVAL2GOBJ(arg1));
        climb_rate = (NIL_P(arg2))? 0.0: NUM2DBL(arg2);
        digits     = (NIL_P(arg3))?   0: NUM2UINT(arg3);
        widget = gtk_spin_button_new(adj, climb_rate, digits);
    } else {
        widget = gtk_spin_button_new_with_range(NUM2DBL(arg1), 
                                                NUM2DBL(arg2), NUM2DBL(arg3));
    }
    RBGTK_INITIALIZE(self, widget);
    return Qnil;
}

static VALUE
sbtn_configure(VALUE self, VALUE adj, VALUE climb_rate, VALUE digits)
{
    gtk_spin_button_configure(_SELF(self), GTK_ADJUSTMENT(RVAL2GOBJ(adj)),
                              NUM2DBL(climb_rate), NUM2UINT(digits));
    return self;
}

static VALUE
sbtn_set_increments(VALUE self, VALUE step, VALUE page)
{
    gtk_spin_button_set_increments(_SELF(self), NUM2DBL(step), NUM2DBL(page));
    return self;
}

static VALUE
sbtn_set_range(VALUE self, VALUE min, VALUE max)
{
    gtk_spin_button_set_range(_SELF(self), NUM2DBL(min), NUM2DBL(max));
    return self;
}


static VALUE
sbtn_get_value_as_int(VALUE self)
{
    return INT2NUM(gtk_spin_button_get_value_as_int(_SELF(self)));
}

static VALUE
sbtn_spin(VALUE self, VALUE direction, VALUE increment)
{
    gtk_spin_button_spin(_SELF(self), RVAL2GENUM(direction, GTK_TYPE_SPIN_TYPE), 
                         NUM2DBL(increment));
    return self;
}

static VALUE
sbtn_update(VALUE self)
{
    gtk_spin_button_update(_SELF(self));
    return self;
}

static VALUE
sbtn_get_increments(VALUE self)
{
    gdouble step, page;
    gtk_spin_button_get_increments(_SELF(self), &step, &page);

    return rb_ary_new3(2, rb_float_new(step), rb_float_new(page));
}

static VALUE
sbtn_get_range(VALUE self)
{
    gdouble min, max;
    gtk_spin_button_get_range(_SELF(self), &min, &max);

    return rb_ary_new3(2, rb_float_new(min), rb_float_new(max));
}

void 
Init_gtk_spin_button(void)
{
    VALUE gSButton = G_DEF_CLASS(GTK_TYPE_SPIN_BUTTON, "SpinButton", mGtk);

    rb_define_method(gSButton, "initialize", sbtn_initialize, -1);
    rb_define_method(gSButton, "configure", sbtn_configure, 3);
    rb_define_method(gSButton, "set_increments", sbtn_set_increments, 2);
    rb_define_method(gSButton, "set_range", sbtn_set_range, 2);
    rb_define_method(gSButton, "value_as_int", sbtn_get_value_as_int, 0);
    rb_define_method(gSButton, "spin", sbtn_spin, 2);
    rb_define_method(gSButton, "update", sbtn_update, 0);
    rb_define_method(gSButton, "increments", sbtn_get_increments, 0);
    rb_define_method(gSButton, "range", sbtn_get_range, 0);

    /* GtkSpinType */
    G_DEF_CLASS(GTK_TYPE_SPIN_TYPE, "Type", gSButton);
    G_DEF_CONSTANTS(gSButton, GTK_TYPE_SPIN_TYPE, "GTK_SPIN_");

    /* GtkSpinButtonUpdatePolicy */
    G_DEF_CLASS(GTK_TYPE_SPIN_BUTTON_UPDATE_POLICY, "UpdatePolicy", gSButton);
    G_DEF_CONSTANTS(gSButton, GTK_TYPE_SPIN_BUTTON_UPDATE_POLICY, "GTK_");
}
