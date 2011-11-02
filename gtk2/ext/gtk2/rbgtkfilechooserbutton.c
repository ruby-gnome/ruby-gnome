/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2005  Masao Mutoh
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

#if GTK_CHECK_VERSION(2,6,0)

#define RG_TARGET_NAMESPACE cFileChooserButton

static VALUE
rg_initialize(int argc, VALUE *argv, VALUE self)
{
    VALUE title, action, backend;
    GtkWidget* widget;

    rb_scan_args(argc, argv, "12", &title, &action, &backend);

    if (TYPE(title) == T_STRING) {
        if (NIL_P(backend)){
            widget = gtk_file_chooser_button_new(RVAL2CSTR(title),
                                                 RVAL2GENUM(action, GTK_TYPE_FILE_CHOOSER_ACTION));
        } else {
            widget = gtk_file_chooser_button_new_with_backend(RVAL2CSTR(title), 
                                                              RVAL2GENUM(action, 
                                                                         GTK_TYPE_FILE_CHOOSER_ACTION),
                                                              RVAL2CSTR(backend));
        }
    } else {
        widget = gtk_file_chooser_button_new_with_dialog(GTK_WIDGET(RVAL2GOBJ(title)));
    }

    RBGTK_INITIALIZE(self, widget);
    return Qnil;
}

#endif

void 
Init_gtk_filechooserbutton(VALUE mGtk)
{
#if GTK_CHECK_VERSION(2,6,0)
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_FILE_CHOOSER_BUTTON, "FileChooserButton", mGtk);
    RG_DEF_METHOD(initialize, -1);
#endif
}
