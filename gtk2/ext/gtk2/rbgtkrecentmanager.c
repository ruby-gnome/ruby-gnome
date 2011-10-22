/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2006  Masao Mutoh
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

#if GTK_CHECK_VERSION(2,10,0)

#define _SELF(self) (GTK_RECENT_MANAGER(RVAL2GOBJ(self)))


static VALUE
rm_initialize(VALUE self)
{
    G_INITIALIZE(self, gtk_recent_manager_new());
    return Qnil;
}

static VALUE
rm_s_get_default(G_GNUC_UNUSED VALUE self)
{
    return GOBJ2RVAL(gtk_recent_manager_get_default());
}

static VALUE
rm_s_get_for_screen(G_GNUC_UNUSED VALUE self, VALUE screen)
{
    return GOBJ2RVAL(gtk_recent_manager_get_for_screen(GDK_SCREEN(RVAL2GOBJ(screen))));
}

static VALUE
rm_set_screen(VALUE self, VALUE screen)
{
    gtk_recent_manager_set_screen(_SELF(self), GDK_SCREEN(RVAL2GOBJ(screen)));
    return self;
}

static VALUE
rm_add_item(int argc, VALUE *argv, VALUE self)
{
    VALUE uri, data;
    gboolean ret;
    rb_scan_args(argc, argv, "11", &uri, &data);

    if (NIL_P(data)){
        ret = gtk_recent_manager_add_item(_SELF(self), RVAL2CSTR(uri));
        if (! ret) rb_raise(rb_eRuntimeError, "Can't add the uri");
    } else {
        ret = gtk_recent_manager_add_full(_SELF(self),
                                          RVAL2CSTR(uri),
                                          RVAL2BOXED(data, GTK_TYPE_RECENT_DATA));
        if (! ret) rb_raise(rb_eRuntimeError, "Can't add the uri or data");
    }
    return self;
}

static VALUE
rm_remove_item(VALUE self, VALUE uri)
{
    GError* error = NULL;
    gboolean ret = gtk_recent_manager_remove_item(_SELF(self), RVAL2CSTR(uri),
                                                  &error);
    if (! ret) RAISE_GERROR(error);
    
    return self;
}

static VALUE
rm_lookup_item(VALUE self, VALUE uri)
{
    GError* error = NULL;
    GtkRecentInfo* info = gtk_recent_manager_lookup_item(_SELF(self), RVAL2CSTR(uri),
                                                         &error);
    if (error) RAISE_GERROR(error);
    return BOXED2RVAL(info, GTK_TYPE_RECENT_INFO);
}

static VALUE
rm_has_item(VALUE self, VALUE uri)
{
    return CBOOL2RVAL(gtk_recent_manager_has_item(_SELF(self), RVAL2CSTR(uri)));
}

static VALUE
rm_move_item(VALUE self, VALUE uri, VALUE new_uri)
{
    GError* error = NULL;
    gboolean ret = gtk_recent_manager_move_item(_SELF(self),
                                                RVAL2CSTR(uri),
                                                RVAL2CSTR(new_uri),
                                                &error);
    if (! ret) RAISE_GERROR(error);
    return self;
}

/* Defined as properties
gint        gtk_recent_manager_get_limit    (GtkRecentManager *manager);
void        gtk_recent_manager_set_limit    (GtkRecentManager *manager,
                                             gint limit);
*/

static VALUE
rm_get_items(VALUE self)
{
    return GLIST2ARY2F(gtk_recent_manager_get_items(_SELF(self)), GTK_TYPE_RECENT_INFO);
}

static VALUE
rm_purge_items(VALUE self)
{
    GError* error = NULL;
    gint ret = gtk_recent_manager_purge_items(_SELF(self), &error);

    if (error) RAISE_GERROR(error);

    return INT2NUM(ret);
}
#endif

void 
Init_gtk_recent_manager(void)
{
#if GTK_CHECK_VERSION(2,10,0)
    VALUE rm = G_DEF_CLASS(GTK_TYPE_RECENT_MANAGER, "RecentManager", mGtk);

    rb_define_method(rm, "initialize", rm_initialize, 0);
    rb_define_singleton_method(rm, "default", rm_s_get_default, 0);
    rb_define_singleton_method(rm, "get_for_screen", rm_s_get_for_screen, 1);
    rb_define_method(rm, "set_screen", rm_set_screen, 1);
    G_DEF_SETTER(rm, "set_screen");
    rb_define_method(rm, "add_item", rm_add_item, -1);
    rb_define_method(rm, "remove_item", rm_remove_item, 1);
    rb_define_method(rm, "lookup_item", rm_lookup_item, 1);
    rb_define_method(rm, "has_item?", rm_has_item, 1);
    rb_define_method(rm, "move_item", rm_move_item, 2);
    rb_define_method(rm, "items", rm_get_items, 0);
    rb_define_method(rm, "purge_items", rm_purge_items, 0);

    /* GtkRecentManagerError */
    G_DEF_ERROR(GTK_RECENT_MANAGER_ERROR, "RecentManagerError", mGtk, rb_eRuntimeError,
                GTK_TYPE_RECENT_MANAGER_ERROR);
#endif
}
