/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2008  Ruby-GNOME2 Project Team
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
#include <glib-enum-types.h> /* From Ruby/GLib2 */

#if GTK_CHECK_VERSION(2, 12, 0)

#define RG_TARGET_NAMESPACE cBuilder
#define _SELF(self) (GTK_BUILDER(RVAL2GOBJ(self)))

static ID id___connect_signals__;

static void
builder_mark(gpointer object)
{
    GtkBuilder *builder = object;
    GSList *objects;

    objects = gtk_builder_get_objects(builder);
    g_slist_foreach(objects, (GFunc)rbgobj_gc_mark_instance, NULL);
    g_slist_free(objects);
}

static VALUE
rg_initialize(VALUE self)
{
    GtkBuilder *builder;

    builder = gtk_builder_new();
    G_INITIALIZE(self, builder);

    return Qnil;
}

static VALUE
rg_add_from_file(VALUE self, VALUE filename)
{
    GError *error = NULL;

    if (gtk_builder_add_from_file(_SELF(self),
                                  RVAL2CSTR(filename),
                                  &error) == 0)
        RAISE_GERROR(error);

    return self;
}

static VALUE
rg_add_from_string(VALUE self, VALUE string)
{
    GError *error = NULL;

    StringValue(string);
    if (gtk_builder_add_from_string(_SELF(self),
                                    RSTRING_PTR(string),
                                    RSTRING_LEN(string),
                                    &error) == 0)
        RAISE_GERROR(error);

    return self;
}

static VALUE
rg_add(VALUE self, VALUE filename_or_xml)
{
    const char xml_detect_re_str[] = "(?:\\A<|[\\r\\n])";
    VALUE xml_detect_re;

    xml_detect_re = rb_reg_new(xml_detect_re_str, strlen(xml_detect_re_str), 0);
    if (NIL_P(rb_reg_match(xml_detect_re, filename_or_xml)))
        return rg_add_from_file(self, filename_or_xml);
    else
        return rg_add_from_string(self, filename_or_xml);
}

static VALUE
rg_get_object(VALUE self, VALUE name)
{
    return GOBJ2RVAL(gtk_builder_get_object(_SELF(self), RVAL2CSTR(name)));
}

static VALUE
rg_objects(VALUE self)
{
    return GSLIST2ARY_FREE(gtk_builder_get_objects(_SELF(self)));
}

static void
connect_signals(GtkBuilder *builder, GObject *object, const gchar *signal_name,
                const gchar *handler_name, GObject *connect_object,
                GConnectFlags flags, gpointer user_data)
{
    VALUE signal_connector = (VALUE)user_data;

    rb_funcall(GOBJ2RVAL(builder), id___connect_signals__, 6,
               signal_connector, GOBJ2RVAL(object), CSTR2RVAL(signal_name),
               CSTR2RVAL(handler_name), GOBJ2RVAL(connect_object),
               GFLAGS2RVAL(flags, G_TYPE_CONNECT_FLAGS));
}

static VALUE
rg_connect_signals(VALUE self)
{
    VALUE signal_connector;

    signal_connector = rb_block_proc();
    G_CHILD_ADD(self, signal_connector);
    gtk_builder_connect_signals_full(_SELF(self), connect_signals,
                                     (gpointer)signal_connector);
    G_CHILD_REMOVE(self, signal_connector);

    return self;
}

static VALUE
rg_get_type(VALUE self, VALUE name)
{
    return GTYPE2CLASS(gtk_builder_get_type_from_name(_SELF(self),
                                                      RVAL2CSTR(name)));
}
#endif

void
Init_gtk_builder(VALUE mGtk)
{
#if GTK_CHECK_VERSION(2, 12, 0)
    VALUE RG_TARGET_NAMESPACE;

    id___connect_signals__ = rb_intern("__connect_signals__");

    RG_TARGET_NAMESPACE = G_DEF_CLASS_WITH_GC_FUNC(GTK_TYPE_BUILDER, "Builder", mGtk,
                                                    builder_mark, NULL);

    G_DEF_CLASS(GTK_TYPE_BUILDER_ERROR, "BuilderError", mGtk);

    RG_DEF_METHOD(initialize, 0);

    RG_DEF_METHOD(add_from_file, 1);
    RG_DEF_METHOD(add_from_string, 1);
    RG_DEF_METHOD(add, 1);
    RG_DEF_ALIAS("<<", "add");

    RG_DEF_METHOD(get_object, 1);
    RG_DEF_ALIAS("[]", "get_object");
    RG_DEF_METHOD(objects, 0);

    RG_DEF_METHOD(connect_signals, 0);

    RG_DEF_METHOD(get_type, 1);
#endif
}
