/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkfilechooserbutton.c -

  $Author: mutoh $
  $Date: 2005/01/09 09:20:30 $

  Copyright (C) 2005 Masao Mutoh
************************************************/

#include "global.h"
                                                                                
#if GTK_CHECK_VERSION(2,6,0)

static VALUE
filechooserbutton_initialize(argc, argv, self)
    int argc;
    VALUE* argv;
    VALUE self;
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

/* Defined as Properties
const gchar* gtk_file_chooser_button_get_title
                                            (GtkFileChooserButton *button);
void        gtk_file_chooser_button_set_title
                                            (GtkFileChooserButton *button,
                                             const gchar *title);
gint        gtk_file_chooser_button_get_width_chars
                                            (GtkFileChooserButton *button);
void        gtk_file_chooser_button_set_width_chars
                                            (GtkFileChooserButton *button,
                                             gint n_chars);
*/
#endif

void 
Init_gtk_filechooserbutton()
{
#if GTK_CHECK_VERSION(2,6,0)
    VALUE gFCButton = G_DEF_CLASS(GTK_TYPE_FILE_CHOOSER_BUTTON, "FileChooserButton", mGtk);
    rb_define_method(gFCButton, "initialize", filechooserbutton_initialize, -1);
#endif
}
