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

#define _SELF(value) \
        RVAL2GAPPINFO(value)

DECLARE(create_from_commandline)(int argc, VALUE *argv, UNUSED(VALUE self))
{
        const char *commandline;
        const char *application_name;
        GAppInfoCreateFlags flags;
        GError *error = NULL;
        GAppInfo *info;

        SCAN_3_ARGS("12",
                    commandline, RVAL2CSTR,
                    application_name, RVAL2CSTR_ACCEPT_NIL,
                    flags, RVAL2GAPPINFOCREATEFLAGSDEFAULT);

        info = g_app_info_create_from_commandline(commandline,
                                                  application_name,
                                                  flags,
                                                  &error);
        if (info == NULL)
                rbgio_raise_io_error(error);

        return GOBJ2RVAL(info);
}

DECLARE(reset_type_associations)(VALUE self, VALUE content_type)
{
        g_app_info_reset_type_associations(RVAL2CSTR(content_type));

        return self;
}

DECLARE(get_all_for_type)(UNUSED(VALUE self), VALUE content_type)
{
        return GLIST2ARY_FREE(g_app_info_get_all_for_type(RVAL2CSTR(content_type)));
}

DECLARE(all)(int argc, VALUE *argv, VALUE self)
{
        VALUE content_type;

        rb_scan_args(argc, argv, "01", &content_type);
        if (!NIL_P(content_type))
                return INTERFACE_METHOD(get_all_for_type)(self, content_type);

        return GLIST2ARY_FREE(g_app_info_get_all());
}

DECLARE(get_default_for_type)(int argc, VALUE *argv, UNUSED(VALUE self))
{
        const char *content_type;
        gboolean must_support_uris;

        SCAN_2_ARGS("11",
                    content_type, RVAL2CSTR,
                    must_support_uris, RVAL2CBOOL);

        return GOBJ2RVAL(g_app_info_get_default_for_type(content_type,
                                                         must_support_uris));
}

DECLARE(get_default_for_uri_scheme)(UNUSED(VALUE self), VALUE uri_scheme)
{
        return GOBJ2RVAL(g_app_info_get_default_for_uri_scheme(RVAL2CSTR(uri_scheme)));
}

DECLARE_VARG(launch_default_for_uri)
{
        const char *uri;
        GAppLaunchContext *launch_context;
        GError *error;

        SCAN_2_ARGS("11",
                    uri, RVAL2CSTR,
                    launch_context, RVAL2GAPPLAUNCHCONTEXT);

        if (!g_app_info_launch_default_for_uri(uri, launch_context, &error))
                rbgio_raise_io_error(error);

        return self;
}

DECLARE(dup)(VALUE self)
{
        return GOBJ2RVAL(g_app_info_dup(_SELF(self)));
}

DECLARE(equal)(VALUE self, VALUE other)
{
        return CBOOL2RVAL(g_app_info_equal(_SELF(self), _SELF(other)));
}

