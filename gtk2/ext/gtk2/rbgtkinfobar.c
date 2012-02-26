/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2012  Ruby-GNOME2 Project Team
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

#if GTK_CHECK_VERSION(2,18,0)
#define RG_TARGET_NAMESPACE cInfoBar
#define _SELF(self) (GTK_INFO_BAR(RVAL2GOBJ(self)))

static VALUE
rg_initialize(int argc, VALUE *argv, VALUE self)
{
    if (argc == 0){
        RBGTK_INITIALIZE(self, gtk_info_bar_new ());
    } else if (argc > 0){
        //TODO gtk_info_bar_new_with_buttons
        rb_raise(rb_eArgError, "method does not yet supported adding buttons");
       } else {
        rb_raise(rb_eArgError, "invalid argument number");
    }
    return Qnil;
}

static VALUE
rg_get_action_area(VALUE self)
{
    return GOBJ2RVAL(gtk_info_bar_get_action_area(_SELF(self)));
}

static VALUE
rg_get_content_area(VALUE self)
{
    return GOBJ2RVAL(gtk_info_bar_get_content_area(_SELF(self)));
}

static VALUE
rg_get_message_type(VALUE self)
{
    return GENUM2RVAL(gtk_info_bar_get_message_type(_SELF(self)),
                      GTK_TYPE_MESSAGE_TYPE);
}

static VALUE
rg_set_message_type(VALUE self, VALUE type)
{
    gtk_info_bar_set_message_type(_SELF(self),
                                  RVAL2GENUM(type, GTK_TYPE_MESSAGE_TYPE));
    return self;
}

static VALUE
rg_set_default_response(VALUE self, VALUE response_id)
{
    gtk_info_bar_set_default_response(_SELF(self),
                                      RVAL2GENUM(response_id, GTK_TYPE_RESPONSE_TYPE));
    return self;
}

static VALUE
rg_add_action_widget(VALUE self, VALUE widget, VALUE response_id)
{
    gtk_info_bar_add_action_widget(_SELF(self),
                                   GTK_WIDGET(RVAL2GOBJ(widget)), 
                                   RVAL2GENUM(response_id, GTK_TYPE_RESPONSE_TYPE));
    return self;
}

static VALUE
rg_add_button(VALUE self, VALUE button_text, VALUE response_id)
{
    const gchar *name;
    if (SYMBOL_P(button_text)) {
        name = rb_id2name(SYM2ID(button_text));
    } else {
        name = RVAL2CSTR(button_text);
    }
    return GOBJ2RVAL(gtk_info_bar_add_button(_SELF(self), name, 
                                             RVAL2GENUM(response_id, GTK_TYPE_RESPONSE_TYPE)));
}

/*
static VALUE
rg_add_buttons(int argc, VALUE *argv, VALUE self)
{
}
*/

static VALUE
rg_set_response_sensitive(VALUE self, VALUE response_id, VALUE sensitive)
{
     gtk_info_bar_set_response_sensitive(_SELF(self),
                                         RVAL2GENUM(response_id, GTK_TYPE_RESPONSE_TYPE),
                                         RVAL2CBOOL(sensitive));
     return self;
}

static VALUE
rg_response(VALUE self, VALUE response_id)
{
    gtk_info_bar_response(_SELF(self), RVAL2GENUM(response_id, GTK_TYPE_RESPONSE_TYPE));
    return self;
}
#endif

void 
Init_gtk_infobar(VALUE mGtk)
{
#if GTK_CHECK_VERSION(2,18,0)
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_INFO_BAR, "InfoBar", mGtk);

    RG_DEF_METHOD(initialize, -1);
    RG_DEF_METHOD(get_action_area, 0);
    RG_DEF_ALIAS("action_area", "get_action_area");
    RG_DEF_METHOD(get_content_area, 0);
    RG_DEF_ALIAS("content_area", "get_content_area");
    RG_DEF_METHOD(get_message_type, 0);
    RG_DEF_ALIAS("message_type", "get_message_type");
    RG_DEF_METHOD(set_message_type, 1);
    G_DEF_SETTER(RG_TARGET_NAMESPACE, "message_type");
    RG_DEF_METHOD(set_default_response, 1);
    RG_DEF_METHOD(add_action_widget, 2);
    RG_DEF_METHOD(add_button, 2);
/*
    RG_DEF_METHOD(add_buttons, -1);
*/
    RG_DEF_METHOD(set_response_sensitive, 2);
    RG_DEF_METHOD(response, 1);
#endif
}

