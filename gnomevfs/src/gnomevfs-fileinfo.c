/*****************************************************************************
 *
 * gnomevfs-fileinfo.c: GnomeVFS::FileInfo class.
 *
 * Copyright (C) 2003 Nikolai :: lone-star :: Weibull <lone-star@home.se>.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307  USA
 *
 * $Author: pcppopper $
 *
 * $Date: 2003/08/11 11:11:10 $
 *
 *****************************************************************************/

/* Includes ******************************************************************/

#include "gnomevfs.h"
#include <unistd.h>
#include <sys/types.h>

/* Defines *******************************************************************/

#define _SELF(s)	\
	((GnomeVFSFileInfo *)RVAL2BOXED(s, GNOMEVFS_TYPE_FILE_INFO))

/* Type Definitions **********************************************************/

/* Function Declarations *****************************************************/

/* Global Variables **********************************************************/

/* Function Implementations **************************************************/

static GnomeVFSFileInfo *
gnome_vfs_file_info_ref_fix(gpointer boxed)
{
	GnomeVFSFileInfo *info;

	info = (GnomeVFSFileInfo *)boxed;
	gnome_vfs_file_info_ref(info);
	return info;
}

GType
gnome_vfs_file_info_get_type(void)
{
	static GType our_type = 0;

	if (our_type == 0) {
		our_type = g_boxed_type_register_static("GnomeVFSFileInfo",
				(GBoxedCopyFunc)gnome_vfs_file_info_ref_fix,
				(GBoxedFreeFunc)gnome_vfs_file_info_unref);
	}

	return our_type;
}

static VALUE
fileinfo_initialize(self)
	VALUE self;
{
	G_INITIALIZE(self, gnome_vfs_file_info_new());
	return Qnil;
}

static VALUE
fileinfo_is_symlink(self)
	VALUE self;
{
	return CBOOL2RVAL(GNOME_VFS_FILE_INFO_SYMLINK(_SELF(self)));
}

static VALUE
fileinfo_set_symlink(self, value)
	VALUE self, value;
{
	GNOME_VFS_FILE_INFO_SET_SYMLINK(_SELF(self), RTEST(value));
	return self;
}

static VALUE
fileinfo_is_local(self)
	VALUE self;
{
	return CBOOL2RVAL(GNOME_VFS_FILE_INFO_LOCAL(_SELF(self)));
}

static VALUE
fileinfo_set_local(self, value)
	VALUE self, value;
{
	GNOME_VFS_FILE_INFO_SET_LOCAL(_SELF(self), RTEST(value));
	return self;
}

static VALUE
fileinfo_is_suid(self)
	VALUE self;
{
	return CBOOL2RVAL(GNOME_VFS_FILE_INFO_SUID(_SELF(self)));
}

static VALUE
fileinfo_set_suid(self, value)
	VALUE self, value;
{
	GNOME_VFS_FILE_INFO_SET_SUID(_SELF(self), RTEST(value));
	return self;
}

static VALUE
fileinfo_is_sgid(self)
	VALUE self;
{
	return CBOOL2RVAL(GNOME_VFS_FILE_INFO_SGID(_SELF(self)));
}

static VALUE
fileinfo_set_sgid(self, value)
	VALUE self, value;
{
	GNOME_VFS_FILE_INFO_SET_SGID(_SELF(self), RTEST(value));
	return self;
}

static VALUE
fileinfo_is_sticky(self)
	VALUE self;
{
	return CBOOL2RVAL(GNOME_VFS_FILE_INFO_STICKY(_SELF(self)));
}

static VALUE
fileinfo_set_sticky(self, value)
	VALUE self, value;
{
	GNOME_VFS_FILE_INFO_SET_STICKY(_SELF(self), RTEST(value));
	return self;
}

static VALUE
fileinfo_get_mime_type(self)
	VALUE self;
{
	return CSTR2RVAL(gnome_vfs_file_info_get_mime_type(_SELF(self)));
}

