/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************
 
  rbgtkfilechooserwidget.c -
 
  $Author: mutoh $
  $Date: 2004/05/30 16:41:13 $
 
  Copyright (C) 2004 Seiya Nishizawa, Masao Mutoh
************************************************/
 
#include "global.h"

#if GTK_CHECK_VERSION(2,4,0)

static VALUE
fchowidget_initialize(argc, argv, self)
    int argc;
    VALUE* argv;
    VALUE self;
{
    VALUE action, back;
    GtkWidget* widget;

    rb_scan_args(argc, argv, "11", &action, &back);

    if (NIL_P(back)){
        widget = gtk_file_chooser_widget_new(RVAL2GENUM(action, GTK_TYPE_FILE_CHOOSER_ACTION));
    } else {
        widget = gtk_file_chooser_widget_new_with_backend(RVAL2GENUM(action, GTK_TYPE_FILE_CHOOSER_ACTION),
                                                          RVAL2CSTR(back));
    }

    RBGTK_INITIALIZE(self, widget);
    return Qnil;
}
#endif

void 
Init_gtk_file_chooser_widget()
{
#if GTK_CHECK_VERSION(2,4,0)

    VALUE gFileChoWidget = G_DEF_CLASS(GTK_TYPE_FILE_CHOOSER_WIDGET, "FileChooserWidget", mGtk);

    rb_define_method(gFileChoWidget, "initialize", fchowidget_initialize, -1);

#endif
}

