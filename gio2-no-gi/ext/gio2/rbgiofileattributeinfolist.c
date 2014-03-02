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

#define RG_TARGET_NAMESPACE cFileAttributeInfoList

#define RVAL2GFILEATTRIBUTEINFOFLAGSDEFAULT(value) \
        RVAL2TYPE_WITH_DEFAULT((value), \
                               RVAL2GFILEATTRIBUTEINFOFLAGS, \
                               G_FILE_ATTRIBUTE_INFO_NONE)

#if !GLIB_CHECK_VERSION(2, 34, 0)
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
#endif

#define _SELF(value) RVAL2GFILEATTRIBUTEINFOLIST(value)

static VALUE
rg_initialize(VALUE self)
{
        G_INITIALIZE(self, g_file_attribute_info_list_new());

        return Qnil;
}

static VALUE
rg_dup(VALUE self)
{
        return GFILEATTRIBUTEINFOLIST2RVAL(g_file_attribute_info_list_dup(_SELF(self)));
}

static VALUE
rg_lookup(VALUE self, VALUE name)
{
        /* TODO: How do we deal with the const? */
        return GFILEATTRIBUTEINFO2RVAL((GFileAttributeInfo *)g_file_attribute_info_list_lookup(_SELF(self),
                                                                                               RVAL2CSTR(name)));
}

static VALUE
rg_add(int argc, VALUE *argv, VALUE self)
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
rg_each(VALUE self)
{
        GFileAttributeInfoList *list = RVAL2GFILEATTRIBUTEINFOLIST(self);
        int i;

        for (i = 0; i < list->n_infos; i++)
                rb_yield(GFILEATTRIBUTEINFO2RVAL(&list->infos[i]));

        return self;
}

void
Init_fileattributeinfolist(VALUE mGio)
{
        VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(G_TYPE_FILE_ATTRIBUTE_INFO_LIST, "FileAttributeInfoList", mGio);

        rb_include_module(RG_TARGET_NAMESPACE, rb_mEnumerable);

        RG_DEF_METHOD(initialize, 0);
        RG_DEF_METHOD(dup, 0);
        RG_DEF_METHOD(lookup, 1);
        RG_DEF_ALIAS("[]", "lookup");
        RG_DEF_METHOD(add, 3);
        RG_DEF_METHOD(each, 0);
}
