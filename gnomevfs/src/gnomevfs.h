/*****************************************************************************
 *
 * gnomevfs.h: GnomeVFS interface
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
 * Latest Revision: 2003-08-04
 *
 *****************************************************************************/

/* Multiple Inclusion Guard and extern "C" specifiers for C++ ****************/

#ifndef GNOMEVFS_H
#define GNOMEVFS_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ******************************************************************/

#include <glib.h>
#include <ruby.h>
#include <libgnomevfs/gnome-vfs.h>

#include <rbglib.h>
#include <rbgobject.h>

#include "gnomevfs-result.h"
#include "gnomevfs-uri.h"
#include "gnomevfs-fileinfo.h"
#include "gnomevfs-directory.h"
#include "gnomevfs-file.h"
#include "gnomevfs-monitor.h"

/* Defines *******************************************************************/

/* XXX: this should be a global Ruby-GNOME2 #define */
#ifndef ULL2NUM
# define ULL2NUM ULONG2NUM
#endif

/* GnomeVFS::Error handling functions */
#define GVFSRESULT2RVAL(res)	(gnomevfs_result_to_rval(res))
#define RAISE_IF_ERROR(res)	((void)gnomevfs_result_to_rval(res))
#define CHECK_RESULT(res, ret)	\
	(((res) == GNOME_VFS_OK) ? (ret) : GVFSRESULT2RVAL(res))

/* Type Definitions **********************************************************/

#define GNOMEVFS_TYPE_DIRECTORY	(gnome_vfs_directory_get_type())
#define GNOMEVFS_TYPE_FILE	(gnome_vfs_file_get_type())
#define GNOMEVFS_TYPE_FILE_INFO	(gnome_vfs_file_info_get_type())
#define GNOMEVFS_TYPE_MONITOR	(gnome_vfs_monitor_get_type())
#define GNOMEVFS_TYPE_URI	(gnome_vfs_uri_get_type())

#define GVFSDIRECTORY2RVAL(uri)	(BOXED2RVAL(uri, GNOMEVFS_TYPE_DIRECTORY))
#define GVFSFILE2RVAL(uri)	(BOXED2RVAL(uri, GNOMEVFS_TYPE_FILE))
#define GVFSFILEINFO2RVAL(info)	(BOXED2RVAL(info, GNOMEVFS_TYPE_FILE_INFO))
#define GVFSURI2RVAL(uri)	(BOXED2RVAL(uri, GNOMEVFS_TYPE_URI))

#define RVAL2GVFSDIRECTORY(uri)	\
	((GnomeVFSDirectoryHandle *)RVAL2BOXED(uri, GNOMEVFS_TYPE_DIRECTORY))
#define RVAL2GVFSFILE(uri)	\
	((GnomeVFSHandle *)RVAL2BOXED(uri, GNOMEVFS_TYPE_FILE))
#define RVAL2GVFSFILEINFO(info)	\
	((GnomeVFSFileInfo *)RVAL2BOXED(info, GNOMEVFS_TYPE_FILE_INFO))
#define RVAL2GVFSURI(uri)	\
	((GnomeVFSURI *)RVAL2BOXED(uri, GNOMEVFS_TYPE_URI))

/* Function Declarations *****************************************************/

GType gnome_vfs_directory_get_type(void);
GType gnome_vfs_file_get_type(void);
GType gnome_vfs_file_info_get_type(void);
GType gnome_vfs_monitor_get_type(void);
GType gnome_vfs_uri_get_type(void);

VALUE gnomevfs_result_to_rval(GnomeVFSResult result);

void Init_gnomevfs_directory(VALUE m_gvfs);
void Init_gnomevfs_file(VALUE m_gvfs);
void Init_gnomevfs_file_info(VALUE m_gvfs);
void Init_gnomevfs_monitor(VALUE m_gvfs);
void Init_gnomevfs_result(VALUE m_gvfs);
void Init_gnomevfs_uri(VALUE m_gvfs);

/* Global Variables **********************************************************/

VALUE g_id_call;		/* rb_intern("call") */

VALUE g_gvfs_uri;		/* GnomeVFS::URI */
VALUE g_gvfs_error;		/* GnomeVFS::Error */
VALUE g_gvfs_file;		/* GnomeVFS::File */
VALUE g_gvfs_dir;		/* GnomeVFS::Directory */

/* End of Multiple Inclusion Guard and extern "C" specifiers for C++ *********/

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* GNOMEVFS_H */

/* vim: set sts=0 sw=8 ts=8: *************************************************/
