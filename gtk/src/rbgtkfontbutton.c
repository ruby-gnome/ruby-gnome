/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkfontbutton.c -

  $Author: mutoh $
  $Date: 2004/05/30 16:41:13 $

  Copyright (C) 2004 Masao Mutoh
************************************************/

#include "global.h"
                                                                                
#if GTK_CHECK_VERSION(2,4,0)

static VALUE
fontbutton_initialize(argc, argv, self)
    int argc;
    VALUE* argv;
    VALUE self;
{
    VALUE fontname;
    GtkWidget* widget;

    rb_scan_args(argc, argv, "01", &fontname);

    if (NIL_P(fontname)){
        widget = gtk_font_button_new();
    } else {
        widget = gtk_font_button_new_with_font(RVAL2CSTR(fontname));
    }

    RBGTK_INITIALIZE(self, widget);
    return Qnil;
}

/* Defined as property.
gboolean    gtk_font_button_set_font_name   (GtkFontButton *font_button,
                                             const gchar *fontname);
G_CONST_RETURN gchar* gtk_font_button_get_font_name
                                            (GtkFontButton *font_button);
void        gtk_font_button_set_show_style  (GtkFontButton *font_button,
                                             gboolean show_style);
gboolean    gtk_font_button_get_show_style  (GtkFontButton *font_button);
void        gtk_font_button_set_show_size   (GtkFontButton *font_button,
                                             gboolean show_size);
gboolean    gtk_font_button_get_show_size   (GtkFontButton *font_button);
void        gtk_font_button_set_use_font    (GtkFontButton *font_button,
                                             gboolean use_font);
gboolean    gtk_font_button_get_use_font    (GtkFontButton *font_button);
void        gtk_font_button_set_use_size    (GtkFontButton *font_button,
                                             gboolean use_size);
gboolean    gtk_font_button_get_use_size    (GtkFontButton *font_button);
void        gtk_font_button_set_title       (GtkFontButton *font_button,
                                             const gchar *title);
G_CONST_RETURN gchar* gtk_font_button_get_title
                                            (GtkFontButton *font_button);
*/

#endif

void 
Init_gtk_fontbutton()
{
#if GTK_CHECK_VERSION(2,4,0)
    VALUE gFontbutton = G_DEF_CLASS(GTK_TYPE_FONT_BUTTON, "FontButton", mGtk);
    rb_define_method(gFontbutton, "initialize", fontbutton_initialize, -1);
#endif
}
