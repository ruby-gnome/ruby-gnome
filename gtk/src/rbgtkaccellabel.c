/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkcontainer.c -

  $Author: mutoh $
  $Date: 2002/10/19 13:20:41 $

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
alabel_get_accel_width(self)
	 VALUE self;
{
    return INT2NUM(gtk_accel_label_get_accel_width(GTK_ACCEL_LABEL(RVAL2GOBJ(self))));
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
  rb_define_method(gAccelLabel, "accel_width", alabel_get_accel_width, 0);
  rb_define_method(gAccelLabel, "refetch", alabel_refetch, 0);
}
