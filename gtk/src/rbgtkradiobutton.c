/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtkradiobutton.c -

  $Author: igapy $
  $Date: 2002/05/30 00:46:41 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

static VALUE
rbtn_initialize(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE arg1, arg2;
    GtkWidget *widget;
    GSList *list = NULL;
    char *label = NULL;
    
    if (rb_scan_args(argc, argv, "02", &arg1, &arg2) == 1 &&
	TYPE(arg1) == T_STRING) {
	label = RSTRING(arg1)->ptr;
    }
    else {
	if (!NIL_P(arg2)) {
	    label = STR2CSTR(arg2);
	}
	if (rb_obj_is_kind_of(arg1, gRButton)) {
	    GtkWidget *b = get_widget(arg1);
	    list = GTK_RADIO_BUTTON(b)->group;
	}
	else {
	    list = ary2gslist(arg1);
	}
    }
    if (label) {
	widget = gtk_radio_button_new_with_label(list, label);
    }
    else {
	widget = gtk_radio_button_new(list);
    }
    set_widget(self, widget);
    return Qnil;
}

static VALUE
rbtn_group(self)
    VALUE self;
{
    GtkWidget *widget = get_widget(self);
    
    return gslist2ary(gtk_radio_button_group(GTK_RADIO_BUTTON(widget)));
}

void Init_gtk_radio_button()
{
    static rbgtk_class_info cinfo;

    gRButton = rb_define_class_under(mGtk, "RadioButton", gCButton);
    cinfo.klass = gRButton;
    cinfo.gtype = GTK_TYPE_RADIO_BUTTON;
    cinfo.mark = 0;
    cinfo.free = 0;
    rbgtk_register_class(&cinfo);

    rb_define_method(gRButton, "initialize", rbtn_initialize, -1);
    rb_define_method(gRButton, "group", rbtn_group, 0);
}
