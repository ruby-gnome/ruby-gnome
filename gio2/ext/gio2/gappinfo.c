/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2008-2009  Ruby-GNOME2 Project Team
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

#include "gio2.h"

#define RG_TARGET_NAMESPACE mAppInfo
#define _SELF(value) RVAL2GAPPINFO(value)

#define RVAL2GAPPINFOCREATEFLAGS(value) \
        RVAL2GFLAGS((value), G_TYPE_APP_INFO_CREATE_FLAGS)

#define RVAL2GAPPINFOCREATEFLAGSDEFAULT(value) \
        RVAL2TYPE_WITH_DEFAULT((value), \
                               RVAL2GAPPINFOCREATEFLAGS, \
                               G_APP_INFO_CREATE_NONE)

static VALUE
appinfo_create_from_commandline(int argc, VALUE *argv, G_GNUC_UNUSED VALUE self)
{
        VALUE commandline, application_name, flags;
        GError *error = NULL;
        GAppInfo *info;

        rb_scan_args(argc, argv, "12", &commandline, &application_name, &flags);
        info = g_app_info_create_from_commandline(RVAL2CSTR(commandline),
                                                  RVAL2CSTR_ACCEPT_NIL(application_name),
                                                  RVAL2GAPPINFOCREATEFLAGSDEFAULT(flags),
                                                  &error);
        if (info == NULL)
                rbgio_raise_error(error);

        return GOBJ2RVAL_UNREF(info);
}

static VALUE
appinfo_reset_type_associations(VALUE self, VALUE content_type)
{
        g_app_info_reset_type_associations(RVAL2CSTR(content_type));

        return self;
}

static VALUE
appinfo_get_all_for_type(G_GNUC_UNUSED VALUE self, VALUE content_type)
{
        return GLIST2ARY_FREE(g_app_info_get_all_for_type(RVAL2CSTR(content_type)));
}

static VALUE
appinfo_get_all(int argc, VALUE *argv, VALUE self)
{
        VALUE content_type;

        rb_scan_args(argc, argv, "01", &content_type);
        if (!NIL_P(content_type))
                return appinfo_get_all_for_type(self, content_type);

        return GLIST2ARY_FREE(g_app_info_get_all());
}

static VALUE
appinfo_get_default_for_type(int argc, VALUE *argv, G_GNUC_UNUSED VALUE self)
{
        VALUE content_type, must_support_uris;

        rb_scan_args(argc, argv, "11", &content_type, &must_support_uris);

        return GOBJ2RVAL_UNREF(g_app_info_get_default_for_type(RVAL2CSTR(content_type),
                                                               RVAL2CBOOL(must_support_uris)));
}

static VALUE
appinfo_get_default_for_uri_scheme(G_GNUC_UNUSED VALUE self, VALUE uri_scheme)
{
        return GOBJ2RVAL_UNREF(g_app_info_get_default_for_uri_scheme(RVAL2CSTR(uri_scheme)));
}

static VALUE
appinfo_launch_default_for_uri(int argc, VALUE *argv, VALUE self)
{
        VALUE uri, launch_context;
        GError *error;

        rb_scan_args(argc, argv, "11", &uri, &launch_context);
        if (!g_app_info_launch_default_for_uri(RVAL2CSTR(uri),
                                               RVAL2GAPPLAUNCHCONTEXT(launch_context),
                                               &error))
                rbgio_raise_error(error);

        return self;
}

static VALUE
appinfo_dup(VALUE self)
{
        return GOBJ2RVAL_UNREF(g_app_info_dup(_SELF(self)));
}

static VALUE
appinfo_equal(VALUE self, VALUE other)
{
        return CBOOL2RVAL(g_app_info_equal(_SELF(self), _SELF(other)));
}

static VALUE
appinfo_get_id(VALUE self)
{
        return CSTR2RVAL(g_app_info_get_id(_SELF(self)));
}

