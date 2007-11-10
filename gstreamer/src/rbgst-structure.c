/*
 * Copyright (C) 2003, 2004 Laurent Sansonetti <lrz@gnome.org>
 * Copyright (C) 2007 Ruby-GNOME2 Project Team
 *
 * This file is part of Ruby/GStreamer.
 *
 * Ruby/GStreamer is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * Ruby/GStreamer is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with Ruby/GStreamer; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307  USA
 */

#include <st.h>
#include "rbgst.h"

/*
 * Since GstStructure is basically a hash it is more natural to
 * automatically convert it as a Ruby Hash object, and vice-versa.
 */

static gboolean
foreach_pair(GQuark field_id, const GValue *value, gpointer user_data)
{
    VALUE hash;

    hash = *(VALUE *)user_data;
    rb_hash_aset(hash,
                 CSTR2RVAL(g_quark_to_string (field_id)),
                 GVAL2RVAL(value));

    return TRUE;
}

VALUE
rbgst_structure_to_hash(GstStructure *structure)
{
    VALUE hash;

    if (!structure)
        return Qnil;

    hash = rb_hash_new();
    gst_structure_foreach(structure, foreach_pair, &hash);
    return hash;
}

static int
each_hash(VALUE key, VALUE value, VALUE data)
{
    GstStructure *structure = (GstStructure *)data;
    GValue g_value = {0, };

    g_value_init(&g_value, RVAL2GTYPE(value));
    rbgobj_rvalue_to_gvalue(value, &g_value);
    gst_structure_set_value(structure, RVAL2CSTR(key), &g_value);
    g_value_unset(&g_value);

    return ST_CONTINUE;
}

GstStructure *
rbgst_hash_to_structure_with_name(VALUE hash, const char *name)
{
    GstStructure *structure;

    if (NIL_P(hash))
        return NULL;

    Check_Type(hash, T_HASH);

    structure = gst_structure_empty_new(name);
    rb_hash_foreach(hash, each_hash, (VALUE)structure);
    return structure;
}

GstStructure *
rbgst_hash_to_structure(VALUE hash)
{
    return rbgst_hash_to_structure_with_name(hash, "");
}
