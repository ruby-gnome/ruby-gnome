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

#include "rbgio2private.h"

#define RG_TARGET_NAMESPACE mAppInfo
#define _SELF(value) RVAL2GAPPINFO(value)

#define RVAL2GAPPINFOCREATEFLAGSDEFAULT(value) \
        RVAL2TYPE_WITH_DEFAULT((value), \
                               RVAL2GAPPINFOCREATEFLAGS, \
                               G_APP_INFO_CREATE_NONE)

static VALUE
rg_s_create_from_commandline(int argc, VALUE *argv, G_GNUC_UNUSED VALUE self)
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
rg_s_reset_type_associations(VALUE self, VALUE content_type)
{
        g_app_info_reset_type_associations(RVAL2CSTR(content_type));

        return self;
}

static VALUE
rg_s_get_all_for_type(G_GNUC_UNUSED VALUE self, VALUE content_type)
{
        return GLIST2ARY_FREE(g_app_info_get_all_for_type(RVAL2CSTR(content_type)));
}

static VALUE
rg_s_all(int argc, VALUE *argv, VALUE self)
{
        VALUE content_type;

        rb_scan_args(argc, argv, "01", &content_type);
        if (!NIL_P(content_type))
                return rg_s_get_all_for_type(self, content_type);

        return GLIST2ARY_FREE(g_app_info_get_all());
}

static VALUE
rg_s_get_default_for_type(int argc, VALUE *argv, G_GNUC_UNUSED VALUE self)
{
        VALUE content_type, must_support_uris;

        rb_scan_args(argc, argv, "11", &content_type, &must_support_uris);

        return GOBJ2RVAL_UNREF(g_app_info_get_default_for_type(RVAL2CSTR(content_type),
                                                               RVAL2CBOOL(must_support_uris)));
}

static VALUE
rg_s_get_default_for_uri_scheme(G_GNUC_UNUSED VALUE self, VALUE uri_scheme)
{
        return GOBJ2RVAL_UNREF(g_app_info_get_default_for_uri_scheme(RVAL2CSTR(uri_scheme)));
}

static VALUE
rg_s_launch_default_for_uri(int argc, VALUE *argv, VALUE self)
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
rg_dup(VALUE self)
{
        return GOBJ2RVAL_UNREF(g_app_info_dup(_SELF(self)));
}

static VALUE
rg_equal_p(VALUE self, VALUE other)
{
        return CBOOL2RVAL(g_app_info_equal(_SELF(self), _SELF(other)));
}

static VALUE
rg_id(VALUE self)
{
        return CSTR2RVAL(g_app_info_get_id(_SELF(self)));
}

static VALUE
rg_name(VALUE self)
{
        return CSTR2RVAL(g_app_info_get_name(_SELF(self)));
}

static VALUE
rg_display_name(VALUE self)
{
        return CSTR2RVAL(g_app_info_get_display_name(_SELF(self)));
}

static VALUE
rg_description(VALUE self)
{
        return CSTR2RVAL(g_app_info_get_description(_SELF(self)));
}

static VALUE
rg_executable(VALUE self)
{
        return CSTR2RVAL(g_app_info_get_executable(_SELF(self)));
}

static VALUE
rg_commandline(VALUE self)
{
        return CSTR2RVAL(g_app_info_get_commandline(_SELF(self)));
}

static VALUE
rg_icon(VALUE self)
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
rg_launch(int argc, VALUE *argv, VALUE self)
{
        return launch_method(g_app_info_launch, argc, argv, self);
}

static VALUE
rg_supports_files_p(VALUE self)
{
        return CBOOL2RVAL(g_app_info_supports_files(_SELF(self)));
}

static VALUE
rg_supports_uris_p(VALUE self)
{
        return CBOOL2RVAL(g_app_info_supports_uris(_SELF(self)));
}

static VALUE
rg_launch_uris(int argc, VALUE *argv, VALUE self)
{
        return launch_method(g_app_info_launch_uris, argc, argv, self);
}

