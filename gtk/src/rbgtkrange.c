/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtkrange.c -

  $Author: sakai $
  $Date: 2002/08/01 17:46:18 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

static VALUE
range_get_adj(self)
	 VALUE self;
{
    return GOBJ2RVAL(gtk_range_get_adjustment(GTK_RANGE(RVAL2GOBJ(self))));
}

static VALUE
range_set_update_policy(self, policy)
    VALUE self, policy;
{
    gtk_range_set_update_policy(GTK_RANGE(RVAL2GOBJ(self)),
				(GtkUpdateType)NUM2INT(policy));
    return self;
}

static VALUE
range_set_adj(self, adj)
    VALUE self, adj;
{
	if (! NIL_P(adj))
	  gtk_range_set_adjustment(GTK_RANGE(RVAL2GOBJ(self)),
							   GTK_ADJUSTMENT(RVAL2GOBJ(adj)));

    return self;
}

void 
Init_gtk_range()
{
  VALUE gRange = G_DEF_CLASS(GTK_TYPE_RANGE, "Range", mGtk);

  rb_define_method(gRange, "get_adjustment", range_get_adj, 0);
  rb_define_method(gRange, "set_update_policy", range_set_update_policy, 1);
  rb_define_method(gRange, "set_adjustment", range_set_adj, 1);
}
