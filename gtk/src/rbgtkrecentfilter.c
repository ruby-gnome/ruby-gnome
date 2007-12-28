/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkrecentfilter.c -

  $Author: sakai $
  $Date: 2007/07/08 03:00:49 $

  Copyright (C) 2006 Masao Mutoh
************************************************/

#include "global.h"

#ifndef HAVE_RB_ERRINFO
#  define rb_errinfo() (ruby_errinfo)
#endif

#if GTK_CHECK_VERSION(2,10,0)

#define _SELF(self) (GTK_RECENT_FILTER(RVAL2GOBJ(self)))

static VALUE
rf_initialize(self)
     VALUE self;
{
  RBGTK_INITIALIZE(self, gtk_recent_filter_new());
  return Qnil;
}

static VALUE
rf_get_name(self)
     VALUE self;
{
  return CSTR2RVAL(gtk_recent_filter_get_name(_SELF(self)));
}

static VALUE
rf_set_name(self, name)
     VALUE self, name;
{
  gtk_recent_filter_set_name(_SELF(self), RVAL2CSTR(name));
  return self;
}

static VALUE
rf_add_mime_type(self, mime_type)
     VALUE self, mime_type;
{
  gtk_recent_filter_add_mime_type(_SELF(self), RVAL2CSTR(mime_type));
  return self;
}

static VALUE
rf_add_pattern(self, pattern)
     VALUE self, pattern;
{
  gtk_recent_filter_add_pattern(_SELF(self), RVAL2CSTR(pattern));
  return self;
}

static VALUE
rf_add_pixbuf_formats(self)
     VALUE self;
{
  gtk_recent_filter_add_pixbuf_formats(_SELF(self));
  return self;
}

static VALUE
rf_add_application(self, application)
     VALUE self, application;
{
  gtk_recent_filter_add_application(_SELF(self), RVAL2CSTR(application));
  return self;
}

static VALUE
rf_add_group(self, group)
     VALUE self, group;
{
  gtk_recent_filter_add_group(_SELF(self), RVAL2CSTR(group));
  return self;
}

static VALUE
rf_add_age(self, days)
     VALUE self, days;
{
  gtk_recent_filter_add_age(_SELF(self), NUM2INT(days));
  return self;
}


struct callback_arg
{
  VALUE callback;
  VALUE info;
};

static VALUE
invoke_callback(VALUE arg_)
{
  struct callback_arg *arg = (struct callback_arg *)arg_;
  return rb_funcall(arg->callback, id_call, 1, arg->info);
}

static gboolean
filter_func(info, func)
     const GtkRecentFilterInfo* info;
     gpointer func;
{
    struct callback_arg arg;

    arg.callback = (VALUE)func;
    arg.info = BOXED2RVAL((gpointer)info, GTK_TYPE_RECENT_FILTER_INFO);
  
    VALUE result = G_PROTECT_CALLBACK(invoke_callback, &arg);
    return NIL_P(rb_errinfo()) ? TRUE : RVAL2CBOOL(result);
}

static void
remove_callback_reference(data)
     gpointer data;
{
    G_CHILD_REMOVE(mGtk, (VALUE)data);
}

static VALUE
rf_add_custom(self, needed)
     VALUE self, needed;
{
  VALUE func = rb_block_proc();
  G_CHILD_ADD(mGtk, func);

  gtk_recent_filter_add_custom(_SELF(self),
			       RVAL2GFLAGS(needed, GTK_TYPE_RECENT_FILTER_FLAGS),
			       (GtkRecentFilterFunc)filter_func, 
			       (gpointer)func, 
			       (GDestroyNotify)remove_callback_reference);
  return self;
}

static VALUE
rf_get_needed(self)
     VALUE self;
{
  return GFLAGS2RVAL(gtk_recent_filter_get_needed(_SELF(self)), GTK_TYPE_RECENT_FILTER_FLAGS);
}


static VALUE
rf_filter_filter(self, filter_info)
    VALUE self, filter_info;
{
    return CBOOL2RVAL(gtk_recent_filter_filter(_SELF(self),
                                               (GtkRecentFilterInfo*)RVAL2BOXED(filter_info, GTK_TYPE_RECENT_FILTER_INFO)));
}
#endif

void 
Init_gtk_recent_filter()
{
#if GTK_CHECK_VERSION(2,10,0)
    VALUE rf = G_DEF_CLASS(GTK_TYPE_RECENT_FILTER, "RecentFilter", mGtk);
    rb_define_method(rf, "initialize", rf_initialize, 0);
    rb_define_method(rf, "name", rf_get_name, 0);
    rb_define_method(rf, "set_name", rf_set_name, 1);
    rb_define_method(rf, "add_mime_type", rf_add_mime_type, 1);
    rb_define_method(rf, "add_pattern", rf_add_pattern, 1);
    rb_define_method(rf, "add_pixbuf_formats", rf_add_pixbuf_formats, 0);
    rb_define_method(rf, "add_application", rf_add_application, 1);
    rb_define_method(rf, "add_group", rf_add_group, 1);
    rb_define_method(rf, "add_age", rf_add_age, 1);
    rb_define_method(rf, "add_custom", rf_add_custom, 1);
    rb_define_method(rf, "needed", rf_get_needed, 0);
    rb_define_method(rf, "filter", rf_filter_filter, 1);

    G_DEF_SETTERS(rf);   

    /* GtkRecentFilterFlags */
    G_DEF_CLASS(GTK_TYPE_RECENT_FILTER_FLAGS, "Flags", rf);
    G_DEF_CONSTANTS(rf, GTK_TYPE_RECENT_FILTER_FLAGS, "GTK_RECENT_FILTER_");
#endif
}
