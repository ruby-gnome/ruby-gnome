/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkdialog.c -

  $Author: mutoh $
  $Date: 2002/10/23 18:02:15 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

#define _SELF(self) (GTK_DIALOG(RVAL2GOBJ(self)))

static VALUE
dialog_initialize(self)
    VALUE self;
{
    RBGTK_INITIALIZE(self, gtk_dialog_new());
    return Qnil;
}

/*
GtkWidget*  gtk_dialog_new_with_buttons     (const gchar *title,
                                             GtkWindow *parent,
                                             GtkDialogFlags flags,
                                             const gchar *first_button_text,
                                             ...);
*/

static VALUE
dialog_run(self)
    VALUE self;
{
    return INT2NUM(gtk_dialog_run(_SELF(self)));
}

static VALUE
dialog_response(self, response_id)
    VALUE self, response_id;
{
    gtk_dialog_response(_SELF(self), NUM2INT(response_id));
    return self;
}

static VALUE
dialog_add_button(self, button_text, response_id)
    VALUE self, button_text, response_id;
{
    return GOBJ2RVAL(gtk_dialog_add_button(_SELF(self), RVAL2CSTR(button_text), 
                                           NUM2INT(response_id)));
}

/*
void        gtk_dialog_add_buttons          (GtkDialog *dialog,
                                             const gchar *first_button_text,
                                             ...);
*/

static VALUE
dialog_add_action_widget(self, child, response_id)
    VALUE self, child, response_id;
{
    gtk_dialog_add_action_widget(_SELF(self), GTK_WIDGET(RVAL2GOBJ(child)), 
                                 NUM2INT(response_id));
    return self;
}

static VALUE
dialog_set_default_response(self, response_id)
    VALUE self, response_id;
{
    gtk_dialog_set_default_response(_SELF(self), NUM2INT(response_id));
    return self;
}

static VALUE
dialog_set_response_sensitive(self, response_id, setting)
    VALUE self, response_id, setting;
{
    gtk_dialog_set_response_sensitive(_SELF(self), NUM2INT(response_id), RTEST(setting));
    return self;
}

static VALUE
dialog_vbox(self)
    VALUE self;
{
    return GOBJ2RVAL(_SELF(self)->vbox);
}

static VALUE
dialog_action_area(self)
    VALUE self;
{
    return GOBJ2RVAL(_SELF(self)->action_area);
}

void 
Init_gtk_dialog()
{
    VALUE gDialog = G_DEF_CLASS(GTK_TYPE_DIALOG, "Dialog", mGtk);

    rb_define_method(gDialog, "initialize", dialog_initialize, 0);
    rb_define_method(gDialog, "run", dialog_run, 0);
    rb_define_method(gDialog, "response", dialog_response, 1);
    rb_define_method(gDialog, "add_button", dialog_add_button, 2);
    rb_define_method(gDialog, "add_action_widget", dialog_add_action_widget, 2);
    rb_define_method(gDialog, "set_default_response", dialog_set_default_response, 1);
    rb_define_method(gDialog, "set_response_sensitive", dialog_set_response_sensitive, 2);
    rb_define_method(gDialog, "vbox", dialog_vbox, 0);
    rb_define_method(gDialog, "action_area", dialog_action_area, 0);
   
    G_DEF_SETTERS(gDialog);
   
    /* GtkDialogFlags */
    rb_define_const(gDialog, "MODAL", INT2FIX(GTK_DIALOG_MODAL));
    rb_define_const(gDialog, "DESTROY_WITH_PARENT", INT2FIX(GTK_DIALOG_DESTROY_WITH_PARENT));
    rb_define_const(gDialog, "NO_SEPARATOR", INT2FIX(GTK_DIALOG_NO_SEPARATOR));

    /* GtkResponseType */
    rb_define_const(gDialog, "RESPONSE_NONE", INT2FIX(GTK_RESPONSE_NONE));
    rb_define_const(gDialog, "RESPONSE_REJECT", INT2FIX(GTK_RESPONSE_REJECT));
    rb_define_const(gDialog, "RESPONSE_ACCEPT", INT2FIX(GTK_RESPONSE_ACCEPT));
    rb_define_const(gDialog, "RESPONSE_DELETE_EVENT", INT2FIX(GTK_RESPONSE_DELETE_EVENT));
    rb_define_const(gDialog, "RESPONSE_OK", INT2FIX(GTK_RESPONSE_OK));
    rb_define_const(gDialog, "RESPONSE_CANCEL", INT2FIX(GTK_RESPONSE_CANCEL));
    rb_define_const(gDialog, "RESPONSE_CLOSE", INT2FIX(GTK_RESPONSE_CLOSE));
    rb_define_const(gDialog, "RESPONSE_YES", INT2FIX(GTK_RESPONSE_YES));
    rb_define_const(gDialog, "RESPONSE_NO", INT2FIX(GTK_RESPONSE_NO));
    rb_define_const(gDialog, "RESPONSE_APPLY", INT2FIX(GTK_RESPONSE_APPLY));
    rb_define_const(gDialog, "RESPONSE_HELP", INT2FIX(GTK_RESPONSE_HELP));

}
