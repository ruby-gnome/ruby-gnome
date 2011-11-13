/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2002-2006 Masao Mutoh
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 *  MA  02110-1301  USA
 */

#include "global.h"

#define RG_TARGET_NAMESPACE cMessageDialog
#define _SELF(s) (GTK_MESSAGE_DIALOG(RVAL2GOBJ(s)))

static VALUE
rg_initialize(int argc, VALUE *argv, VALUE self)
{
    VALUE parent, flags, type, buttons, message;
    GtkWidget* w;

    rb_scan_args(argc, argv, "41", &parent, &flags, &type, &buttons, &message);

    w = gtk_message_dialog_new(NIL_P(parent) ? NULL : GTK_WINDOW(RVAL2GOBJ(parent)), 
                               RVAL2GFLAGS(flags, GTK_TYPE_DIALOG_FLAGS), 
                               RVAL2GENUM(type, GTK_TYPE_MESSAGE_TYPE), 
                               RVAL2GENUM(buttons, GTK_TYPE_BUTTONS_TYPE),
                               "%s",
                               NIL_P(message) ? "": RVAL2CSTR(message));
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
rg_set_markup(VALUE self, VALUE str)
{
    gtk_message_dialog_set_markup(_SELF(self), RVAL2CSTR(str));
    return self;
}
#endif

/* Not needed in Ruby
void                gtk_message_dialog_format_secondary_text
                                                        (GtkMessageDialog *message_dialog,
                                                         const gchar *message_format,
                                                         ...);
void                gtk_message_dialog_format_secondary_markup
                                                        (GtkMessageDialog *message_dialog,
                                                         const gchar *message_format,
                                                         ...);
*/

void 
Init_gtk_message_dialog(VALUE mGtk)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_MESSAGE_DIALOG, "MessageDialog", mGtk);

    RG_DEF_METHOD(initialize, -1);
#if GTK_CHECK_VERSION(2,4,0)
    RG_DEF_METHOD(set_markup, 1);
    G_DEF_SETTER(RG_TARGET_NAMESPACE, "markup");
#endif

    /* GtkMessageType */
    G_DEF_CLASS(GTK_TYPE_MESSAGE_TYPE, "Type", RG_TARGET_NAMESPACE);
    G_DEF_CONSTANTS(RG_TARGET_NAMESPACE, GTK_TYPE_MESSAGE_TYPE, "GTK_MESSAGE_");

    /* GtkButtonsType */
    G_DEF_CLASS(GTK_TYPE_BUTTONS_TYPE, "ButtonsType", RG_TARGET_NAMESPACE);
    G_DEF_CONSTANTS(RG_TARGET_NAMESPACE, GTK_TYPE_BUTTONS_TYPE, "GTK_");
}
