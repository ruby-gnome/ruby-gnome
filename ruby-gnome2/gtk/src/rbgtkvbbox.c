/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtkvbbox.c -

  $Author: mutoh $
  $Date: 2002/05/19 12:29:35 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

static VALUE
vbbox_initialize(self)
    VALUE self;
{
    set_widget(self, gtk_vbutton_box_new());
    return Qnil;
}

static VALUE
vbbox_get_spacing_default(self)
    VALUE self;
{
    int i = gtk_vbutton_box_get_spacing_default();
    
    return INT2FIX(i);
}

static VALUE
vbbox_get_layout_default(self)
    VALUE self;
{
    int i = gtk_vbutton_box_get_layout_default();
    
    return INT2FIX(i);
}

static VALUE
vbbox_set_spacing_default(self, spacing)
    VALUE self, spacing;
{
    gtk_vbutton_box_set_spacing_default(NUM2INT(spacing));
    return Qnil;
}

static VALUE
vbbox_set_layout_default(self, layout)
    VALUE self, layout;
{
    gtk_vbutton_box_set_layout_default(NUM2INT(layout));
    return Qnil;
}

void Init_gtk_vbutton_box()
{
    gVBBox = rb_define_class_under(mGtk, "VButtonBox", gBBox);

    rb_define_method(gVBBox, "initialize", vbbox_initialize, 0);
    rb_define_singleton_method(gVBBox, "get_spacing_default",
			       vbbox_get_spacing_default, 0);
    rb_define_singleton_method(gVBBox, "get_layout_default",
			       vbbox_get_layout_default, 0);
    rb_define_singleton_method(gVBBox, "set_spacing_default",
			       vbbox_set_spacing_default, 1);
    rb_define_singleton_method(gVBBox, "set_layout_default",
			       vbbox_set_layout_default, 1);
}
