/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************
 
  rbgtkradioaction.c -
 
  $Author: mutoh $
  $Date: 2004/05/30 16:41:13 $
 
  Copyright (C) 2004 Masao Mutoh
************************************************/
 
#include "global.h"

#if GTK_CHECK_VERSION(2,4,0)

#define _SELF(self) (GTK_RADIO_ACTION(RVAL2GOBJ(self)))

static VALUE
raction_initialize(self, name, label, tooltip, stock_id, value)
    VALUE self, name, label, tooltip, stock_id, value;
{
    G_INITIALIZE(self, gtk_radio_action_new(RVAL2CSTR(name),
                                            RVAL2CSTR(label),
                                            RVAL2CSTR(tooltip),
                                            RVAL2CSTR(stock_id),
                                            NUM2INT(value)));
    return Qnil;
}
/* Defined as Property
GSList*     gtk_radio_action_get_group      (GtkRadioAction *action);
void        gtk_radio_action_set_group      (GtkRadioAction *action,
                                             GSList *group);
*/

static VALUE
raction_get_current_value(self)
    VALUE self;
{
    return INT2NUM(gtk_radio_action_get_current_value(_SELF(self)));
}
    
#endif

void 
Init_gtk_radio_action()
{
#if GTK_CHECK_VERSION(2,4,0)

    VALUE gRadioAction = G_DEF_CLASS(GTK_TYPE_RADIO_ACTION, "RadioAction", mGtk);

    rb_define_method(gRadioAction, "initialize", raction_initialize, 5);
    rb_define_method(gRadioAction, "current_value", raction_get_current_value, 0);
#endif
}

