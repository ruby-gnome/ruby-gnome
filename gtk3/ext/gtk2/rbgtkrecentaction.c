/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2002,2003 Ruby-GNOME2 Project Team
 *  Copyright (C) 1998-2000 Yukihiro Matsumoto,
 *                          Daisuke Kanda,
 *                          Hiroshi Igarashi
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

#if GTK_CHECK_VERSION(2,12,0)

#define RG_TARGET_NAMESPACE cRecentAction

static VALUE
rg_initialize(int argc, VALUE *argv, VALUE self)
{
    VALUE  name, label, tooltip, stock_id, manager;
    rb_scan_args(argc, argv, "23", &name, &label, &tooltip, &stock_id, &manager);
    if (NIL_P(manager)) {
        G_INITIALIZE(self, gtk_recent_action_new(RVAL2CSTR(name),
                                                 RVAL2CSTR(label),
                                                 NIL_P(tooltip) ? NULL : RVAL2CSTR(tooltip),
                                                 NIL_P(stock_id) ? NULL : RVAL2CSTR(stock_id)));
    } else {
        G_INITIALIZE(self, gtk_recent_action_new_for_manager(RVAL2CSTR(name),
                                                             RVAL2CSTR(label),
                                                             NIL_P(tooltip) ? NULL : RVAL2CSTR(tooltip),
                                                             NIL_P(stock_id) ? NULL : RVAL2CSTR(stock_id),
                                                             GTK_RECENT_MANAGER(RVAL2GOBJ(manager))));
    }
    return Qnil;
}

#endif

void 
Init_gtk_recentaction(VALUE mGtk)
{
#if GTK_CHECK_VERSION(2,12,0)
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_RECENT_ACTION, "RecentAction", mGtk);
    RG_DEF_METHOD(initialize, -1);
#endif
}
