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

#define _SELF(self) (GTK_EXPANDER(RVAL2GOBJ(self)))

static VALUE
expander_initialize(int argc, VALUE *argv, VALUE self)
{
    VALUE label, with_mnemonic;
    GtkWidget* widget = NULL;

    rb_scan_args(argc, argv, "11", &label, &with_mnemonic);
    if (NIL_P(with_mnemonic) || ! RVAL2CBOOL(with_mnemonic)){
        widget = gtk_expander_new(RVAL2CSTR(label));
    } else {
        widget = gtk_expander_new_with_mnemonic(RVAL2CSTR(label));
    }

    RBGTK_INITIALIZE(self, widget);
    return Qnil;
}
/* Defined as Property.
void        gtk_expander_set_expanded       (GtkExpander *expander,
                                             gboolean expanded);
gboolean    gtk_expander_get_expanded       (GtkExpander *expander);
void        gtk_expander_set_spacing        (GtkExpander *expander,
                                             gint spacing);
gint        gtk_expander_get_spacing        (GtkExpander *expander);
void        gtk_expander_set_label          (GtkExpander *expander,
                                             const gchar *label);
G_CONST_RETURN gchar* gtk_expander_get_label
                                            (GtkExpander *expander);
void        gtk_expander_set_use_underline  (GtkExpander *expander,
                                             gboolean use_underline);
gboolean    gtk_expander_get_use_underline  (GtkExpander *expander);
void        gtk_expander_set_use_markup     (GtkExpander *expander,
                                             gboolean use_markup);
gboolean    gtk_expander_get_use_markup     (GtkExpander *expander);
void        gtk_expander_set_label_widget   (GtkExpander *expander,
                                             GtkWidget *label_widget);
GtkWidget*  gtk_expander_get_label_widget   (GtkExpander *expander);
 */
#endif

void 
Init_gtk_expander(void)
{
#if GTK_CHECK_VERSION(2,4,0)
    VALUE gExpander = G_DEF_CLASS(GTK_TYPE_EXPANDER, "Expander", mGtk);
    rb_define_method(gExpander, "initialize", expander_initialize, -1);
#endif
}
