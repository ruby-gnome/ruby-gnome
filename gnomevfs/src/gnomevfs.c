/*****************************************************************************
 *
 * gnomevfs.c: The GnomeVFS module.
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
 * Author: Nikolai :: lone-star :: Weibull <lone-star@home.se>
 *
 * Latest Revision: 2003-07-27
 *
 *****************************************************************************/

/* Includes ******************************************************************/

#include "gnomevfs.h"

/* Defines *******************************************************************/

/* Type Definitions **********************************************************/

/* Function Declarations *****************************************************/

/* Global Variables **********************************************************/

/* Function Implementations **************************************************/

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

void
Init_gnomevfs(void)
{
	VALUE m_gvfs = rb_define_module("GnomeVFS");

	g_id_call = rb_intern("call");

	rb_define_module_function(m_gvfs, "init", gnomevfs_init, 0);
	rb_define_module_function(m_gvfs, "initialized?", gnomevfs_initialized,
				  0);
	rb_define_module_function(m_gvfs, "shutdown", gnomevfs_shutdown, 0);
	rb_define_module_function(m_gvfs, "find_directory",
				  gnomevfs_find_directory, -1);
	rb_define_const(m_gvfs,
			"DIRECTORY_KIND_DESKTOP",
			INT2FIX(GNOME_VFS_DIRECTORY_KIND_DESKTOP));
	rb_define_const(m_gvfs,
			"DIRECTORY_KIND_TRASH",
			INT2FIX(GNOME_VFS_DIRECTORY_KIND_TRASH));

	Init_gnomevfs_result(m_gvfs);
	Init_gnomevfs_uri(m_gvfs);
	Init_gnomevfs_file_info(m_gvfs);
	Init_gnomevfs_directory(m_gvfs);
	Init_gnomevfs_file(m_gvfs);
	Init_gnomevfs_monitor(m_gvfs);
}

/* vim: set sts=0 sw=8 ts=8: *************************************************/
