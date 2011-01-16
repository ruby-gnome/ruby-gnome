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

#define RVAL2GEMBLEMORIGIN(value) \
        RVAL2GENUM((value), G_TYPE_EMBLEM_ORIGIN)

static VALUE
emblem_initialize(int argc, VALUE *argv, VALUE self)
{
        VALUE rbicon, origin;
        GIcon *icon;
        GEmblem *emblem;

        rb_scan_args(argc, argv, "11", &rbicon, &origin);
        icon = RVAL2GICON(rbicon);
        emblem = NIL_P(origin) ?
                g_emblem_new(icon) :
                g_emblem_new_with_origin(icon, RVAL2GEMBLEMORIGIN(origin));
        G_INITIALIZE(self, emblem);

        return Qnil;
}

void
Init_gemblem(VALUE glib)
{
        VALUE emblem = G_DEF_CLASS(G_TYPE_EMBLEM, "Emblem", glib);

        G_DEF_CLASS(G_TYPE_EMBLEM_ORIGIN, "Origin", emblem);
        G_DEF_CONSTANTS(emblem, G_TYPE_EMBLEM_ORIGIN, "G_EMBLEM_");

        rb_define_method(emblem, "initialize", emblem_initialize, -1);
}
