/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkmenutoolbutton.c -

  $Author: mutoh $
  $Date: 2005/01/09 09:20:30 $

  Copyright (C) 2005 Masao Mutoh
************************************************/

#include "global.h"
                                                                                
#if GTK_CHECK_VERSION(2,6,0)

#define _SELF(self) (GTK_MENU_TOOL_BUTTON(RVAL2GOBJ(self)))

static VALUE
menutoolbutton_initialize(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    GtkToolItem* item;

    if (argc == 1){
        VALUE stock_id;
        rb_scan_args(argc, argv, "10", &stock_id);

        if (TYPE(stock_id) == T_SYMBOL){
            item = gtk_menu_tool_button_new_from_stock(rb_id2name(SYM2ID(stock_id)));
        } else {
            item = gtk_menu_tool_button_new_from_stock(RVAL2CSTR(stock_id));
        }
    } else {
        VALUE icon_widget, label;
        rb_scan_args(argc, argv, "20", &icon_widget, label);

        item = gtk_menu_tool_button_new(GTK_WIDGET(RVAL2GOBJ(icon_widget)),
                                        RVAL2CSTR(label));
    }

    RBGTK_INITIALIZE(self, item);
    return Qnil;
}

static VALUE
menutoolbutton_set_arrow_tooltip(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE tooltips, tip_text, tip_private;

    rb_scan_args(argc, argv, "12", &tooltips, &tip_text, &tip_private);
    gtk_menu_tool_button_set_arrow_tooltip(_SELF(self), GTK_TOOLTIPS(RVAL2GOBJ(self)),
                                           NIL_P(tip_text) ? NULL : RVAL2CSTR(tip_text),
                                           NIL_P(tip_private) ? NULL : RVAL2CSTR(tip_private));
    return self;
}
#endif

void 
Init_gtk_menutoolbutton()
{
#if GTK_CHECK_VERSION(2,6,0)
    VALUE gMenuToolButton = G_DEF_CLASS(GTK_TYPE_MENU_TOOL_BUTTON, "MenuToolButton", mGtk);

    rb_define_method(gMenuToolButton, "initialize", menutoolbutton_initialize, -1);
    rb_define_method(gMenuToolButton, "set_arrow_tooltip", menutoolbutton_set_arrow_tooltip, -1);
#endif
}
