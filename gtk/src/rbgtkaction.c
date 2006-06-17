/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkaction.c -

  $Author: mutoh $
  $Date: 2006/06/17 06:59:32 $

  Copyright (C) 2004-2006 Masao Mutoh
************************************************/

#include "global.h"
                                                                                
#if GTK_CHECK_VERSION(2,4,0)

#define _SELF(self) (GTK_ACTION(RVAL2GOBJ(self)))
#define RVAL2WIDGET(w) (GTK_WIDGET(RVAL2GOBJ(w)))

static VALUE
action_initialize(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE name, label, tooltip, stock_id;
    gchar* stock = (gchar*)NULL;

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
action_is_sensitive(self)
    VALUE self;
{
    return CBOOL2RVAL(gtk_action_is_sensitive(_SELF(self)));
}

static VALUE
action_is_visible(self)
    VALUE self;
{
    return CBOOL2RVAL(gtk_action_is_visible(_SELF(self)));
}

/* Defined as property.
const gchar* gtk_action_get_name            (GtkAction *action);
gboolean    gtk_action_get_sensitive        (GtkAction *action);
gboolean    gtk_action_get_visible          (GtkAction *action);
*/

static VALUE
action_activate(self)
    VALUE self;
{
    gtk_action_activate(_SELF(self));
    return self;
}

static VALUE
action_create_icon(self, icon_size)
    VALUE self, icon_size;
{
    return GOBJ2RVAL(gtk_action_create_icon(_SELF(self), 
                                            RVAL2GENUM(icon_size, GTK_TYPE_ICON_SIZE)));
}

static VALUE
action_create_menu_item(self)
    VALUE self;
{
    return GOBJ2RVAL(gtk_action_create_menu_item(_SELF(self)));
}

static VALUE
action_create_tool_item(self)
    VALUE self;
{
    return GOBJ2RVAL(gtk_action_create_tool_item(_SELF(self)));
}

static VALUE
action_connect_proxy(self, proxy)
    VALUE self, proxy;
{
    gtk_action_connect_proxy(_SELF(self), RVAL2WIDGET(proxy));
    return self;
}

static VALUE
action_disconnect_proxy(self, proxy)
    VALUE self, proxy;
{
    gtk_action_disconnect_proxy(_SELF(self), RVAL2WIDGET(proxy));
    return self;
}

static VALUE
action_get_proxies(self)
    VALUE self;
{
    /* Owned by GTK+ */
    return GSLIST2ARY(gtk_action_get_proxies(_SELF(self)));
}

static VALUE
action_connect_accelerator(self)
    VALUE self;
{
    gtk_action_connect_accelerator(_SELF(self));
    return self;
}

static VALUE
action_disconnect_accelerator(self)
    VALUE self;
{
    gtk_action_disconnect_accelerator(_SELF(self));
    return self;
}

static VALUE
action_block_activate_from(self, proxy)
    VALUE self, proxy;
{
    gtk_action_block_activate_from(_SELF(self), RVAL2WIDGET(proxy));
    return self;
}

static VALUE
action_unblock_activate_from(self, proxy)
    VALUE self, proxy;
{
    gtk_action_unblock_activate_from(_SELF(self), RVAL2WIDGET(proxy));
    return self;
}

static VALUE
action_set_accel_path(self, accel_path)
    VALUE self, accel_path;
{
    gtk_action_set_accel_path(_SELF(self), RVAL2CSTR(accel_path));
    return self;
}
static VALUE
action_set_accel_group(self, accel_group)
    VALUE self, accel_group;
{
    gtk_action_set_accel_group(_SELF(self), RVAL2GOBJ(accel_group));
    return self;
}

#if GTK_CHECK_VERSION(2,6,0)
/* This set "sensitive" value and call g_object_notify with "sensitive". 
   If you want not to call g_object_notify,() 
   Use GLib::Object#set_property("sensitive", val) instead.
 */
static VALUE
action_set_sensitive(self, sensitive)
    VALUE self, sensitive;
{
    gtk_action_set_sensitive(_SELF(self), RTEST(sensitive));
    return self;
}

/* This set "visible" value and call g_object_notify with "visible". 
   If you want not to call g_object_notify(), 
   Use GLib::Object#set_property("visible", val) instead.
*/
static VALUE
action_set_visible(self, visible)
    VALUE self, visible;
{
    gtk_action_set_visible(_SELF(self), RTEST(visible));
    return self;
}

static VALUE
action_get_accel_path(self)
    VALUE self;
{
    return CSTR2RVAL(gtk_action_get_accel_path(_SELF(self)));
}
#endif

#if GTK_CHECK_VERSION(2,8,0)
static VALUE
action_get_accel_closure(self)
    VALUE self;
{
    return BOXED2RVAL(gtk_action_get_accel_closure(_SELF(self)), G_TYPE_CLOSURE);
}
#endif

#endif

void 
Init_gtk_action()
{
#if GTK_CHECK_VERSION(2,4,0)
    VALUE gAction = G_DEF_CLASS(GTK_TYPE_ACTION, "Action", mGtk);

    rb_define_method(gAction, "initialize", action_initialize, -1);
    /* (NOTICE) Gtk::Action#is_sensitive?, #is_visible are special.
       Because there are also Gtk::Action#sensitive?, #visible? as property 
       accessors. 
       Gtk::Action#is_sensitive?, #is_visible? check its associated 
       action group.
       Gtk::Action#sensitive?, #visible? returns the value of the properties.
    */
    rb_define_method(gAction, "is_sensitive?", action_is_sensitive, 0);
    rb_define_method(gAction, "is_visible?", action_is_visible, 0);
    rb_define_method(gAction, "activate", action_activate, 0);
    rb_define_method(gAction, "create_icon", action_create_icon, 1);
    rb_define_method(gAction, "create_menu_item", action_create_menu_item, 0);
    rb_define_method(gAction, "create_tool_item", action_create_tool_item, 0);
    rb_define_method(gAction, "connect_proxy", action_connect_proxy, 1);
    rb_define_method(gAction, "disconnect_proxy", action_disconnect_proxy, 1);
    rb_define_method(gAction, "proxies", action_get_proxies, 0);
    rb_define_method(gAction, "connect_accelerator", action_connect_accelerator, 0);
    rb_define_method(gAction, "disconnect_accelerator", action_disconnect_accelerator, 0);
    rb_define_method(gAction, "block_activate_from", action_block_activate_from, 1);
    rb_define_method(gAction, "unblock_activate_from", action_unblock_activate_from, 1);
    rb_define_method(gAction, "set_accel_path", action_set_accel_path, 1);
    rb_define_method(gAction, "set_accel_group", action_set_accel_group, 1);

#if GTK_CHECK_VERSION(2,6,0)
    rb_define_method(gAction, "set_sensitive", action_set_sensitive, 1);
    rb_define_method(gAction, "set_visible", action_set_visible, 1);
    rb_define_method(gAction, "accel_path", action_get_accel_path, 0);
#endif

#if GTK_CHECK_VERSION(2,8,0)
    rb_define_method(gAction, "accel_closure", action_get_accel_closure, 0);
#endif

    G_DEF_SETTERS(gAction);
#endif
}
