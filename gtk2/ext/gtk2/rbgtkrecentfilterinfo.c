/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkrecentfilterinfo.c -

  $Author: mutoh $
  $Date: 2006/11/12 15:28:40 $

  Copyright (C) 2006 Masao Mutoh
************************************************/

#include "global.h"

#if GTK_CHECK_VERSION(2,10,0)

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

#define _SELF(w) ((GtkRecentFilterInfo*)RVAL2BOXED(w, GTK_TYPE_RECENT_FILTER_INFO))

static VALUE
rf_initialize(VALUE self)
{   
    GtkRecentFilterInfo finfo;
    G_INITIALIZE(self, &finfo);
    return Qnil;
}

static VALUE
rf_get_contains(VALUE self)
{
    return GFLAGS2RVAL(_SELF(self)->contains, GTK_TYPE_RECENT_FILTER_FLAGS);
}

static VALUE
rf_set_contains(VALUE self, VALUE contains)
{
    _SELF(self)->contains = RVAL2GFLAGS(contains, GTK_TYPE_RECENT_FILTER_FLAGS);
    return self;
}

#define FINFO_STR(name)\
static VALUE \
rf_get_ ## name (VALUE self)\
{\
    return CSTR2RVAL(_SELF(self)->name);\
}\
static VALUE \
rf_set_ ## name (VALUE self, VALUE val)\
{\
    _SELF(self)->name = RVAL2CSTR(val);\
    return self;\
}

FINFO_STR(uri);
FINFO_STR(display_name);
FINFO_STR(mime_type);

static VALUE
rf_get_applications(VALUE self)
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
rf_set_applications(VALUE self, VALUE applications)
{
    gint i;
    gint len = RARRAY_LEN(applications);
    gchar** apps = g_new(gchar*, len + 1);
    for (i = 0; i < len; i++) {
        apps[i] = RVAL2CSTR(RARRAY_PTR(applications)[i]);
    }
    apps[len] = NULL;
    _SELF(self)->applications = (const gchar**)apps;

    return self;
}

static VALUE
rf_get_groups(VALUE self)
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
rf_set_groups(VALUE self, VALUE groups)
{
    gint i;
    gint len = RARRAY_LEN(groups);
    gchar** grps = g_new(gchar*, len + 1);
    for (i = 0; i < len; i++) {
        grps[i] = RVAL2CSTR(RARRAY_PTR(groups)[i]);
    }
    grps[len] = NULL;
    _SELF(self)->groups = (const gchar**)grps;

    return self;
}

static VALUE
rf_get_age(VALUE self)
{
    return INT2NUM(_SELF(self)->age);
}

static VALUE
rf_set_age(VALUE self, VALUE age)
{
    _SELF(self)->age = NUM2INT(age);
    return self;
}

#endif

void 
Init_gtk_recent_filter_info()
{
#if GTK_CHECK_VERSION(2,10,0)
    VALUE rf = G_DEF_CLASS(GTK_TYPE_RECENT_FILTER_INFO, "RecentFilterInfo", mGtk);

    rb_define_method(rf, "initialize", rf_initialize, 0);
    rb_define_method(rf, "contains", rf_get_contains, 0);
    rb_define_method(rf, "set_contains", rf_set_contains, 1);
    rb_define_method(rf, "uri", rf_get_uri, 0);
    rb_define_method(rf, "set_uri", rf_set_uri, 1);
    rb_define_method(rf, "display_name", rf_get_display_name, 0);
    rb_define_method(rf, "set_display_name", rf_set_display_name, 1);
    rb_define_method(rf, "mime_type", rf_get_mime_type, 0);
    rb_define_method(rf, "set_mime_type", rf_set_mime_type, 1);
    rb_define_method(rf, "applications", rf_get_applications, 0);
    rb_define_method(rf, "set_applications", rf_set_applications, 1);
    rb_define_method(rf, "groups", rf_get_groups, 0);
    rb_define_method(rf, "set_groups", rf_set_groups, 1);
    rb_define_method(rf, "age", rf_get_age, 0);
    rb_define_method(rf, "set_age", rf_set_age, 1);

    G_DEF_SETTERS(rf);   
#endif
}
