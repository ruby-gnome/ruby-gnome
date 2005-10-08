/*****************************************************************************
 *
 * gnomevfs.c: The GnomeVFS module.
 *
 * Copyright (C) 2004,2005 Ruby-GNOME2 Project Team
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
 * $Author: mutoh $
 *
 * $Date: 2005/10/08 18:31:12 $
 *
 *****************************************************************************/

/* Includes ******************************************************************/

#include "gnomevfs.h"
#include <libgnomevfs/gnome-vfs-mime-utils.h>
#include <libgnomevfs/gnome-vfs-mime-info.h>

/* Defines *******************************************************************/

/* Type Definitions **********************************************************/

/* Function Declarations *****************************************************/

/* Global Variables **********************************************************/

VALUE g_id_call;		/* rb_intern("call") */

VALUE g_gvfs_uri;	/* GnomeVFS::URI */
VALUE g_gvfs_error;	/* GnomeVFS::Error */
VALUE g_gvfs_file;	/* GnomeVFS::File */
VALUE g_gvfs_dir;	/* GnomeVFS::Directory */

/* Function Implementations **************************************************/

GList *
strary2glist(ary)
	VALUE ary;
{
	GList *list = NULL;
	VALUE str;
	int i = 0;

	Check_Type(ary, T_ARRAY);
	for (i = 0; i < RARRAY(ary)->len; i++) {
		str = rb_ary_entry(ary, i);
		Check_Type(str, T_STRING);
		list = g_list_append(list, RVAL2CSTR(str));
	}

	return list;
}

VALUE
glist2strary(list)
	GList *list;
{
	VALUE ary;
	
	ary = rb_ary_new();
	while (list) {
		rb_ary_push(ary, CSTR2RVAL(list->data));
		list = list->next;
	}

	return ary;
}

static VALUE
gnomevfs_init(void)
{
	return CBOOL2RVAL(gnome_vfs_init());
}

static VALUE
gnomevfs_initialized(void)
{
	return CBOOL2RVAL(gnome_vfs_initialized());
}

static VALUE
gnomevfs_shutdown(void)
{
	gnome_vfs_shutdown();
	return Qnil;
}

static VALUE
gnomevfs_find_directory(argc, argv, self)
	int argc;
	VALUE *argv;
	VALUE self;
{
	VALUE near_uri, kind, create, find, permissions;
	GnomeVFSURI *uri;
	GnomeVFSResult result;

	argc = rb_scan_args(argc, argv, "23", &near_uri, &kind, &create, &find,
			    &permissions);
	/* XXX: are these viable defaults? */
	if (argc < 5) {
		permissions = UINT2NUM(0777);
	}
	if (argc < 4) {
		find = Qtrue;
	}
	if (argc < 3) {
		create = Qfalse;
	}

	result = gnome_vfs_find_directory(RVAL2GVFSURI(near_uri),
					  FIX2INT(kind),
					  &uri,
					  RTEST(create),
					  RTEST(find),
					  FIX2UINT(permissions));
	if (result == GNOME_VFS_OK) {
		return GVFSURI2RVAL(uri);
	} else {
		return GVFSRESULT2RVAL(result);
	}
}

static VALUE
gnomevfs_get_mime_type_for_data(self, r_data)
	VALUE self, r_data;
{
	const char *data;
	int size;

	data = RVAL2CSTR(r_data);
	size = strlen(data);
	return CSTR2RVAL(gnome_vfs_get_mime_type_for_data(data, size));
}

static VALUE
gnomevfs_get_mime_type(self, uri)
	VALUE self, uri;
{
	char *mime;
	VALUE str;

	mime = gnome_vfs_get_mime_type(RVAL2CSTR(uri));
	str = CSTR2RVAL(mime);
	g_free(mime);
	return str;
}

static VALUE
gnomevfs_get_registered_mime_types(self, mime)
	VALUE self, mime;
{
	GList *list;
	VALUE ary;

	list = gnome_vfs_get_registered_mime_types();
	ary = GLIST2STRARY(list);
	gnome_vfs_mime_registered_mime_type_list_free(list);
	return ary;
}

void
Init_gnomevfs(void)
{
	VALUE m_gvfs = rb_define_module("GnomeVFS");

	g_id_call = rb_intern("call");

        rb_define_const(m_gvfs, "BUILD_VERSION",
                    rb_ary_new3(3,
                                INT2FIX(GNOMEVFS_MAJOR_VERSION),
                                INT2FIX(GNOMEVFS_MINOR_VERSION),
                                INT2FIX(GNOMEVFS_MICRO_VERSION)));        

	rb_define_module_function(m_gvfs, "init", gnomevfs_init, 0);
	rb_define_module_function(m_gvfs, "initialized?", gnomevfs_initialized,
				  0);
	rb_define_module_function(m_gvfs, "shutdown", gnomevfs_shutdown, 0);
	rb_define_module_function(m_gvfs, "find_directory",
				  gnomevfs_find_directory, -1);
	rb_define_module_function(m_gvfs, "get_mime_type_for_data",
				  gnomevfs_get_mime_type_for_data, 1);
	rb_define_module_function(m_gvfs, "get_mime_type",
				  gnomevfs_get_mime_type, 1);
	rb_define_module_function(m_gvfs, "get_registered_mime_types",
				  gnomevfs_get_registered_mime_types, 1);

	rb_define_const(m_gvfs,
			"DIRECTORY_KIND_DESKTOP",
			INT2FIX(GNOME_VFS_DIRECTORY_KIND_DESKTOP));
	rb_define_const(m_gvfs,
			"DIRECTORY_KIND_TRASH",
			INT2FIX(GNOME_VFS_DIRECTORY_KIND_TRASH));
	rb_define_const(m_gvfs,
			"MIME_TYPE_UNKNOWN",
			CSTR2RVAL(GNOME_VFS_MIME_TYPE_UNKNOWN));

	Init_gnomevfs_result(m_gvfs);
	Init_gnomevfs_uri(m_gvfs);
	Init_gnomevfs_file_info(m_gvfs);
	Init_gnomevfs_directory(m_gvfs);
	Init_gnomevfs_file(m_gvfs);
	Init_gnomevfs_monitor(m_gvfs);
	Init_gnomevfs_application_registry(m_gvfs);
}

/* vim: set sts=0 sw=8 ts=8: *************************************************/
