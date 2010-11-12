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

/* NOTE: g_io_error_from_errno doesn’t make sense from Ruby. */

void
Init_gioerror(VALUE glib)
{
        VALUE io, error;
        
        /* TODO: Is this naming scheme correct/the best? */
        io = rb_define_module_under(glib, "IO");

        error = rbgio_define_domain_error(io, "Error", G_IO_ERROR, "GIOErrorEnum", rb_eIOError);

        rbgio_define_error(io, "FailedError", G_IO_ERROR_FAILED, error);
        rbgio_define_error(io, "NotFoundError", G_IO_ERROR_NOT_FOUND, error);
        rbgio_define_error(io, "ExistsError", G_IO_ERROR_EXISTS, error);
        rbgio_define_error(io, "IsDirectoryError", G_IO_ERROR_IS_DIRECTORY, error);
        rbgio_define_error(io, "NotDirectoryError", G_IO_ERROR_NOT_DIRECTORY, error);
        rbgio_define_error(io, "NotEmptyError", G_IO_ERROR_NOT_EMPTY, error);
        rbgio_define_error(io, "NotRegularFileError", G_IO_ERROR_NOT_REGULAR_FILE, error);
        rbgio_define_error(io, "NotSymbolicLinkError", G_IO_ERROR_NOT_SYMBOLIC_LINK, error);
        rbgio_define_error(io, "NotMountableFileError", G_IO_ERROR_NOT_MOUNTABLE_FILE, error);
        rbgio_define_error(io, "FilenameTooLongError", G_IO_ERROR_FILENAME_TOO_LONG, error);
        rbgio_define_error(io, "InvalidFilenameError", G_IO_ERROR_INVALID_FILENAME, error);
        rbgio_define_error(io, "TooManyLinksError", G_IO_ERROR_TOO_MANY_LINKS, error);
        rbgio_define_error(io, "NoSpaceError", G_IO_ERROR_NO_SPACE, error);
        rbgio_define_error(io, "InvalidArgumentError", G_IO_ERROR_INVALID_ARGUMENT, error);
        rbgio_define_error(io, "PermissionDeniedError", G_IO_ERROR_PERMISSION_DENIED, error);
        rbgio_define_error(io, "NotSupportedError", G_IO_ERROR_NOT_SUPPORTED, error);
        rbgio_define_error(io, "NotMountedError", G_IO_ERROR_NOT_MOUNTED, error);
        rbgio_define_error(io, "AlreadyMountedError", G_IO_ERROR_ALREADY_MOUNTED, error);
        rbgio_define_error(io, "ClosedError", G_IO_ERROR_CLOSED, error);
        rbgio_define_error(io, "CancelledError", G_IO_ERROR_CANCELLED, error);
        rbgio_define_error(io, "PendingError", G_IO_ERROR_PENDING, error);
        rbgio_define_error(io, "ReadOnlyError", G_IO_ERROR_READ_ONLY, error);
        rbgio_define_error(io, "CantCreateBackupError", G_IO_ERROR_CANT_CREATE_BACKUP, error);
        rbgio_define_error(io, "WrongEtagError", G_IO_ERROR_WRONG_ETAG, error);
        rbgio_define_error(io, "TimedOutError", G_IO_ERROR_TIMED_OUT, error);
        rbgio_define_error(io, "WouldRecurseError", G_IO_ERROR_WOULD_RECURSE, error);
        rbgio_define_error(io, "BusyError", G_IO_ERROR_BUSY, error);
        rbgio_define_error(io, "WouldBlockError", G_IO_ERROR_WOULD_BLOCK, error);
        rbgio_define_error(io, "HostNotFoundError", G_IO_ERROR_HOST_NOT_FOUND, error);
        rbgio_define_error(io, "WouldMergeError", G_IO_ERROR_WOULD_MERGE, error);
        rbgio_define_error(io, "FailedHandledError", G_IO_ERROR_FAILED_HANDLED, error);
        rbgio_define_error(io, "TooManyOpenFilesError", G_IO_ERROR_TOO_MANY_OPEN_FILES, error);
        rbgio_define_error(io, "NotInitializedError", G_IO_ERROR_NOT_INITIALIZED, error);
        rbgio_define_error(io, "AddressInUseError", G_IO_ERROR_ADDRESS_IN_USE, error);
        rbgio_define_error(io, "PartialInputError", G_IO_ERROR_PARTIAL_INPUT, error);
        rbgio_define_error(io, "InvalidDataError", G_IO_ERROR_INVALID_DATA, error);
}
