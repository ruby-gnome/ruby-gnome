/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtkrange.c -

  $Author: mutoh $
  $Date: 2002/05/19 12:39:08 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

static VALUE
range_get_adj(self)
	 VALUE self;
{
    GtkWidget *widget = get_widget(self);
    GtkAdjustment *adj = gtk_range_get_adjustment(GTK_RANGE(widget));

    return make_gobject(gAdjustment, GTK_OBJECT(adj));
}

static VALUE
range_set_update_policy(self, policy)
    VALUE self, policy;
{
    gtk_range_set_update_policy(GTK_RANGE(get_widget(self)),
				(GtkUpdateType)NUM2INT(policy));
    return self;
}

static VALUE
range_set_adj(self, adj)
    VALUE self, adj;
{
	if (! NIL_P(adj))
	  gtk_range_set_adjustment(GTK_RANGE(get_widget(self)),
							   GTK_ADJUSTMENT(get_gobject(adj)));

    return self;
}

static VALUE
range_draw_bg(self)
    VALUE self;
{
    gtk_range_draw_background(GTK_RANGE(get_widget(self)));
    return self;
}

static VALUE
range_draw_trough(self)
    VALUE self;
{
    gtk_range_draw_trough(GTK_RANGE(get_widget(self)));
    return self;
}

static VALUE
range_draw_slider(self)
    VALUE self;
{
    gtk_range_draw_slider(GTK_RANGE(get_widget(self)));
    return self;
}

static VALUE
range_draw_step_forw(self)
    VALUE self;
{
    gtk_range_draw_step_forw(GTK_RANGE(get_widget(self)));
    return self;
}

static VALUE
range_draw_step_back(self)
    VALUE self;
{
    gtk_range_draw_step_back(GTK_RANGE(get_widget(self)));
    return self;
}

static VALUE
range_slider_update(self)
    VALUE self;
{
    gtk_range_slider_update(GTK_RANGE(get_widget(self)));
    return self;
}

static VALUE
range_trough_click(self, x, y)
    VALUE self, x, y;
{
    int i;

    i = gtk_range_trough_click(GTK_RANGE(get_widget(self)),
			       NUM2INT(x), NUM2INT(y),
			       0);
    return INT2FIX(i);
}

static VALUE
range_default_hslider_update(self)
    VALUE self;
{
    gtk_range_default_hslider_update(GTK_RANGE(get_widget(self)));
    return self;
}

static VALUE
range_default_vslider_update(self)
    VALUE self;
{
    gtk_range_default_vslider_update(GTK_RANGE(get_widget(self)));
    return self;
}

static VALUE
range_default_htrough_click(self, x, y)
    VALUE self, x, y;
{
    gfloat jump_perc;
    int i = gtk_range_default_htrough_click(GTK_RANGE(get_widget(self)),
					NUM2INT(x), NUM2INT(y), &jump_perc);
    return rb_ary_new3(2, INT2FIX(i), rb_float_new(jump_perc));
}

static VALUE
range_default_vtrough_click(self, x, y)
    VALUE self, x, y;
{
    gfloat jump_perc;
    int i = gtk_range_default_vtrough_click(GTK_RANGE(get_widget(self)),
					NUM2INT(x), NUM2INT(y), &jump_perc);
    return rb_ary_new3(2, INT2FIX(i), rb_float_new(jump_perc));
}

static VALUE
range_default_hmotion(self, xdelta, ydelta)
    VALUE self, xdelta, ydelta;
{
    gtk_range_default_hmotion(GTK_RANGE(get_widget(self)),
			      NUM2INT(xdelta), NUM2INT(ydelta));
    return self;
}

static VALUE
range_default_vmotion(self, xdelta, ydelta)
    VALUE self, xdelta, ydelta;
{
    gtk_range_default_vmotion(GTK_RANGE(get_widget(self)),
			      NUM2INT(xdelta), NUM2INT(ydelta));
    return self;
}

void Init_gtk_range()
{
  gRange = rb_define_class_under(mGtk, "Range", gWidget);

  rb_define_method(gRange, "get_adjustment", range_get_adj, 0);
  rb_define_method(gRange, "set_update_policy", range_set_update_policy, 1);
  rb_define_method(gRange, "set_adjustment", range_set_adj, 1);
  rb_define_method(gRange, "draw_background", range_draw_bg, 0);
  rb_define_method(gRange, "draw_trough", range_draw_trough, 0);
  rb_define_method(gRange, "draw_slider", range_draw_slider, 0);
  rb_define_method(gRange, "draw_step_forw", range_draw_step_forw, 0);
  rb_define_method(gRange, "draw_step_back", range_draw_step_back, 0);
  rb_define_method(gRange, "slider_update", range_slider_update, 0);
  rb_define_method(gRange, "trough_click", range_trough_click, 2);
  rb_define_method(gRange, "draw_background", range_draw_bg, 2);
  rb_define_method(gRange, "default_hslider_update", range_default_hslider_update, 0);
  rb_define_method(gRange, "default_vslider_update", range_default_vslider_update, 0);
  rb_define_method(gRange, "default_htrough_click", range_default_htrough_click, 2);
  rb_define_method(gRange, "default_vtrough_click", range_default_vtrough_click, 2);
  rb_define_method(gRange, "default_hmotion", range_default_hmotion, 2);
  rb_define_method(gRange, "default_vmotion", range_default_vmotion, 2);

  /* child init */
  Init_gtk_scale();
  Init_gtk_scrollbar();
}
