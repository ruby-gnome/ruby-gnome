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

#include "rbgio2private.h"

#define RG_TARGET_NAMESPACE mContentType

static VALUE
rg_m_equals_p(G_GNUC_UNUSED VALUE self, VALUE arg1, VALUE arg2)
{
        return CBOOL2RVAL(g_content_type_equals(RVAL2CSTR(arg1), RVAL2CSTR(arg2)));
}

static VALUE
rg_m_is_a_p(G_GNUC_UNUSED VALUE self, VALUE arg1, VALUE arg2)
{
        return CBOOL2RVAL(g_content_type_is_a(RVAL2CSTR(arg1), RVAL2CSTR(arg2)));
}

static VALUE
rg_m_unknown_p(G_GNUC_UNUSED VALUE type)
{
        return CBOOL2RVAL(g_content_type_is_unknown(RVAL2CSTR(type)));
}

static VALUE
rg_m_get_description(G_GNUC_UNUSED VALUE type)
{
        return CSTR2RVAL_FREE(g_content_type_get_description(RVAL2CSTR(type)));
}

static VALUE
rg_m_get_mime_type(G_GNUC_UNUSED VALUE type)
{
        return CSTR2RVAL_FREE(g_content_type_get_mime_type(RVAL2CSTR(type)));
}

static VALUE
rg_m_get_icon(G_GNUC_UNUSED VALUE type)
{
        return GOBJ2RVAL_UNREF(g_content_type_get_icon(RVAL2CSTR(type)));
}

static VALUE
rg_m_can_be_executable_p(G_GNUC_UNUSED VALUE type)
{
        return CBOOL2RVAL(g_content_type_can_be_executable(RVAL2CSTR(type)));
}

static VALUE
rg_m_from_mime_type(G_GNUC_UNUSED VALUE type)
{
        return CSTR2RVAL(g_content_type_from_mime_type(RVAL2CSTR(type)));
}

static VALUE
rg_m_guess(int argc, VALUE *argv, G_GNUC_UNUSED VALUE self)
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
rg_m_guess_for_tree(G_GNUC_UNUSED VALUE self, VALUE root)
{
        return STRV2RVAL_FREE(g_content_type_guess_for_tree(RVAL2GFILE(root)));
}

static VALUE
rg_m_registered(G_GNUC_UNUSED VALUE self)
{
        return GLIST2ARY_STR_FREE(g_content_types_get_registered());
}

void
Init_gcontenttype(VALUE mGio)
{
        VALUE RG_TARGET_NAMESPACE = rb_define_module_under(mGio, "ContentType");

        /* TODO: Should wrap this in a class. */
        RG_DEF_MODFUNC_P(equals, 2);
        /* TODO: This name isn't great. */
        RG_DEF_MODFUNC_P(is_a, 2);
        RG_DEF_MODFUNC_P(unknown, 1);
        RG_DEF_MODFUNC(get_description, 1);
        RG_DEF_MODFUNC(get_mime_type, 1);
        RG_DEF_MODFUNC(get_icon, 1);
        RG_DEF_MODFUNC_P(can_be_executable, 1);
        RG_DEF_MODFUNC(from_mime_type, 1);
        RG_DEF_MODFUNC(guess, -1);
        RG_DEF_MODFUNC(guess_for_tree, 1);
        RG_DEF_MODFUNC(registered, 0);
}
