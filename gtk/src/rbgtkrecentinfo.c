/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkrecentinfo.c -

  $Author: mutoh $
  $Date: 2006/11/12 15:28:40 $

  Copyright (C) 2006 Masao Mutoh
************************************************/

#include "global.h"

#if GTK_CHECK_VERSION(2,10,0)

#define _SELF(self) ((GtkRecentInfo*)RVAL2BOXED(self, GTK_TYPE_RECENT_INFO))


static VALUE
ri_get_uri(self)
    VALUE self;
{
    return CSTR2RVAL(gtk_recent_info_get_uri(_SELF(self)));
}

static VALUE
ri_get_display_name(self)
    VALUE self;
{
    return CSTR2RVAL(gtk_recent_info_get_display_name(_SELF(self)));
}

static VALUE
ri_get_description(self)
    VALUE self;
{
    return CSTR2RVAL(gtk_recent_info_get_description(_SELF(self)));
}

static VALUE
ri_get_mime_type(self)
    VALUE self;
{
    return CSTR2RVAL(gtk_recent_info_get_mime_type(_SELF(self)));
}

static VALUE
ri_get_added(self)
    VALUE self;
{
    time_t t = gtk_recent_info_get_added(_SELF(self));
    return rb_funcall(rb_cTime, rb_intern("at"), 1, INT2NUM(t));
}

static VALUE
ri_get_modified(self)
    VALUE self;
{
    time_t t = gtk_recent_info_get_modified(_SELF(self));
    return rb_funcall(rb_cTime, rb_intern("at"), 1, INT2NUM(t));
}

static VALUE
ri_get_visited(self)
    VALUE self;
{
    time_t t = gtk_recent_info_get_visited(_SELF(self));
    return rb_funcall(rb_cTime, rb_intern("at"), 1, INT2NUM(t));
}

static VALUE
ri_get_private_hint(self)
    VALUE self;
{
    return CBOOL2RVAL(gtk_recent_info_get_private_hint(_SELF(self)));
}

static VALUE
ri_get_application_info(self, app_name)
    VALUE self, app_name;
{
    const gchar *app_exec;
    guint count;
    time_t t;
    gboolean ret = gtk_recent_info_get_application_info(_SELF(self),
                                                        RVAL2CSTR(app_name),
                                                        &app_exec,
                                                        &count,
                                                        &t);
    VALUE ary = rb_ary_new();
    if (ret){
        rb_ary_push(ary, CSTR2RVAL(app_exec));
        rb_ary_push(ary, UINT2NUM(count));
        rb_ary_push(ary, rb_funcall(rb_cTime, rb_intern("at"), 1, INT2NUM(t)));
    }
    return ary;
}    

static VALUE
ri_get_applications(self)
    VALUE self;
{
    gint i;
    gsize length;
    gchar** str = gtk_recent_info_get_applications(_SELF(self), &length);
    VALUE ary = rb_ary_new();
    for(i = 0; i < length; i++){
        rb_ary_push(ary, CSTR2RVAL(str[i]));
    }
    g_strfreev(str);
    return ary;
}

static VALUE
ri_last_application(self)
    VALUE self;
{
    return CSTR2RVAL(gtk_recent_info_last_application(_SELF(self)));
}

static VALUE
ri_get_groups(self)
    VALUE self;
{
    gint i;
    gsize length;
    gchar** str = gtk_recent_info_get_groups(_SELF(self), &length);
    VALUE ary = rb_ary_new();
    for(i = 0; i < length; i++){
        rb_ary_push(ary, CSTR2RVAL(str[i]));
    }
    g_strfreev(str);
    return ary;
}

static VALUE
ri_has_group(self, group_name)
    VALUE self, group_name;
{
    return CBOOL2RVAL(gtk_recent_info_has_group(_SELF(self), 
                                                RVAL2CSTR(group_name)));
}

static VALUE
ri_has_application(self, app_name)
    VALUE self, app_name;
{
    return CBOOL2RVAL(gtk_recent_info_has_application(_SELF(self), 
                                                RVAL2CSTR(app_name)));
}

static VALUE
ri_get_icon(self, size)
    VALUE self, size;
{
    return GOBJ2RVAL(gtk_recent_info_get_icon(_SELF(self), NUM2INT(size)));
}

static VALUE
ri_get_short_name(self)
    VALUE self;
{
    return CSTR2RVAL(gtk_recent_info_get_short_name(_SELF(self)));
}

static VALUE
ri_get_uri_display(self)
    VALUE self;
{
    return CSTR2RVAL(gtk_recent_info_get_uri_display(_SELF(self)));
}

static VALUE
ri_get_age(self)
    VALUE self;
{
    return INT2NUM(gtk_recent_info_get_age(_SELF(self)));
}

static VALUE
ri_is_local(self)
    VALUE self;
{
    return CBOOL2RVAL(gtk_recent_info_is_local(_SELF(self)));
}

static VALUE
ri_exist(self)
    VALUE self;
{
    return CBOOL2RVAL(gtk_recent_info_exists(_SELF(self)));
}

static VALUE
ri_match(self, other)
    VALUE self, other;
{
    return CBOOL2RVAL(gtk_recent_info_match(_SELF(self), _SELF(other)));
}

#endif

void 
Init_gtk_recent_info()
{
#if GTK_CHECK_VERSION(2,10,0)
    VALUE ri = G_DEF_CLASS(GTK_TYPE_RECENT_INFO, "RecentInfo", mGtk);
    rb_define_method(ri, "uri", ri_get_uri, 0);
    rb_define_method(ri, "display_name", ri_get_display_name, 0);
    rb_define_method(ri, "description", ri_get_description, 0);
    rb_define_method(ri, "mime_type", ri_get_mime_type, 0);
    rb_define_method(ri, "added", ri_get_added, 0);
    rb_define_method(ri, "modified", ri_get_modified, 0);
    rb_define_method(ri, "visited", ri_get_visited, 0);
    rb_define_method(ri, "private_hint?", ri_get_private_hint, 0);
    rb_define_method(ri, "application_info", ri_get_application_info, 1);
    rb_define_method(ri, "applications", ri_get_applications, 0);
    rb_define_method(ri, "last_application", ri_last_application, 0);
    rb_define_method(ri, "groups", ri_get_groups, 0);
    rb_define_method(ri, "has_group?", ri_has_group, 1);
    rb_define_method(ri, "has_application?", ri_has_application, 1);
    rb_define_method(ri, "get_icon", ri_get_icon, 1);
    rb_define_method(ri, "short_name", ri_get_short_name, 0);
    rb_define_method(ri, "uri_display", ri_get_uri_display, 0);
    rb_define_method(ri, "age", ri_get_age, 0);
    rb_define_method(ri, "local?", ri_is_local, 0);
    rb_define_method(ri, "exist?", ri_exist, 0);
    rb_define_method(ri, "==", ri_match, 1);

    G_DEF_SETTERS(ri);   
#endif
}
