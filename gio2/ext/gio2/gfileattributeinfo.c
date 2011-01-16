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

#define RVAL2GFILEATTRIBUTEINFO(object) \
        ((GFileAttributeInfo *)(RVAL2BOXED(object, G_TYPE_FILE_ATTRIBUTE_INFO)))

#define _SELF(value) RVAL2GFILEATTRIBUTEINFO(value)

#define GFILEATTRIBUTEINFOFLAGS2RVAL(value) \
        GFLAGS2RVAL((value), G_TYPE_FILE_ATTRIBUTE_INFO_FLAGS)

static VALUE
fileattributeinfo_name(VALUE self)
{
        return CSTR2RVAL(_SELF(self)->name);
}

static VALUE
fileattributeinfo_type(VALUE self)
{
        return GFILEATTRIBUTETYPE2RVAL(_SELF(self)->type);
}

static VALUE
fileattributeinfo_flags(VALUE self)
{
        return GFILEATTRIBUTEINFOFLAGS2RVAL(_SELF(self)->flags);
}

void
Init_gfileattributeinfo(VALUE glib)
{
        VALUE fileattributeinfo;

        fileattributeinfo = G_DEF_CLASS(G_TYPE_FILE_ATTRIBUTE_INFO, "FileAttributeInfo", glib);

        G_DEF_CLASS(G_TYPE_FILE_ATTRIBUTE_INFO_FLAGS, "Flags", fileattributeinfo);
        G_DEF_CONSTANTS(fileattributeinfo, G_TYPE_FILE_ATTRIBUTE_INFO_FLAGS, "G_FILE_ATTRIBUTE_INFO_");

        rbgobj_boxed_not_copy_obj(G_TYPE_FILE_ATTRIBUTE_INFO);

        rb_define_method(fileattributeinfo, "name", fileattributeinfo_name, 0);
        rb_define_method(fileattributeinfo, "type", fileattributeinfo_type, 0);
        rb_define_method(fileattributeinfo, "flags", fileattributeinfo_flags, 0);
}
