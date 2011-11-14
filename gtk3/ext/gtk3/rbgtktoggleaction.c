/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2004,2005 Masao Mutoh
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

#if GTK_CHECK_VERSION(2,4,0)

#define RG_TARGET_NAMESPACE cToggleAction
#define _SELF(self) (GTK_TOGGLE_ACTION(RVAL2GOBJ(self)))

static VALUE
rg_initialize(VALUE self, VALUE name, VALUE label, VALUE tooltip, VALUE stock_id)
{
    const gchar *gstockid = NULL;

    if (TYPE(stock_id) == T_SYMBOL){
        gstockid = rb_id2name(SYM2ID(stock_id));
    } else if (TYPE(stock_id) == T_STRING){
        gstockid = RVAL2CSTR(stock_id);
    }

    G_INITIALIZE(self, gtk_toggle_action_new(RVAL2CSTR(name),
                                             RVAL2CSTR(label),
                                             NIL_P(tooltip) ? NULL : RVAL2CSTR(tooltip),
                                             gstockid));
    return Qnil;
}

static VALUE
rg_toggled(VALUE self)
{
    gtk_toggle_action_toggled(_SELF(self));
    return self;
}

#endif

void 
Init_gtk_toggle_action(VALUE mGtk)
{
#if GTK_CHECK_VERSION(2,4,0)

    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_TOGGLE_ACTION, "ToggleAction", mGtk);

    RG_DEF_METHOD(initialize, 4);
    RG_DEF_METHOD(toggled, 0);
#endif
}
