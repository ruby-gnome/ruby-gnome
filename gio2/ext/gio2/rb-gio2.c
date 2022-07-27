/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2014-2015  Ruby-GNOME2 Project Team
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

#include "rb-gio2.h"

#define RG_TARGET_NAMESPACE rb_mGio

static void
rb_gio2_action_mark(gpointer object)
{
    GAction *action = G_ACTION(object);
    rbgobj_gc_mark_instance(action);
}

static void
rb_gio2_action_proxy_mark(gpointer object)
{
    gchar **names;
    gchar *name;
    GAction *action;

    names = g_action_group_list_actions(G_ACTION_GROUP(object));
    for (size_t n = 0;  names[n] != NULL; n++) {
        name = names[n];
        action = g_action_map_lookup_action(G_ACTION_MAP(object), name);
        if (action != NULL) {
            rb_gio2_action_mark(action);
        }
    }
    g_strfreev(names);
}

static void
rb_gio2_application_mark(gpointer object)
{
    GApplication *app = G_APPLICATION(object);
    rbgobj_gc_mark_instance(app);
    if (g_application_get_is_registered(app)) {
        rb_gio2_action_proxy_mark(app);
    }
}

static void
rb_gio2_simple_action_group_mark(gpointer object)
{
    GSimpleActionGroup *group = G_SIMPLE_ACTION_GROUP(object);
    rbgobj_gc_mark_instance(group);
    rb_gio2_action_proxy_mark(group);
}


void
Init_gio2 (void)
{
    VALUE RG_TARGET_NAMESPACE;

    RG_TARGET_NAMESPACE = rb_define_module("Gio");

    rb_gio2_init_application(RG_TARGET_NAMESPACE);
    rb_gio2_init_pollable_source(RG_TARGET_NAMESPACE);

    rbgobj_register_mark_func(G_TYPE_ACTION, rb_gio2_action_mark);
    rbgobj_register_mark_func(G_TYPE_APPLICATION, rb_gio2_application_mark);
    rbgobj_register_mark_func(G_TYPE_SIMPLE_ACTION_GROUP, rb_gio2_simple_action_group_mark);
}