static VALUE
appinfo_get_name(VALUE self)
{
        return CSTR2RVAL(g_app_info_get_name(_SELF(self)));
}

static VALUE
appinfo_get_display_name(VALUE self)
{
        return CSTR2RVAL(g_app_info_get_display_name(_SELF(self)));
}

static VALUE
appinfo_get_description(VALUE self)
{
        return CSTR2RVAL(g_app_info_get_description(_SELF(self)));
}

static VALUE
appinfo_get_executable(VALUE self)
{
        return CSTR2RVAL(g_app_info_get_executable(_SELF(self)));
}

static VALUE
appinfo_get_commandline(VALUE self)
{
        return CSTR2RVAL(g_app_info_get_commandline(_SELF(self)));
}

static VALUE
appinfo_get_icon(VALUE self)
{
        return GOBJ2RVAL(g_app_info_get_icon(_SELF(self)));
}

typedef gboolean (*LaunchMethod)(GAppInfo *,
                                 GList *,
                                 GAppLaunchContext *,
                                 GError **);

static VALUE
launch_method(LaunchMethod method, int argc, VALUE *argv, VALUE self)
{
        VALUE rbfiles, rblaunch_context;
        GList *files;
        GAppLaunchContext *launch_context;
        GError *error = NULL;
        gboolean launched;

        rb_scan_args(argc, argv, "02", &rbfiles, &rblaunch_context);
        launch_context = RVAL2GAPPLAUNCHCONTEXT(rblaunch_context);
        files = GFILEARY2GLIST_ACCEPT_NIL(rbfiles);
        launched = method(_SELF(self), files, launch_context, &error);
        g_list_free(files);

        if (!launched)
                rbgio_raise_error(error);

        return self;
}

static VALUE
appinfo_launch(int argc, VALUE *argv, VALUE self)
{
        return launch_method(g_app_info_launch, argc, argv, self);
}

static VALUE
appinfo_supports_files(VALUE self)
{
        return CBOOL2RVAL(g_app_info_supports_files(_SELF(self)));
}

static VALUE
appinfo_supports_uris(VALUE self)
{
        return CBOOL2RVAL(g_app_info_supports_uris(_SELF(self)));
}

static VALUE
appinfo_launch_uris(int argc, VALUE *argv, VALUE self)
{
        return launch_method(g_app_info_launch_uris, argc, argv, self);
}

static VALUE
appinfo_should_show(VALUE self)
{
        return CBOOL2RVAL(g_app_info_should_show(_SELF(self)));
}

static VALUE
appinfo_can_delete(VALUE self)
{
        return CBOOL2RVAL(g_app_info_can_delete(_SELF(self)));
}

static VALUE
appinfo_delete(VALUE self)
{
        return CBOOL2RVAL(g_app_info_delete(_SELF(self)));
}

static VALUE
appinfo_set_as_default_for_type(VALUE self, VALUE value)
{
        GError *error = NULL;

        if (!g_app_info_set_as_default_for_type(_SELF(self), RVAL2CSTR(value), &error))
                rbgio_raise_error(error);

        return self;
}

static VALUE
appinfo_set_as_default_for_extension(VALUE self, VALUE value)
{
        GError *error = NULL;

        if (!g_app_info_set_as_default_for_extension(_SELF(self), RVAL2CSTR(value), &error))
                rbgio_raise_error(error);

        return self;
}

static VALUE
appinfo_add_supports_type(VALUE self, VALUE value)
{
        GError *error = NULL;

        if (!g_app_info_add_supports_type(_SELF(self), RVAL2CSTR(value), &error))
                rbgio_raise_error(error);

        return self;
}

static VALUE
appinfo_remove_supports_type(VALUE self, VALUE value)
{
        GError *error = NULL;

        if (!g_app_info_remove_supports_type(_SELF(self), RVAL2CSTR(value), &error))
                rbgio_raise_error(error);

        return self;
}

static VALUE
appinfo_can_remove_supports_type(VALUE self)
{
        return CBOOL2RVAL(g_app_info_can_remove_supports_type(_SELF(self)));
}

