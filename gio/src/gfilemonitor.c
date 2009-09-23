/* -*- c-file-style: "ruby" -*- */
/*
 * Ruby/GIO: a Ruby binding of gio-2.0.x.
 * Copyright (C) 2008-2009  Ruby-GNOME2 Project Team
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "gio2.h"

#define _SELF(value) \
        G_FILE_MONITOR(RVAL2GOBJ(value))

#define DEFSIMPLEMETHOD(name, type) \
        DECLARE(name)(VALUE self){ return type(g_file_monitor_##name(_SELF(self))); }

DEFSIMPLEMETHOD(cancel, CBOOL2RVAL)
DEFSIMPLEMETHOD(is_cancelled, CBOOL2RVAL)

DECLARE(set_rate_limit)(VALUE self, VALUE rate_limit)
{
        g_file_monitor_set_rate_limit(_SELF(self), NUM2INT(rate_limit));

        return self;
}

void
Init_gfilemonitor(VALUE glib)
{
        VALUE filemonitor = G_DEF_CLASS(G_TYPE_FILE_MONITOR,
                                        "FileMonitor",
                                        glib);

        DEF_CONSTANTS_CLASS(G_TYPE_FILE_MONITOR_FLAGS,
                            "Flags",
                            "G_FILE_MONITOR_",
                            filemonitor);
        DEF_CONSTANTS_CLASS(G_TYPE_FILE_MONITOR_EVENT,
                            "Event",
                            "G_FILE_MONITOR_",
                            filemonitor);

        rb_undef_alloc_func(filemonitor);

        DEF_METHOD(filemonitor, cancel, 0);
        DEF_IS_METHOD(filemonitor, cancelled, 0);
        DEF_SET_METHOD(filemonitor, rate_limit);
        /* TODO: Do we need #emit_event? */
}
