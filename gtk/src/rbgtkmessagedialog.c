/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkmessagedialog.c -

  $Author: sakai $
  $Date: 2003/08/20 17:07:04 $

  Copyright (C) 2002,2003 Masao Mutoh
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
    G_DEF_CLASS(GTK_TYPE_MESSAGE_TYPE, "Type", gMessageDialog);
    G_DEF_CONSTANTS(gMessageDialog, GTK_TYPE_MESSAGE_TYPE, "GTK_MESSAGE_");

    /* GtkButtonsType */
    G_DEF_CLASS(GTK_TYPE_BUTTONS_TYPE, "ButtonsType", gMessageDialog);
    G_DEF_CONSTANTS(gMessageDialog, GTK_TYPE_BUTTONS_TYPE, "GTK_");
}
