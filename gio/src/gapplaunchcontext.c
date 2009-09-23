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
        RVAL2GAPPLAUNCHCONTEXT(value)

DECLARE(initialize)(VALUE self)
{
        G_INITIALIZE(self, g_app_launch_context_new());

        return Qnil;
}

typedef char *(*GetMethod)(GAppLaunchContext *, GAppInfo *, GList *);

static VALUE
get_value_by(GetMethod method, int argc, VALUE *argv, VALUE self)
{
        GAppInfo *info;
        GList *files;
        char *value;

        SCAN_2_ARGS("11",
                    info, RVAL2GAPPINFO,
                    files, g_file_ary_to_glist);

        value = method(_SELF(self), info, files);

        g_list_free(files);

        return CSTR2RVAL_FREE(value);
}

DECLARE_VARG(get_display)
{
        return get_value_by(g_app_launch_context_get_display, argc, argv, self);
}

DECLARE_VARG(get_startup_notify_id)
{
        return get_value_by(g_app_launch_context_get_startup_notify_id,
                            argc, argv, self);
}

DECLARE(launch_failed)(VALUE self, VALUE sn_id)
{
        g_app_launch_context_launch_failed(_SELF(self), RVAL2CSTR(sn_id));

        return self;
}

void
Init_gapplaunchcontext(VALUE glib)
{
        VALUE applaunchcontext = G_DEF_CLASS(G_TYPE_APP_LAUNCH_CONTEXT,
                                             "AppLaunchContext",
                                             glib);

        DEF_METHOD(applaunchcontext, initialize, 0);
        DEF_METHOD(applaunchcontext, get_display, -1);
        DEF_METHOD(applaunchcontext, get_startup_notify_id, -1);
        DEF_METHOD(applaunchcontext, launch_failed, 1);
}
