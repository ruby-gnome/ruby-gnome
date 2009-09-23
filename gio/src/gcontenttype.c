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

#define DEFTWOSTRINGMETHOD(name) \
        DECLARE(name)(UNUSED(VALUE self), VALUE arg1, VALUE arg2) \
        { \
                return CBOOL2RVAL(g_content_type_##name(RVAL2CSTR(arg1), \
                                                        RVAL2CSTR(arg2))); \
        }

DEFTWOSTRINGMETHOD(equals)
DEFTWOSTRINGMETHOD(is_a)

#define DEFSIMPLEMETHOD(name, type) \
        DECLARE(name)(UNUSED(VALUE self), VALUE value) \
        { \
                return type(g_content_type_##name(RVAL2CSTR(value))); \
        }

DEFSIMPLEMETHOD(is_unknown, CBOOL2RVAL)
DEFSIMPLEMETHOD(get_description, CSTR2RVAL)
DEFSIMPLEMETHOD(get_mime_type, CSTR2RVAL)
DEFSIMPLEMETHOD(get_icon, GOBJ2RVAL)
DEFSIMPLEMETHOD(can_be_executable, CBOOL2RVAL)
DEFSIMPLEMETHOD(from_mime_type, CSTR2RVAL)

DECLARE(guess)(int argc, VALUE *argv, UNUSED(VALUE self))
{
        VALUE rbfilename,
              rbdata;
        const char *filename;
        const char *data;
        gsize data_size;
        gboolean result_uncertain;
        char *type;

        rb_scan_args(argc, argv, "02", &rbfilename, &rbdata);

        if (NIL_P(rbfilename) && NIL_P(rbdata))
                rb_raise(rb_eArgError,
                         "Either filename or data can be nil but not both");

        filename = RVAL2CSTR_ACCEPT_NIL(rbfilename);
        data = RVAL2CSTR_ACCEPT_NIL(rbdata);
        if (data != NULL) {
                StringValue(rbdata);
                data_size = RSTRING_LEN(rbdata);
        } else {
                data_size = 0;
        }

        type = g_content_type_guess(filename,
                                    data,
                                    data_size,
                                    &result_uncertain);

        return rb_assoc_new(CSTR2RVAL_FREE(type), CBOOL2RVAL(result_uncertain));
}

DECLARE(guess_for_tree)(UNUSED(VALUE self), VALUE root)
{
        return str_vector_to_ary(g_content_type_guess_for_tree(RVAL2GFILE(root)));
}

DECLARE(registered)(UNUSED(VALUE self))
{
        return GLIST2ARY_STR_FREE(g_content_types_get_registered());
}

void
Init_gcontenttype(VALUE glib)
{
        VALUE contenttype = rb_define_module_under(glib, "ContentType");

        DEF_SINGLETON_BOOL_METHOD(contenttype, equals, 2);
        DEF_SINGLETON_BOOL_METHOD(contenttype, is_a, 2);
        DEF_SINGLETON_IS_METHOD(contenttype, unknown, 1);
        DEF_SINGLETON_METHOD(contenttype, get_description, 1);
        DEF_SINGLETON_METHOD(contenttype, get_mime_type, 1);
        DEF_SINGLETON_METHOD(contenttype, get_icon, 1);
        DEF_SINGLETON_BOOL_METHOD(contenttype, can_be_executable, 1);
        DEF_SINGLETON_METHOD(contenttype, from_mime_type, 1);
        DEF_SINGLETON_METHOD(contenttype, guess, -1);
        DEF_SINGLETON_METHOD(contenttype, guess_for_tree, 1);
        DEF_SINGLETON_METHOD(contenttype, registered, 0);
}
