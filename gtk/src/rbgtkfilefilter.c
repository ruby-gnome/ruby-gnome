/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************
 
  rbgtkfilefilter.c -
 
  $Author: mutoh $
  $Date: 2004/07/31 05:44:45 $
 
  Copyright (C) 2004 Seiya Nishizawa, Masao Mutoh
************************************************/
 
#include "global.h"

#if GTK_CHECK_VERSION(2,4,0)

#define _SELF(self) GTK_FILE_FILTER(RVAL2GOBJ(self))

static VALUE
ffil_initialize(self)
    VALUE self;
{
  RBGTK_INITIALIZE(self, gtk_file_filter_new());
  return Qnil;
}

static VALUE
ffil_set_name(self, name)
     VALUE self, name;
{
  gtk_file_filter_set_name(_SELF(self), RVAL2CSTR(name));
  return self;
}

static VALUE
ffil_get_name(self)
     VALUE self;
{
  return CSTR2RVAL(gtk_file_filter_get_name(_SELF(self)));
}

static VALUE
ffil_add_mime_type(self, mime)
     VALUE self, mime;
{
  gtk_file_filter_add_mime_type(_SELF(self), RVAL2CSTR(mime));
  return self;
}

static VALUE
ffil_add_pattern(self, pattern)
     VALUE self, pattern;
{
  gtk_file_filter_add_pattern(_SELF(self), RVAL2CSTR(pattern));
  return self;
}

/* Should return true/false */
static gboolean
filter_func(info, func)
    const GtkFileFilterInfo* info;
    gpointer func;
{
    return CBOOL2RVAL(rb_funcall((VALUE)func, 5, 
                                 GFLAGS2RVAL(info->contains, GTK_TYPE_FILE_FILTER_FLAGS), 
                                 CSTR2RVAL(info->filename), CSTR2RVAL(info->uri), 
                                 CSTR2RVAL(info->display_name), CSTR2RVAL(info->mime_type)));
}

static VALUE
ffil_add_custom(self, needed)
    VALUE self, needed;
{
    VALUE func = G_BLOCK_PROC();
    G_RELATIVE(self, func);
    gtk_file_filter_add_custom(_SELF(self), RVAL2GFLAGS(needed, GTK_TYPE_FILE_FILTER_FLAGS), 
                               filter_func, (gpointer)func, NULL);
    return self;
}

static VALUE
ffil_get_needed(self)
    VALUE self;
{
    return GFLAGS2RVAL(gtk_file_filter_get_needed(_SELF(self)), GTK_TYPE_FILE_FILTER_FLAGS);
}

static VALUE
ffil_filter(self, contains, filename, uri, display_name, mime_type)
    VALUE self, contains, filename, uri, display_name, mime_type;
{
    GtkFileFilterInfo info;
    info.contains = RVAL2GFLAGS(contains, GTK_TYPE_FILE_FILTER_FLAGS);
    info.filename = RVAL2CSTR(filename);
    info.uri = RVAL2CSTR(uri);
    info.display_name = RVAL2CSTR(display_name);
    info.mime_type = RVAL2CSTR(mime_type);

    return CBOOL2RVAL(gtk_file_filter_filter(_SELF(self), &info));
}
#endif

void 
Init_gtk_file_filter()
{
#if GTK_CHECK_VERSION(2,4,0)

    VALUE gFileFilter = G_DEF_CLASS(GTK_TYPE_FILE_FILTER, "FileFilter", mGtk);

    rb_define_method(gFileFilter, "initialize", ffil_initialize, 0);
    rb_define_method(gFileFilter, "set_name", ffil_set_name, 1);
    rb_define_method(gFileFilter, "name", ffil_get_name, 0);
    rb_define_method(gFileFilter, "add_mime_type", ffil_add_mime_type, 1);
    rb_define_method(gFileFilter, "add_pattern", ffil_add_pattern, 1);
    rb_define_method(gFileFilter, "add_custom", ffil_add_custom, 1);
    rb_define_method(gFileFilter, "needed", ffil_get_needed, 0);
    rb_define_method(gFileFilter, "filter?", ffil_filter, 5);

    G_DEF_SETTERS(gFileFilter);

    G_DEF_CLASS(GTK_TYPE_FILE_FILTER_FLAGS, "Flags", gFileFilter);
    G_DEF_CONSTANTS(gFileFilter, GTK_TYPE_FILE_FILTER_FLAGS, "GTK_FILE_FILTER_");


#endif
}