void
Init_gappinfo(VALUE glib)
{
        VALUE RG_TARGET_NAMESPACE = G_DEF_INTERFACE(G_TYPE_APP_INFO, "AppInfo", glib);

        G_DEF_CLASS(G_TYPE_APP_INFO_CREATE_FLAGS, "CreateFlags", RG_TARGET_NAMESPACE);
        G_DEF_CONSTANTS(RG_TARGET_NAMESPACE, G_TYPE_APP_INFO_CREATE_FLAGS, "G_APP_INFO_");

        rb_define_singleton_method(RG_TARGET_NAMESPACE, "create_from_commandline", appinfo_create_from_commandline, -1);
        rb_define_singleton_method(RG_TARGET_NAMESPACE, "reset_type_associations", appinfo_reset_type_associations, -1);
        rb_define_singleton_method(RG_TARGET_NAMESPACE, "all", appinfo_get_all, 0);
        rb_define_singleton_method(RG_TARGET_NAMESPACE, "get_all_for_type", appinfo_get_all_for_type, 1);
        rb_define_singleton_method(RG_TARGET_NAMESPACE, "get_default_for_type", appinfo_get_default_for_type, -1);
        rb_define_singleton_method(RG_TARGET_NAMESPACE, "get_default_for_uri_scheme", appinfo_get_default_for_uri_scheme, 1);
        rb_define_singleton_method(RG_TARGET_NAMESPACE, "launch_default_for_uri", appinfo_launch_default_for_uri, 0);

        rb_define_method(RG_TARGET_NAMESPACE, "dup", appinfo_dup, 0);
        rb_define_method(RG_TARGET_NAMESPACE, "equal?", appinfo_equal, 1);
        rb_define_method(RG_TARGET_NAMESPACE, "id", appinfo_get_id, 0);
        rb_define_method(RG_TARGET_NAMESPACE, "name", appinfo_get_name, 0);
        rb_define_method(RG_TARGET_NAMESPACE, "display_name", appinfo_get_display_name, 0);
        rb_define_method(RG_TARGET_NAMESPACE, "description", appinfo_get_description, 0);
        rb_define_method(RG_TARGET_NAMESPACE, "executable", appinfo_get_executable, 0);
        rb_define_method(RG_TARGET_NAMESPACE, "commandline", appinfo_get_commandline, 0);
        rb_define_method(RG_TARGET_NAMESPACE, "icon", appinfo_get_icon, 0);
        rb_define_method(RG_TARGET_NAMESPACE, "launch", appinfo_launch, -1);
        rb_define_method(RG_TARGET_NAMESPACE, "supports_files?", appinfo_supports_files, 0);
        rb_define_method(RG_TARGET_NAMESPACE, "supports_uris?", appinfo_supports_uris, 0);
        rb_define_method(RG_TARGET_NAMESPACE, "launch_uris", appinfo_launch_uris, -1);
        rb_define_method(RG_TARGET_NAMESPACE, "should_show?", appinfo_should_show, 0);
        rb_define_method(RG_TARGET_NAMESPACE, "can_delete?", appinfo_can_delete, 0);
        rb_define_method(RG_TARGET_NAMESPACE, "delete", appinfo_delete, 0);
        rb_define_method(RG_TARGET_NAMESPACE, "set_as_default_for_type", appinfo_set_as_default_for_type, 1);
        rb_define_method(RG_TARGET_NAMESPACE, "set_as_default_for_extension", appinfo_set_as_default_for_extension, 1);
        rb_define_method(RG_TARGET_NAMESPACE, "add_supports_type", appinfo_add_supports_type, 1);
        rb_define_method(RG_TARGET_NAMESPACE, "can_remove_supports_type?", appinfo_can_remove_supports_type, 1);
        rb_define_method(RG_TARGET_NAMESPACE, "remove_supports_type", appinfo_remove_supports_type, 1);
}
