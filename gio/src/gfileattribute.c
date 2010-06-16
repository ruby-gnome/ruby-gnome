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
fileattributeinfo_free(UNUSED(const GFileAttributeInfo *info))
{
        return;
}

static GType
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

#define G_TYPE_FILE_ATTRIBUTE_INFO (g_file_attribute_info_get_type())

#define GFILEATTRIBUTEINFO2RVAL(object) \
        BOXED2RVAL(object, G_TYPE_FILE_ATTRIBUTE_INFO)

#define _SELF(value) RVAL2GFILEATTRIBUTEINFO(value)

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

#undef _SELF

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
        /* TODO: How do we deal with this? */
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

/* TODO: How do we name these? */
void
Init_gfileattribute(VALUE glib)
{
        VALUE fileattribute,
              fileattributeinfo,
              fileattributeinfolist;

        fileattribute = rb_define_module_under(glib, "FileAttribute");

        rb_define_const(fileattribute, "STANDARD_TYPE", CSTR2RVAL(G_FILE_ATTRIBUTE_STANDARD_TYPE));
        rb_define_const(fileattribute, "STANDARD_IS_HIDDEN", CSTR2RVAL(G_FILE_ATTRIBUTE_STANDARD_IS_HIDDEN));
        rb_define_const(fileattribute, "STANDARD_IS_BACKUP", CSTR2RVAL(G_FILE_ATTRIBUTE_STANDARD_IS_BACKUP));
        rb_define_const(fileattribute, "STANDARD_IS_SYMLINK", CSTR2RVAL(G_FILE_ATTRIBUTE_STANDARD_IS_SYMLINK));
        rb_define_const(fileattribute, "STANDARD_IS_VIRTUAL", CSTR2RVAL(G_FILE_ATTRIBUTE_STANDARD_IS_VIRTUAL));
        rb_define_const(fileattribute, "STANDARD_NAME", CSTR2RVAL(G_FILE_ATTRIBUTE_STANDARD_NAME));
        rb_define_const(fileattribute, "STANDARD_DISPLAY_NAME", CSTR2RVAL(G_FILE_ATTRIBUTE_STANDARD_DISPLAY_NAME));
        rb_define_const(fileattribute, "STANDARD_EDIT_NAME", CSTR2RVAL(G_FILE_ATTRIBUTE_STANDARD_EDIT_NAME));
        rb_define_const(fileattribute, "STANDARD_COPY_NAME", CSTR2RVAL(G_FILE_ATTRIBUTE_STANDARD_COPY_NAME));
        rb_define_const(fileattribute, "STANDARD_DESCRIPTION", CSTR2RVAL(G_FILE_ATTRIBUTE_STANDARD_DESCRIPTION));
        rb_define_const(fileattribute, "STANDARD_ICON", CSTR2RVAL(G_FILE_ATTRIBUTE_STANDARD_ICON));
        rb_define_const(fileattribute, "STANDARD_CONTENT_TYPE", CSTR2RVAL(G_FILE_ATTRIBUTE_STANDARD_CONTENT_TYPE));
        rb_define_const(fileattribute, "STANDARD_FAST_CONTENT_TYPE", CSTR2RVAL(G_FILE_ATTRIBUTE_STANDARD_FAST_CONTENT_TYPE));
        rb_define_const(fileattribute, "STANDARD_SIZE", CSTR2RVAL(G_FILE_ATTRIBUTE_STANDARD_SIZE));
        rb_define_const(fileattribute, "STANDARD_ALLOCATED_SIZE", CSTR2RVAL(G_FILE_ATTRIBUTE_STANDARD_ALLOCATED_SIZE));
        rb_define_const(fileattribute, "STANDARD_SYMLINK_TARGET", CSTR2RVAL(G_FILE_ATTRIBUTE_STANDARD_SYMLINK_TARGET));
        rb_define_const(fileattribute, "STANDARD_TARGET_URI", CSTR2RVAL(G_FILE_ATTRIBUTE_STANDARD_TARGET_URI));
        rb_define_const(fileattribute, "STANDARD_SORT_ORDER", CSTR2RVAL(G_FILE_ATTRIBUTE_STANDARD_SORT_ORDER));
        rb_define_const(fileattribute, "ETAG_VALUE", CSTR2RVAL(G_FILE_ATTRIBUTE_ETAG_VALUE));
        rb_define_const(fileattribute, "ID_FILE", CSTR2RVAL(G_FILE_ATTRIBUTE_ID_FILE));
        rb_define_const(fileattribute, "ID_FILESYSTEM", CSTR2RVAL(G_FILE_ATTRIBUTE_ID_FILESYSTEM));
        rb_define_const(fileattribute, "ACCESS_CAN_READ", CSTR2RVAL(G_FILE_ATTRIBUTE_ACCESS_CAN_READ));
        rb_define_const(fileattribute, "ACCESS_CAN_WRITE", CSTR2RVAL(G_FILE_ATTRIBUTE_ACCESS_CAN_WRITE));
        rb_define_const(fileattribute, "ACCESS_CAN_EXECUTE", CSTR2RVAL(G_FILE_ATTRIBUTE_ACCESS_CAN_EXECUTE));
        rb_define_const(fileattribute, "ACCESS_CAN_DELETE", CSTR2RVAL(G_FILE_ATTRIBUTE_ACCESS_CAN_DELETE));
        rb_define_const(fileattribute, "ACCESS_CAN_TRASH", CSTR2RVAL(G_FILE_ATTRIBUTE_ACCESS_CAN_TRASH));
        rb_define_const(fileattribute, "ACCESS_CAN_RENAME", CSTR2RVAL(G_FILE_ATTRIBUTE_ACCESS_CAN_RENAME));
        rb_define_const(fileattribute, "MOUNTABLE_CAN_MOUNT", CSTR2RVAL(G_FILE_ATTRIBUTE_MOUNTABLE_CAN_MOUNT));
        rb_define_const(fileattribute, "MOUNTABLE_CAN_UNMOUNT", CSTR2RVAL(G_FILE_ATTRIBUTE_MOUNTABLE_CAN_UNMOUNT));
        rb_define_const(fileattribute, "MOUNTABLE_CAN_EJECT", CSTR2RVAL(G_FILE_ATTRIBUTE_MOUNTABLE_CAN_EJECT));
        rb_define_const(fileattribute, "MOUNTABLE_UNIX_DEVICE", CSTR2RVAL(G_FILE_ATTRIBUTE_MOUNTABLE_UNIX_DEVICE));
        rb_define_const(fileattribute, "MOUNTABLE_HAL_UDI", CSTR2RVAL(G_FILE_ATTRIBUTE_MOUNTABLE_HAL_UDI));
        rb_define_const(fileattribute, "TIME_MODIFIED", CSTR2RVAL(G_FILE_ATTRIBUTE_TIME_MODIFIED));
        rb_define_const(fileattribute, "TIME_MODIFIED_USEC", CSTR2RVAL(G_FILE_ATTRIBUTE_TIME_MODIFIED_USEC));
        rb_define_const(fileattribute, "TIME_ACCESS", CSTR2RVAL(G_FILE_ATTRIBUTE_TIME_ACCESS));
        rb_define_const(fileattribute, "TIME_ACCESS_USEC", CSTR2RVAL(G_FILE_ATTRIBUTE_TIME_ACCESS_USEC));
        rb_define_const(fileattribute, "TIME_CHANGED", CSTR2RVAL(G_FILE_ATTRIBUTE_TIME_CHANGED));
        rb_define_const(fileattribute, "TIME_CHANGED_USEC", CSTR2RVAL(G_FILE_ATTRIBUTE_TIME_CHANGED_USEC));
        rb_define_const(fileattribute, "TIME_CREATED", CSTR2RVAL(G_FILE_ATTRIBUTE_TIME_CREATED));
        rb_define_const(fileattribute, "TIME_CREATED_USEC", CSTR2RVAL(G_FILE_ATTRIBUTE_TIME_CREATED_USEC));
        rb_define_const(fileattribute, "UNIX_DEVICE", CSTR2RVAL(G_FILE_ATTRIBUTE_UNIX_DEVICE));
        rb_define_const(fileattribute, "UNIX_INODE", CSTR2RVAL(G_FILE_ATTRIBUTE_UNIX_INODE));
        rb_define_const(fileattribute, "UNIX_MODE", CSTR2RVAL(G_FILE_ATTRIBUTE_UNIX_MODE));
        rb_define_const(fileattribute, "UNIX_NLINK", CSTR2RVAL(G_FILE_ATTRIBUTE_UNIX_NLINK));
        rb_define_const(fileattribute, "UNIX_UID", CSTR2RVAL(G_FILE_ATTRIBUTE_UNIX_UID));
        rb_define_const(fileattribute, "UNIX_GID", CSTR2RVAL(G_FILE_ATTRIBUTE_UNIX_GID));
        rb_define_const(fileattribute, "UNIX_RDEV", CSTR2RVAL(G_FILE_ATTRIBUTE_UNIX_RDEV));
        rb_define_const(fileattribute, "UNIX_BLOCK_SIZE", CSTR2RVAL(G_FILE_ATTRIBUTE_UNIX_BLOCK_SIZE));
        rb_define_const(fileattribute, "UNIX_BLOCKS", CSTR2RVAL(G_FILE_ATTRIBUTE_UNIX_BLOCKS));
        rb_define_const(fileattribute, "UNIX_IS_MOUNTPOINT", CSTR2RVAL(G_FILE_ATTRIBUTE_UNIX_IS_MOUNTPOINT));
        rb_define_const(fileattribute, "DOS_IS_ARCHIVE", CSTR2RVAL(G_FILE_ATTRIBUTE_DOS_IS_ARCHIVE));
        rb_define_const(fileattribute, "DOS_IS_SYSTEM", CSTR2RVAL(G_FILE_ATTRIBUTE_DOS_IS_SYSTEM));
        rb_define_const(fileattribute, "OWNER_USER", CSTR2RVAL(G_FILE_ATTRIBUTE_OWNER_USER));
        rb_define_const(fileattribute, "OWNER_USER_REAL", CSTR2RVAL(G_FILE_ATTRIBUTE_OWNER_USER_REAL));
        rb_define_const(fileattribute, "OWNER_GROUP", CSTR2RVAL(G_FILE_ATTRIBUTE_OWNER_GROUP));
        rb_define_const(fileattribute, "THUMBNAIL_PATH", CSTR2RVAL(G_FILE_ATTRIBUTE_THUMBNAIL_PATH));
        rb_define_const(fileattribute, "THUMBNAILING_FAILED", CSTR2RVAL(G_FILE_ATTRIBUTE_THUMBNAILING_FAILED));
        rb_define_const(fileattribute, "PREVIEW_ICON", CSTR2RVAL(G_FILE_ATTRIBUTE_PREVIEW_ICON));
        rb_define_const(fileattribute, "FILESYSTEM_SIZE", CSTR2RVAL(G_FILE_ATTRIBUTE_FILESYSTEM_SIZE));
        rb_define_const(fileattribute, "FILESYSTEM_FREE", CSTR2RVAL(G_FILE_ATTRIBUTE_FILESYSTEM_FREE));
        rb_define_const(fileattribute, "FILESYSTEM_TYPE", CSTR2RVAL(G_FILE_ATTRIBUTE_FILESYSTEM_TYPE));
        rb_define_const(fileattribute, "FILESYSTEM_READONLY", CSTR2RVAL(G_FILE_ATTRIBUTE_FILESYSTEM_READONLY));
        rb_define_const(fileattribute, "FILESYSTEM_USE_PREVIEW", CSTR2RVAL(G_FILE_ATTRIBUTE_FILESYSTEM_USE_PREVIEW));
        rb_define_const(fileattribute, "GVFS_BACKEND", CSTR2RVAL(G_FILE_ATTRIBUTE_GVFS_BACKEND));
        rb_define_const(fileattribute, "SELINUX_CONTEXT", CSTR2RVAL(G_FILE_ATTRIBUTE_SELINUX_CONTEXT));
        rb_define_const(fileattribute, "TRASH_ITEM_COUNT", CSTR2RVAL(G_FILE_ATTRIBUTE_TRASH_ITEM_COUNT));

        G_DEF_CLASS(G_TYPE_FILE_ATTRIBUTE_TYPE, "Type", fileattribute);
	G_DEF_CONSTANTS(fileattribute, G_TYPE_FILE_ATTRIBUTE_TYPE, "G_FILE_ATTRIBUTE_");

        G_DEF_CLASS(G_TYPE_FILE_ATTRIBUTE_INFO_FLAGS, "InfoFlags", fileattribute);
	G_DEF_CONSTANTS(fileattribute, G_TYPE_FILE_ATTRIBUTE_INFO_FLAGS, "G_FILE_ATTRIBUTE_");

        G_DEF_CLASS(G_TYPE_FILE_ATTRIBUTE_STATUS, "Status", fileattribute);
	G_DEF_CONSTANTS(fileattribute, G_TYPE_FILE_ATTRIBUTE_STATUS, "G_FILE_ATTRIBUTE_");

        fileattributeinfo = G_DEF_CLASS(G_TYPE_FILE_ATTRIBUTE_INFO, "Info", fileattribute);

        rb_undef_alloc_func(fileattributeinfo);

        rb_define_method(fileattributeinfo, "name", fileattributeinfo_name, 0);
        rb_define_method(fileattributeinfo, "type", fileattributeinfo_type, 0);
        rb_define_method(fileattributeinfo, "flags", fileattributeinfo_flags, 0);

        fileattributeinfolist = G_DEF_CLASS(G_TYPE_FILE_ATTRIBUTE_INFO_LIST, "InfoList", fileattributeinfo);

        rb_define_method(fileattributeinfolist, "initialize", fileattributeinfolist_initialize, 0);
        rb_define_method(fileattributeinfolist, "dup", fileattributeinfolist_dup, 0);
        rb_define_method(fileattributeinfolist, "lookup", fileattributeinfolist_lookup, 1);
        rb_define_alias(fileattributeinfolist, "[]", "lookup");
        rb_define_method(fileattributeinfolist, "add", fileattributeinfolist_add, 3);
        rb_define_method(fileattributeinfolist, "each", fileattributeinfolist_each, 0);
        rb_include_module(fileattributeinfolist, rb_mEnumerable);
}
