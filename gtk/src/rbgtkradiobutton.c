/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkradiobutton.c -

  $Author: mutoh $
  $Date: 2002/11/30 17:40:47 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

static VALUE gRButton;

#define _GROUP(s) (NIL_P(s) ? NULL : GTK_RADIO_BUTTON(RVAL2GOBJ(s)))

static GtkWidget*
create_button(group, label, use_underline)
    VALUE group, label, use_underline;
{
    GtkWidget* widget = NULL;

    if (TYPE(label) == T_STRING){
        if (NIL_P(use_underline) || RTEST(use_underline)){
            widget = gtk_radio_button_new_with_mnemonic_from_widget(_GROUP(group), RVAL2CSTR(label));
        } else {
            widget = gtk_radio_button_new_with_label_from_widget(_GROUP(group), RVAL2CSTR(label));
        }
    } else if (TYPE(label) == T_SYMBOL){
        widget = gtk_radio_button_new_with_label_from_widget(_GROUP(group), rb_id2name(SYM2ID(label)));
        gtk_button_set_use_stock(GTK_BUTTON(widget), TRUE);
    } else {
        rb_raise(rb_eArgError, "invalid argument %s (expect Symbol(Gtk::Stock constants) or String)", 
                         rb_class2name(CLASS_OF(label)));
    }
    return widget;
}

static VALUE
rbtn_initialize(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE group_or_label, label_or_use_underline, use_underline;
    GtkWidget *widget;

    if (rb_scan_args(argc, argv, "03", &group_or_label, &label_or_use_underline, &use_underline) > 0) {
        if (rb_obj_is_kind_of(group_or_label, gRButton)){
            widget = create_button(group_or_label, label_or_use_underline, use_underline);
        } else {
            widget = create_button(Qnil, group_or_label, label_or_use_underline);
        }
    } else {
        widget = gtk_radio_button_new(NULL);
    }
   
    RBGTK_INITIALIZE(self, widget);
    return Qnil;
}

static VALUE
rbtn_group(self)
    VALUE self;
{
    return GSLIST2ARY(gtk_radio_button_get_group(GTK_RADIO_BUTTON(RVAL2GOBJ(self))));
}

void 
Init_gtk_radio_button()
{
    gRButton = G_DEF_CLASS(GTK_TYPE_RADIO_BUTTON, "RadioButton", mGtk);

    rb_define_method(gRButton, "initialize", rbtn_initialize, -1);
    rb_define_method(gRButton, "group", rbtn_group, 0);
}
