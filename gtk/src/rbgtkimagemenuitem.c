/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkimagemenuitem.c -

  $Author: mutoh $
  $Date: 2003/01/19 14:28:25 $

  Copyright (C) 2002,2003 Masao Mutoh
************************************************/

#include "global.h"

static VALUE
imitem_initialize(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE label, use_underline_or_accel_group;
    GtkWidget *widget = NULL;

    if (rb_scan_args(argc, argv, "02", &label, &use_underline_or_accel_group) > 0) {
        if (TYPE(label) == T_STRING){
            if (NIL_P(use_underline_or_accel_group) || RTEST(use_underline_or_accel_group)){
                widget = gtk_image_menu_item_new_with_mnemonic(RVAL2CSTR(label));
            } else {
                widget = gtk_image_menu_item_new_with_label(RVAL2CSTR(label));
            }
        } else if (TYPE(label) == T_SYMBOL){
            widget = gtk_image_menu_item_new_from_stock(rb_id2name(SYM2ID(label)),
                                                        NIL_P(use_underline_or_accel_group) ? NULL :
                                                        GTK_ACCEL_GROUP(RVAL2GOBJ(use_underline_or_accel_group)));
        } else {
            rb_raise(rb_eArgError, "invalid argument %s (expect Symbol(Gtk::Stock constants) or String)", 
                     rb_class2name(CLASS_OF(label)));
        }
    } else {
	widget = gtk_image_menu_item_new();
    }

    RBGTK_INITIALIZE(self, widget);
    return Qnil;
}

void 
Init_gtk_image_menu_item()
{
    VALUE gIMenuItem = G_DEF_CLASS(GTK_TYPE_IMAGE_MENU_ITEM, "ImageMenuItem", mGtk);

    rb_define_method(gIMenuItem, "initialize", imitem_initialize, -1);
}
