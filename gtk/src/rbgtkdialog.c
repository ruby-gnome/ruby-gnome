/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtkdialog.c -

  $Author: mutoh $
  $Date: 2002/05/19 12:39:05 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

static VALUE
dialog_initialize(self)
    VALUE self;
{
    set_widget(self, gtk_dialog_new());
    return Qnil;
}

static VALUE
dialog_vbox(self)
    VALUE self;
{
    GtkWidget *dialog = get_widget(self);
    return make_gobject(gVBox, GTK_OBJECT(GTK_DIALOG(dialog)->vbox));
}

static VALUE
dialog_action_area(self)
    VALUE self;
{
    GtkWidget *dialog = get_widget(self);
    return make_gobject(gHBox, GTK_OBJECT(GTK_DIALOG(dialog)->action_area));
}

void Init_gtk_dialog()
{
    gDialog = rb_define_class_under(mGtk, "Dialog", gWindow);

    /* rb_define_const(gDialog, "SIGNAL_", rb_str_new2("")); */

    rb_define_method(gDialog, "initialize", dialog_initialize, 0);
    rb_define_method(gDialog, "vbox", dialog_vbox, 0);
    rb_define_method(gDialog, "action_area", dialog_action_area, 0);

    /* child initialization */
    Init_gtk_input_dialog();
}
