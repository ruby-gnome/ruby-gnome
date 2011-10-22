/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2002-2004 Ruby-GNOME2 Project Team
 *  Copyright (C) 1998-2000 Yukihiro Matsumoto,
 *                          Daisuke Kanda,
 *                          Hiroshi Igarashi
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

#define _SELF(self) (GTK_BUTTON(RVAL2GOBJ(self)))

static VALUE
button_initialize(int argc, VALUE *argv, VALUE self)
{
    VALUE label, use_underline;
    GtkWidget *widget = NULL;

    if (rb_scan_args(argc, argv, "02", &label, &use_underline) > 0) {
        if (TYPE(label) == T_STRING){
            if (NIL_P(use_underline) || RVAL2CBOOL(use_underline)){
                widget = gtk_button_new_with_mnemonic(RVAL2CSTR(label));
            } else {
                widget = gtk_button_new_with_label(RVAL2CSTR(label));
            }
        } else if (TYPE(label) == T_SYMBOL){
            widget = gtk_button_new_from_stock(rb_id2name(SYM2ID(label)));
        } else {
            rb_raise(rb_eArgError, "invalid argument %s (expect Symbol(Gtk::Stock constants) or String)", 
                     rb_class2name(CLASS_OF(label)));
        }
    } else {
	widget = gtk_button_new();
    }

    RBGTK_INITIALIZE(self, widget); 
    return Qnil;
}

static VALUE
button_pressed(VALUE self)
{
    gtk_button_pressed(GTK_BUTTON(RVAL2GOBJ(self)));
    return self;
}

static VALUE
button_released(VALUE self)
{
    gtk_button_released(_SELF(self));
    return self;
}

static VALUE
button_enter(VALUE self)
{
    gtk_button_enter(_SELF(self));
    return self;
}

static VALUE
button_leave(VALUE self)
{
    gtk_button_leave(_SELF(self));
    return self;
}

#if GTK_CHECK_VERSION(2,4,0)
static VALUE
button_set_alignment(VALUE self, VALUE xalign, VALUE yalign)
{
    gtk_button_set_alignment(_SELF(self), NUM2DBL(xalign), NUM2DBL(yalign));
    return self;
}

static VALUE
button_get_alignment(VALUE self)
{
    gfloat xalign, yalign;
    gtk_button_get_alignment(_SELF(self), &xalign, &yalign);

    return rb_assoc_new(rb_float_new(xalign), rb_float_new(yalign));
}
#endif

/* Defined as properties.
void        gtk_button_set_relief           (GtkButton *button,
                                             GtkReliefStyle newstyle);
GtkReliefStyle gtk_button_get_relief        (GtkButton *button);
const gchar* gtk_button_get_label           (GtkButton *button);
void        gtk_button_set_label            (GtkButton *button,
                                             const gchar *label);
gboolean    gtk_button_get_use_stock        (GtkButton *button);
void        gtk_button_set_use_stock        (GtkButton *button,
                                             gboolean use_stock);
gboolean    gtk_button_get_use_underline    (GtkButton *button);
void        gtk_button_set_use_underline    (GtkButton *button,
                                             gboolean use_underline);
void        gtk_button_set_focus_on_click   (GtkButton *button,
                                             gboolean focus_on_click);
gboolean    gtk_button_get_focus_on_click   (GtkButton *button);
void        gtk_button_set_image            (GtkButton *button,
                                             GtkWidget *image);
GtkWidget*  gtk_button_get_image            (GtkButton *button);
void        gtk_button_set_image_position   (GtkButton *button,
                                             GtkPositionType position);
GtkPositionType gtk_button_get_image_position
                                            (GtkButton *button);
*/

void 
Init_gtk_button(void)
{
    VALUE gButton = G_DEF_CLASS(GTK_TYPE_BUTTON, "Button", mGtk);

    rb_define_method(gButton, "initialize", button_initialize, -1);
    rb_define_method(gButton, "pressed", button_pressed, 0);
    rb_define_method(gButton, "released", button_released, 0);
    rb_define_method(gButton, "enter", button_enter, 0);
    rb_define_method(gButton, "leave", button_leave, 0);

#if GTK_CHECK_VERSION(2,4,0)
    rb_define_method(gButton, "set_alignment", button_set_alignment, 2);
    G_DEF_SETTER(gButton, "alignment");
    rb_define_method(gButton, "alignment", button_get_alignment, 0);
#endif
}
