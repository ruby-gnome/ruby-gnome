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

#define _SELF(value) RVAL2GFILEATTRIBUTEMATCHER(value)

static VALUE
fileattributematcher_initialize(VALUE self, VALUE attributes)
{
        G_INITIALIZE(self, g_file_attribute_matcher_new(RVAL2CSTR(attributes)));

        return Qnil;
}

static VALUE
fileattributematcher_matches(VALUE self, VALUE attribute)
{
        return CBOOL2RVAL(g_file_attribute_matcher_matches(_SELF(self), RVAL2CSTR(attribute)));
}

static VALUE
fileattributematcher_matches_only(VALUE self, VALUE attribute)
{
        return CBOOL2RVAL(g_file_attribute_matcher_matches_only(_SELF(self), RVAL2CSTR(attribute)));
}

static VALUE
fileattributematcher_enumerate_namespace(VALUE self, VALUE ns)
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
fileattributematcher_enumerate_next(VALUE self)
{
        return CSTR2RVAL(g_file_attribute_matcher_enumerate_next(_SELF(self)));
}

void
Init_gfileattributematcher(VALUE glib)
{
        /* TODO: Should be defined under G::FileAttribute as Matcher? */
        VALUE fileattributematcher = G_DEF_CLASS(G_TYPE_FILE_ATTRIBUTE_MATCHER, "FileAttributeMatcher", glib);

        rb_define_method(fileattributematcher, "initialize", fileattributematcher_initialize, 1);
        rb_define_method(fileattributematcher, "matches?", fileattributematcher_matches, 1);
        /* TODO: Is this confusing when we have both #matches and
         * #matches_only?  What does #=~ call? */
        rb_define_alias(fileattributematcher, "=~", "matches?");
        rb_define_method(fileattributematcher, "matches_only?", fileattributematcher_matches_only, 1);
        rb_define_method(fileattributematcher, "enumerate_namespace", fileattributematcher_enumerate_namespace, 1);
        rb_define_method(fileattributematcher, "enumerate_next", fileattributematcher_enumerate_next, 0);
}
