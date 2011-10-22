/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2004  Masao Mutoh
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
Init_gtk_colorbutton(void)
{
#if GTK_CHECK_VERSION(2,4,0)
    VALUE gColorbutton = G_DEF_CLASS(GTK_TYPE_COLOR_BUTTON, "ColorButton", mGtk);
    rb_define_method(gColorbutton, "initialize", colorbutton_initialize, -1);
#endif
}
