/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkruler.c -

  $Author: mutoh $
  $Date: 2003/08/31 15:29:44 $

  Copyright (C) 2002,2003 Ruby-GNOME2 Project Team
  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

#define _SELF(self) (GTK_RULER(RVAL2GOBJ(self)))

static VALUE
ruler_set_metric(self, metric)
    VALUE self, metric;
{
    gtk_ruler_set_metric(_SELF(self), RVAL2GENUM(metric, GTK_TYPE_METRIC_TYPE));
    return self;
}

static VALUE
ruler_set_range(self, lower, upper, position, max_size)
    VALUE self, lower, upper, position, max_size;
{
    gtk_ruler_set_range(_SELF(self), NUM2DBL(lower), NUM2DBL(upper),
			NUM2DBL(position), NUM2DBL(max_size));
    return self;
}

static VALUE
ruler_get_metric(self)
    VALUE self;
{
    return INT2FIX(gtk_ruler_get_metric(_SELF(self)));
}

static VALUE
ruler_get_range(self)
    VALUE self;
{
    gdouble lower, upper, position, max_size;
    gtk_ruler_get_range(_SELF(self), &lower, &upper, &position, &max_size);
    return rb_ary_new3(4, rb_float_new(lower), rb_float_new(upper), 
                       rb_float_new(position), rb_float_new(max_size));
}

static VALUE
ruler_draw_ticks(self)
    VALUE self;
{
    gtk_ruler_draw_ticks(_SELF(self));
    return self;
}

static VALUE
ruler_draw_pos(self)
    VALUE self;
{
    gtk_ruler_draw_pos(_SELF(self));
    return self;
}

void 
Init_gtk_ruler()
{
    VALUE gRuler = G_DEF_CLASS(GTK_TYPE_RULER, "Ruler", mGtk);

    rb_define_method(gRuler, "set_metric", ruler_set_metric, 1);
    rb_define_method(gRuler, "set_range", ruler_set_range, 4);
    rb_define_method(gRuler, "metric", ruler_get_metric, 0);
    rb_define_method(gRuler, "range", ruler_get_range, 0);
    rb_define_method(gRuler, "draw_ticks", ruler_draw_ticks, 0);
    rb_define_method(gRuler, "draw_pos", ruler_draw_pos, 0);

    G_DEF_SETTERS(gRuler);
}
