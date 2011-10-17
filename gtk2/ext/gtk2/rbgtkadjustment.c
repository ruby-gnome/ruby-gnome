/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkadjustment.c -

  $Author: mutoh $
  $Date: 2003/01/19 14:28:24 $

  Copyright (C) 2002,2003 Masao Mutoh

  This file is derived from rbgtkdata.c.
  rbgtkdata.c -
  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

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
    VALUE gAdjustment = G_DEF_CLASS(GTK_TYPE_ADJUSTMENT, "Adjustment", mGtk);

    rb_define_method(gAdjustment, "initialize", adj_initialize, 6);
    rb_define_method(gAdjustment, "clamp_page", adj_clamp_page, 2);
    rb_define_method(gAdjustment, "changed", adj_changed, 0);
    rb_define_method(gAdjustment, "value_changed", adj_value_changed, 0);
}
