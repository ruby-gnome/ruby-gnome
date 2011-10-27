/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2008-2009  Ruby-GNOME2 Project Team
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

#include "gio2.h"

#define RG_TARGET_NAMESPACE mContentType

static VALUE
contenttype_equals(G_GNUC_UNUSED VALUE self, VALUE arg1, VALUE arg2)
{
        return CBOOL2RVAL(g_content_type_equals(RVAL2CSTR(arg1), RVAL2CSTR(arg2)));
}

static VALUE
contenttype_is_a(G_GNUC_UNUSED VALUE self, VALUE arg1, VALUE arg2)
{
        return CBOOL2RVAL(g_content_type_is_a(RVAL2CSTR(arg1), RVAL2CSTR(arg2)));
}

static VALUE
contenttype_is_unknown(G_GNUC_UNUSED VALUE type)
{
        return CBOOL2RVAL(g_content_type_is_unknown(RVAL2CSTR(type)));
}

static VALUE
contenttype_get_description(G_GNUC_UNUSED VALUE type)
{
        return CSTR2RVAL_FREE(g_content_type_get_description(RVAL2CSTR(type)));
}

static VALUE
contenttype_get_mime_type(G_GNUC_UNUSED VALUE type)
{
        return CSTR2RVAL_FREE(g_content_type_get_mime_type(RVAL2CSTR(type)));
}

static VALUE
contenttype_get_icon(G_GNUC_UNUSED VALUE type)
{
        return GOBJ2RVAL_UNREF(g_content_type_get_icon(RVAL2CSTR(type)));
}

static VALUE
contenttype_can_be_executable(G_GNUC_UNUSED VALUE type)
{
        return CBOOL2RVAL(g_content_type_can_be_executable(RVAL2CSTR(type)));
}

static VALUE
contenttype_from_mime_type(G_GNUC_UNUSED VALUE type)
{
        return CSTR2RVAL(g_content_type_from_mime_type(RVAL2CSTR(type)));
}

static VALUE
contenttype_guess(int argc, VALUE *argv, G_GNUC_UNUSED VALUE self)
{
        VALUE rbfilename,
              rbdata;
        const char *filename;
        const guchar *data;
        gboolean result_uncertain;
        char *type;

        rb_scan_args(argc, argv, "02", &rbfilename, &rbdata);

        if (NIL_P(rbfilename) && NIL_P(rbdata))
                rb_raise(rb_eArgError,
                         "Either filename or data can be nil but not both");

        filename = RVAL2CSTR_ACCEPT_NIL(rbfilename);
        data = (guchar *)RVAL2CSTR_ACCEPT_NIL(rbdata);

        type = g_content_type_guess(filename,
                                    data,
                                    (gsize)((data != NULL) ? RSTRING_LEN(rbdata) : 0),
                                    &result_uncertain);

        return rb_assoc_new(CSTR2RVAL_FREE(type), CBOOL2RVAL(result_uncertain));
}

static VALUE
contenttype_guess_for_tree(G_GNUC_UNUSED VALUE self, VALUE root)
{
        return STRV2RVAL_FREE(g_content_type_guess_for_tree(RVAL2GFILE(root)));
}

static VALUE
contenttype_get_registered(G_GNUC_UNUSED VALUE self)
{
        return GLIST2ARY_STR_FREE(g_content_types_get_registered());
}

void
Init_gcontenttype(VALUE glib)
{
        VALUE RG_TARGET_NAMESPACE = rb_define_module_under(glib, "ContentType");

        /* TODO: Should wrap this in a class. */
        rb_define_module_function(RG_TARGET_NAMESPACE, "equals?", contenttype_equals, 2);
        /* TODO: This name isn't great. */
        rb_define_module_function(RG_TARGET_NAMESPACE, "is_a?", contenttype_is_a, 2);
        rb_define_module_function(RG_TARGET_NAMESPACE, "unknown?", contenttype_is_unknown, 1);
        rb_define_module_function(RG_TARGET_NAMESPACE, "get_description", contenttype_get_description, 1);
        rb_define_module_function(RG_TARGET_NAMESPACE, "get_mime_type", contenttype_get_mime_type, 1);
        rb_define_module_function(RG_TARGET_NAMESPACE, "get_icon", contenttype_get_icon, 1);
        rb_define_module_function(RG_TARGET_NAMESPACE, "can_be_executable?", contenttype_can_be_executable, 1);
        rb_define_module_function(RG_TARGET_NAMESPACE, "from_mime_type", contenttype_from_mime_type, 1);
        rb_define_module_function(RG_TARGET_NAMESPACE, "guess", contenttype_guess, -1);
        rb_define_module_function(RG_TARGET_NAMESPACE, "guess_for_tree", contenttype_guess_for_tree, 1);
        rb_define_module_function(RG_TARGET_NAMESPACE, "registered", contenttype_get_registered, 0);
}
