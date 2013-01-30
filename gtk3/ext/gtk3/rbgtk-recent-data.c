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

#include "rbgtk3private.h"

/*****************************************/
static GtkRecentData*
rd_copy(const GtkRecentData* data)
{
    GtkRecentData* new_data;
    g_return_val_if_fail (data != NULL, NULL);
    new_data = g_new(GtkRecentData, 1);
    *new_data = *data;
    return new_data;
}

static void
rd_free(GtkRecentData* boxed)
{
    if (boxed){
        g_free(boxed->groups);
        g_free(boxed);
    }
}

GType
gtk_recent_data_get_type(void)
{
  static GType our_type = 0;
  if (our_type == 0)
    our_type = g_boxed_type_register_static ("GtkRecentData",
                    (GBoxedCopyFunc)rd_copy,
                    (GBoxedFreeFunc)rd_free);
  return our_type;
}
/*****************************************/

#define RG_TARGET_NAMESPACE cRecentData
#define _SELF(w) (RVAL2GTKRECENTDATA(w))

static VALUE
rg_initialize(VALUE self)
{   
    GtkRecentData data;
    G_INITIALIZE(self, &data);
    return Qnil;
}

static VALUE
rg_display_name(VALUE self)
{
    return CSTR2RVAL(_SELF(self)->display_name);
}

static VALUE
rg_set_display_name(VALUE self, VALUE val)
{
    _SELF(self)->display_name = (gchar *) RVAL2CSTR(val);
    return self;
}

static VALUE
rg_description(VALUE self)
{
    return CSTR2RVAL(_SELF(self)->description);
}

static VALUE
rg_set_description(VALUE self, VALUE val)
{
    _SELF(self)->description = (gchar *) RVAL2CSTR(val);
    return self;
}

static VALUE
rg_mime_type(VALUE self)
{
    return CSTR2RVAL(_SELF(self)->mime_type);
}

static VALUE
rg_set_mime_type(VALUE self, VALUE val)
{
    _SELF(self)->mime_type = (gchar *) RVAL2CSTR(val);
    return self;
}

static VALUE
rg_app_name(VALUE self)
{
    return CSTR2RVAL(_SELF(self)->app_name);
}

static VALUE
rg_set_app_name(VALUE self, VALUE val)
{
    _SELF(self)->app_name = (gchar *) RVAL2CSTR(val);
    return self;
}

static VALUE
rg_app_exec(VALUE self)
{
    return CSTR2RVAL(_SELF(self)->app_exec);
}

static VALUE
rg_set_app_exec(VALUE self, VALUE val)
{
    _SELF(self)->app_exec = (gchar *) RVAL2CSTR(val);
    return self;
}

static VALUE
rg_groups(VALUE self)
{
    gchar** grps = _SELF(self)->groups;
    VALUE ary = rb_ary_new();

    gint i = 0;
    while (grps[i]) {
        rb_ary_push(ary, CSTR2RVAL((gchar*)grps[i]));
        i++;
    }
    return ary;
}

static VALUE
rg_set_groups(VALUE self, VALUE groups)
{
    _SELF(self)->groups = RVAL2STRV_DUP(groups);

    return self;
}

static VALUE
rg_private_p(VALUE self)
{
    return CBOOL2RVAL(_SELF(self)->is_private);
}

static VALUE
rg_set_private(VALUE self, VALUE is_private)
{
    _SELF(self)->is_private = RVAL2CBOOL(is_private);
    return self;
}

void
Init_gtk_recent_data(VALUE mGtk)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_RECENT_DATA, "RecentData", mGtk);

    RG_DEF_METHOD(initialize, 0);
    RG_DEF_METHOD(display_name, 0);
    RG_DEF_METHOD(set_display_name, 1);
    RG_DEF_METHOD(description, 0);
    RG_DEF_METHOD(set_description, 1);
    RG_DEF_METHOD(mime_type, 0);
    RG_DEF_METHOD(set_mime_type, 1);
    RG_DEF_METHOD(app_name, 0);
    RG_DEF_METHOD(set_app_name, 1);
    RG_DEF_METHOD(app_exec, 0);
    RG_DEF_METHOD(set_app_exec, 1);
    RG_DEF_METHOD(groups, 0);
    RG_DEF_METHOD(set_groups, 1);
    RG_DEF_METHOD_P(private, 0);
    RG_DEF_METHOD(set_private, 1);
}
