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
    GError *error = NULL;

    rb_scan_args(argc, argv, "12", &rb_namespace, &rb_version, &rb_flags);

    namespace_ = RVAL2CSTR(rb_namespace);
    version = RVAL2CSTR_ACCEPT_NIL(rb_version);
    if (!NIL_P(rb_flags)) {
	flags = RVAL2GI_REPOSITORY_LOAD_FLAGS(rb_flags);
    }

    g_irepository_require(SELF(self), namespace_, version, flags, &error);
    if (error) {
	RAISE_GERROR(error);
    }

    return Qnil;
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

typedef struct {
    VALUE self;
    GIRepository *repository;
    gchar **namespaces;
} EachData;

static VALUE
rg_each_body(VALUE arg)
{
    EachData *data = (EachData *)arg;
    gint i;

    for (i = 0; data->namespaces[i]; i++) {
	const gchar *namespace_;
	gint j, n_infos;

	namespace_ = data->namespaces[i];
	n_infos = g_irepository_get_n_infos(data->repository, namespace_);
	for (j = 0; j < n_infos; j++) {
	    GIBaseInfo *info;
	    info = g_irepository_get_info(data->repository, namespace_, j);
	    rb_yield(GI_BASE_INFO2RVAL(info));
	}
    }

    return Qnil;
}

static VALUE
rg_each_ensure(VALUE arg)
{
    EachData *data = (EachData *)arg;

    g_strfreev(data->namespaces);

    return Qnil;
}

static VALUE
rg_each(VALUE self)
{
    EachData data;

    RETURN_ENUMERATOR(self, 0, NULL);

    data.self = self;
    data.repository = SELF(self);
    data.namespaces = g_irepository_get_loaded_namespaces(data.repository);
    return rb_ensure(rg_each_body,  (VALUE)(&data),
		     rg_each_ensure, (VALUE)(&data));
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

    rb_include_module(RG_TARGET_NAMESPACE, rb_mEnumerable);

    RG_DEF_SMETHOD(default, 0);
    RG_DEF_METHOD(require, -1);
    RG_DEF_METHOD(get_n_infos, 1);
    RG_DEF_METHOD(each, 0);
    RG_DEF_METHOD(find, -1);

    G_DEF_CLASS(G_TYPE_I_REPOSITORY_LOAD_FLAGS, "RepositoryLoadFlags", rb_mGI);
    G_DEF_CLASS(G_TYPE_I_REPOSITORY_ERROR, "RepositoryError", rb_mGI);
}