static VALUE
rg_should_show_p(VALUE self)
{
        return CBOOL2RVAL(g_app_info_should_show(_SELF(self)));
}

static VALUE
rg_can_delete_p(VALUE self)
{
        return CBOOL2RVAL(g_app_info_can_delete(_SELF(self)));
}

static VALUE
rg_delete(VALUE self)
{
        return CBOOL2RVAL(g_app_info_delete(_SELF(self)));
}

static VALUE
rg_set_as_default_for_type(VALUE self, VALUE value)
{
        GError *error = NULL;

        if (!g_app_info_set_as_default_for_type(_SELF(self), RVAL2CSTR(value), &error))
                rbgio_raise_error(error);

        return self;
}

static VALUE
rg_set_as_default_for_extension(VALUE self, VALUE value)
{
        GError *error = NULL;

        if (!g_app_info_set_as_default_for_extension(_SELF(self), RVAL2CSTR(value), &error))
                rbgio_raise_error(error);

        return self;
}

static VALUE
rg_add_supports_type(VALUE self, VALUE value)
{
        GError *error = NULL;

        if (!g_app_info_add_supports_type(_SELF(self), RVAL2CSTR(value), &error))
                rbgio_raise_error(error);

        return self;
}

static VALUE
rg_remove_supports_type(VALUE self, VALUE value)
{
        GError *error = NULL;

        if (!g_app_info_remove_supports_type(_SELF(self), RVAL2CSTR(value), &error))
                rbgio_raise_error(error);

        return self;
}

static VALUE
rg_can_remove_supports_type_p(VALUE self)
{
        return CBOOL2RVAL(g_app_info_can_remove_supports_type(_SELF(self)));
}

void
Init_gappinfo(VALUE mGio)
{
        VALUE RG_TARGET_NAMESPACE = G_DEF_INTERFACE(G_TYPE_APP_INFO, "AppInfo", mGio);

        G_DEF_CLASS(G_TYPE_APP_INFO_CREATE_FLAGS, "CreateFlags", RG_TARGET_NAMESPACE);
        G_DEF_CONSTANTS(RG_TARGET_NAMESPACE, G_TYPE_APP_INFO_CREATE_FLAGS, "G_APP_INFO_");

        RG_DEF_SMETHOD(create_from_commandline, -1);
        RG_DEF_SMETHOD(reset_type_associations, -1);
        RG_DEF_SMETHOD(all, 0);
        RG_DEF_SMETHOD(get_all_for_type, 1);
        RG_DEF_SMETHOD(get_default_for_type, -1);
        RG_DEF_SMETHOD(get_default_for_uri_scheme, 1);
        RG_DEF_SMETHOD(launch_default_for_uri, 0);

        RG_DEF_METHOD(dup, 0);
        RG_DEF_METHOD_P(equal, 1);
        RG_DEF_METHOD(id, 0);
        RG_DEF_METHOD(name, 0);
        RG_DEF_METHOD(display_name, 0);
        RG_DEF_METHOD(description, 0);
        RG_DEF_METHOD(executable, 0);
        RG_DEF_METHOD(commandline, 0);
        RG_DEF_METHOD(icon, 0);
        RG_DEF_METHOD(launch, -1);
        RG_DEF_METHOD_P(supports_files, 0);
        RG_DEF_METHOD_P(supports_uris, 0);
        RG_DEF_METHOD(launch_uris, -1);
        RG_DEF_METHOD_P(should_show, 0);
        RG_DEF_METHOD_P(can_delete, 0);
        RG_DEF_METHOD(delete, 0);
        RG_DEF_METHOD(set_as_default_for_type, 1);
        RG_DEF_METHOD(set_as_default_for_extension, 1);
        RG_DEF_METHOD(add_supports_type, 1);
        RG_DEF_METHOD_P(can_remove_supports_type, 1);
        RG_DEF_METHOD(remove_supports_type, 1);
}
