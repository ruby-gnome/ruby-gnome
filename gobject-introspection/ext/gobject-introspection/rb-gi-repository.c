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

#define RG_TARGET_NAMESPACE rb_cGIRepository
#define SELF(self) RVAL2GI_REPOSITORY(self)

static VALUE
rg_s_default(G_GNUC_UNUSED VALUE klass)
{
    return GOBJ2RVAL(g_irepository_get_default());
}

static VALUE
rg_require(int argc, VALUE *argv, VALUE self)
{
    VALUE rb_namespace, rb_version, rb_flags;
    const gchar *namespace_, *version;
    GIRepositoryLoadFlags flags = G_IREPOSITORY_LOAD_FLAG_LAZY;
    GITypelib *typelib;
    GError *error = NULL;

    rb_scan_args(argc, argv, "12", &rb_namespace, &rb_version, &rb_flags);

    namespace_ = RVAL2CSTR(rb_namespace);
    version = RVAL2CSTR_ACCEPT_NIL(rb_version);
    if (!NIL_P(rb_flags)) {
	flags = RVAL2GI_REPOSITORY_LOAD_FLAGS(rb_flags);
    }

    typelib = g_irepository_require(SELF(self),
				    namespace_,
				    version,
				    flags,
				    &error);
    if (error) {
	RAISE_GERROR(error);
    }

    return GI_TYPELIB2RVAL(typelib);
}

static VALUE
rg_get_n_infos(VALUE self, VALUE rb_namespace)
{
    const gchar *namespace_;
    gint n_infos;

    namespace_ = RVAL2CSTR(rb_namespace);
    n_infos = g_irepository_get_n_infos(SELF(self), namespace_);

    return INT2NUM(n_infos);
}

void
rb_gi_repository_init(VALUE rb_mGI)
{
    VALUE RG_TARGET_NAMESPACE;

    RG_TARGET_NAMESPACE = G_DEF_CLASS(G_TYPE_IREPOSITORY, "Repository", rb_mGI);

    G_DEF_CLASS(G_TYPE_I_REPOSITORY_LOAD_FLAGS, "RepositoryLoadFlags", rb_mGI);
    G_DEF_CLASS(G_TYPE_I_REPOSITORY_ERROR, "RepositoryError", rb_mGI);

    RG_DEF_SMETHOD(default, 0);
    RG_DEF_METHOD(require, -1);
    RG_DEF_METHOD(get_n_infos, 1);
}
