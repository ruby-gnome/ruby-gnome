/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtktogglebutton.c -

  $Author: mutoh $
  $Date: 2002/06/23 16:13:32 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

static VALUE
tbtn_initialize(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE label;
    GtkWidget *widget;

    if (rb_scan_args(argc, argv, "01", &label) == 1) {
	widget = gtk_toggle_button_new_with_label(STR2CSTR(label));
    }
    else {
	widget = gtk_toggle_button_new();
    }

    RBGTK_INITIALIZE(self, widget);
    return Qnil;
}

static VALUE
tbtn_set_mode(self, mode)
    VALUE self, mode;
{
    gtk_toggle_button_set_mode(GTK_TOGGLE_BUTTON(RVAL2GOBJ(self)),
			       RTEST(mode));
    return self;
}

static VALUE
tbtn_set_state(self, state)
    VALUE self, state;
{
    gtk_toggle_button_set_state(GTK_TOGGLE_BUTTON(RVAL2GOBJ(self)),
				RTEST(state));
    return self;
}

static VALUE
tbtn_toggled(self)
    VALUE self;
{
    gtk_toggle_button_toggled(GTK_TOGGLE_BUTTON(RVAL2GOBJ(self)));
    return self;
}

static VALUE
tbtn_is_active(self)
    VALUE self;
{
    if (GTK_TOGGLE_BUTTON(RVAL2GOBJ(self))->active)
	return Qtrue;
    return Qfalse;
}
static VALUE
tbtn_set_active(self, is_active)
    VALUE self, is_active;
{
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(RVAL2GOBJ(self)),
				 RTEST(is_active));
    return self;
}

void Init_gtk_toggle_button()
{
    static RGObjClassInfo cinfo;

    gTButton = rb_define_class_under(mGtk, "ToggleButton", gButton);
    cinfo.klass = gTButton;
    cinfo.gtype = GTK_TYPE_TOGGLE_BUTTON;
    cinfo.mark = 0;
    cinfo.free = 0;
    rbgtk_register_class(&cinfo);

    rb_define_method(gTButton, "initialize", tbtn_initialize, -1);
    rb_define_method(gTButton, "set_mode", tbtn_set_mode, 1);
    rb_define_method(gTButton, "set_state", tbtn_set_state, 1); /* back-compat. */
    rb_define_method(gTButton, "toggled", tbtn_toggled, 0);
    rb_define_method(gTButton, "active", tbtn_is_active, 0);
    rb_define_method(gTButton, "set_active", tbtn_set_active, 1);

    rb_define_alias(gTButton, "active?", "active");
    rb_define_alias(gTButton, "active=", "set_active");

    rb_define_const(gTButton, "SIGNAL_TOGGLED", rb_str_new2("toggled"));

    /* child initialization */
    Init_gtk_check_button();
}
