/************************************************

  rbgtkcontainer.c -

  $Author: mutoh $
  $Date: 2002/05/19 12:39:11 $

  Copyright (C) 2001 Neil Conway
************************************************/

#include "global.h"

VALUE gAccelLabel;

static VALUE
alabel_initialize(self, string)
    VALUE self, string;
{
    GtkWidget* accel_label = gtk_accel_label_new(STR2CSTR(string));
    set_widget(self, accel_label);
    return Qnil;
}

static VALUE
alabel_set_accel_widget(self, a_widget)
    VALUE self, a_widget;
{
    gtk_accel_label_set_accel_widget(GTK_ACCEL_LABEL(get_widget(self)),
                                     get_widget(a_widget));
    return Qnil;
}

static VALUE
alabel_get_accel_width(self)
    VALUE self;
{
    return INT2NUM(gtk_accel_label_get_accel_width(
                GTK_ACCEL_LABEL(get_widget(self))));
}

static VALUE
alabel_refetch(self)
    VALUE self;
{
    gtk_accel_label_refetch(GTK_ACCEL_LABEL(get_widget(self)));
    return Qfalse; // always returns false
}

void
Init_gtk_accel_label()
{
    gAccelLabel = rb_define_class_under(mGtk, "AccelLabel", gLabel);
    rb_define_method(gAccelLabel, "initialize", alabel_initialize, 1);
    rb_define_method(gAccelLabel, "set_accel_widget",
            alabel_set_accel_widget, 1);
    rb_define_method(gAccelLabel, "get_accel_width",
            alabel_get_accel_width, 0);
    rb_define_method(gAccelLabel, "refetch", alabel_refetch, 0);
}
