/************************************************

  rbgtkcontainer.c -

  $Author: mutoh $
  $Date: 2002/09/14 15:43:40 $

  Copyright (C) 2001 Neil Conway
************************************************/

#include "global.h"

static VALUE
alabel_initialize(self, string)
	 VALUE self, string;
{
  RBGTK_INITIALIZE(self, gtk_accel_label_new(RVAL2CSTR(string)));
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
  VALUE gAccelLabel = G_DEF_CLASS(GTK_TYPE_ACCEL_LABEL, "AccelLabel", mGtk);

  rb_define_method(gAccelLabel, "initialize", alabel_initialize, 1);
  rb_define_method(gAccelLabel, "set_accel_widget",
				   alabel_set_accel_widget, 1);
  rb_define_method(gAccelLabel, "get_accel_width",
				   alabel_get_accel_width, 0);
  rb_define_method(gAccelLabel, "refetch", alabel_refetch, 0);
}
