/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2012  Ruby-GNOME2 Project Team
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

#include "rb-gobject-introspection.h"

#define RG_TARGET_NAMESPACE rb_cGITypelib
#define SELF(self) RVAL2GI_TYPELIB(self)

static GITypelib *
g_itypelib_copy(GITypelib *typelib)
{
    return typelib;
}

static void
g_itypelib_free(GITypelib *typelib)
{
    g_typelib_free(typelib);
}

GType
g_itypelib_get_type(void)
{
    static GType type = 0;
    if (type == 0) {
	type = g_boxed_type_register_static("GITypelib",
					    (GBoxedCopyFunc)g_itypelib_copy,
					    (GBoxedFreeFunc)g_itypelib_free);
    }
    return type;
}

static VALUE
rg_namespace(VALUE self)
{
    const gchar *namespace_;

    namespace_ = g_typelib_get_namespace(SELF(self));
    return CSTR2RVAL(namespace_);
}

void
rb_gi_typelib_init(VALUE rb_mGI)
{
    VALUE RG_TARGET_NAMESPACE;

    RG_TARGET_NAMESPACE = G_DEF_CLASS(G_TYPE_ITYPELIB, "Typelib", rb_mGI);

    RG_DEF_METHOD(namespace, 0);
}
