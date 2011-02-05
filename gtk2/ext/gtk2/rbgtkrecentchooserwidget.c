/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkrecentchooserwidget.c -

  $Author: mutoh $
  $Date: 2006/11/12 15:28:40 $

  Copyright (C) 2006 Masao Mutoh
************************************************/

#include "global.h"

#if GTK_CHECK_VERSION(2,10,0)

#define _SELF(self) (GTK_RECENT_CHOOSER_WIDGET(RVAL2GOBJ(self)))

static VALUE
rcw_initialize(int argc, VALUE *argv, VALUE self)
{
    VALUE manager;
    GtkWidget* widget;
    rb_scan_args(argc, argv, "01", &manager);
    if (NIL_P(manager)){
      widget = gtk_recent_chooser_widget_new_for_manager(GTK_RECENT_MANAGER(RVAL2GOBJ(manager)));
    } else {
      widget = gtk_recent_chooser_widget_new();
    }
    RBGTK_INITIALIZE(self, widget);
    return Qnil;
}
#endif

void 
Init_gtk_recent_chooser_widget()
{
#if GTK_CHECK_VERSION(2,10,0)
  VALUE rcw = G_DEF_CLASS(GTK_TYPE_RECENT_CHOOSER_WIDGET, "RecentChooserWidget", mGtk);

  rb_define_method(rcw, "initialize", rcw_initialize, -1);
#endif
}
