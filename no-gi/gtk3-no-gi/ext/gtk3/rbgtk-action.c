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

#include "rbgtk3private.h"

#define RG_TARGET_NAMESPACE cAction
#define _SELF(self) (RVAL2GTKACTION(self))

static VALUE
rg_initialize(int argc, VALUE *argv, VALUE self)
{
    VALUE name, options, label, tooltip, stock_id, buffer;

    rb_scan_args(argc, argv, "11", &name, &options);
    rbg_scan_options(options,
                     "label", &label,
                     "tooltip", &tooltip,
                     "stock_id", &stock_id,
                     NULL);

    G_INITIALIZE(self, gtk_action_new(RVAL2CSTR(name),
                                      RVAL2CSTR_ACCEPT_NIL(label),
                                      RVAL2CSTR_ACCEPT_NIL(tooltip),
                                      RVAL2GLIBID_ACCEPT_NIL(stock_id, buffer)));

    return Qnil;
}

static VALUE
rg_is_sensitive_p(VALUE self)
{
    return CBOOL2RVAL(gtk_action_is_sensitive(_SELF(self)));
}

static VALUE
rg_is_visible_p(VALUE self)
{
    return CBOOL2RVAL(gtk_action_is_visible(_SELF(self)));
}

static VALUE
rg_activate(VALUE self)
{
    gtk_action_activate(_SELF(self));
    return self;
}

static VALUE
rg_create_icon(VALUE self, VALUE icon_size)
{
    return GOBJ2RVAL(gtk_action_create_icon(_SELF(self), 
                                            RVAL2GTKICONSIZE(icon_size)));
}

static VALUE
rg_create_menu_item(VALUE self)
{
    return GOBJ2RVAL(gtk_action_create_menu_item(_SELF(self)));
}

static VALUE
rg_create_menu(VALUE self)
{
    return GOBJ2RVAL(gtk_action_create_menu(_SELF(self)));
}

static VALUE
rg_create_tool_item(VALUE self)
{
    return GOBJ2RVAL(gtk_action_create_tool_item(_SELF(self)));
}

static VALUE
rg_proxies(VALUE self)
{
    return GOBJGSLIST2RVAL(gtk_action_get_proxies(_SELF(self)));
}

static VALUE
rg_connect_accelerator(VALUE self)
{
    gtk_action_connect_accelerator(_SELF(self));
    return self;
}

static VALUE
rg_disconnect_accelerator(VALUE self)
{
    gtk_action_disconnect_accelerator(_SELF(self));
    return self;
}

static VALUE
rg_set_accel_path(VALUE self, VALUE accel_path)
{
    gtk_action_set_accel_path(_SELF(self), RVAL2CSTR(accel_path));
    return self;
}

static VALUE
rg_set_accel_group(VALUE self, VALUE accel_group)
{
    gtk_action_set_accel_group(_SELF(self),
                               NIL_P(accel_group) ? NULL : RVAL2GTKACCELGROUP(accel_group));
    return self;
}

static VALUE
rg_accel_path(VALUE self)
{
    return CSTR2RVAL(gtk_action_get_accel_path(_SELF(self)));
}

static VALUE
rg_accel_closure(VALUE self)
{
    return GCLOSURE2RVAL(gtk_action_get_accel_closure(_SELF(self)));
}

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

static VALUE
rg_unblock_activate(VALUE self)
{
    gtk_action_unblock_activate(_SELF(self));

    return self;
}

static VALUE
rg_block_activate(VALUE self)
{
    gtk_action_block_activate(_SELF(self));
    if (rb_block_given_p())
        rb_ensure(rb_yield, self, rg_unblock_activate, self);

    return self;
}

void
Init_gtk_action(VALUE mGtk)
{
    VALUE RG_TARGET_NAMESPACE;

    RG_TARGET_NAMESPACE = G_DEF_CLASS_WITH_GC_FUNC(GTK_TYPE_ACTION, "Action", mGtk,
                                                   action_mark, NULL);

    RG_DEF_METHOD(initialize, -1);
    /* (NOTICE) Gtk::Action#is_sensitive?, #is_visible are special.
       Because there are also Gtk::Action#sensitive?, #visible? as property 
       accessors. 
       Gtk::Action#is_sensitive?, #is_visible? check its associated 
       action group.
       Gtk::Action#sensitive?, #visible? returns the value of the properties.
    */
    RG_DEF_METHOD_P(is_sensitive, 0);
    RG_DEF_METHOD_P(is_visible, 0);
    RG_DEF_METHOD(activate, 0);
    RG_DEF_METHOD(create_icon, 1);
    RG_DEF_METHOD(create_menu_item, 0);
    RG_DEF_METHOD(create_menu, 0);
    RG_DEF_METHOD(create_tool_item, 0);
    RG_DEF_METHOD(proxies, 0);
    RG_DEF_METHOD(connect_accelerator, 0);
    RG_DEF_METHOD(disconnect_accelerator, 0);
    RG_DEF_METHOD(set_accel_path, 1);
    RG_DEF_METHOD(set_accel_group, 1);
    RG_DEF_METHOD(accel_path, 0);
    RG_DEF_METHOD(accel_closure, 0);
    RG_DEF_METHOD(block_activate, 0);
    RG_DEF_METHOD(unblock_activate, 0);

    RG_REG_GLIBID_SETTER("stock-id");
}
