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

#define RG_TARGET_NAMESPACE cAppLaunchContext
#define _SELF(value) RVAL2GAPPLAUNCHCONTEXT(value)

static VALUE
rg_initialize(VALUE self)
{
        G_INITIALIZE(self, g_app_launch_context_new());

        return Qnil;
}

typedef char *(*GetMethod)(GAppLaunchContext *, GAppInfo *, GList *);

static VALUE
get_value_by(GetMethod method, int argc, VALUE *argv, VALUE self)
{
        VALUE info, rbfiles;
        GList *files;
        char *value;

        rb_scan_args(argc, argv, "11", &info, &rbfiles);
        files = GFILEARY2GLIST_ACCEPT_NIL(rbfiles);
        value = method(_SELF(self), RVAL2GAPPINFO(info), files);
        g_list_free(files);

        return CSTR2RVAL_FREE(value);
}

static VALUE
rg_get_display(int argc, VALUE *argv, VALUE self)
{
        return get_value_by(g_app_launch_context_get_display, argc, argv, self);
}

static VALUE
rg_get_startup_notify_id(int argc, VALUE *argv, VALUE self)
{
        return get_value_by(g_app_launch_context_get_startup_notify_id, argc, argv, self);
}

static VALUE
rg_launch_failed(VALUE self, VALUE sn_id)
{
        g_app_launch_context_launch_failed(_SELF(self), RVAL2CSTR(sn_id));

        return self;
}

void
Init_gapplaunchcontext(VALUE mGio)
{
        VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(G_TYPE_APP_LAUNCH_CONTEXT, "AppLaunchContext", mGio);

        RG_DEF_METHOD(initialize, 0);
        RG_DEF_METHOD(get_display, -1);
        RG_DEF_METHOD(get_startup_notify_id, -1);
        RG_DEF_METHOD(launch_failed, 1);
}
