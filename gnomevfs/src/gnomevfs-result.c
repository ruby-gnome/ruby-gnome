/*****************************************************************************
 *
 * gnomevfs-result.c: GnomeVFSResult wrapper.
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

/* Includes ******************************************************************/

#include "gnomevfs.h"

/* Defines *******************************************************************/

/* Type Definitions **********************************************************/

/* Function Declarations *****************************************************/

/* Global Variables **********************************************************/

static struct _GVFSToMy {
	int gvfs_code;
	char *rb_class_name;
	VALUE rb_class;
} s_gvfs_to_my[] = {
	{ GNOME_VFS_ERROR_NOT_FOUND, "NotFoundError" },
	{ GNOME_VFS_ERROR_GENERIC, "GenericError" },
	{ GNOME_VFS_ERROR_INTERNAL, "InternalError" },
	{ GNOME_VFS_ERROR_BAD_PARAMETERS, "BadParametersError" },
	{ GNOME_VFS_ERROR_NOT_SUPPORTED, "NotSupportedError" },
	{ GNOME_VFS_ERROR_IO, "IOError" },
	{ GNOME_VFS_ERROR_CORRUPTED_DATA, "CorruptedDataError" },
	{ GNOME_VFS_ERROR_WRONG_FORMAT, "WrongFormatError" },
	{ GNOME_VFS_ERROR_BAD_FILE, "badFileError" },
	{ GNOME_VFS_ERROR_TOO_BIG, "TooBigError" },
	{ GNOME_VFS_ERROR_NO_SPACE, "NoSpaceError" },
	{ GNOME_VFS_ERROR_READ_ONLY, "ReadOnlyError" },
	{ GNOME_VFS_ERROR_INVALID_URI, "InvalidURIError" },
	{ GNOME_VFS_ERROR_NOT_OPEN, "NotOpenError" },
	{ GNOME_VFS_ERROR_INVALID_OPEN_MODE, "InvalidOpenModeError" },
	{ GNOME_VFS_ERROR_ACCESS_DENIED, "AccessDeniedError" },
	{ GNOME_VFS_ERROR_TOO_MANY_OPEN_FILES, "TooManyOpenFilesError" },
	{ GNOME_VFS_ERROR_EOF, "EOFError" },
	{ GNOME_VFS_ERROR_NOT_A_DIRECTORY, "NotADirectoryError" },
	{ GNOME_VFS_ERROR_IN_PROGRESS, "InProgressError" },
	{ GNOME_VFS_ERROR_INTERRUPTED, "InterruptedError" },
	{ GNOME_VFS_ERROR_FILE_EXISTS, "FileExistsError" },
	{ GNOME_VFS_ERROR_LOOP, "LoopError" },
	{ GNOME_VFS_ERROR_NOT_PERMITTED, "NotPermittedError" },
	{ GNOME_VFS_ERROR_IS_DIRECTORY, "IsDirectoryError" },
	{ GNOME_VFS_ERROR_NO_MEMORY, "NoMemoryError" },
	{ GNOME_VFS_ERROR_HOST_NOT_FOUND, "HostNotFoundError" },
	{ GNOME_VFS_ERROR_INVALID_HOST_NAME, "InvalidHostNameError" },
	{ GNOME_VFS_ERROR_HOST_HAS_NO_ADDRESS, "HostHasNoAddressError" },
	{ GNOME_VFS_ERROR_LOGIN_FAILED, "LoginFailedError" },
	{ GNOME_VFS_ERROR_CANCELLED, "CancelledError" },
	{ GNOME_VFS_ERROR_DIRECTORY_BUSY, "DirectoryBusyError" },
	{ GNOME_VFS_ERROR_DIRECTORY_NOT_EMPTY, "DirectoryNotEmptyError" },
	{ GNOME_VFS_ERROR_TOO_MANY_LINKS, "TooManyLinksError" },
	{ GNOME_VFS_ERROR_READ_ONLY_FILE_SYSTEM, "ReadOnlyFileSystemError" },
	{ GNOME_VFS_ERROR_NOT_SAME_FILE_SYSTEM, "NotSameFileSystemError" },
	{ GNOME_VFS_ERROR_NAME_TOO_LONG, "NameTooLongError" },
	{ GNOME_VFS_ERROR_SERVICE_NOT_AVAILABLE, "ServiceNotAvailableError" },
	{ GNOME_VFS_ERROR_SERVICE_OBSOLETE, "ServiceObsoleteError" },
	/* XXX: rename to ProtocolError? */
	{ GNOME_VFS_ERROR_PROTOCOL_ERROR, "ProtocolError" },
	{ GNOME_VFS_ERROR_NO_MASTER_BROWSER, "NoMasterBrowserError" }
};

/* Function Implementations **************************************************/

VALUE
gnomevfs_result_to_rval(result)
	GnomeVFSResult result;
{
	int i, n;

	if (result == GNOME_VFS_OK) {
		return Qtrue;
	} else {
		for (i = 0, n = G_N_ELEMENTS(s_gvfs_to_my); i < n; i++) {
			if (result == s_gvfs_to_my[i].gvfs_code) {
				rb_raise(s_gvfs_to_my[i].rb_class,
					"GnomeVFS Error: %s",
					gnome_vfs_result_to_string(result));
				return Qnil;
			}
		}
		/* XXX: should raise error */
		return Qnil;
	}
}

void
Init_gnomevfs_result(m_gvfs)
	VALUE m_gvfs;
{
	VALUE gvfs_error;
	int i, n;

	gvfs_error = rb_define_class_under(m_gvfs, "Error",
					   rb_eStandardError);
	for (i = 0, n = G_N_ELEMENTS(s_gvfs_to_my); i < n; i++) {
		s_gvfs_to_my[i].rb_class = rb_define_class_under(m_gvfs,
						s_gvfs_to_my[i].rb_class_name,
						gvfs_error);
	}
}

/* vim: set sts=0 sw=8 ts=8: *************************************************/
