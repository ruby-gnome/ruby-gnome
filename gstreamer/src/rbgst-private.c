/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 * Copyright (C) 2008 Ruby-GNOME2 Project Team
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

#include "rbgst.h"
#include "rbgst-private.h"

char *
_rbgst_inspect(VALUE object)
{
    VALUE inspected;
    inspected = rb_funcall(object, rb_intern("inspect"), 0);
    return StringValueCStr(inspected);
}

void
_rbgst_define_class_if_need(VALUE klass, GType type, const gchar *prefix)
{
    VALUE parent;
    const gchar *type_name;
    gchar *class_name = NULL;
    static ID id_gtype = 0;

    if (rb_class2name(klass)[0] != '#')
        return;

    type_name = g_type_name(type);
    if (g_str_has_prefix(type_name, "Gst"))
        type_name += 3;

    if (prefix)
        class_name = g_strconcat(prefix, type_name, NULL);
    G_DEF_CLASS(type, class_name ? class_name : type_name, mGst);
    g_free(class_name);

    parent = rb_ary_entry(rb_mod_ancestors(klass), 1);
    if (!id_gtype)
        id_gtype = rb_intern("gtype");
    if (rb_respond_to(parent, id_gtype))
        _rbgst_define_class_if_need(parent, CLASS2GTYPE(parent), prefix);
}
