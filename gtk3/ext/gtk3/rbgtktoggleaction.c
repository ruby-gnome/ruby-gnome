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

#include "rbgtk3private.h"

#define RG_TARGET_NAMESPACE cToggleAction
#define _SELF(self) (RVAL2GTKTOGGLEACTION(self))

static VALUE
rg_initialize(int argc, VALUE *argv, VALUE self)
{
    VALUE name, options, label, tooltip, stock_id, buffer;

    rb_scan_args(argc, argv, "11", &name, &options);
    rbg_scan_options(options,
                     "label", &label,
                     "tooltip", &tooltip,
                     "stock_id", &stock_id,
                     NULL);

    G_INITIALIZE(self, gtk_toggle_action_new(RVAL2CSTR(name),
                                             RVAL2CSTR_ACCEPT_NIL(label),
                                             RVAL2CSTR_ACCEPT_NIL(tooltip),
                                             RVAL2GLIBID_ACCEPT_NIL(stock_id, buffer)));

    return Qnil;
}

static VALUE
rg_toggled(VALUE self)
{
    gtk_toggle_action_toggled(_SELF(self));
    return self;
}

void 
Init_gtk_toggle_action(VALUE mGtk)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_TOGGLE_ACTION, "ToggleAction", mGtk);

    RG_DEF_METHOD(initialize, -1);
    RG_DEF_METHOD(toggled, 0);
}
