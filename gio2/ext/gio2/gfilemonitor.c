/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2008-2009  Ruby-GNOME2 Project Team
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

#include "gio2.h"

#define _SELF(value) G_FILE_MONITOR(RVAL2GOBJ(value))

static VALUE
filemonitor_cancel(VALUE self)
{
        return CBOOL2RVAL(g_file_monitor_cancel(_SELF(self)));
}

void
Init_gfilemonitor(VALUE glib)
{
        VALUE filemonitor = G_DEF_CLASS(G_TYPE_FILE_MONITOR, "FileMonitor", glib);

        G_DEF_CLASS(G_TYPE_FILE_MONITOR_EVENT, "Event", filemonitor);
        G_DEF_CONSTANTS(filemonitor, G_TYPE_FILE_MONITOR_EVENT, "G_FILE_MONITOR_");

        G_DEF_CLASS(G_TYPE_FILE_MONITOR_FLAGS, "Flags", filemonitor);
        G_DEF_CONSTANTS(filemonitor, G_TYPE_FILE_MONITOR_FLAGS, "G_FILE_MONITOR_");

        rb_define_method(filemonitor, "cancel", filemonitor_cancel, 0);
        /* TODO: Do we need #emit_event? */
}
