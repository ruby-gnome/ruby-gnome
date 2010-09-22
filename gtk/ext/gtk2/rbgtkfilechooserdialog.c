/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************
 
  rbgtkfilechooserdialog.c -
 
  $Author: mutoh $
  $Date: 2006/12/26 16:11:13 $
 
  Copyright (C) 2004 Seiya Nishizawa, Masao Mutoh
************************************************/
 
#include "global.h"

#if GTK_CHECK_VERSION(2,4,0)

#define _SELF(self) GTK_FILE_CHOOSER_DIALOG(RVAL2GOBJ(self))

static VALUE
fchodiag_initialize(argc, argv, self)
    int argc;
    VALUE* argv;
    VALUE self;
{
    VALUE title, parent, action, back, button_ary;
    GtkWidget* dialog;
    gchar* gtitle;
    GtkWindow *gparent;
    GtkFileChooserAction gaction;
    gchar* gback;

    rb_scan_args(argc, argv, "04*", &title, &parent, &action, &back, &button_ary);
    gtitle = NIL_P(title) ? NULL : RVAL2CSTR(title);
    gparent = NIL_P(parent) ? NULL : GTK_WINDOW(RVAL2GOBJ(parent));
    gaction = NIL_P(action) ? GTK_FILE_CHOOSER_ACTION_OPEN : RVAL2GENUM(action, GTK_TYPE_FILE_CHOOSER_ACTION);
    gback = NIL_P(back) ? NULL : RVAL2CSTR(back);
    dialog = gtk_file_chooser_dialog_new_with_backend((const gchar*)gtitle, 
                                                      gparent, gaction, 
                                                      (const gchar*)gback, 
                                                      (const gchar*)NULL, NULL);
    RBGTK_INITIALIZE(self, dialog);
    rbgtk_dialog_add_buttons_internal(self, button_ary);
    return Qnil;
}

#endif

void 
Init_gtk_file_chooser_dialog()
{
#if GTK_CHECK_VERSION(2,4,0)

    VALUE gFileChoDiag = G_DEF_CLASS(GTK_TYPE_FILE_CHOOSER_DIALOG, "FileChooserDialog", mGtk);

    rb_define_method(gFileChoDiag, "initialize", fchodiag_initialize, -1);

#endif
}

