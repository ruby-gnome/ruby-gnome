/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkradiobutton.c -

  $Author: mutoh $
  $Date: 2002/09/12 19:06:02 $

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
		if (rb_obj_is_kind_of(arg1, GTYPE2CLASS(GTK_TYPE_RADIO_BUTTON))) {
			list = GTK_RADIO_BUTTON(RVAL2GOBJ(arg1))->group;
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
    RBGTK_INITIALIZE(self, widget);
    return Qnil;
}

static VALUE
rbtn_group(self)
    VALUE self;
{
    return gslist2ary(gtk_radio_button_group(GTK_RADIO_BUTTON(RVAL2GOBJ(self))));
}

void 
Init_gtk_radio_button()
{
    VALUE gRButton = G_DEF_CLASS(GTK_TYPE_RADIO_BUTTON, "RadioButton", mGtk);

    rb_define_method(gRButton, "initialize", rbtn_initialize, -1);
    rb_define_method(gRButton, "group", rbtn_group, 0);
}
