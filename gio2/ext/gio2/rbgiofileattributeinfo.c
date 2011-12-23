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

#define RG_TARGET_NAMESPACE cFileAttributeInfo

static GFileAttributeInfo *
fileattributeinfo_copy(const GFileAttributeInfo *info)
{
        return (GFileAttributeInfo *)info;
}

static void
fileattributeinfo_free(G_GNUC_UNUSED GFileAttributeInfo *info)
{
        return;
}

GType
g_file_attribute_info_get_type(void)
{
        static GType our_type = 0;

        if (our_type == 0)
                our_type = g_boxed_type_register_static("GFileAttributeInfo",
                                                        (GBoxedCopyFunc)fileattributeinfo_copy,
                                                        (GBoxedFreeFunc)fileattributeinfo_free);

        return our_type;
}

#define _SELF(value) RVAL2GFILEATTRIBUTEINFO(value)

static VALUE
rg_name(VALUE self)
{
        return CSTR2RVAL(_SELF(self)->name);
}

static VALUE
rg_type(VALUE self)
{
        return GFILEATTRIBUTETYPE2RVAL(_SELF(self)->type);
}

static VALUE
rg_flags(VALUE self)
{
        return GFILEATTRIBUTEINFOFLAGS2RVAL(_SELF(self)->flags);
}

void
Init_gfileattributeinfo(VALUE mGio)
{
        VALUE RG_TARGET_NAMESPACE;

        RG_TARGET_NAMESPACE = G_DEF_CLASS(G_TYPE_FILE_ATTRIBUTE_INFO, "FileAttributeInfo", mGio);

        G_DEF_CLASS(G_TYPE_FILE_ATTRIBUTE_INFO_FLAGS, "Flags", RG_TARGET_NAMESPACE);
        G_DEF_CONSTANTS(RG_TARGET_NAMESPACE, G_TYPE_FILE_ATTRIBUTE_INFO_FLAGS, "G_FILE_ATTRIBUTE_INFO_");

        rbgobj_boxed_not_copy_obj(G_TYPE_FILE_ATTRIBUTE_INFO);

        RG_DEF_METHOD(name, 0);
        RG_DEF_METHOD(type, 0);
        RG_DEF_METHOD(flags, 0);
}
