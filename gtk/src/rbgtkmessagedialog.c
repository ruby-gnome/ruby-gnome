/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkmessagedialog.c -

  $Author: mutoh $
  $Date: 2002/10/23 18:02:17 $

  Copyright (C) 2002 Masao Mutoh
************************************************/

#include "global.h"

static VALUE
mdiag_initialize(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE parent, flags, type, buttons, message;
    GtkWidget* w;

    rb_scan_args(argc, argv, "41", &parent, &flags, &type, &buttons, &message);

    w = gtk_message_dialog_new(NIL_P(parent) ? NULL : GTK_WINDOW(RVAL2GOBJ(parent)), 
                               FIX2INT(flags), FIX2INT(type), FIX2INT(buttons),
                               (const gchar*)(NIL_P(message) ? "": RVAL2CSTR(message)));
    RBGTK_INITIALIZE(self, w);
    return Qnil;
}

void 
Init_gtk_message_dialog()
{
    VALUE gMessageDialog = G_DEF_CLASS(GTK_TYPE_MESSAGE_DIALOG, "MessageDialog", mGtk);

    rb_define_method(gMessageDialog, "initialize", mdiag_initialize, -1);

    /* GtkMessageType */
    rb_define_const(gMessageDialog, "INFO", INT2FIX(GTK_MESSAGE_INFO));
    rb_define_const(gMessageDialog, "WARNING", INT2FIX(GTK_MESSAGE_WARNING));
    rb_define_const(gMessageDialog, "QUESTION", INT2FIX(GTK_MESSAGE_QUESTION));
    rb_define_const(gMessageDialog, "ERROR", INT2FIX(GTK_MESSAGE_ERROR));

    /* GtkButtonsType */
    rb_define_const(gMessageDialog, "BUTTONS_NONE", INT2FIX(GTK_BUTTONS_NONE));
    rb_define_const(gMessageDialog, "BUTTONS_OK", INT2FIX(GTK_BUTTONS_OK));
    rb_define_const(gMessageDialog, "BUTTONS_CLOSE", INT2FIX(GTK_BUTTONS_CLOSE));
    rb_define_const(gMessageDialog, "BUTTONS_CANCEL", INT2FIX(GTK_BUTTONS_CANCEL));
    rb_define_const(gMessageDialog, "BUTTONS_YES_NO", INT2FIX(GTK_BUTTONS_YES_NO));
    rb_define_const(gMessageDialog, "BUTTONS_OK_CANCEL", INT2FIX(GTK_BUTTONS_OK_CANCEL));
}
