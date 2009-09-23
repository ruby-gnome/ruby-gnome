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
        G_EMBLEMED_ICON(RVAL2GOBJ(value))

DECLARE(initialize)(VALUE self, VALUE icon, VALUE emblem)
{
        G_INITIALIZE(self,
                     g_emblemed_icon_new(RVAL2GICON(icon),
                                         RVAL2GEMBLEM(emblem)));

        return Qnil;
}

#define DEFGETMETHOD(name, type) \
        DECLARE(name)(VALUE self) \
        { \
                return type(g_emblemed_icon_get_##name(_SELF(self))); \
        }

DEFGETMETHOD(icon, GOBJ2RVAL)
DEFGETMETHOD(emblems, GLIST2ARY_FREE)

DECLARE(add_emblem)(VALUE self, VALUE emblem)
{
        g_emblemed_icon_add_emblem(_SELF(self), RVAL2GEMBLEM(emblem));

        return self;
}

void
Init_gemblemedicon(VALUE glib)
{
        VALUE emblemedicon = G_DEF_INTERFACE(G_TYPE_EMBLEMED_ICON,
                                             "EmblemedIcon",
                                             glib);

        DEF_METHOD(emblemedicon, initialize, 2);
        DEF_METHOD(emblemedicon, icon, 0);
        DEF_METHOD(emblemedicon, emblems, 0);
        DEF_METHOD(emblemedicon, add_emblem, 1);
        rb_define_alias(emblemedicon, "<<", "add_emblem");
}
