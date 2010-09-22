/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtktoggletoolbutton.c -

  $Author: ggc $
  $Date: 2007/07/13 16:07:32 $

  Copyright (C) 2004 Masao Mutoh
************************************************/

#include "global.h"
                                                                                
#if GTK_CHECK_VERSION(2,4,0)

#define _SELF(self) (GTK_TOGGLE_TOOL_BUTTON(RVAL2GOBJ(self)))
static VALUE
toggletoolbutton_initialize(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE stock_id;
    GtkToolItem* item;

    rb_scan_args(argc, argv, "01", &stock_id);

    if (NIL_P(stock_id)){
        item = gtk_toggle_tool_button_new();
    } else {
        if (TYPE(stock_id) == T_SYMBOL){
            item = gtk_toggle_tool_button_new_from_stock(rb_id2name(SYM2ID(stock_id)));
        } else {
            item = gtk_toggle_tool_button_new_from_stock(RVAL2CSTR(stock_id));
        }
    }
    RBGTK_INITIALIZE(self, item);
    return Qnil;
}

static VALUE
toggletoolbutton_set_active(self, is_active)
    VALUE self, is_active;
{
    gtk_toggle_tool_button_set_active(_SELF(self), RVAL2CBOOL(is_active));
    return self;
}

static VALUE
toggletoolbutton_get_active(self)
    VALUE self;
{
    return CBOOL2RVAL(gtk_toggle_tool_button_get_active(_SELF(self)));
}

#endif

void 
Init_gtk_toggletoolbutton()
{
#if GTK_CHECK_VERSION(2,4,0)
    VALUE gToggleToolButton = G_DEF_CLASS(GTK_TYPE_TOGGLE_TOOL_BUTTON, "ToggleToolButton", mGtk);

    rb_define_method(gToggleToolButton, "initialize", toggletoolbutton_initialize, -1);
    rb_define_method(gToggleToolButton, "set_active", toggletoolbutton_set_active, 1);
    rb_define_method(gToggleToolButton, "active?", toggletoolbutton_get_active, 0);

    G_DEF_SETTERS(gToggleToolButton);
#endif
}
