/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkcheckbutton.c -

  $Author: mutoh $
  $Date: 2002/11/30 17:40:47 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

static VALUE
cbtn_initialize(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE label, use_underline;
    GtkWidget *widget;

    if (rb_scan_args(argc, argv, "02", &label, &use_underline) > 0) {
        if (TYPE(label) == T_STRING){
            if (NIL_P(use_underline) || RTEST(use_underline)){
                widget = gtk_check_button_new_with_mnemonic(RVAL2CSTR(label));
            } else {
                widget = gtk_check_button_new_with_label(RVAL2CSTR(label));
            }
        } else if (TYPE(label) == T_SYMBOL){
            widget = gtk_check_button_new_with_label(rb_id2name(SYM2ID(label)));
            gtk_button_set_use_stock(GTK_BUTTON(widget), TRUE);
        } else {
            rb_raise(rb_eArgError, "invalid argument %s (expect Symbol(Gtk::Stock constants) or String)", 
                     rb_class2name(CLASS_OF(label)));
        }
    }
    else {
        widget = gtk_check_button_new();
    }

    RBGTK_INITIALIZE(self, widget);
    return Qnil;
}

void 
Init_gtk_check_button()
{
    VALUE gCButton = G_DEF_CLASS(GTK_TYPE_CHECK_BUTTON, "CheckButton", mGtk);

    rb_define_method(gCButton, "initialize", cbtn_initialize, -1);
}
