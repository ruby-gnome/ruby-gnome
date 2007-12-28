/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************
 
  rbgtkradioaction.c -
 
  $Author: mutoh $
  $Date: 2006/12/26 16:11:13 $
 
  Copyright (C) 2004-2006 Masao Mutoh
************************************************/
 
#include "global.h"

#if GTK_CHECK_VERSION(2,4,0)

#define _SELF(self) (GTK_RADIO_ACTION(RVAL2GOBJ(self)))

static VALUE
raction_initialize(self, name, label, tooltip, stock_id, value)
    VALUE self, name, label, tooltip, stock_id, value;
{
    const gchar *gstock = NULL;

    if (TYPE(stock_id) == T_STRING){
        gstock = RVAL2CSTR(stock_id);
    } else if (TYPE(stock_id) == T_SYMBOL) {
        gstock = rb_id2name(SYM2ID(stock_id));
    }
    G_INITIALIZE(self, gtk_radio_action_new(RVAL2CSTR(name),
                                            RVAL2CSTR(label),
                                            NIL_P(tooltip) ? NULL : RVAL2CSTR(tooltip),
                                            gstock,
                                            NUM2INT(value)));
    return Qnil;
}

static VALUE
raction_get_group(self)
    VALUE self;
{
    /* Owned by GTK+ */
    return GSLIST2ARY(gtk_radio_action_get_group(_SELF(self)));
}

static VALUE
raction_set_group(self, group)
    VALUE self, group;
{
    long i;
    GSList *glist = NULL;

    if (TYPE(group) == T_ARRAY){
        for (i = 0; i < RARRAY(group)->len; i++) {
            glist = g_slist_append(glist, RVAL2GOBJ(RARRAY(group)->ptr[i]));
        }
        gtk_radio_action_set_group(_SELF(group), glist);
        g_slist_free(glist);
    } else {
        glist = gtk_radio_action_get_group(GTK_RADIO_ACTION(RVAL2GOBJ(group))); 
        gtk_radio_action_set_group(_SELF(group), glist);
    }
    return self;
}

#if ! GTK_CHECK_VERSION(2,10,0)
static VALUE
raction_get_current_value(self)
    VALUE self;
{
    return INT2NUM(gtk_radio_action_get_current_value(_SELF(self)));
}
#endif
    
#endif

/* Defined as Properties
void        gtk_radio_action_set_current_value
                                            (GtkRadioAction *action,
                                             gint current_value);
*/

void 
Init_gtk_radio_action()
{
#if GTK_CHECK_VERSION(2,4,0)

    VALUE gRadioAction = G_DEF_CLASS(GTK_TYPE_RADIO_ACTION, "RadioAction", mGtk);

    rb_define_method(gRadioAction, "initialize", raction_initialize, 5);
#if ! GTK_CHECK_VERSION(2,10,0)
    /* Define as Property since 2.10 */
    rb_define_method(gRadioAction, "current_value", raction_get_current_value, 0);
#endif
    rb_undef_method(gRadioAction, "group");
    rb_undef_method(gRadioAction, "set_group");
    rb_define_method(gRadioAction, "group", raction_get_group, 0);
    rb_define_method(gRadioAction, "set_group", raction_set_group, 1);

    G_DEF_SETTERS(gRadioAction);
#endif
}

