/************************************************

  rbgtkcontainer.c -

  $Author: mutoh $
  $Date: 2002/06/22 19:50:57 $

  Copyright (C) 2001 Neil Conway
************************************************/

#include "global.h"

VALUE gAccelLabel;

static VALUE
alabel_initialize(self, string)
	 VALUE self, string;
{
  RBGTK_INITIALIZE(self, gtk_accel_label_new(STR2CSTR(string)));
  return Qnil;
}

static VALUE
alabel_set_accel_widget(self, a_widget)
	 VALUE self, a_widget;
{
  gtk_accel_label_set_accel_widget(GTK_ACCEL_LABEL(RVAL2GOBJ(self)),
								   GTK_WIDGET(RVAL2GOBJ(a_widget)));
  return Qnil;
}

static VALUE
alabel_get_accel_width(self)
	 VALUE self;
{
  return INT2NUM(gtk_accel_label_get_accel_width(
												 GTK_ACCEL_LABEL(RVAL2GOBJ(self))));
}

static VALUE
alabel_refetch(self)
	 VALUE self;
{
  gtk_accel_label_refetch(GTK_ACCEL_LABEL(RVAL2GOBJ(self)));
  return Qfalse; // always returns false
}

void
Init_gtk_accel_label()
{
  static rbgtk_class_info cinfo;

  gAccelLabel = rb_define_class_under(mGtk, "AccelLabel", gLabel);
  cinfo.klass = gAccelLabel;
  cinfo.gtype = GTK_TYPE_ACCEL_LABEL;
  cinfo.mark = 0;
  cinfo.free = 0;
  rbgtk_register_class(&cinfo);

  rb_define_method(gAccelLabel, "initialize", alabel_initialize, 1);
  rb_define_method(gAccelLabel, "set_accel_widget",
				   alabel_set_accel_widget, 1);
  rb_define_method(gAccelLabel, "get_accel_width",
				   alabel_get_accel_width, 0);
  rb_define_method(gAccelLabel, "refetch", alabel_refetch, 0);
}