static VALUE
fileinfo_matches(self, other)
	VALUE self, other;
{
	return CBOOL2RVAL(gnome_vfs_file_info_matches(_SELF(self),
						      _SELF(other)));
}

static VALUE
fileinfo_name(self)
	VALUE self;
{
	return CSTR2RVAL(_SELF(self)->name);
}

static struct _FileTypeConversion
{
	GnomeVFSFileType type;
	char *string;
} s_ftc[] = {
	{ GNOME_VFS_FILE_TYPE_UNKNOWN, "unknown" },
	{ GNOME_VFS_FILE_TYPE_REGULAR, "file" },
	{ GNOME_VFS_FILE_TYPE_DIRECTORY, "directory" },
	{ GNOME_VFS_FILE_TYPE_FIFO, "fifo" },
	{ GNOME_VFS_FILE_TYPE_SOCKET, "socket" },
	{ GNOME_VFS_FILE_TYPE_CHARACTER_DEVICE, "characterSpecial" },
	{ GNOME_VFS_FILE_TYPE_BLOCK_DEVICE, "blockSpecial" },
	{ GNOME_VFS_FILE_TYPE_SYMBOLIC_LINK, "link" }
};

static VALUE
fileinfo_filetype(self)
	VALUE self;
{
	GnomeVFSFileType type;
	int i, n;

	type = _SELF(self)->type;
	for (i = 0, n = G_N_ELEMENTS(s_ftc); i < n; i++) {
		if (type == s_ftc[i].type) {
			return CSTR2RVAL(s_ftc[i].string);
		}
	}
	return CSTR2RVAL(s_ftc[0].string);
}

static VALUE
fileinfo_uid(self)
	VALUE self;
{
	return UINT2NUM(_SELF(self)->uid);
}

static VALUE
fileinfo_gid(self)
	VALUE self;
{
	return UINT2NUM(_SELF(self)->gid);
}

static VALUE
fileinfo_dev(self)
	VALUE self;
{
	return ULL2NUM(_SELF(self)->device);
}

static VALUE
fileinfo_inode(self)
	VALUE self;
{
	return ULL2NUM(_SELF(self)->inode);
}

static VALUE
fileinfo_size(self)
	VALUE self;
{
	return ULL2NUM(_SELF(self)->size);
}

static VALUE
fileinfo_has_size(self)
	VALUE self;
{
	if (_SELF(self)->size == 0) {
		return Qnil;
	} else {
		return ULL2NUM(_SELF(self)->size);
	}
}

static VALUE
fileinfo_zero(self)
	VALUE self;
{
	return CBOOL2RVAL(_SELF(self)->size == 0);
}

static VALUE
fileinfo_blocks(self)
	VALUE self;
{
	return ULL2NUM(_SELF(self)->block_count);
}

static VALUE
fileinfo_blksize(self)
	VALUE self;
{
	return UINT2NUM(_SELF(self)->io_block_size);
}

static VALUE
fileinfo_atime(self)
	VALUE self;
{
	return rb_time_new(_SELF(self)->atime, 0);
}

static VALUE
fileinfo_mtime(self)
	VALUE self;
{
	return rb_time_new(_SELF(self)->mtime, 0);
}

static VALUE
fileinfo_ctime(self)
	VALUE self;
{
	return rb_time_new(_SELF(self)->ctime, 0);
}

static VALUE
fileinfo_symlink_to(self)
	VALUE self;
{
	return CSTR2RVAL(_SELF(self)->symlink_name);
}

static VALUE
fileinfo_link_count(self)
	VALUE self;
{
	return UINT2NUM(_SELF(self)->link_count);
}

static VALUE
fileinfo_blockdev_p(self)
	VALUE self;
{
	return CBOOL2RVAL(
		_SELF(self)->type == GNOME_VFS_FILE_TYPE_BLOCK_DEVICE);
}

