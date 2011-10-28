/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2004-2006 Masao Mutoh
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

#include "global.h"

#if GTK_CHECK_VERSION(2,4,0)

#define RG_TARGET_NAMESPACE cAction
#define _SELF(self) (GTK_ACTION(RVAL2GOBJ(self)))
#define RVAL2WIDGET(w) (GTK_WIDGET(RVAL2GOBJ(w)))

static VALUE
action_initialize(int argc, VALUE *argv, VALUE self)
{
    VALUE name, label, tooltip, stock_id;
    const gchar *stock = NULL;

    rb_scan_args(argc, argv, "22", &name, &label, &tooltip, &stock_id);

    if (TYPE(stock_id) == T_SYMBOL) {
        stock = rb_id2name(SYM2ID(stock_id));
    } else if (TYPE(stock_id) == T_STRING){
        stock = RVAL2CSTR(stock_id);
    }

    G_INITIALIZE(self, gtk_action_new(RVAL2CSTR(name), RVAL2CSTR(label),
                                      NIL_P(tooltip) ? NULL : RVAL2CSTR(tooltip),
                                      stock));
    return Qnil;
}

static VALUE
action_is_sensitive(VALUE self)
{
    return CBOOL2RVAL(gtk_action_is_sensitive(_SELF(self)));
}

static VALUE
action_is_visible(VALUE self)
{
    return CBOOL2RVAL(gtk_action_is_visible(_SELF(self)));
}

static VALUE
action_activate(VALUE self)
{
    gtk_action_activate(_SELF(self));
    return self;
}

static VALUE
action_create_icon(VALUE self, VALUE icon_size)
{
    return GOBJ2RVAL(gtk_action_create_icon(_SELF(self), 
                                            RVAL2GENUM(icon_size, GTK_TYPE_ICON_SIZE)));
}

static VALUE
action_create_menu_item(VALUE self)
{
    return GOBJ2RVAL(gtk_action_create_menu_item(_SELF(self)));
}

#if GTK_CHECK_VERSION(2, 12, 0)
static VALUE
action_create_menu(VALUE self)
{
    return GOBJ2RVAL(gtk_action_create_menu(_SELF(self)));
}
#endif

static VALUE
action_create_tool_item(VALUE self)
{
    return GOBJ2RVAL(gtk_action_create_tool_item(_SELF(self)));
}

static VALUE
action_connect_proxy(VALUE self, VALUE proxy)
{
    gtk_action_connect_proxy(_SELF(self), RVAL2WIDGET(proxy));
    return self;
}

static VALUE
action_disconnect_proxy(VALUE self, VALUE proxy)
{
    gtk_action_disconnect_proxy(_SELF(self), RVAL2WIDGET(proxy));
    return self;
}

static VALUE
action_get_proxies(VALUE self)
{
    /* Owned by GTK+ */
    return GSLIST2ARY(gtk_action_get_proxies(_SELF(self)));
}

static VALUE
action_connect_accelerator(VALUE self)
{
    gtk_action_connect_accelerator(_SELF(self));
    return self;
}

static VALUE
action_disconnect_accelerator(VALUE self)
{
    gtk_action_disconnect_accelerator(_SELF(self));
    return self;
}

static VALUE
action_block_activate_from(VALUE self, VALUE proxy)
{
    gtk_action_block_activate_from(_SELF(self), RVAL2WIDGET(proxy));
    return self;
}

static VALUE
action_unblock_activate_from(VALUE self, VALUE proxy)
{
    gtk_action_unblock_activate_from(_SELF(self), RVAL2WIDGET(proxy));
    return self;
}

static VALUE
action_set_accel_path(VALUE self, VALUE accel_path)
{
    gtk_action_set_accel_path(_SELF(self), RVAL2CSTR(accel_path));
    return self;
}

static VALUE
action_set_accel_group(VALUE self, VALUE accel_group)
{
    gtk_action_set_accel_group(_SELF(self), RVAL2GOBJ(accel_group));
    return self;
}

#if GTK_CHECK_VERSION(2,6,0)
static VALUE
action_get_accel_path(VALUE self)
{
    return CSTR2RVAL(gtk_action_get_accel_path(_SELF(self)));
}
#endif

#if GTK_CHECK_VERSION(2,8,0)
static VALUE
action_get_accel_closure(VALUE self)
{
    return BOXED2RVAL(gtk_action_get_accel_closure(_SELF(self)), G_TYPE_CLOSURE);
}
#endif

static void
action_mark(void *p)
{
    GtkAction *action;
    GSList *node;

    action = GTK_ACTION(p);
    for (node = gtk_action_get_proxies(action);
	 node;
	 node = g_slist_next(node)) {
	GtkWidget *proxy = node->data;
	rbgobj_gc_mark_instance(proxy);
    }
}
#endif

void
Init_gtk_action(void)
{
#if GTK_CHECK_VERSION(2,4,0)
    VALUE RG_TARGET_NAMESPACE;

    RG_TARGET_NAMESPACE = G_DEF_CLASS_WITH_GC_FUNC(GTK_TYPE_ACTION, "Action", mGtk,
				       action_mark, NULL);

    rb_define_method(RG_TARGET_NAMESPACE, "initialize", action_initialize, -1);
    /* (NOTICE) Gtk::Action#is_sensitive?, #is_visible are special.
       Because there are also Gtk::Action#sensitive?, #visible? as property 
       accessors. 
       Gtk::Action#is_sensitive?, #is_visible? check its associated 
       action group.
       Gtk::Action#sensitive?, #visible? returns the value of the properties.
    */
    rb_define_method(RG_TARGET_NAMESPACE, "is_sensitive?", action_is_sensitive, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "is_visible?", action_is_visible, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "activate", action_activate, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "create_icon", action_create_icon, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "create_menu_item", action_create_menu_item, 0);
#if GTK_CHECK_VERSION(2, 12, 0)
    rb_define_method(RG_TARGET_NAMESPACE, "create_menu", action_create_menu, 0);
#endif
    rb_define_method(RG_TARGET_NAMESPACE, "create_tool_item", action_create_tool_item, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "connect_proxy", action_connect_proxy, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "disconnect_proxy", action_disconnect_proxy, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "proxies", action_get_proxies, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "connect_accelerator", action_connect_accelerator, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "disconnect_accelerator", action_disconnect_accelerator, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "block_activate_from", action_block_activate_from, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "unblock_activate_from", action_unblock_activate_from, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "set_accel_path", action_set_accel_path, 1);
    G_DEF_SETTER(RG_TARGET_NAMESPACE, "accel_path");
    rb_define_method(RG_TARGET_NAMESPACE, "set_accel_group", action_set_accel_group, 1);
    G_DEF_SETTER(RG_TARGET_NAMESPACE, "accel_group");

#if GTK_CHECK_VERSION(2,6,0)
    rb_define_method(RG_TARGET_NAMESPACE, "accel_path", action_get_accel_path, 0);
#endif

#if GTK_CHECK_VERSION(2,8,0)
    rb_define_method(RG_TARGET_NAMESPACE, "accel_closure", action_get_accel_closure, 0);
#endif
#endif
}
