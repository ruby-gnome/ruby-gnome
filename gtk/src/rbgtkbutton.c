/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkbutton.c -

  $Author: mutoh $
  $Date: 2004/05/24 17:22:58 $

  Copyright (C) 2002-2004 Ruby-GNOME2 Project Team
  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

#define _SELF(self) (GTK_BUTTON(RVAL2GOBJ(self)))

static VALUE
button_initialize(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE label, use_underline;
    GtkWidget *widget = NULL;

    if (rb_scan_args(argc, argv, "02", &label, &use_underline) > 0) {
        if (TYPE(label) == T_STRING){
            if (NIL_P(use_underline) || RTEST(use_underline)){
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
button_pressed(self)
    VALUE self;
{
    gtk_button_pressed(GTK_BUTTON(RVAL2GOBJ(self)));
    return self;
}

static VALUE
button_released(self)
    VALUE self;
{
    gtk_button_released(_SELF(self));
    return self;
}

static VALUE
button_clicked(self)
    VALUE self;
{
    gtk_button_clicked(_SELF(self));
    return self;
}

static VALUE
button_enter(self)
    VALUE self;
{
    gtk_button_enter(_SELF(self));
    return self;
}

static VALUE
button_leave(self)
    VALUE self;
{
    gtk_button_leave(_SELF(self));
    return self;
}

#if GTK_CHECK_VERSION(2,4,0)
static VALUE
button_set_alignment(self, xalign, yalign)
    VALUE self, xalign, yalign;
{
    gtk_button_set_alignment(_SELF(self), NUM2DBL(xalign), NUM2DBL(yalign));
    return self;
}

static VALUE
button_get_alignment(self)
    VALUE self;
{
    gfloat xalign, yalign;
    gtk_button_get_alignment(_SELF(self), &xalign, &yalign);

    return rb_assoc_new(rb_float_new(xalign), rb_float_new(yalign));
}
#endif

void 
Init_gtk_button()
{
    VALUE gButton = G_DEF_CLASS(GTK_TYPE_BUTTON, "Button", mGtk);

    rb_define_method(gButton, "initialize", button_initialize, -1);
    rb_define_method(gButton, "pressed", button_pressed, 0);
    rb_define_method(gButton, "released", button_released, 0);
    rb_define_method(gButton, "clicked", button_clicked, 0);
    rb_define_method(gButton, "enter", button_enter, 0);
    rb_define_method(gButton, "leave", button_leave, 0);

#if GTK_CHECK_VERSION(2,4,0)
    rb_define_method(gButton, "set_alignment", button_set_alignment, 2);
    rb_define_method(gButton, "alignment", button_get_alignment, 0);
#endif
    G_DEF_SETTERS(gButton);
}
