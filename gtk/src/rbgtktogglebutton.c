/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtktogglebutton.c -

  $Author: ggc $
  $Date: 2007/07/13 16:07:32 $

  Copyright (C) 2002,2003 Ruby-GNOME2 Project Team
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
    VALUE label, use_underline;
    GtkWidget *widget;

    if (rb_scan_args(argc, argv, "02", &label, &use_underline) > 0) {
        if (TYPE(label) == T_STRING){
            if (NIL_P(use_underline) || RVAL2CBOOL(use_underline)){
                widget = gtk_toggle_button_new_with_mnemonic(RVAL2CSTR(label));
            } else {
                widget = gtk_toggle_button_new_with_label(RVAL2CSTR(label));
            }
        } else if (TYPE(label) == T_SYMBOL){
            widget = gtk_toggle_button_new_with_label(rb_id2name(SYM2ID(label)));
            gtk_button_set_use_stock(GTK_BUTTON(widget), TRUE);
        } else {
            rb_raise(rb_eArgError, "invalid argument %s (expect Symbol(Gtk::Stock constants) or String)", 
                     rb_class2name(CLASS_OF(label)));
        }
    } else {
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
			       RVAL2CBOOL(mode));
    return self;
}

static VALUE
tbtn_get_mode(self)
    VALUE self;
{
    return CBOOL2RVAL(gtk_toggle_button_get_mode(GTK_TOGGLE_BUTTON(RVAL2GOBJ(self))));
}

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
