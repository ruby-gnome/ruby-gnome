/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkhscale.c -

  $Author: mutoh $
  $Date: 2003/01/19 14:28:25 $

  Copyright (C) 2002,2003 The Ruby-GNOME2 Project
  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

static VALUE
hscale_initialize(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE arg1, arg2, arg3;
    GtkAdjustment *adj = NULL;
    GtkWidget *widget;

    rb_scan_args(argc, argv, "03", &arg1, &arg2, &arg3);

    if (NIL_P(arg1) || RVAL2GTYPE(arg1) == GTK_TYPE_ADJUSTMENT){
        if (!NIL_P(arg1)) adj = GTK_ADJUSTMENT(RVAL2GOBJ(arg1));
        widget = gtk_hscale_new(adj);
    } else {
        widget = gtk_hscale_new_with_range(NUM2DBL(arg1), 
                                           NUM2DBL(arg2), NUM2DBL(arg3));
    }
    RBGTK_INITIALIZE(self, widget);
    return Qnil;
}

void 
Init_gtk_hscale()
{
    VALUE gHScale = G_DEF_CLASS(GTK_TYPE_HSCALE, "HScale", mGtk);
    rb_define_method(gHScale, "initialize", hscale_initialize, -1);
}
