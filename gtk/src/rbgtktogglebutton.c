/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtktogglebutton.c -

  $Author: mutoh $
  $Date: 2002/10/21 17:29:30 $

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
	widget = gtk_toggle_button_new_with_label(RVAL2CSTR(label));
    }
    else {
	widget = gtk_toggle_button_new();
    }

    RBGTK_INITIALIZE(self, widget);
    return Qnil;
}
/* Shouldn't we implement this?
GtkWidget*  gtk_toggle_button_new_with_mnemonic
                                            (const gchar *label);
*/

static VALUE
tbtn_set_mode(self, mode)
    VALUE self, mode;
{
    gtk_toggle_button_set_mode(GTK_TOGGLE_BUTTON(RVAL2GOBJ(self)),
			       RTEST(mode));
    return self;
}

static VALUE
tbtn_get_mode(self)
    VALUE self;
{
    return gtk_toggle_button_get_mode(GTK_TOGGLE_BUTTON(RVAL2GOBJ(self))) ? Qtrue : Qfalse;
}

/*
gboolean    gtk_toggle_button_get_mode      (GtkToggleButton *toggle_button);
*/

static VALUE
tbtn_toggled(self)
    VALUE self;
{
    gtk_toggle_button_toggled(GTK_TOGGLE_BUTTON(RVAL2GOBJ(self)));
    return self;
}


void 
Init_gtk_toggle_button()
{
    VALUE gTButton = G_DEF_CLASS(GTK_TYPE_TOGGLE_BUTTON, "ToggleButton", mGtk);

    rb_define_method(gTButton, "initialize", tbtn_initialize, -1);
    rb_define_method(gTButton, "set_mode", tbtn_set_mode, 1);
    rb_define_method(gTButton, "mode?", tbtn_get_mode, 0);
    rb_define_method(gTButton, "toggled", tbtn_toggled, 0);

    G_DEF_SETTERS(gTButton);
}