static VALUE
fileinfo_chardev_p(self)
	VALUE self;
{
	return CBOOL2RVAL(
		_SELF(self)->type == GNOME_VFS_FILE_TYPE_CHARACTER_DEVICE);
}

static VALUE
fileinfo_directory_p(self)
	VALUE self;
{
	return CBOOL2RVAL(_SELF(self)->type == GNOME_VFS_FILE_TYPE_DIRECTORY);
}

static VALUE
fileinfo_regular_p(self)
	VALUE self;
{
	return CBOOL2RVAL(_SELF(self)->type == GNOME_VFS_FILE_TYPE_REGULAR);
}

static VALUE
fileinfo_socket_p(self)
	VALUE self;
{
	return CBOOL2RVAL(_SELF(self)->type == GNOME_VFS_FILE_TYPE_SOCKET);
}

static VALUE
fileinfo_fifo_p(self)
	VALUE self;
{
	return CBOOL2RVAL(_SELF(self)->type == GNOME_VFS_FILE_TYPE_FIFO);
}

static VALUE
fileinfo_executable_real_p(self)
	VALUE self;
{
	return CBOOL2RVAL(_SELF(self)->permissions &
			  GNOME_VFS_PERM_ACCESS_EXECUTABLE);
}

static VALUE
fileinfo_readable_real_p(self)
	VALUE self;
{
	return CBOOL2RVAL(_SELF(self)->permissions &
			  GNOME_VFS_PERM_ACCESS_READABLE);
}

static VALUE
fileinfo_writable_real_p(self)
	VALUE self;
{
	return CBOOL2RVAL(_SELF(self)->permissions &
			  GNOME_VFS_PERM_ACCESS_WRITABLE);
}

static VALUE
fileinfo_grpowned_p(self)
	VALUE self;
{
	return CBOOL2RVAL(_SELF(self)->gid == getegid());
}

static VALUE
fileinfo_owned_p(self)
	VALUE self;
{
	return CBOOL2RVAL(_SELF(self)->uid == geteuid());
}

static VALUE
fileinfo_permissions(self)
	VALUE self;
{
	return INT2FIX(_SELF(self)->permissions);
}

static VALUE
fileinfo_set_name(self, name)
	VALUE self, name;
{
	char *str;

	str = RVAL2CSTR(name);
	g_free(_SELF(self)->name);
	_SELF(self)->name = g_strdup(str);
	return self;
}

static VALUE
fileinfo_set_permissions(self, mode)
	VALUE self, mode;
{
	_SELF(self)->permissions = FIX2INT(mode);
	return self;
}

static VALUE
fileinfo_set_uid(self, uid)
	VALUE self, uid;
{
	_SELF(self)->uid = FIX2INT(uid);
	return self;
}

static VALUE
fileinfo_set_gid(self, gid)
	VALUE self, gid;
{
	_SELF(self)->gid = FIX2INT(gid);
	return self;
}

static VALUE
fileinfo_set_owner(self, uid, gid)
	VALUE self, uid, gid;
{
	if (!NIL_P(uid)) {
		_SELF(self)->uid = FIX2INT(uid);
	}
	if (!NIL_P(gid)) {
		_SELF(self)->gid = FIX2INT(gid);
	}
	return self;
}

static VALUE
fileinfo_utime(self, atime, mtime)
	VALUE self, atime, mtime;
{
	VALUE secs;
	ID tv_sec;

	tv_sec = rb_intern("tv_sec");
	if (!NIL_P(atime)) {
		if (rb_respond_to(atime, tv_sec)) {
			secs = rb_funcall(atime, tv_sec, 0);
			_SELF(self)->atime = NUM2INT(secs);
		} else {
			rb_raise(rb_eArgError, "couldn't get time of object");
		}
	}
	if (!NIL_P(mtime)) {
		if (rb_respond_to(mtime, tv_sec)) {
			secs = rb_funcall(mtime, tv_sec, 0);
			_SELF(self)->mtime = NUM2INT(secs);
		} else {
			rb_raise(rb_eArgError, "couldn't get time of object");
		}
	}
	return self;
}

