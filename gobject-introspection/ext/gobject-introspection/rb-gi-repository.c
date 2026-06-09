/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2012-2015  Ruby-GNOME2 Project Team
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


/* GObjectIntrospection::Repository is used to manage repositories of
 * namespaces. Namespaces are represented on disk by type libraries
 * (.typelib files).
 */

#include "rb-gi-private.h"

#define RG_TARGET_NAMESPACE rb_cGIRepository
#define SELF(self) RVAL2GI_REPOSITORY(self)


/* Returns the singleton process-global default
 * GObjectIntrospection::Repository. It is not currently supported to have
 * multiple repositories in a particular process, but this function is provided
 * in the unlikely eventuality that it would become possible, and as a
 * convenience for higher level language bindings to conform to the GObject
 * method call conventions.
 *
 * @return [GObjectIntrospection::Repository] The global singleton
 *         GObjectIntrospection::Repository.
 */
static VALUE
rg_s_default(G_GNUC_UNUSED VALUE klass)
{
    return GOBJ2RVAL(g_irepository_get_default());
}

static VALUE
rg_s_prepend_search_path(VALUE klass, VALUE rb_path)
{
    g_irepository_prepend_search_path(RVAL2CSTR(rb_path));
    return klass;
}

static VALUE
rg_s_search_path(G_GNUC_UNUSED VALUE klass)
{
    return FILENAMEGSLIST2RVAL(g_irepository_get_search_path());
}


/* Force the namespace to be loaded if it isn't already. If namespace is not
 * loaded, this function will search for a ".typelib" file using the repository
 * search path. In addition, a version of namespace may be specified. If version
 * is not specified, the latest will be used.
 *
 * @param [String] namespace The namespace to load
 * @param [String, nil] version An optional version
 */
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


/* Return an array of all (transitive) versioned dependencies for namespace.
 * Returned strings are of the form "namespace-version".
 * Note: namespace must have already been loaded using a function such as
 * GObjectIntrospection::Repository.require() before calling this method.
 *
 * @param [String] namespace The namespace to get the dependencies for.
 *
 * @return [Array<String>] The dependencies.
 */
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
    if (!dependencies) {
        return Qnil;
    }

    rb_dependencies = rb_ary_new();
    for (i = 0; dependencies[i]; i++) {
        rb_ary_push(rb_dependencies, CSTR2RVAL(dependencies[i]));
    }
    g_strfreev(dependencies);

    return rb_dependencies;
}


/* Return the list of currently loaded namespaces.
 *
 * @return [Array<String>] The loaded namespaces.
 */
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


/* This method returns the number of metadata entries in given namespace. The
 * namespace must have already been loaded before calling this function.
 *
 * @param [String] namespace The namespace to fetch the entries from.
 *
 * @return [Integer] The number of metadata entries.
 */
static VALUE
rg_get_n_infos(VALUE self, VALUE rb_namespace)
{
    const gchar *namespace_;
    gint n_infos;

    namespace_ = RVAL2CSTR(rb_namespace);
    n_infos = g_irepository_get_n_infos(SELF(self), namespace_);

    return INT2NUM(n_infos);
}


/* This method returns a particular metadata entry in the given namespace. The
 *  namespace must have already been loaded before calling this function. See
 *  GObjectIntrospection::Repository#get_n_infos() to find the maximum number
 *  of entries.
 *
 * @param [String] namespace The namespace to fetch the metadata entry from.
 * @param [Fixnum] index The index of the entry.
 *
 * @return [GObjectIntrospection::BaseInfo] The metadata entry.
 */
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


/* This method searches for a particular type or name. If only one argument is
 * given, it is interpreted as a gtype and all loaded namespaces are searched
 * for it. If two arguments are given the first is a particular namespace and
 * the second the name of an entry to search for.
 *
 * @overload find(type)
 *
 * @param [String] type The type to search for.
 *
 * @return [GObjectIntrospection::BaseInfo] The metadata entry.
 *
 *
 * @overload find(namespace, type)
 *
 * @param [String] namespace The namespace to search in.
 *
 * @param [String] type The type to search for.
 *
 * @return [GObjectIntrospection::BaseInfo] The metadata entry.
 */

static VALUE
rg_find(int argc, VALUE *argv, VALUE self)
{
    GIBaseInfo *info;

    if (argc == 1) {
        VALUE rb_gtype;
        GType gtype;
        rb_gtype = argv[0];
        gtype = rbgobj_gtype_from_ruby(rb_gtype);
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

static VALUE
rg_get_version(VALUE self, VALUE rb_namespace)
{
    const gchar *version;
    version = g_irepository_get_version(SELF(self), RVAL2CSTR(rb_namespace));
    return CSTR2RVAL(version);
}

void
rb_gi_repository_init(VALUE rb_mGI)
{
    VALUE RG_TARGET_NAMESPACE;

    RG_TARGET_NAMESPACE = G_DEF_CLASS(G_TYPE_IREPOSITORY, "Repository", rb_mGI);

    RG_DEF_SMETHOD(default, 0);
    RG_DEF_SMETHOD(prepend_search_path, 1);
    RG_DEF_SMETHOD(search_path, 0);
    RG_DEF_METHOD(require, -1);
    RG_DEF_METHOD(get_dependencies, 1);
    RG_DEF_METHOD(loaded_namespaces, 0);
    RG_DEF_METHOD(get_n_infos, 1);
    RG_DEF_METHOD(get_info, 2);
    RG_DEF_METHOD(find, -1);
    RG_DEF_METHOD(get_version, 1);

    G_DEF_CLASS(G_TYPE_I_REPOSITORY_LOAD_FLAGS, "RepositoryLoadFlags", rb_mGI);
    G_DEF_ERROR(G_IREPOSITORY_ERROR,
                "RepositoryError",
                rb_mGI,
                rb_eLoadError,
                G_TYPE_I_REPOSITORY_ERROR);
}
