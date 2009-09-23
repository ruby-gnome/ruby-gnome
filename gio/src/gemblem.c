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
        RVAL2GEMBLEM(value)

DECLARE_VARG(initialize)
{
        VALUE rbicon,
              origin;
        GIcon *icon;

        rb_scan_args(argc, argv, "11", &rbicon, &origin);
        icon = RVAL2GICON(rbicon);

        if (!NIL_P(origin))
                G_INITIALIZE(self,
                             g_emblem_new_with_origin(icon,
                                                      RVAL2GEMBLEMORIGIN(origin)));
        else
                G_INITIALIZE(self, g_emblem_new(icon));

        return Qnil;
}

#define DEFGETMETHOD(name, type) \
        DECLARE(name)(VALUE self){ return type(g_emblem_get_##name(_SELF(self))); }

DEFGETMETHOD(icon, GOBJ2RVAL)
DEFGETMETHOD(origin, GEMBLEMORIGIN2RVAL)

void
Init_gemblem(VALUE glib)
{
        VALUE emblem = G_DEF_INTERFACE(G_TYPE_EMBLEM, "Emblem", glib);

        DEF_CONSTANTS_CLASS(G_TYPE_EMBLEM_ORIGIN,
                            "Origin",
                            "G_EMBLEM_",
                            emblem);

        DEF_METHOD(emblem, initialize, -1);
        DEF_METHOD(emblem, icon, 0);
        DEF_METHOD(emblem, origin, 0);
}
