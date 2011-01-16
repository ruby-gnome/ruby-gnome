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

#define RVAL2GFILEATTRIBUTEINFOFLAGS(value) \
        RVAL2GFLAGS((value), G_TYPE_FILE_ATTRIBUTE_INFO_FLAGS)

#define RVAL2GFILEATTRIBUTEINFOFLAGSDEFAULT(value) \
        RVAL2TYPE_WITH_DEFAULT((value), \
                               RVAL2GFILEATTRIBUTEINFOFLAGS, \
                               G_FILE_ATTRIBUTE_INFO_NONE)

GType
g_file_attribute_info_list_get_type(void)
{
        static GType our_type = 0;
        if (our_type == 0)
                our_type = g_boxed_type_register_static("GFileAttributeInfoList",
                                                        (GBoxedCopyFunc)g_file_attribute_info_list_ref,
                                                        (GBoxedFreeFunc)g_file_attribute_info_list_unref);
        return our_type;
}

#define RVAL2GFILEATTRIBUTEINFOLIST(object) \
        ((GFileAttributeInfoList *)(RVAL2BOXED(object, G_TYPE_FILE_ATTRIBUTE_INFO_LIST)))

#define _SELF(value) RVAL2GFILEATTRIBUTEINFOLIST(value)

static VALUE
fileattributeinfolist_initialize(VALUE self)
{
        G_INITIALIZE(self, g_file_attribute_info_list_new());

        return Qnil;
}

static VALUE
fileattributeinfolist_dup(VALUE self)
{
        return GFILEATTRIBUTEINFOLIST2RVAL(g_file_attribute_info_list_dup(_SELF(self)));
}

static VALUE
fileattributeinfolist_lookup(VALUE self, VALUE name)
{
        /* TODO: How do we deal with the const? */
        return GFILEATTRIBUTEINFO2RVAL((GFileAttributeInfo *)g_file_attribute_info_list_lookup(_SELF(self),
                                                                                               RVAL2CSTR(name)));
}

static VALUE
fileattributeinfolist_add(int argc, VALUE *argv, VALUE self)
{
        VALUE name, type, flags;

        rb_scan_args(argc, argv, "21", &name, &type, &flags);

        g_file_attribute_info_list_add(_SELF(self),
                                       RVAL2CSTR(name),
                                       RVAL2GFILEATTRIBUTETYPE(type),
                                       RVAL2GFILEATTRIBUTEINFOFLAGSDEFAULT(flags));

        return self;
}

static VALUE
fileattributeinfolist_each(VALUE self)
{
        GFileAttributeInfoList *list = RVAL2GFILEATTRIBUTEINFOLIST(self);
        int i;

        for (i = 0; i < list->n_infos; i++)
                rb_yield(GFILEATTRIBUTEINFO2RVAL(&list->infos[i]));

        return self;
}

void
Init_fileattributeinfolist(VALUE glib)
{
        VALUE fileattributeinfolist = G_DEF_CLASS(G_TYPE_FILE_ATTRIBUTE_INFO_LIST, "FileAttributeInfoList", glib);

        rb_include_module(fileattributeinfolist, rb_mEnumerable);

        rb_define_method(fileattributeinfolist, "initialize", fileattributeinfolist_initialize, 0);
        rb_define_method(fileattributeinfolist, "dup", fileattributeinfolist_dup, 0);
        rb_define_method(fileattributeinfolist, "lookup", fileattributeinfolist_lookup, 1);
        rb_define_alias(fileattributeinfolist, "[]", "lookup");
        rb_define_method(fileattributeinfolist, "add", fileattributeinfolist_add, 3);
        rb_define_method(fileattributeinfolist, "each", fileattributeinfolist_each, 0);
}
