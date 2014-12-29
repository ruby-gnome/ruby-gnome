/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2014 Ruby-GNOME2 Project Team
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

#if GTK_CHECK_VERSION(3, 10, 0)

#define RG_TARGET_NAMESPACE cSearchBar
#define _SELF(self) (RVAL2GTKSEARCHBAR(self))

static VALUE
rg_initialize(VALUE self)
{
    RBGTK_INITIALIZE(self, gtk_search_bar_new());
    return Qnil;
}

static VALUE
rg_connect_entry(VALUE self, VALUE entry)
{
    gtk_search_bar_connect_entry(_SELF(self), RVAL2GTKENTRY(entry));
    return self;
}

static VALUE
rg_handle_event_p(VALUE self, VALUE event)
{
    gboolean handle = gtk_search_bar_handle_event(_SELF(self),
                                                  RVAL2GDKEVENT(event));
    return CBOOL2RVAL(handle);
}

void
Init_gtk_search_bar(VALUE mGtk)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_SEARCH_BAR, "SearchBar", mGtk);

    RG_DEF_METHOD(initialize, 0);
    RG_DEF_METHOD(connect_entry, 1);
    RG_DEF_METHOD_P(handle_event, 1);
}
#endif
