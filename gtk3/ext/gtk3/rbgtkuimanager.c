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

#define RG_TARGET_NAMESPACE cUIManager
#define _SELF(self) (GTK_UI_MANAGER(RVAL2GOBJ(self)))

static VALUE
rg_initialize(VALUE self)
{
    G_INITIALIZE(self, gtk_ui_manager_new());
    return Qnil;
}

static VALUE
rg_insert_action_group(VALUE self, VALUE action_group, VALUE pos)
{
    gtk_ui_manager_insert_action_group(_SELF(self), RVAL2GOBJ(action_group), NUM2INT(pos));
    G_CHILD_ADD(self, action_group);
    return self;
}

static VALUE
rg_remove_action_group(VALUE self, VALUE action_group)
{
    gtk_ui_manager_remove_action_group(_SELF(self), RVAL2GOBJ(action_group));
    G_CHILD_REMOVE(self, action_group);
    return self;
}

static VALUE
rg_action_groups(VALUE self)
{
    /* Owned by GTK+ */
    return GLIST2ARY(gtk_ui_manager_get_action_groups(_SELF(self)));
}

static VALUE
rg_accel_group(VALUE self)
{
    return GOBJ2RVAL(gtk_ui_manager_get_accel_group(_SELF(self)));
}

static VALUE
rg_get_widget(VALUE self, VALUE path)
{
    GtkWidget* widget = gtk_ui_manager_get_widget(_SELF(self), RVAL2CSTR(path));
    if (! widget)
        rb_raise(rb_eArgError, "no widget was found.");

    return GOBJ2RVAL(widget);
}

static VALUE
rg_get_toplevels(VALUE self, VALUE types)
{
    return GSLIST2ARYF(gtk_ui_manager_get_toplevels(_SELF(self), 
                                                    RVAL2GFLAGS(types, GTK_TYPE_UI_MANAGER_ITEM_TYPE)));
}

static VALUE
rg_get_action(VALUE self, VALUE path)
{
    return GOBJ2RVAL(gtk_ui_manager_get_action(_SELF(self), RVAL2CSTR(path)));
}

static VALUE
rg_add_ui(int argc, VALUE *argv, VALUE self)
{
    GError* error = NULL;
    guint ret;
    VALUE buffer;

    if (argc == 1) {
        VALUE buffer_or_filename;
        rb_scan_args(argc, argv, "10", &buffer_or_filename);

        StringValue(buffer_or_filename);
        buffer = rb_funcall(buffer_or_filename, rb_intern("include?"), 1, CSTR2RVAL("<ui>"));

        if (RVAL2CBOOL(buffer)){
            StringValue(buffer_or_filename);
            ret = gtk_ui_manager_add_ui_from_string(_SELF(self),
                                                    RSTRING_PTR(buffer_or_filename),
                                                    RSTRING_LEN(buffer_or_filename),
                                                    &error);
        } else {
            ret = gtk_ui_manager_add_ui_from_file(_SELF(self), RVAL2CSTR(buffer_or_filename),
                                                  &error);
        }
    } else {
        VALUE merge_id, path, name, action, type, top;

        rb_scan_args(argc, argv, "60", &merge_id, &path, &name, &action, &type, &top);

        ret = NUM2UINT(merge_id);
        gtk_ui_manager_add_ui(_SELF(self), ret, RVAL2CSTR(path),
                              RVAL2CSTR(name), RVAL2CSTR(action), 
                              RVAL2GFLAGS(type, GTK_TYPE_UI_MANAGER_ITEM_TYPE),
                              RVAL2CBOOL(top));
    }

    if (ret == 0) RAISE_GERROR(error);

    return UINT2NUM(ret);
}

static VALUE
rg_new_merge_id(VALUE self)
{
    return UINT2NUM(gtk_ui_manager_new_merge_id(_SELF(self)));
}

static VALUE
rg_remove_ui(VALUE self, VALUE merge_id)
{
    gtk_ui_manager_remove_ui(_SELF(self), NUM2UINT(merge_id));
    return self;
}

static VALUE
rg_ensure_update(VALUE self)
{
    gtk_ui_manager_ensure_update(_SELF(self));
    return self;
}

static void
rbuimanager_mark(void *p)
{
    GtkUIManager *manager;
    GList *node;

    manager = GTK_UI_MANAGER(p);
    for (node = gtk_ui_manager_get_action_groups(manager);
         node;
         node = g_list_next(node)) {
        GtkWidget *action_group = node->data;
        rbgobj_gc_mark_instance(action_group);
    }

    rbgobj_gc_mark_instance(gtk_ui_manager_get_accel_group(manager));
}
#endif

void
Init_gtk_uimanager(VALUE mGtk)
{
#if GTK_CHECK_VERSION(2,4,0)
    VALUE RG_TARGET_NAMESPACE;

    RG_TARGET_NAMESPACE = G_DEF_CLASS_WITH_GC_FUNC(GTK_TYPE_UI_MANAGER, "UIManager", mGtk,
                                                   rbuimanager_mark, NULL);

    RG_DEF_METHOD(initialize, 0);
    RG_DEF_METHOD(insert_action_group, 2);
    RG_DEF_METHOD(remove_action_group, 1);
    RG_DEF_METHOD(action_groups, 0);
    RG_DEF_METHOD(accel_group, 0);
    RG_DEF_METHOD(get_widget, 1);
    RG_DEF_ALIAS("[]", "get_widget");
    RG_DEF_METHOD(get_toplevels, 1);
    RG_DEF_METHOD(get_action, 1);
    RG_DEF_METHOD(add_ui, -1);
    RG_DEF_METHOD(new_merge_id, 0);
    RG_DEF_METHOD(remove_ui, 1);
    RG_DEF_METHOD(ensure_update, 0);

    /* GtkUIManagerItemType */
    G_DEF_CLASS(GTK_TYPE_UI_MANAGER_ITEM_TYPE, "ItemType", RG_TARGET_NAMESPACE);
    G_DEF_CONSTANTS(RG_TARGET_NAMESPACE, GTK_TYPE_UI_MANAGER_ITEM_TYPE, "GTK_UI_MANAGER_");

#endif
}
