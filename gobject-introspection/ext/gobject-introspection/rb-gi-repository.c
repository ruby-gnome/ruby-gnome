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
    GIRepositoryLoadFlags flags = 0;
    GError *error = NULL;

    rb_scan_args(argc, argv, "12", &rb_namespace, &rb_version, &rb_flags);

    namespace_ = RVAL2CSTR(rb_namespace);
    version = RVAL2CSTR_ACCEPT_NIL(rb_version);
    if (!NIL_P(rb_flags)) {
	flags = RVAL2GI_REPOSITORY_LOAD_FLAGS(rb_flags);
    }

    g_irepository_require(SELF(self), namespace_, version, flags, &error);
    if (error) {
	RG_RAISE_ERROR(error);
    }

    return Qnil;
}

static VALUE
rg_get_dependencies(VALUE self, VALUE rb_namespace)
{
    GIRepository *repository;
    const gchar *namespace_;
    VALUE rb_dependencies;
    gchar **dependencies;
    gint i;

    repository = SELF(self);
    namespace_ = RVAL2CSTR(rb_namespace);
    dependencies = g_irepository_get_dependencies(repository, namespace_);
    rb_dependencies = rb_ary_new();
    for (i = 0; dependencies[i]; i++) {
        rb_ary_push(rb_dependencies, CSTR2RVAL(dependencies[i]));
    }
    g_strfreev(dependencies);

    return rb_dependencies;
}

static VALUE
rg_loaded_namespaces(VALUE self)
{
    GIRepository *repository;
    VALUE rb_namespaces;
    gchar **namespaces;
    gint i;

    repository = SELF(self);
    namespaces = g_irepository_get_loaded_namespaces(repository);
    rb_namespaces = rb_ary_new();
    for (i = 0; namespaces[i]; i++) {
        rb_ary_push(rb_namespaces, CSTR2RVAL(namespaces[i]));
    }
    g_strfreev(namespaces);

    return rb_namespaces;
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

static VALUE
rg_get_info(VALUE self, VALUE rb_namespace, VALUE rb_n)
{
    GIRepository *repository;
    const gchar *namespace_;
    gint n;
    GIBaseInfo *info;

    repository = SELF(self);
    namespace_ = RVAL2CSTR(rb_namespace);
    n = NUM2INT(rb_n);
    info = g_irepository_get_info(repository, namespace_, n);
    return GI_BASE_INFO2RVAL_WITH_UNREF(info);
}

static VALUE
rg_find(int argc, VALUE *argv, VALUE self)
{
    GIBaseInfo *info;

    if (argc == 1) {
	VALUE rb_gtype;
	GType gtype;
	rb_gtype = argv[0];
	gtype = NUM2UINT(rb_gtype);
	info = g_irepository_find_by_gtype(SELF(self), gtype);
    } else {
	VALUE rb_namespace, rb_name;
	const gchar *namespace_, *name;

	rb_scan_args(argc, argv, "2", &rb_namespace, &rb_name);
	namespace_ = RVAL2CSTR(rb_namespace);
	name = RVAL2CSTR(rb_name);
	info = g_irepository_find_by_name(SELF(self), namespace_, name);
    }

    return GI_BASE_INFO2RVAL(info);
}

void
rb_gi_repository_init(VALUE rb_mGI)
{
    VALUE RG_TARGET_NAMESPACE;

    RG_TARGET_NAMESPACE = G_DEF_CLASS(G_TYPE_IREPOSITORY, "Repository", rb_mGI);

    RG_DEF_SMETHOD(default, 0);
    RG_DEF_METHOD(require, -1);
    RG_DEF_METHOD(get_dependencies, 1);
    RG_DEF_METHOD(loaded_namespaces, 0);
    RG_DEF_METHOD(get_n_infos, 1);
    RG_DEF_METHOD(get_info, 2);
    RG_DEF_METHOD(find, -1);

    G_DEF_CLASS(G_TYPE_I_REPOSITORY_LOAD_FLAGS, "RepositoryLoadFlags", rb_mGI);
    G_DEF_CLASS(G_TYPE_I_REPOSITORY_ERROR, "RepositoryError", rb_mGI);
}
