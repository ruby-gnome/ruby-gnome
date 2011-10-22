/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2002-2006 Ruby-GNOME2 Project Team
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

#define _SELF(self) (GTK_RANGE(RVAL2GOBJ(self)))

/* Defined as Properties 
GtkAdjustment* gtk_range_get_adjustment     (GtkRange *range);
void        gtk_range_set_adjustment        (GtkRange *range,
                                             GtkAdjustment *adjustment);
void        gtk_range_set_update_policy     (GtkRange *range,
                                             GtkUpdateType policy);
GtkUpdateType gtk_range_get_update_policy   (GtkRange *range);
gboolean    gtk_range_get_inverted          (GtkRange *range);
void        gtk_range_set_inverted          (GtkRange *range,
                                             gboolean setting);
void        gtk_range_set_upper_stepper_sensitivity
                                            (GtkRange *range,
                                             GtkSensitivityType sensitivity);
GtkSensitivityType gtk_range_get_upper_stepper_sensitivity
                                            (GtkRange *range);
void        gtk_range_set_lower_stepper_sensitivity
                                            (GtkRange *range,
                                             GtkSensitivityType sensitivity);
GtkSensitivityType gtk_range_get_lower_stepper_sensitivity
                                            (GtkRange *range);
void               gtk_range_set_show_fill_level           (GtkRange      *range,
                                                            gboolean       show_fill_level);
gboolean           gtk_range_get_show_fill_level           (GtkRange      *range);
void               gtk_range_set_restrict_to_fill_level    (GtkRange      *range,
                                                            gboolean       restrict_to_fill_level);
gboolean           gtk_range_get_restrict_to_fill_level    (GtkRange      *range);
void               gtk_range_set_fill_level                (GtkRange      *range,
                                                            gdouble        fill_level);
gdouble            gtk_range_get_fill_level                (GtkRange      *range);
*/

static VALUE
range_get_value(VALUE self)
{
    return rb_float_new(gtk_range_get_value(_SELF(self)));
}

static VALUE
range_set_increments(VALUE self, VALUE step, VALUE page)
{
    gtk_range_set_increments(_SELF(self), NUM2DBL(step), NUM2DBL(page));
    return self;
}

static VALUE
range_set_range(VALUE self, VALUE min, VALUE max)
{
    gtk_range_set_range(_SELF(self), NUM2DBL(min), NUM2DBL(max));
    return self;
}

static VALUE
range_set_value(VALUE self, VALUE value)
{
    gtk_range_set_value(_SELF(self), NUM2DBL(value));
    return self;
}

void 
Init_gtk_range(void)
{
  VALUE gRange = G_DEF_CLASS(GTK_TYPE_RANGE, "Range", mGtk);

  rb_define_method(gRange, "value", range_get_value, 0);
  rb_define_method(gRange, "set_increments", range_set_increments, 2);
  rb_define_method(gRange, "set_range", range_set_range, 2);
  rb_define_method(gRange, "set_value", range_set_value, 1);
  G_DEF_SETTER(gRange, "value");

#if GTK_CHECK_VERSION(2,10,0)
  /* GtkSensitivityType */
  G_DEF_CLASS(GTK_TYPE_SENSITIVITY_TYPE, "SensitivityType", gRange);
  G_DEF_CONSTANTS(gRange, GTK_TYPE_SENSITIVITY_TYPE, "GTK_");
#endif
}
