/* Gnome::DEntryEdit class for Ruby-Gnome
 * Copyright (C) 2001 Neil Conway <neilconway@rogers.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include "rbgnome.h"

VALUE gnoDEntryEdit;

static VALUE
dentry_s_new_notebook(self, notebook)
    VALUE self, notebook;
{
    GtkObject* dentry = gnome_dentry_edit_new_notebook(
                            GTK_NOTEBOOK(get_gobject(notebook)));
    set_gobject(self, dentry);
    return Qnil;
}

static VALUE
dentry_initialize(self)
    VALUE self;
{
    set_gobject(self, gnome_dentry_edit_new());
    return Qnil;
}

static VALUE
dentry_child1(self)
    VALUE self;
{
    GtkWidget* child;
    child = gnome_dentry_edit_child1(GNOME_DENTRY_EDIT(get_gobject(self)));
    return make_gnobject_auto_type(GTK_OBJECT(child));
}

static VALUE
dentry_child2(self)
    VALUE self;
{
    GtkWidget* child;
    child = gnome_dentry_edit_child2(GNOME_DENTRY_EDIT(get_gobject(self)));
    return make_gnobject_auto_type(GTK_OBJECT(child));
}

static VALUE
dentry_clear(self)
    VALUE self;
{
    gnome_dentry_edit_clear(GNOME_DENTRY_EDIT(get_gobject(self)));
    return Qnil;
}

static VALUE
dentry_load_file(self, path)
    VALUE self, path;
{
    gnome_dentry_edit_load_file(GNOME_DENTRY_EDIT(get_gobject(self)),
                                STR2CSTR(path));
    return Qnil;
}

static VALUE
dentry_get_icon(self)
    VALUE self;
{
    return rb_str_new2(
            gnome_dentry_edit_get_icon(GNOME_DENTRY_EDIT(get_gobject(self))));
}

static VALUE
dentry_get_name(self)
    VALUE self;
{
    return rb_str_new2(
            gnome_dentry_edit_get_name(GNOME_DENTRY_EDIT(get_gobject(self))));
}

static VALUE
dentry_get_dentry(self)
    VALUE self;
{
    return make_dentry(gnome_dentry_edit_get_dentry(
                GNOME_DENTRY_EDIT(get_gobject(self))));
}

static VALUE
dentry_set_dentry(self, dentry)
    VALUE self, dentry;
{
    gnome_dentry_edit_set_dentry(GNOME_DENTRY_EDIT(get_gobject(self)),
                                 get_dentry(dentry));
    return Qnil;
}

void Init_gnome_dentry_edit()
{
    gnoDEntryEdit = rb_define_class_under(mGnome, "DEntryEdit", gObject);

    rb_define_singleton_method(gnoDEntryEdit,
            "new_notebook", dentry_s_new_notebook, 1);
    rb_define_method(gnoDEntryEdit, "initialize", dentry_initialize, 0);
    rb_define_method(gnoDEntryEdit, "child1", dentry_child1, 0);
    rb_define_method(gnoDEntryEdit, "child2", dentry_child2, 0);
    rb_define_method(gnoDEntryEdit, "clear", dentry_clear, 0);
    rb_define_method(gnoDEntryEdit, "load_file", dentry_load_file, 1);
    rb_define_method(gnoDEntryEdit, "icon", dentry_get_icon, 0);
    rb_define_method(gnoDEntryEdit, "name", dentry_get_name, 0);
    rb_define_method(gnoDEntryEdit, "dentry", dentry_get_dentry, 0);
    rb_define_method(gnoDEntryEdit, "dentry=", dentry_set_dentry, 1);
    rb_define_alias(gnoDEntryEdit, "set_dentry", "dentry=");

    rb_define_const(gnoDEntryEdit, "SIGNAL_CHANGED", rb_str_new2("changed"));
    rb_define_const(gnoDEntryEdit, "SIGNAL_ICON_CHANGED", rb_str_new2("icon_changed"));
    rb_define_const(gnoDEntryEdit, "SIGNAL_NAME_CHANGED", rb_str_new2("name_changed"));
}
