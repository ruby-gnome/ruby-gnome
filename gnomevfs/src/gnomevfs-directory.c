/*****************************************************************************
 *
 * gnomevfs-directory.c: GnomeVFS Directory class.
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
 * $Author: ggc $
 *
 * $Date: 2007/07/13 16:07:30 $
 *
 *****************************************************************************/

/* Includes ******************************************************************/

#include "gnomevfs.h"

/* Defines *******************************************************************/

#define _SELF(s)	\
     ((GnomeVFSDirectoryHandle *)RVAL2BOXED(s, GNOMEVFS_TYPE_DIRECTORY))

/* Type Definitions **********************************************************/

/* Function Declarations *****************************************************/

/* Global Variables **********************************************************/

/* Function Implementations **************************************************/

static VALUE directory_each(VALUE self);
static VALUE directory_close(VALUE self);

static VALUE
directory_foreach(self, uri)
	VALUE self, uri;
{
	VALUE dir;

	dir = rb_funcall(g_gvfs_dir, rb_intern("open"), 1, uri);
	rb_ensure(directory_each, dir, directory_close, dir);
	return Qnil;
}

static VALUE
directory_list_load(argc, argv, self)
	int argc;
	VALUE *argv;
	VALUE self;
{
	VALUE uri, r_options, ary;
	GnomeVFSFileInfoOptions options;
	GList *list;
	GnomeVFSResult result;

	if (rb_scan_args(argc, argv, "11", &uri, &r_options) == 2) {
		options = RVAL2GFLAGS(r_options, GNOME_VFS_TYPE_VFS_FILE_INFO_OPTIONS);
	} else {
		/* XXX: or? */
		options = GNOME_VFS_FILE_INFO_DEFAULT;
	}

	result = gnome_vfs_directory_list_load(&list, RVAL2CSTR(uri), options);
	if (result == GNOME_VFS_OK) {
		ary = GLIST2ARY2(list, GNOMEVFS_TYPE_FILE_INFO);
		g_list_free(list);
		return ary;
	} else {
		return GVFSRESULT2RVAL(result);
	}
}

static VALUE
directory_make_directory(argc, argv, self)
	int argc;
	VALUE *argv;
	VALUE self;
{
	VALUE uri;
	VALUE v_perm;
	guint perm;
	GnomeVFSResult result;

	if (rb_scan_args(argc, argv, "11", &uri, &v_perm) == 2) {
		perm = NUM2UINT(v_perm);
	} else {
		perm = 0777;
	}

	if (RVAL2CBOOL(rb_obj_is_kind_of(uri, g_gvfs_uri))) {
		result = gnome_vfs_make_directory_for_uri(RVAL2GVFSURI(uri),
							  perm);
	} else {
		SafeStringValue(uri);
		rb_secure(2);
		result = gnome_vfs_make_directory(RVAL2CSTR(uri), perm);
	}

	return GVFSRESULT2RVAL(result);
}

static VALUE
directory_remove_directory(self, uri)
	VALUE self, uri;
{
	GnomeVFSResult result;

	/* XXX: how to use this?
	SafeStringValue(uri);
	*/
	rb_secure(2);
	if (RVAL2CBOOL(rb_obj_is_kind_of(uri, g_gvfs_uri))) {
		result = gnome_vfs_remove_directory_from_uri(
							RVAL2GVFSURI(uri));
	} else {
		result = gnome_vfs_remove_directory(RVAL2CSTR(uri));
	}

	return GVFSRESULT2RVAL(result);
}

static gboolean
directory_visit_callback(rel_path, info, recursing_will_loop, data, recurse)
	const gchar *rel_path;
	GnomeVFSFileInfo *info;
	gboolean recursing_will_loop;
	gpointer data;
	gboolean *recurse;
{
	*recurse = RVAL2CBOOL(rb_funcall((VALUE)data,
				    g_id_call,
				    3,
				    CSTR2RVAL(rel_path),
				    GVFSFILEINFO2RVAL(info),
				    CBOOL2RVAL(recursing_will_loop)));
	return TRUE;
}

