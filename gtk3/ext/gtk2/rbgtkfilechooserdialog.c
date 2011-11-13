/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2004  Seiya Nishizawa, Masao Mutoh
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

#if GTK_CHECK_VERSION(2,4,0)

#define RG_TARGET_NAMESPACE cFileChooserDialog
#define _SELF(self) GTK_FILE_CHOOSER_DIALOG(RVAL2GOBJ(self))

static VALUE
rg_initialize(int argc, VALUE *argv, VALUE self)
{
    VALUE title, parent, action, back, button_ary;
    GtkWidget* dialog;
    const gchar *gtitle;
    GtkWindow *gparent;
    GtkFileChooserAction gaction;
    const gchar *gback;

    rb_scan_args(argc, argv, "04*", &title, &parent, &action, &back, &button_ary);
    gtitle = NIL_P(title) ? NULL : RVAL2CSTR(title);
    gparent = NIL_P(parent) ? NULL : GTK_WINDOW(RVAL2GOBJ(parent));
    gaction = NIL_P(action) ? GTK_FILE_CHOOSER_ACTION_OPEN : RVAL2GENUM(action, GTK_TYPE_FILE_CHOOSER_ACTION);
    gback = NIL_P(back) ? NULL : RVAL2CSTR(back);
    dialog = gtk_file_chooser_dialog_new_with_backend(gtitle, 
                                                      gparent,
                                                      gaction, 
                                                      gback, 
                                                      NULL,
                                                      NULL);
    RBGTK_INITIALIZE(self, dialog);
    rbgtk_dialog_add_buttons_internal(self, button_ary);
    return Qnil;
}

#endif

void 
Init_gtk_file_chooser_dialog(VALUE mGtk)
{
#if GTK_CHECK_VERSION(2,4,0)

    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_FILE_CHOOSER_DIALOG, "FileChooserDialog", mGtk);

    RG_DEF_METHOD(initialize, -1);

#endif
}
