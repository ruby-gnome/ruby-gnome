/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtkbbox.c -

  $Author: mutoh $
  $Date: 2002/05/19 12:29:31 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

static VALUE
bbox_get_child_size_default(self)
    VALUE self;
{
    int min_width, max_width;

    gtk_button_box_get_child_size_default(&min_width, &max_width);

    return rb_assoc_new(INT2FIX(min_width), INT2FIX(max_width));
}

static VALUE
bbox_get_child_ipadding_default(self)
    VALUE self;
{
    int ipad_x, ipad_y;

    gtk_button_box_get_child_ipadding_default(&ipad_x, &ipad_y);
    return rb_assoc_new(INT2FIX(ipad_x), INT2FIX(ipad_y));
}

static VALUE
bbox_set_child_size_default(self, min_width, max_width)
    VALUE self, min_width, max_width;
{
    gtk_button_box_set_child_size_default(NUM2INT(min_width),
					  NUM2INT(max_width));
    return Qnil;
}

static VALUE
bbox_set_child_ipadding_default(self, ipad_x, ipad_y)
    VALUE self, ipad_x, ipad_y;
{
    gtk_button_box_set_child_ipadding_default(NUM2INT(ipad_x),
					      NUM2INT(ipad_y));
    return Qnil;
}

static VALUE
bbox_get_spacing(self)
    VALUE self;
{
    int i = gtk_button_box_get_spacing(GTK_BUTTON_BOX(get_widget(self)));

    return INT2FIX(i);
}

static VALUE
bbox_get_layout(self)
    VALUE self;
{
    int i = gtk_button_box_get_layout(GTK_BUTTON_BOX(get_widget(self)));

    return INT2FIX(i);
}

static VALUE
bbox_get_child_size(self)
    VALUE self;
{
    int min_width, max_width;

    gtk_button_box_get_child_size(GTK_BUTTON_BOX(get_widget(self)),
				  &min_width, &max_width);
    return rb_assoc_new(INT2FIX(min_width), INT2FIX(max_width));
}

static VALUE
bbox_get_child_ipadding(self)
    VALUE self;
{
    int ipad_x, ipad_y;

    gtk_button_box_get_child_ipadding(GTK_BUTTON_BOX(get_widget(self)),
				      &ipad_x, &ipad_y);
    return rb_assoc_new(INT2FIX(ipad_x), INT2FIX(ipad_y));
}

static VALUE
bbox_set_spacing(self, spacing)
    VALUE self, spacing;
{
    gtk_button_box_set_spacing(GTK_BUTTON_BOX(get_widget(self)),
			       NUM2INT(spacing));
    return self;
}

static VALUE
bbox_set_layout(self, layout)
    VALUE self, layout;
{
    gtk_button_box_set_layout(GTK_BUTTON_BOX(get_widget(self)),
			      NUM2INT(layout));
    return self;
}

static VALUE
bbox_set_child_size(self, min_width, max_width)
    VALUE self, min_width, max_width;
{
    gtk_button_box_set_child_size(GTK_BUTTON_BOX(get_widget(self)),
				  NUM2INT(min_width),
				  NUM2INT(max_width));
    return self;
}

static VALUE
bbox_set_child_ipadding(self, ipad_x, ipad_y)
    VALUE self, ipad_x, ipad_y;
{
    gtk_button_box_set_child_ipadding(GTK_BUTTON_BOX(get_widget(self)),
				      NUM2INT(ipad_x),
				      NUM2INT(ipad_y));
    return self;
}

void Init_gtk_button_box()
{
    gBBox = rb_define_class_under(mGtk, "ButtonBox", gBox);

    rb_define_singleton_method(gBBox, "get_child_size_default",
			       bbox_get_child_size_default, 0);
    rb_define_singleton_method(gBBox, "get_child_ipadding_default",
			       bbox_get_child_ipadding_default, 0);
    rb_define_singleton_method(gBBox, "set_child_size_default",
			       bbox_set_child_size_default, 2);
    rb_define_singleton_method(gBBox, "set_child_ipadding_default",
			       bbox_set_child_ipadding_default, 2);
    rb_define_method(gBBox, "get_spacing", bbox_get_spacing, 0);
    rb_define_method(gBBox, "get_layout", bbox_get_layout, 0);
    rb_define_method(gBBox, "get_child_size", bbox_get_child_size, 0);
    rb_define_method(gBBox, "get_child_ipadding", bbox_get_child_ipadding, 0);
    rb_define_method(gBBox, "set_spacing", bbox_set_spacing, 1);
    rb_define_method(gBBox, "set_layout", bbox_set_layout, 1);
    rb_define_method(gBBox, "set_child_size", bbox_set_child_size, 2);
    rb_define_method(gBBox, "set_child_ipadding", bbox_set_child_ipadding, 2);

    /* child initialization */
    Init_gtk_hbutton_box();
    Init_gtk_vbutton_box();
}
