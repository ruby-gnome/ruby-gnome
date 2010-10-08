/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkrecentchooserdialog.c -

  $Author: mutoh $
  $Date: 2006/11/27 17:41:06 $

  Copyright (C) 2006 Masao Mutoh
************************************************/

#include "global.h"

#if GTK_CHECK_VERSION(2,10,0)

#define _SELF(self) (GTK_RECENT_CHOOSER_DIALOG(RVAL2GOBJ(self)))

static VALUE
rcd_initialize(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
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
Init_gtk_recent_chooser_dialog()
{
#if GTK_CHECK_VERSION(2,10,0)
  VALUE rcd = G_DEF_CLASS(GTK_TYPE_RECENT_CHOOSER_DIALOG, "RecentChooserDialog", mGtk);

  rb_define_method(rcd, "initialize", rcd_initialize, -1);
#endif
}
