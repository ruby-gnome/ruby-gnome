/* -*- c-file-style: "ruby" -*- */
/*
 * Ruby/GIO: a Ruby binding of gio-2.0.x.
 * Copyright (C) 2008-2009  Ruby-GNOME2 Project Team
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "gio2.h"

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
        return STRVECTOR2ARY_FREE(g_content_type_guess_for_tree(RVAL2GFILE(root)));
}

static VALUE
contenttype_get_registered(G_GNUC_UNUSED VALUE self)
{
        return GLIST2ARY_STR_FREE(g_content_types_get_registered());
}

void
Init_gcontenttype(VALUE glib)
{
        VALUE contenttype = rb_define_module_under(glib, "ContentType");

        /* TODO: Should wrap this in a class. */
        rb_define_module_function(contenttype, "equals?", contenttype_equals, 2);
        /* TODO: This name isn’t great. */
        rb_define_module_function(contenttype, "is_a?", contenttype_is_a, 2);
        rb_define_module_function(contenttype, "unknown?", contenttype_is_unknown, 1);
        rb_define_module_function(contenttype, "get_description", contenttype_get_description, 1);
        rb_define_module_function(contenttype, "get_mime_type", contenttype_get_mime_type, 1);
        rb_define_module_function(contenttype, "get_icon", contenttype_get_icon, 1);
        rb_define_module_function(contenttype, "can_be_executable?", contenttype_can_be_executable, 1);
        rb_define_module_function(contenttype, "from_mime_type", contenttype_from_mime_type, 1);
        rb_define_module_function(contenttype, "guess", contenttype_guess, -1);
        rb_define_module_function(contenttype, "guess_for_tree", contenttype_guess_for_tree, 1);
        rb_define_module_function(contenttype, "registered", contenttype_get_registered, 0);
}
