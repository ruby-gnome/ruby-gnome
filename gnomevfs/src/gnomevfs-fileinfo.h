/*****************************************************************************
 *
 * gnomevfs-fileinfo.h: GnomeVFS::FileInfo class.
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
 * Latest Revision: 2003-07-24
 *
 *****************************************************************************/

/* Multiple Inclusion Guard and extern "C" specifiers for C++ ****************/

#ifndef GNOMEVFS_FILEINFO_H
#define GNOMEVFS_FILEINFO_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ******************************************************************/

/* Defines *******************************************************************/

#define GNOMEVFS_TYPE_FILE_INFO	(gnome_vfs_file_info_get_type())
#define GVFSFILEINFO2RVAL(info)	(BOXED2RVAL(info, GNOMEVFS_TYPE_FILE_INFO))
#define RVAL2GVFSFILEINFO(info)	\
	((GnomeVFSFileInfo *)RVAL2BOXED(info, GNOMEVFS_TYPE_FILE_INFO))

/* Type Definitions **********************************************************/

/* Function Declarations *****************************************************/

GType gnome_vfs_file_info_get_type(void);
void Init_gnomevfs_file_info(VALUE m_gvfs);

/* Global Variables **********************************************************/

/* End of Multiple Inclusion Guard and extern "C" specifiers for C++ *********/

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* GNOMEVFS_FILEINFO_H */

/* vim: set sts=0 sw=8 ts=8: *************************************************/
