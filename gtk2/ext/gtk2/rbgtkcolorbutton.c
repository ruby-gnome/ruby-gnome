/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkcolorbutton.c -

  $Author: mutoh $
  $Date: 2004/05/30 16:41:13 $

  Copyright (C) 2004 Masao Mutoh
************************************************/

#include "global.h"
                                                                                
#if GTK_CHECK_VERSION(2,4,0)

static VALUE
colorbutton_initialize(int argc, VALUE *argv, VALUE self)
{
    VALUE color;
    GtkWidget* widget;

    rb_scan_args(argc, argv, "01", &color);

    if (NIL_P(color)){
        widget = gtk_color_button_new();
    } else {
        widget = gtk_color_button_new_with_color(RVAL2GDKCOLOR(color));
    }

    RBGTK_INITIALIZE(self, widget);
    return Qnil;
}

/* Defined as property.
void        gtk_color_button_set_color      (GtkColorButton *color_button,
                                             const GdkColor *color);
void        gtk_color_button_get_color      (GtkColorButton *color_button,
                                             GdkColor *color);
void        gtk_color_button_set_alpha      (GtkColorButton *color_button,
                                             guint16 alpha);
guint16     gtk_color_button_get_alpha      (GtkColorButton *color_button);
void        gtk_color_button_set_use_alpha  (GtkColorButton *color_button,
                                             gboolean use_alpha);
gboolean    gtk_color_button_get_use_alpha  (GtkColorButton *color_button);
void        gtk_color_button_set_title      (GtkColorButton *color_button,
                                             const gchar *title);
G_CONST_RETURN gchar* gtk_color_button_get_title
                                            (GtkColorButton *color_button);
*/

#endif

void 
Init_gtk_colorbutton()
{
#if GTK_CHECK_VERSION(2,4,0)
    VALUE gColorbutton = G_DEF_CLASS(GTK_TYPE_COLOR_BUTTON, "ColorButton", mGtk);
    rb_define_method(gColorbutton, "initialize", colorbutton_initialize, -1);
#endif
}