static VALUE
directory_visit(argc, argv, self)
	int argc;
	VALUE *argv;
	VALUE self;
{
	VALUE uri, info_options, visit_options;
	VALUE func;

	argc = rb_scan_args(argc, argv, "12&", &uri, &info_options,
			     &visit_options, &func);
	if (argc < 3) {
		visit_options = GFLAGS2RVAL(GNOME_VFS_DIRECTORY_VISIT_DEFAULT, GNOME_VFS_TYPE_VFS_DIRECTORY_VISIT_OPTIONS);
	}
	if (argc < 2) {
		info_options = GFLAGS2RVAL(GNOME_VFS_FILE_INFO_DEFAULT, GNOME_VFS_TYPE_VFS_FILE_INFO_OPTIONS);
	}

	if (NIL_P(func)) {
		func = rb_block_proc();
	}
	G_RELATIVE(self, func);

	if (RVAL2CBOOL(rb_obj_is_kind_of(uri, g_gvfs_uri))) {
		RAISE_IF_ERROR(gnome_vfs_directory_visit_uri(RVAL2GVFSURI(uri),
			RVAL2GFLAGS(info_options, GNOME_VFS_TYPE_VFS_FILE_INFO_OPTIONS),
			RVAL2GFLAGS(visit_options, GNOME_VFS_TYPE_VFS_DIRECTORY_VISIT_OPTIONS),
			(GnomeVFSDirectoryVisitFunc)directory_visit_callback,
			(gpointer)func));
	} else {
		RAISE_IF_ERROR(gnome_vfs_directory_visit(RVAL2CSTR(uri),
			RVAL2GFLAGS(info_options, GNOME_VFS_TYPE_VFS_FILE_INFO_OPTIONS),
			RVAL2GFLAGS(visit_options, GNOME_VFS_TYPE_VFS_DIRECTORY_VISIT_OPTIONS),
			(GnomeVFSDirectoryVisitFunc)directory_visit_callback,
			(gpointer)func));
	}

	return Qnil;
}

static VALUE
directory_visit_files(argc, argv, self)
	int argc;
	VALUE *argv;
	VALUE self;
{
	VALUE uri, r_list, info_options, visit_options;
	GList *list = NULL;
	VALUE func;
	GnomeVFSResult result;

	argc = rb_scan_args(argc, argv, "22&", &uri, &r_list, &info_options,
			     &visit_options, &func);
	if (argc < 4) {
		visit_options = GENUM2RVAL(GNOME_VFS_DIRECTORY_VISIT_DEFAULT, GNOME_VFS_TYPE_VFS_DIRECTORY_VISIT_OPTIONS);
	}
	if (argc < 3) {
		info_options = GFLAGS2RVAL(GNOME_VFS_FILE_INFO_DEFAULT, GNOME_VFS_TYPE_VFS_FILE_INFO_OPTIONS);
	}

	if (NIL_P(func)) {
		func = rb_block_proc();
	}
	G_RELATIVE(self, func);

	list = STRARY2GLIST(r_list);

	if (RVAL2CBOOL(rb_obj_is_kind_of(uri, g_gvfs_uri))) {
		result = gnome_vfs_directory_visit_files_at_uri(
			RVAL2GVFSURI(uri),
			list,
			RVAL2GFLAGS(info_options, GNOME_VFS_TYPE_VFS_FILE_INFO_OPTIONS),
			RVAL2GFLAGS(visit_options, GNOME_VFS_TYPE_VFS_DIRECTORY_VISIT_OPTIONS),
			(GnomeVFSDirectoryVisitFunc)directory_visit_callback,
			(gpointer)func);
	} else {
		result = gnome_vfs_directory_visit_files(
			RVAL2CSTR(uri),
			list,
			RVAL2GFLAGS(info_options, GNOME_VFS_TYPE_VFS_FILE_INFO_OPTIONS),
			RVAL2GFLAGS(visit_options, GNOME_VFS_TYPE_VFS_DIRECTORY_VISIT_OPTIONS),
			(GnomeVFSDirectoryVisitFunc)directory_visit_callback,
			(gpointer)func);
	}

	g_list_free(list);

	return CHECK_RESULT(result, Qnil);
}

static GnomeVFSDirectoryHandle *
gnome_vfs_directory_copy(gpointer boxed)
{
	return (GnomeVFSDirectoryHandle *)boxed;
}

void
gnome_vfs_directory_free(gpointer boxed)
{
	/*
	 * do nothing for now...
	 * we can probably say that #close() is enough
	 */
}

GType
gnome_vfs_directory_get_type(void)
{
	static GType our_type = 0;

	if (our_type == 0) {
		our_type = g_boxed_type_register_static("GnomeVFSDirectory",
				(GBoxedCopyFunc)gnome_vfs_directory_copy,
				(GBoxedFreeFunc)gnome_vfs_directory_free);
	}

	return our_type;
}

static VALUE
directory_initialize(argc, argv, self)
	int argc;
	VALUE *argv;
	VALUE self;
{
	VALUE uri, r_options;
	GnomeVFSFileInfoOptions options;
	GnomeVFSDirectoryHandle *handle;
	GnomeVFSResult result;

	/* XXX: how to do this?
	 * SafeStringValue(uri);
	 */
	if (rb_scan_args(argc, argv, "11", &uri, &r_options) == 2) {
		options = RVAL2GFLAGS(r_options, GNOME_VFS_TYPE_VFS_FILE_INFO_OPTIONS);
	} else {
		/* XXX: or? */
		options = GNOME_VFS_FILE_INFO_DEFAULT;
	}

	if (RVAL2CBOOL(rb_obj_is_kind_of(uri, g_gvfs_uri))) {
		result = gnome_vfs_directory_open_from_uri(&handle,
						RVAL2GVFSURI(uri), options);
	} else {
		result = gnome_vfs_directory_open(&handle, RVAL2CSTR(uri),
						  options);
	}

	RAISE_IF_ERROR(result);

	G_INITIALIZE(self, handle);
	return Qnil;
}

