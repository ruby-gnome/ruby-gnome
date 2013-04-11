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

#define RG_TARGET_NAMESPACE cFileAttributeMatcher

#if !GLIB_CHECK_VERSION(2, 34, 0)
GType
g_file_attribute_matcher_get_type(void)
{
        static GType our_type = 0;
        if (our_type == 0)
                our_type = g_boxed_type_register_static("GFileAttributeMatcher",
                                                        (GBoxedCopyFunc)g_file_attribute_matcher_ref,
                                                        (GBoxedFreeFunc)g_file_attribute_matcher_unref);
        return our_type;
}
#endif

#define _SELF(value) RVAL2GFILEATTRIBUTEMATCHER(value)

static VALUE
rg_initialize(VALUE self, VALUE attributes)
{
        G_INITIALIZE(self, g_file_attribute_matcher_new(RVAL2CSTR(attributes)));

        return Qnil;
}

static VALUE
rg_matches_p(VALUE self, VALUE attribute)
{
        return CBOOL2RVAL(g_file_attribute_matcher_matches(_SELF(self), RVAL2CSTR(attribute)));
}

static VALUE
rg_matches_only_p(VALUE self, VALUE attribute)
{
        return CBOOL2RVAL(g_file_attribute_matcher_matches_only(_SELF(self), RVAL2CSTR(attribute)));
}

static VALUE
rg_enumerate_namespace(VALUE self, VALUE ns)
{
        const char *match;
        gboolean matches_all = g_file_attribute_matcher_enumerate_namespace(_SELF(self),
                                                                            RVAL2CSTR(ns));
        if (!rb_block_given_p())
                return CBOOL2RVAL(matches_all);

        while ((match = g_file_attribute_matcher_enumerate_next(_SELF(self))) != NULL)
                rb_yield(CSTR2RVAL(match));

        return self;
}

static VALUE
rg_enumerate_next(VALUE self)
{
        return CSTR2RVAL(g_file_attribute_matcher_enumerate_next(_SELF(self)));
}

void
Init_gfileattributematcher(VALUE mGio)
{
        VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(G_TYPE_FILE_ATTRIBUTE_MATCHER, "FileAttributeMatcher", mGio);

        RG_DEF_METHOD(initialize, 1);
        RG_DEF_METHOD_P(matches, 1);
        /* TODO: Is this confusing when we have both #matches and
         * #matches_only?  What does #=~ call? */
        RG_DEF_ALIAS("=~", "matches?");
        RG_DEF_METHOD_P(matches_only, 1);
        RG_DEF_METHOD(enumerate_namespace, 1);
        RG_DEF_METHOD(enumerate_next, 0);
}