#define DEFGETMETHOD(name, type) \
        DECLARE(name)(VALUE self){ return type(g_app_info_get_##name(_SELF(self))); }

DEFGETMETHOD(id, CSTR2RVAL)
DEFGETMETHOD(name, CSTR2RVAL)
DEFGETMETHOD(description, CSTR2RVAL)
DEFGETMETHOD(executable, CSTR2RVAL)
DEFGETMETHOD(commandline, CSTR2RVAL)
DEFGETMETHOD(icon, GOBJ2RVAL)

typedef gboolean (*LaunchMethod)(GAppInfo *,
                                 GList *,
                                 GAppLaunchContext *,
                                 GError **);

static VALUE
launch_by(LaunchMethod method, int argc, VALUE *argv, VALUE self)
{
        VALUE rbfiles,
              rblaunch_context;
        GList *files;
        GAppLaunchContext *launch_context;
        GError *error = NULL;
        gboolean launched;

        rb_scan_args(argc, argv, "02", &rbfiles, &rblaunch_context);
        launch_context = RVAL2GAPPLAUNCHCONTEXT(rblaunch_context);
        files = g_file_ary_to_glist(rbfiles);

        launched = method(_SELF(self), files, launch_context, &error);

        g_list_free(files);

        if (!launched)
                rbgio_raise_io_error(error);

        return self;
}

DECLARE_VARG(launch)
{
        return launch_by(g_app_info_launch, argc, argv, self);
}

#define DEFSIMPLEMETHOD(name, type) \
        DECLARE(name)(VALUE self){ return type(g_app_info_##name(_SELF(self))); }

DEFSIMPLEMETHOD(supports_files, CBOOL2RVAL)
DEFSIMPLEMETHOD(supports_uris, CBOOL2RVAL)

DECLARE_VARG(launch_uris)
{
        return launch_by(g_app_info_launch_uris, argc, argv, self);
}

DEFSIMPLEMETHOD(should_show, CBOOL2RVAL)
DEFSIMPLEMETHOD(can_delete, CBOOL2RVAL)
DEFSIMPLEMETHOD(delete, CBOOL2RVAL)

#define DEFSUPPORTSMETHOD(name) \
        DECLARE(name)(VALUE self, VALUE value) \
        { \
                GError *error = NULL; \
                if (!g_app_info_##name(_SELF(self), RVAL2CSTR(value), &error)) \
                        rbgio_raise_io_error(error); \
                return self; \
        }

DEFSUPPORTSMETHOD(set_as_default_for_type)
DEFSUPPORTSMETHOD(set_as_default_for_extension)
DEFSUPPORTSMETHOD(add_supports_type)
DEFSUPPORTSMETHOD(remove_supports_type)

DEFSIMPLEMETHOD(can_remove_supports_type, CBOOL2RVAL)

void
Init_gappinfo(VALUE glib)
{
        VALUE appinfo = G_DEF_INTERFACE(G_TYPE_APP_INFO, "AppInfo", glib);

        DEF_CONSTANTS_CLASS(G_TYPE_APP_INFO_CREATE_FLAGS,
                            "CreateFlags",
                            "G_APP_INFO_",
                            appinfo);

        DEF_SINGLETON_METHOD(appinfo, create_from_commandline, -1);
        DEF_SINGLETON_METHOD(appinfo, reset_type_associations, -1);
        DEF_SINGLETON_METHOD(appinfo, all, 0);
        DEF_SINGLETON_METHOD(appinfo, get_all_for_type, 1);
        DEF_SINGLETON_METHOD(appinfo, get_default_for_type, -1);
        DEF_SINGLETON_METHOD(appinfo, get_default_for_uri_scheme, 1);
        DEF_SINGLETON_METHOD(appinfo, launch_default_for_uri, 0);

        DEF_METHOD(appinfo, dup, 0);
        DEF_BOOL_METHOD(appinfo, equal, 1);
        DEF_METHOD(appinfo, id, 0);
        DEF_METHOD(appinfo, name, 0);
        DEF_METHOD(appinfo, description, 0);
        DEF_METHOD(appinfo, executable, 0);
        DEF_METHOD(appinfo, commandline, 0);
        DEF_METHOD(appinfo, icon, 0);
        DEF_METHOD(appinfo, launch, -1);
        DEF_BOOL_METHOD(appinfo, supports_files, 0);
        DEF_BOOL_METHOD(appinfo, supports_uris, 0);
        DEF_METHOD(appinfo, launch_uris, -1);
        DEF_BOOL_METHOD(appinfo, should_show, 0);
        DEF_BOOL_METHOD(appinfo, can_delete, 0);
        DEF_METHOD(appinfo, delete, 0);
        DEF_METHOD(appinfo, set_as_default_for_type, 1);
        DEF_METHOD(appinfo, set_as_default_for_extension, 1);
        DEF_METHOD(appinfo, add_supports_type, 1);
        DEF_BOOL_METHOD(appinfo, can_remove_supports_type, 1);
        DEF_METHOD(appinfo, remove_supports_type, 1);
}