static VALUE
directory_close(self)
	VALUE self;
{
	return CHECK_RESULT(gnome_vfs_directory_close(_SELF(self)), Qnil);
}

static VALUE
directory_open(argc, argv, self)
	int argc;
	VALUE *argv;
	VALUE self;
{
	VALUE dir;

	dir = rb_funcall2(self, rb_intern("new"), argc, argv);
	if (rb_block_given_p()) {
		rb_ensure(rb_yield, dir, directory_close, dir);
		return Qnil;
	}
	return dir;
}

static VALUE
directory_each(self)
	VALUE self;
{
	GnomeVFSDirectoryHandle *handle = _SELF(self);
	GnomeVFSResult result = GNOME_VFS_OK;

	while (TRUE) {
		GnomeVFSFileInfo *info;

		info = gnome_vfs_file_info_new();
		result = gnome_vfs_directory_read_next(handle, info);
		if (result != GNOME_VFS_OK) {
			gnome_vfs_file_info_unref(info);
			break;
		}
		rb_yield(GVFSFILEINFO2RVAL(info));
	}

	if (result == GNOME_VFS_OK || result == GNOME_VFS_ERROR_EOF) {
		return Qnil;
	} else {
		return GVFSRESULT2RVAL(result);
	}
}

static VALUE
directory_read_next(self)
	VALUE self;
{
	GnomeVFSFileInfo *info;
	GnomeVFSResult result;

	info = gnome_vfs_file_info_new();
	result = gnome_vfs_directory_read_next(_SELF(self), info);
	if (result == GNOME_VFS_OK) {
		return GVFSFILEINFO2RVAL(info);
	} else if (result == GNOME_VFS_ERROR_EOF) {
		return Qnil;
	} else {
		gnome_vfs_file_info_unref(info);
		return GVFSRESULT2RVAL(result);
	}
}

void
Init_gnomevfs_directory(m_gvfs)
	VALUE m_gvfs;
{
	g_gvfs_dir = G_DEF_CLASS(GNOMEVFS_TYPE_DIRECTORY, "Directory", m_gvfs);
	rb_define_const(m_gvfs, "Dir", g_gvfs_dir);

	rb_include_module(g_gvfs_dir, rb_mEnumerable);

	rb_define_singleton_method(g_gvfs_dir, "delete",
				   directory_remove_directory, 1);
	rb_define_singleton_method(g_gvfs_dir, "entries", directory_list_load,
				   -1);
	rb_define_singleton_method(g_gvfs_dir, "foreach", directory_foreach, 1);
	rb_define_singleton_method(g_gvfs_dir, "list_load",
				   directory_list_load, -1);
	rb_define_singleton_method(g_gvfs_dir, "make_directory",
				   directory_make_directory, -1);
	rb_define_singleton_method(g_gvfs_dir, "mkdir",
				   directory_make_directory, -1);
	rb_define_singleton_method(g_gvfs_dir, "remove_directory",
				   directory_remove_directory, 1);
	rb_define_singleton_method(g_gvfs_dir, "open", directory_open, -1);
	rb_define_singleton_method(g_gvfs_dir, "rmdir",
				   directory_remove_directory, 1);
	rb_define_singleton_method(g_gvfs_dir, "unlink",
				   directory_remove_directory, 1);
	rb_define_singleton_method(g_gvfs_dir, "visit", directory_visit, -1);
	rb_define_singleton_method(g_gvfs_dir, "visit_files",
				   directory_visit_files, -1);

	rb_define_method(g_gvfs_dir, "initialize", directory_initialize, -1);
	rb_define_method(g_gvfs_dir, "close", directory_close, 0);
	rb_define_method(g_gvfs_dir, "each", directory_each, 0);
	rb_define_method(g_gvfs_dir, "read", directory_read_next, 0);
	rb_define_alias(g_gvfs_dir, "read_next", "read");

        G_DEF_CLASS(GNOME_VFS_TYPE_VFS_DIRECTORY_VISIT_OPTIONS, "VisitOptions", g_gvfs_dir);
        G_DEF_CONSTANTS(g_gvfs_dir, GNOME_VFS_TYPE_VFS_DIRECTORY_VISIT_OPTIONS, "GNOME_VFS_DIRECTORY_");
}

/* vim: set sts=0 sw=8 ts=8: *************************************************/
