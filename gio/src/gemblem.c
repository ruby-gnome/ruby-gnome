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

static VALUE
emblem_initialize(int argc, VALUE *argv, VALUE self)
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

static VALUE
emblem_icon(VALUE self)
{
        return GOBJ2RVAL(g_emblem_get_icon(_SELF(self)));
}

static VALUE
emblem_origin(VALUE self)
{
        return GEMBLEMORIGIN2RVAL(g_emblem_get_origin(_SELF(self)));
}

void
Init_gemblem(VALUE glib)
{
        VALUE emblem = G_DEF_INTERFACE(G_TYPE_EMBLEM, "Emblem", glib);

        G_DEF_CLASS(G_TYPE_EMBLEM_ORIGIN, "Origin", emblem);
	G_DEF_CONSTANTS(emblem, G_TYPE_EMBLEM_ORIGIN, "G_EMBLEM_");

        rb_define_method(emblem, "initialize", emblem_initialize, -1);
        rb_define_method(emblem, "icon", emblem_icon, 0);
        rb_define_method(emblem, "origin", emblem_origin, 0);
}
