/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkrecentchoosermenu.c -

  $Author: mutoh $
  $Date: 2006/11/27 17:41:06 $

  Copyright (C) 2006 Masao Mutoh
************************************************/

#include "global.h"

#if GTK_CHECK_VERSION(2,10,0)

#define _SELF(self) (GTK_RECENT_CHOOSER_MENU(RVAL2GOBJ(self)))

static VALUE
rcm_initialize(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE manager;
    GtkWidget* menu;
    
    rb_scan_args(argc, argv, "01", &manager);

    if (NIL_P(manager)){
        menu = gtk_recent_chooser_menu_new_for_manager(GTK_RECENT_MANAGER(RVAL2GOBJ(manager)));
    } else {
        menu = gtk_recent_chooser_menu_new();
    }
    RBGTK_INITIALIZE(self, menu);
    return Qnil;
}

/* Defined as properties
gboolean    gtk_recent_chooser_menu_get_show_numbers
                                            (GtkRecentChooserMenu *menu);
void        gtk_recent_chooser_menu_set_show_numbers
                                            (GtkRecentChooserMenu *menu,
                                             gboolean show_numbers);
*/
#endif

void 
Init_gtk_recent_chooser_menu()
{
#if GTK_CHECK_VERSION(2,10,0)
  VALUE rcm = G_DEF_CLASS(GTK_TYPE_RECENT_CHOOSER_MENU, "RecentChooserMenu", mGtk);

  rb_define_method(rcm, "initialize", rcm_initialize, -1);
#endif
}
