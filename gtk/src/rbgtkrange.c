/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtkrange.c -

  $Author: mutoh $
  $Date: 2002/05/19 15:48:28 $

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

void Init_gtk_range()
{
  gRange = rb_define_class_under(mGtk, "Range", gWidget);

  rb_define_method(gRange, "get_adjustment", range_get_adj, 0);
  rb_define_method(gRange, "set_update_policy", range_set_update_policy, 1);
  rb_define_method(gRange, "set_adjustment", range_set_adj, 1);

  /* child init */
  Init_gtk_scale();
  Init_gtk_scrollbar();
}
