/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtkruler.c -

  $Author: mutoh $
  $Date: 2002/07/31 17:23:54 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

static VALUE
ruler_set_metric(self, metric)
    VALUE self, metric;
{
    gtk_ruler_set_metric(GTK_RULER(RVAL2GOBJ(self)), 
			 (GtkMetricType)NUM2INT(metric));

    return self;
}

static VALUE
ruler_set_range(self, lower, upper, position, max_size)
    VALUE self, lower, upper, position, max_size;
{
    gtk_ruler_set_range(GTK_RULER(RVAL2GOBJ(self)), 
			NUM2DBL(lower), NUM2DBL(upper),
			NUM2DBL(position), NUM2DBL(max_size));

    return self;
}

static VALUE
ruler_draw_ticks(self)
    VALUE self;
{
    gtk_ruler_draw_ticks(GTK_RULER(RVAL2GOBJ(self)));
    return self;
}

static VALUE
ruler_draw_pos(self)
    VALUE self;
{
    gtk_ruler_draw_pos(GTK_RULER(RVAL2GOBJ(self)));
    return self;
}

void 
Init_gtk_ruler()
{
    VALUE gRuler = G_DEF_CLASS(GTK_TYPE_RULER, "Ruler", mGtk);

    rb_define_method(gRuler, "set_metric", ruler_set_metric, 1);
    rb_define_method(gRuler, "set_range", ruler_set_range, 4);
    rb_define_method(gRuler, "draw_ticks", ruler_draw_ticks, 0);
    rb_define_method(gRuler, "draw_pos", ruler_draw_pos, 0);

    /* child initialize */
    Init_gtk_hruler();
    Init_gtk_vruler();
}
