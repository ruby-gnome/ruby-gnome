/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtkviewport.c -

  $Author: sakai $
  $Date: 2002/06/21 18:31:00 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

static VALUE
vport_initialize(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE arg1, arg2;
    GtkAdjustment *h_adj = NULL;
    GtkAdjustment *v_adj = NULL;

    rb_scan_args(argc, argv, "02", &arg1, &arg2);
    if (!NIL_P(arg1)) h_adj = GTK_ADJUSTMENT(get_gobject(arg1));
    if (!NIL_P(arg2)) v_adj = GTK_ADJUSTMENT(get_gobject(arg2));

    set_widget(self, gtk_viewport_new(h_adj, v_adj));
    return Qnil;
}

static VALUE
vport_get_hadj(self)
    VALUE self;
{
    GtkAdjustment *adj = gtk_viewport_get_hadjustment(GTK_VIEWPORT(get_widget(self)));

    return GOBJ2RVAL(adj);
}

static VALUE
vport_get_vadj(self)
    VALUE self;
{
    GtkWidget *widget = get_widget(self);
    GtkAdjustment *adj = gtk_viewport_get_vadjustment(GTK_VIEWPORT(widget));

    return GOBJ2RVAL(adj);
}

static VALUE
vport_set_vadj(self, adj)
    VALUE self, adj;
{
    gtk_viewport_set_vadjustment(GTK_VIEWPORT(get_widget(self)),
				 GTK_ADJUSTMENT(get_gobject(adj)));

    return self;
}

static VALUE
vport_set_hadj(self, adj)
    VALUE self, adj;
{
    gtk_viewport_set_hadjustment(GTK_VIEWPORT(get_widget(self)),
				 GTK_ADJUSTMENT(get_gobject(adj)));

    return self;
}

static VALUE
vport_set_shadow(self, type)
    VALUE self, type;
{
    gtk_viewport_set_shadow_type(GTK_VIEWPORT(get_widget(self)),
				 (GtkShadowType)NUM2INT(type));

    return self;
}

void Init_gtk_viewport()
{
    static rbgtk_class_info cinfo;

    gViewport = rb_define_class_under(mGtk, "Viewport", gBin);
    cinfo.klass = gViewport;
    cinfo.gtype = GTK_TYPE_VIEWPORT;
    cinfo.mark = 0;
    cinfo.free = 0;
    rbgtk_register_class(&cinfo);

    rb_define_method(gViewport, "initialize", vport_initialize, -1);
    rb_define_method(gViewport, "get_hadjustment", vport_get_hadj, 0);
    rb_define_method(gViewport, "get_vadjustment", vport_get_vadj, 0);
    rb_define_method(gViewport, "set_hadjustment", vport_set_hadj, 1);
    rb_define_method(gViewport, "set_vadjustment", vport_set_vadj, 1);
    rb_define_method(gViewport, "set_shadow_type", vport_set_shadow, 1);
}
