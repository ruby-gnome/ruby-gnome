/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtkbutton.c -

  $Author: mutoh $
  $Date: 2002/06/22 19:50:57 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

static VALUE
button_initialize(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE label;
    GtkWidget *widget;

    if (rb_scan_args(argc, argv, "01", &label) == 1) {
	widget = gtk_button_new_with_label(STR2CSTR(label));
    }
    else {
	widget = gtk_button_new();
    }

    RBGTK_INITIALIZE(self, widget);
    return Qnil;
}

static VALUE
button_pressed(self)
    VALUE self;
{
    gtk_button_pressed(GTK_BUTTON(RVAL2GOBJ(self)));
    return self;
}

static VALUE
button_released(self)
    VALUE self;
{
    gtk_button_released(GTK_BUTTON(RVAL2GOBJ(self)));
    return self;
}

static VALUE
button_clicked(self)
    VALUE self;
{
    gtk_button_clicked(GTK_BUTTON(RVAL2GOBJ(self)));
    return self;
}

static VALUE
button_enter(self)
    VALUE self;
{
    gtk_button_enter(GTK_BUTTON(RVAL2GOBJ(self)));
    return self;
}

static VALUE
button_leave(self)
    VALUE self;
{
    gtk_button_leave(GTK_BUTTON(RVAL2GOBJ(self)));
    return self;
}

static VALUE
button_set_relief(self, style)
    VALUE self, style;
{
    gtk_button_set_relief(GTK_BUTTON(RVAL2GOBJ(self)), NUM2INT(style));
    return self;
}

static VALUE
button_get_relief(self)
    VALUE self;
{
    GtkReliefStyle style;
    style = gtk_button_get_relief(GTK_BUTTON(RVAL2GOBJ(self)));
    return INT2FIX(style);
}

void Init_gtk_button()
{
    static rbgtk_class_info cinfo;

    gButton = rb_define_class_under(mGtk, "Button", gBin);
    cinfo.klass = gButton;
    cinfo.gtype = GTK_TYPE_BUTTON;
    cinfo.mark = 0;
    cinfo.free = 0;
    rbgtk_register_class(&cinfo);

    rb_define_const(gButton, "SIGNAL_PRESSED", rb_str_new2("pressed"));
    rb_define_const(gButton, "SIGNAL_RELEASED", rb_str_new2("released"));
    rb_define_const(gButton, "SIGNAL_CLICKED", rb_str_new2("clicked"));
    rb_define_const(gButton, "SIGNAL_ENTER", rb_str_new2("enter"));
    rb_define_const(gButton, "SIGNAL_LEAVE", rb_str_new2("leave"));

    rb_define_method(gButton, "initialize", button_initialize, -1);
    rb_define_method(gButton, "pressed", button_pressed, 0);
    rb_define_method(gButton, "released", button_released, 0);
    rb_define_method(gButton, "clicked", button_clicked, 0);
    rb_define_method(gButton, "enter", button_enter, 0);
    rb_define_method(gButton, "leave", button_leave, 0);
    rb_define_method(gButton, "set_relief", button_set_relief, 1);
    rb_define_method(gButton, "get_relief", button_get_relief, 0);

    /* child initialization */
    Init_gtk_option_menu();
    Init_gtk_toggle_button();
}
