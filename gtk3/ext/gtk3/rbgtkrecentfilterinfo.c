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
static GtkRecentFilterInfo*
rf_copy(const GtkRecentFilterInfo* info)
{
    GtkRecentFilterInfo* new_info;
    g_return_val_if_fail (info != NULL, NULL);
    new_info = g_new(GtkRecentFilterInfo, 1);
    *new_info = *info;
    return new_info;
}

static void
rf_free(GtkRecentFilterInfo* boxed)
{
    if (boxed){
        g_free(boxed->applications);
        g_free(boxed->groups);
        g_free(boxed);
    }
}

GType
gtk_recent_filter_info_get_type(void)
{
  static GType our_type = 0;
  if (our_type == 0)
    our_type = g_boxed_type_register_static ("GtkRecentFilterInfo",
                    (GBoxedCopyFunc)rf_copy,
                    (GBoxedFreeFunc)rf_free);
  return our_type;
}
/*****************************************/

#define RG_TARGET_NAMESPACE cRecentFilterInfo
#define _SELF(w) (RVAL2GTKRECENTFILTERINFO(w))

static VALUE
rg_initialize(VALUE self)
{   
    GtkRecentFilterInfo finfo;
    G_INITIALIZE(self, &finfo);
    return Qnil;
}

static VALUE
rg_contains(VALUE self)
{
    return GTKRECENTFILTERFLAGS2RVAL(_SELF(self)->contains);
}

static VALUE
rg_set_contains(VALUE self, VALUE contains)
{
    _SELF(self)->contains = RVAL2GTKRECENTFILTERFLAGS(contains);
    return self;
}

static VALUE
rg_uri(VALUE self)
{
    return CSTR2RVAL(_SELF(self)->uri);
}

static VALUE
rg_set_uri(VALUE self, VALUE val)
{
    _SELF(self)->uri = RVAL2CSTR(val);
    return self;
}

static VALUE
rg_display_name(VALUE self)
{
    return CSTR2RVAL(_SELF(self)->display_name);
}

static VALUE
rg_set_display_name(VALUE self, VALUE val)
{
    _SELF(self)->display_name = RVAL2CSTR(val);
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
    _SELF(self)->mime_type = RVAL2CSTR(val);
    return self;
}

static VALUE
rg_applications(VALUE self)
{
    const gchar** apps = _SELF(self)->applications;
    VALUE ary = rb_ary_new();

    gint i = 0;
    while (apps[i]) {
        rb_ary_push(ary, CSTR2RVAL((gchar*)apps[i]));
        i++;
    }
    return ary;
}

static VALUE
rg_set_applications(VALUE self, VALUE applications)
{
    /* NOTE: This can't be right.  What guarantees that the entries in
     * applications will be around?  It should be RVAL2STRV_DUP and use
     * g_strfreev in free above. */
    _SELF(self)->applications = RVAL2STRV(applications);

    return self;
}

static VALUE
rg_groups(VALUE self)
{
    const gchar** grps = _SELF(self)->groups;
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
    _SELF(self)->groups = RVAL2STRV(groups);

    return self;
}

static VALUE
rg_age(VALUE self)
{
    return INT2NUM(_SELF(self)->age);
}

static VALUE
rg_set_age(VALUE self, VALUE age)
{
    _SELF(self)->age = NUM2INT(age);
    return self;
}

void 
Init_gtk_recent_filter_info(VALUE mGtk)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_RECENT_FILTER_INFO, "RecentFilterInfo", mGtk);

    RG_DEF_METHOD(initialize, 0);
    RG_DEF_METHOD(contains, 0);
    RG_DEF_METHOD(set_contains, 1);
    RG_DEF_METHOD(uri, 0);
    RG_DEF_METHOD(set_uri, 1);
    RG_DEF_METHOD(display_name, 0);
    RG_DEF_METHOD(set_display_name, 1);
    RG_DEF_METHOD(mime_type, 0);
    RG_DEF_METHOD(set_mime_type, 1);
    RG_DEF_METHOD(applications, 0);
    RG_DEF_METHOD(set_applications, 1);
    RG_DEF_METHOD(groups, 0);
    RG_DEF_METHOD(set_groups, 1);
    RG_DEF_METHOD(age, 0);
    RG_DEF_METHOD(set_age, 1);
}
