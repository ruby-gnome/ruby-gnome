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

#define _SELF(self) (GTK_UI_MANAGER(RVAL2GOBJ(self)))

static VALUE
rbuimanager_initialize(VALUE self)
{
    G_INITIALIZE(self, gtk_ui_manager_new());
    return Qnil;
}

/* Defined as Properties.
void        gtk_ui_manager_set_add_tearoffs (GtkUIManager *self,
                                             gboolean add_tearoffs);
gboolean    gtk_ui_manager_get_add_tearoffs (GtkUIManager *self);
*/

static VALUE
rbuimanager_insert_action_group(VALUE self, VALUE action_group, VALUE pos)
{
    gtk_ui_manager_insert_action_group(_SELF(self), RVAL2GOBJ(action_group), NUM2INT(pos));
    G_CHILD_ADD(self, action_group);
    return self;
}

static VALUE
rbuimanager_remove_action_group(VALUE self, VALUE action_group)
{
    gtk_ui_manager_remove_action_group(_SELF(self), RVAL2GOBJ(action_group));
    G_CHILD_REMOVE(self, action_group);
    return self;
}

static VALUE
rbuimanager_get_action_groups(VALUE self)
{
    /* Owned by GTK+ */
    return GLIST2ARY(gtk_ui_manager_get_action_groups(_SELF(self)));
}

static VALUE
rbuimanager_get_accel_group(VALUE self)
{
    return GOBJ2RVAL(gtk_ui_manager_get_accel_group(_SELF(self)));
}

static VALUE
rbuimanager_get_widget(VALUE self, VALUE path)
{
    GtkWidget* widget = gtk_ui_manager_get_widget(_SELF(self), RVAL2CSTR(path));
    if (! widget)
        rb_raise(rb_eArgError, "no widget was found.");

    return GOBJ2RVAL(widget);
}

static VALUE
rbuimanager_get_toplevels(VALUE self, VALUE types)
{
    return GSLIST2ARYF(gtk_ui_manager_get_toplevels(_SELF(self), 
                                                    RVAL2GFLAGS(types, GTK_TYPE_UI_MANAGER_ITEM_TYPE)));
}
    
static VALUE
rbuimanager_get_action(VALUE self, VALUE path)
{
    return GOBJ2RVAL(gtk_ui_manager_get_action(_SELF(self), RVAL2CSTR(path)));
}

static VALUE
rbuimanager_add_ui(int argc, VALUE *argv, VALUE self)
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
rbuimanager_new_merge_id(VALUE self)
{
    return UINT2NUM(gtk_ui_manager_new_merge_id(_SELF(self)));
}

static VALUE
rbuimanager_remove_ui(VALUE self, VALUE merge_id)
{
    gtk_ui_manager_remove_ui(_SELF(self), NUM2UINT(merge_id));
    return self;
}

/* Defined as Properties
gchar*      gtk_ui_manager_get_ui           (GtkUIManager *self);
*/

static VALUE
rbuimanager_ensure_update(VALUE self)
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
Init_gtk_uimanager(void)
{
#if GTK_CHECK_VERSION(2,4,0)
    VALUE gUI;

    gUI = G_DEF_CLASS_WITH_GC_FUNC(GTK_TYPE_UI_MANAGER, "UIManager", mGtk,
				   rbuimanager_mark, NULL);

    rb_define_method(gUI, "initialize", rbuimanager_initialize, 0);
    rb_define_method(gUI, "insert_action_group", rbuimanager_insert_action_group, 2);
    rb_define_method(gUI, "remove_action_group", rbuimanager_remove_action_group, 1);
    rb_define_method(gUI, "action_groups", rbuimanager_get_action_groups, 0);
    rb_define_method(gUI, "accel_group", rbuimanager_get_accel_group, 0);
    rb_define_method(gUI, "get_widget", rbuimanager_get_widget, 1);
    rb_define_alias(gUI, "[]", "get_widget");
    rb_define_method(gUI, "get_toplevels", rbuimanager_get_toplevels, 1);
    rb_define_method(gUI, "get_action", rbuimanager_get_action, 1);
    rb_define_method(gUI, "add_ui", rbuimanager_add_ui, -1);
    rb_define_method(gUI, "new_merge_id", rbuimanager_new_merge_id, 0);
    rb_define_method(gUI, "remove_ui", rbuimanager_remove_ui, 1);
    rb_define_method(gUI, "ensure_update", rbuimanager_ensure_update, 0);

    /* GtkUIManagerItemType */
    G_DEF_CLASS(GTK_TYPE_UI_MANAGER_ITEM_TYPE, "ItemType", gUI);
    G_DEF_CONSTANTS(gUI, GTK_TYPE_UI_MANAGER_ITEM_TYPE, "GTK_UI_MANAGER_");
                                                                                
#endif
}
