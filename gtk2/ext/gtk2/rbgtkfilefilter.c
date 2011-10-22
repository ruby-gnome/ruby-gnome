/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2005  Ruby-GNOME2 Project Team
 *  Copyright (C) 2004  Seiya Nishizawa, Masao Mutoh
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

#define _SELF(self) GTK_FILE_FILTER(RVAL2GOBJ(self))

static VALUE
ffil_initialize(VALUE self)
{
  RBGTK_INITIALIZE(self, gtk_file_filter_new());
  return Qnil;
}

static VALUE
ffil_set_name(VALUE self, VALUE name)
{
  gtk_file_filter_set_name(_SELF(self), RVAL2CSTR(name));
  return self;
}

static VALUE
ffil_get_name(VALUE self)
{
  return CSTR2RVAL(gtk_file_filter_get_name(_SELF(self)));
}

static VALUE
ffil_add_mime_type(VALUE self, VALUE mime)
{
  gtk_file_filter_add_mime_type(_SELF(self), RVAL2CSTR(mime));
  return self;
}

static VALUE
ffil_add_pattern(VALUE self, VALUE pattern)
{
  gtk_file_filter_add_pattern(_SELF(self), RVAL2CSTR(pattern));
  return self;
}

/* Should return true/false */
static gboolean
filter_func(const GtkFileFilterInfo *info, gpointer func)
{
    return CBOOL2RVAL(rb_funcall((VALUE)func,
                                 id_call,
                                 5,
                                 GFLAGS2RVAL(info->contains, GTK_TYPE_FILE_FILTER_FLAGS),
                                 CSTR2RVAL(info->filename),
                                 CSTR2RVAL(info->uri),
                                 CSTR2RVAL(info->display_name),
                                 CSTR2RVAL(info->mime_type)));
}

#if GTK_CHECK_VERSION(2,6,0)
static VALUE
ffil_add_pixbuf_formats(VALUE self)
{
    gtk_file_filter_add_pixbuf_formats(_SELF(self));
    return self;
}
#endif

static VALUE
ffil_add_custom(VALUE self, VALUE needed)
{
    VALUE func = rb_block_proc();
    G_RELATIVE(self, func);
    gtk_file_filter_add_custom(_SELF(self), RVAL2GFLAGS(needed, GTK_TYPE_FILE_FILTER_FLAGS), 
                               (GtkFileFilterFunc)filter_func, (gpointer)func, NULL);
    return self;
}

static VALUE
ffil_get_needed(VALUE self)
{
    return GFLAGS2RVAL(gtk_file_filter_get_needed(_SELF(self)), GTK_TYPE_FILE_FILTER_FLAGS);
}

static VALUE
ffil_filter(VALUE self, VALUE contains, VALUE filename, VALUE uri, VALUE display_name, VALUE mime_type)
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
Init_gtk_file_filter(void)
{
#if GTK_CHECK_VERSION(2,4,0)

    VALUE gFileFilter = G_DEF_CLASS(GTK_TYPE_FILE_FILTER, "FileFilter", mGtk);

    rb_define_method(gFileFilter, "initialize", ffil_initialize, 0);
    rb_define_method(gFileFilter, "set_name", ffil_set_name, 1);
    rb_define_method(gFileFilter, "name", ffil_get_name, 0);
    rb_define_method(gFileFilter, "add_mime_type", ffil_add_mime_type, 1);
    rb_define_method(gFileFilter, "add_pattern", ffil_add_pattern, 1);
#if GTK_CHECK_VERSION(2,6,0)
    rb_define_method(gFileFilter, "add_pixbuf_formats", ffil_add_pixbuf_formats, 0);
#endif
    rb_define_method(gFileFilter, "add_custom", ffil_add_custom, 1);
    rb_define_method(gFileFilter, "needed", ffil_get_needed, 0);
    rb_define_method(gFileFilter, "filter?", ffil_filter, 5);

    G_DEF_SETTERS(gFileFilter);

    G_DEF_CLASS(GTK_TYPE_FILE_FILTER_FLAGS, "Flags", gFileFilter);
    G_DEF_CONSTANTS(gFileFilter, GTK_TYPE_FILE_FILTER_FLAGS, "GTK_FILE_FILTER_");


#endif
}
