/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2006  Masao Mutoh
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

#if GTK_CHECK_VERSION(2,10,0)

#define RG_TARGET_NAMESPACE cRecentChooserDialog
#define _SELF(self) (GTK_RECENT_CHOOSER_DIALOG(RVAL2GOBJ(self)))

static VALUE
rg_initialize(int argc, VALUE *argv, VALUE self)
{
    VALUE title, parent, button_ary;
    GtkWidget* dialog;
    if (rb_obj_is_kind_of(argv[2], GTYPE2CLASS(GTK_TYPE_RECENT_MANAGER))){
      VALUE manager;
      rb_scan_args(argc, argv, "03*", &title, &parent, &manager, &button_ary);

      dialog = gtk_recent_chooser_dialog_new_for_manager((const gchar*)RVAL2CSTR_ACCEPT_NIL(title),
                                                         GTK_WINDOW(RVAL2GOBJ(parent)),
                                                         GTK_RECENT_MANAGER(RVAL2GOBJ(manager)),
                                                         (const gchar*)NULL, NULL);
    } else {
      rb_scan_args(argc, argv, "02*", &title, &parent, &button_ary);

      dialog = gtk_recent_chooser_dialog_new((const gchar*)RVAL2CSTR_ACCEPT_NIL(title),
                                             GTK_WINDOW(RVAL2GOBJ(parent)),
                                             (const gchar*)NULL, NULL);
    }

    RBGTK_INITIALIZE(self, dialog);

    if (button_ary != Qnil){
      rbgtk_dialog_add_buttons_internal(self, button_ary);
    }
    return Qnil;
}
#endif

void 
Init_gtk_recent_chooser_dialog(VALUE mGtk)
{
#if GTK_CHECK_VERSION(2,10,0)
  VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_RECENT_CHOOSER_DIALOG, "RecentChooserDialog", mGtk);

  RG_DEF_METHOD(initialize, -1);
#endif
}
