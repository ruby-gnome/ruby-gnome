/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkmessagedialog.c -

  $Author: mutoh $
  $Date: 2004/06/19 16:21:34 $

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
                               RVAL2GFLAGS(flags, GTK_TYPE_DIALOG_FLAGS), 
                               RVAL2GENUM(type, GTK_TYPE_MESSAGE_TYPE), 
                               RVAL2GENUM(buttons, GTK_TYPE_BUTTONS_TYPE),
                               (const gchar*)(NIL_P(message) ? "": RVAL2CSTR(message)));
    RBGTK_INITIALIZE(self, w);
    return Qnil;
}

/*
Don't implement. Use Gtk::MessageDialog#set_markup.
GtkWidget*  gtk_message_dialog_new_with_markup
                                            (GtkWindow *parent,
                                             GtkDialogFlags flags,
                                             GtkMessageType type,
                                             GtkButtonsType buttons,
                                             const gchar *message_format,
                                             ...);
*/
#if GTK_CHECK_VERSION(2,4,0)
static VALUE
mdiag_set_markup(self, str)
    VALUE self, str;
{
    gtk_message_dialog_set_markup(GTK_MESSAGE_DIALOG(RVAL2GOBJ(self)), RVAL2CSTR(str));
    return self;
}
#endif
void 
Init_gtk_message_dialog()
{
    VALUE gMessageDialog = G_DEF_CLASS(GTK_TYPE_MESSAGE_DIALOG, "MessageDialog", mGtk);

    rb_define_method(gMessageDialog, "initialize", mdiag_initialize, -1);
#if GTK_CHECK_VERSION(2,4,0)
    rb_define_method(gMessageDialog, "set_markup", mdiag_set_markup, 1);
#endif
    G_DEF_SETTERS(gMessageDialog);

    /* GtkMessageType */
    G_DEF_CLASS(GTK_TYPE_MESSAGE_TYPE, "Type", gMessageDialog);
    G_DEF_CONSTANTS(gMessageDialog, GTK_TYPE_MESSAGE_TYPE, "GTK_MESSAGE_");

    /* GtkButtonsType */
    G_DEF_CLASS(GTK_TYPE_BUTTONS_TYPE, "ButtonsType", gMessageDialog);
    G_DEF_CONSTANTS(gMessageDialog, GTK_TYPE_BUTTONS_TYPE, "GTK_");
}
