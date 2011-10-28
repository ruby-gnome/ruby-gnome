/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2002,2003 Masao Mutoh
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

#define RG_TARGET_NAMESPACE cAdjustment
#define _SELF(self) (GTK_ADJUSTMENT(RVAL2GOBJ(self)))

static VALUE
adj_initialize(VALUE self, VALUE value, VALUE lower, VALUE upper, VALUE step_inc, VALUE page_inc, VALUE page_size)
{
    RBGTK_INITIALIZE(self, gtk_adjustment_new(NUM2DBL(value),
                                              NUM2DBL(lower),
                                              NUM2DBL(upper),
                                              NUM2DBL(step_inc),
                                              NUM2DBL(page_inc),
                                              NUM2DBL(page_size)));
    return Qnil;
}

static VALUE
adj_clamp_page(VALUE self, VALUE lower, VALUE upper)
{
    gtk_adjustment_clamp_page(_SELF(self), NUM2DBL(lower), NUM2DBL(upper));
    return self;
}

static VALUE
adj_changed(VALUE self)
{
    gtk_adjustment_changed(_SELF(self));
    return self;
}

static VALUE
adj_value_changed(VALUE self)
{
    gtk_adjustment_value_changed(_SELF(self));
    return self;
}

/* Properties:
gdouble             gtk_adjustment_get_value            (GtkAdjustment *adjustment);
void                gtk_adjustment_set_value            (GtkAdjustment *adjustment,
                                                         gdouble value);
gdouble             gtk_adjustment_get_lower            (GtkAdjustment *adjustment);
gdouble             gtk_adjustment_get_page_increment   (GtkAdjustment *adjustment);
gdouble             gtk_adjustment_get_page_size        (GtkAdjustment *adjustment);
gdouble             gtk_adjustment_get_step_increment   (GtkAdjustment *adjustment);
gdouble             gtk_adjustment_get_upper            (GtkAdjustment *adjustment);
void                gtk_adjustment_set_lower            (GtkAdjustment *adjustment,
                                                         gdouble lower);
void                gtk_adjustment_set_page_increment   (GtkAdjustment *adjustment,
                                                         gdouble page_increment);
void                gtk_adjustment_set_page_size        (GtkAdjustment *adjustment,
                                                         gdouble page_size);
void                gtk_adjustment_set_step_increment   (GtkAdjustment *adjustment,
                                                         gdouble step_increment);
void                gtk_adjustment_set_upper            (GtkAdjustment *adjustment,
                                                         gdouble upper);
*/

void 
Init_gtk_adjustment(void)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_ADJUSTMENT, "Adjustment", mGtk);

    rb_define_method(RG_TARGET_NAMESPACE, "initialize", adj_initialize, 6);
    rb_define_method(RG_TARGET_NAMESPACE, "clamp_page", adj_clamp_page, 2);
    rb_define_method(RG_TARGET_NAMESPACE, "changed", adj_changed, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "value_changed", adj_value_changed, 0);
}