void
Init_gnomevfs_file_info(m_gvfs)
	VALUE m_gvfs;
{
	VALUE g_gvs_fileinfo;

	rbgobj_boxed_not_copy_obj(GNOMEVFS_TYPE_FILE_INFO);

	g_gvs_fileinfo = G_DEF_CLASS(GNOMEVFS_TYPE_FILE_INFO, "FileInfo",
				     m_gvfs);

	rb_define_method(g_gvs_fileinfo, "initialize", fileinfo_initialize, 0);
	rb_define_method(g_gvs_fileinfo, "symlink?", fileinfo_is_symlink, 0);
	rb_define_method(g_gvs_fileinfo, "symbolic_link?", fileinfo_is_symlink,
			 0);
	rb_define_method(g_gvs_fileinfo, "set_symlink", fileinfo_set_symlink,
			 1);
	rb_define_method(g_gvs_fileinfo, "local?", fileinfo_is_local, 0);
	rb_define_method(g_gvs_fileinfo, "set_local", fileinfo_set_local, 1);
	rb_define_method(g_gvs_fileinfo, "suid?", fileinfo_is_suid, 0);
	rb_define_method(g_gvs_fileinfo, "set_suid", fileinfo_set_suid, 1);
	rb_define_method(g_gvs_fileinfo, "sgid?", fileinfo_is_sgid, 0);
	rb_define_method(g_gvs_fileinfo, "set_sgid", fileinfo_set_sgid, 1);
	rb_define_method(g_gvs_fileinfo, "sticky?", fileinfo_is_sticky, 0);
	rb_define_method(g_gvs_fileinfo, "set_sticky", fileinfo_set_sticky, 1);
	rb_define_method(g_gvs_fileinfo, "mime_type?", fileinfo_get_mime_type,
			 0);
	rb_define_method(g_gvs_fileinfo, "==", fileinfo_matches, 1);
	rb_define_method(g_gvs_fileinfo, "name", fileinfo_name, 0);
	rb_define_method(g_gvs_fileinfo, "ftype", fileinfo_filetype, 0);
	rb_define_method(g_gvs_fileinfo, "uid", fileinfo_uid, 0);
	rb_define_method(g_gvs_fileinfo, "gid", fileinfo_gid, 0);
	rb_define_method(g_gvs_fileinfo, "dev", fileinfo_dev, 0);
	rb_define_method(g_gvs_fileinfo, "size", fileinfo_size, 0);
	rb_define_method(g_gvs_fileinfo, "size?", fileinfo_has_size, 0);
	rb_define_method(g_gvs_fileinfo, "zero?", fileinfo_zero, 0);
	rb_define_method(g_gvs_fileinfo, "blocks", fileinfo_blocks, 0);
	rb_define_method(g_gvs_fileinfo, "blksize", fileinfo_blksize, 0);
	rb_define_method(g_gvs_fileinfo, "atime", fileinfo_atime, 0);
	rb_define_method(g_gvs_fileinfo, "mtime", fileinfo_mtime, 0);
	rb_define_method(g_gvs_fileinfo, "ctime", fileinfo_ctime, 0);
	rb_define_method(g_gvs_fileinfo, "symlink_to", fileinfo_symlink_to, 0);
	rb_define_method(g_gvs_fileinfo, "ino", fileinfo_inode, 0);
	rb_define_method(g_gvs_fileinfo, "link_count", fileinfo_link_count, 0);
	rb_define_method(g_gvs_fileinfo, "nlink", fileinfo_link_count, 0);
	rb_define_method(g_gvs_fileinfo, "blockdev?", fileinfo_blockdev_p, 0);
	rb_define_method(g_gvs_fileinfo, "block_device?", fileinfo_blockdev_p,
			 0);
	rb_define_method(g_gvs_fileinfo, "chardev?", fileinfo_chardev_p, 0);
	rb_define_method(g_gvs_fileinfo, "character_device?",
			 fileinfo_chardev_p, 0);
	rb_define_method(g_gvs_fileinfo, "directory?", fileinfo_directory_p,
			 0);
	rb_define_method(g_gvs_fileinfo, "file?", fileinfo_regular_p, 0);
	rb_define_method(g_gvs_fileinfo, "regular?", fileinfo_regular_p, 0);
	rb_define_method(g_gvs_fileinfo, "pipe?", fileinfo_fifo_p, 0);
	rb_define_method(g_gvs_fileinfo, "fifo?", fileinfo_fifo_p, 0);
	rb_define_method(g_gvs_fileinfo, "socket?", fileinfo_socket_p, 0);
	/* XXX: these do the same thing for now... */
	rb_define_method(g_gvs_fileinfo, "executable_real?",
			 fileinfo_executable_real_p, 0);
	rb_define_method(g_gvs_fileinfo, "executable?",
			 fileinfo_executable_real_p, 0);
	rb_define_method(g_gvs_fileinfo, "readable_real?",
			 fileinfo_readable_real_p, 0);
	rb_define_method(g_gvs_fileinfo, "readable?",
			 fileinfo_readable_real_p, 0);
	rb_define_method(g_gvs_fileinfo, "writable_real?",
			 fileinfo_writable_real_p, 0);
	rb_define_method(g_gvs_fileinfo, "writable?",
			 fileinfo_writable_real_p, 0);
	rb_define_method(g_gvs_fileinfo, "grpowned?", fileinfo_grpowned_p, 0);
	rb_define_method(g_gvs_fileinfo, "owned?", fileinfo_owned_p, 0);
	rb_define_method(g_gvs_fileinfo, "permissions", fileinfo_permissions,
			 0);
	rb_define_method(g_gvs_fileinfo, "mode", fileinfo_permissions, 0);
	rb_define_method(g_gvs_fileinfo, "set_name", fileinfo_set_name, 1);
	rb_define_method(g_gvs_fileinfo, "set_permissions", fileinfo_set_permissions, 1);
	rb_define_method(g_gvs_fileinfo, "set_mode", fileinfo_set_permissions, 1);
	rb_define_method(g_gvs_fileinfo, "set_uid", fileinfo_set_uid, 1);
	rb_define_method(g_gvs_fileinfo, "set_gid", fileinfo_set_gid, 1);
	rb_define_method(g_gvs_fileinfo, "set_owner", fileinfo_set_owner, 2);
	rb_define_method(g_gvs_fileinfo, "set_utime", fileinfo_utime, 2);
        

	/* XXX: this is renamed...bad, but their naming is worse */
	rb_define_const(g_gvs_fileinfo,
			"DEFAULT_OPTIONS",
			INT2FIX(GNOME_VFS_FILE_INFO_DEFAULT));
	rb_define_const(g_gvs_fileinfo,
			"GET_MIME_TYPE",
			INT2FIX(GNOME_VFS_FILE_INFO_GET_MIME_TYPE));
	rb_define_const(g_gvs_fileinfo,
			"FORCE_FAST_MIME_TYPE",
			INT2FIX(GNOME_VFS_FILE_INFO_FORCE_FAST_MIME_TYPE));
	rb_define_const(g_gvs_fileinfo,
			"FORCE_SLOW_MIME_TYPE",
			INT2FIX(GNOME_VFS_FILE_INFO_FORCE_SLOW_MIME_TYPE));
	rb_define_const(g_gvs_fileinfo,
			"FOLLOW_LINKS",
			INT2FIX(GNOME_VFS_FILE_INFO_FOLLOW_LINKS));
	rb_define_const(g_gvs_fileinfo,
			"GET_ACCESS_RIGHTS",
			INT2FIX(GNOME_VFS_FILE_INFO_GET_ACCESS_RIGHTS));

	rb_define_const(g_gvs_fileinfo,
			"SET_NONE",
			INT2FIX(GNOME_VFS_SET_FILE_INFO_NONE));
	rb_define_const(g_gvs_fileinfo,
			"SET_NAME",
			INT2FIX(GNOME_VFS_SET_FILE_INFO_NAME));
	rb_define_const(g_gvs_fileinfo,
			"SET_PERMISSIONS",
			INT2FIX(GNOME_VFS_SET_FILE_INFO_PERMISSIONS));
	rb_define_const(g_gvs_fileinfo,
			"SET_OWNER",
			INT2FIX(GNOME_VFS_SET_FILE_INFO_OWNER));
	rb_define_const(g_gvs_fileinfo,
			"SET_TIME",
			INT2FIX(GNOME_VFS_SET_FILE_INFO_TIME));

	rb_define_const(g_gvs_fileinfo,
			"PERM_SUID",
			INT2FIX(GNOME_VFS_PERM_SUID));
	rb_define_const(g_gvs_fileinfo,
			"PERM_SGID",
			INT2FIX(GNOME_VFS_PERM_SGID));
	rb_define_const(g_gvs_fileinfo,
			"PERM_STICKY",
			INT2FIX(GNOME_VFS_PERM_STICKY));
	rb_define_const(g_gvs_fileinfo,
			"PERM_USER_READ",
			INT2FIX(GNOME_VFS_PERM_USER_READ));
	rb_define_const(g_gvs_fileinfo,
			"PERM_USER_WRITE",
			INT2FIX(GNOME_VFS_PERM_USER_WRITE));
	rb_define_const(g_gvs_fileinfo,
			"PERM_USER_EXEC",
			INT2FIX(GNOME_VFS_PERM_USER_EXEC));
	rb_define_const(g_gvs_fileinfo,
			"PERM_USER_ALL",
			INT2FIX(GNOME_VFS_PERM_USER_ALL));
	rb_define_const(g_gvs_fileinfo,
			"PERM_GROUP_READ",
			INT2FIX(GNOME_VFS_PERM_GROUP_READ));
	rb_define_const(g_gvs_fileinfo,
			"PERM_GROUP_WRITE",
			INT2FIX(GNOME_VFS_PERM_GROUP_WRITE));
	rb_define_const(g_gvs_fileinfo,
			"PERM_GROUP_EXEC",
			INT2FIX(GNOME_VFS_PERM_GROUP_EXEC));
	rb_define_const(g_gvs_fileinfo,
			"PERM_GROUP_ALL",
			INT2FIX(GNOME_VFS_PERM_GROUP_ALL));
	rb_define_const(g_gvs_fileinfo,
			"PERM_OTHER_READ",
			INT2FIX(GNOME_VFS_PERM_OTHER_READ));
	rb_define_const(g_gvs_fileinfo,
			"PERM_OTHER_WRITE",
			INT2FIX(GNOME_VFS_PERM_OTHER_WRITE));
	rb_define_const(g_gvs_fileinfo,
			"PERM_OTHER_EXEC",
			INT2FIX(GNOME_VFS_PERM_OTHER_EXEC));
	rb_define_const(g_gvs_fileinfo,
			"PERM_OTHER_ALL",
			INT2FIX(GNOME_VFS_PERM_OTHER_ALL));
	rb_define_const(g_gvs_fileinfo,
			"PERM_ACCESS_READABLE",
			INT2FIX(GNOME_VFS_PERM_ACCESS_READABLE));
	rb_define_const(g_gvs_fileinfo,
			"PERM_ACCESS_WRITABLE",
			INT2FIX(GNOME_VFS_PERM_ACCESS_WRITABLE));
	rb_define_const(g_gvs_fileinfo,
			"PERM_ACCESS_EXECUTABLE",
			INT2FIX(GNOME_VFS_PERM_ACCESS_EXECUTABLE));

	G_DEF_SETTERS(g_gvs_fileinfo);
}

/* vim: set sts=0 sw=8 ts=8: *************************************************/
