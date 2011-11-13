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

#define RG_TARGET_NAMESPACE cRecentFilter
#define _SELF(self) (GTK_RECENT_FILTER(RVAL2GOBJ(self)))

static VALUE
rg_initialize(VALUE self)
{
  RBGTK_INITIALIZE(self, gtk_recent_filter_new());
  return Qnil;
}

static VALUE
rg_name(VALUE self)
{
  return CSTR2RVAL(gtk_recent_filter_get_name(_SELF(self)));
}

static VALUE
rg_set_name(VALUE self, VALUE name)
{
  gtk_recent_filter_set_name(_SELF(self), RVAL2CSTR(name));
  return self;
}

static VALUE
rg_add_mime_type(VALUE self, VALUE mime_type)
{
  gtk_recent_filter_add_mime_type(_SELF(self), RVAL2CSTR(mime_type));
  return self;
}

static VALUE
rg_add_pattern(VALUE self, VALUE pattern)
{
  gtk_recent_filter_add_pattern(_SELF(self), RVAL2CSTR(pattern));
  return self;
}

static VALUE
rg_add_pixbuf_formats(VALUE self)
{
  gtk_recent_filter_add_pixbuf_formats(_SELF(self));
  return self;
}

static VALUE
rg_add_application(VALUE self, VALUE application)
{
  gtk_recent_filter_add_application(_SELF(self), RVAL2CSTR(application));
  return self;
}

static VALUE
rg_add_group(VALUE self, VALUE group)
{
  gtk_recent_filter_add_group(_SELF(self), RVAL2CSTR(group));
  return self;
}

static VALUE
rg_add_age(VALUE self, VALUE days)
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
filter_func(const GtkRecentFilterInfo *info, gpointer func)
{
    VALUE result;
    struct callback_arg arg;

    arg.callback = (VALUE)func;
    arg.info = BOXED2RVAL((gpointer)info, GTK_TYPE_RECENT_FILTER_INFO);

    result = G_PROTECT_CALLBACK(invoke_callback, &arg);
    return NIL_P(rb_errinfo()) ? TRUE : RVAL2CBOOL(result);
}

static void
remove_callback_reference(gpointer data)
{
    G_CHILD_REMOVE(mGtk, (VALUE)data);
}

static VALUE
rg_add_custom(VALUE self, VALUE needed)
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
rg_needed(VALUE self)
{
  return GFLAGS2RVAL(gtk_recent_filter_get_needed(_SELF(self)), GTK_TYPE_RECENT_FILTER_FLAGS);
}

static VALUE
rg_filter(VALUE self, VALUE filter_info)
{
    return CBOOL2RVAL(gtk_recent_filter_filter(_SELF(self),
                                               (GtkRecentFilterInfo*)RVAL2BOXED(filter_info, GTK_TYPE_RECENT_FILTER_INFO)));
}
#endif

void 
Init_gtk_recent_filter(VALUE mGtk)
{
#if GTK_CHECK_VERSION(2,10,0)
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_RECENT_FILTER, "RecentFilter", mGtk);
    RG_DEF_METHOD(initialize, 0);
    RG_DEF_METHOD(name, 0);
    RG_DEF_METHOD(set_name, 1);
    RG_DEF_METHOD(add_mime_type, 1);
    RG_DEF_METHOD(add_pattern, 1);
    RG_DEF_METHOD(add_pixbuf_formats, 0);
    RG_DEF_METHOD(add_application, 1);
    RG_DEF_METHOD(add_group, 1);
    RG_DEF_METHOD(add_age, 1);
    RG_DEF_METHOD(add_custom, 1);
    RG_DEF_METHOD(needed, 0);
    RG_DEF_METHOD(filter, 1);

    G_DEF_SETTERS(RG_TARGET_NAMESPACE);   

    /* GtkRecentFilterFlags */
    G_DEF_CLASS(GTK_TYPE_RECENT_FILTER_FLAGS, "Flags", RG_TARGET_NAMESPACE);
    G_DEF_CONSTANTS(RG_TARGET_NAMESPACE, GTK_TYPE_RECENT_FILTER_FLAGS, "GTK_RECENT_FILTER_");
#endif
}
