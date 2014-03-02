/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2008-2009  Ruby-GNOME2 Project Team
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 *  MA  02110-1301  USA
 */

#include "rbgio2private.h"

#define RG_TARGET_NAMESPACE mIO

/* NOTE: g_io_error_from_errno doesn't make sense from Ruby. */

void
Init_gioerror(VALUE mGio)
{
        VALUE RG_TARGET_NAMESPACE, error;

        RG_TARGET_NAMESPACE = rb_define_module_under(mGio, "IO");

        error = rbgio_define_domain_error(RG_TARGET_NAMESPACE, "Error", G_IO_ERROR, "GIOErrorEnum", rb_eIOError);

        rbgio_define_error(RG_TARGET_NAMESPACE, "FailedError", G_IO_ERROR_FAILED, error);
        rbgio_define_error(RG_TARGET_NAMESPACE, "NotFoundError", G_IO_ERROR_NOT_FOUND, error);
        rbgio_define_error(RG_TARGET_NAMESPACE, "ExistsError", G_IO_ERROR_EXISTS, error);
        rbgio_define_error(RG_TARGET_NAMESPACE, "IsDirectoryError", G_IO_ERROR_IS_DIRECTORY, error);
        rbgio_define_error(RG_TARGET_NAMESPACE, "NotDirectoryError", G_IO_ERROR_NOT_DIRECTORY, error);
        rbgio_define_error(RG_TARGET_NAMESPACE, "NotEmptyError", G_IO_ERROR_NOT_EMPTY, error);
        rbgio_define_error(RG_TARGET_NAMESPACE, "NotRegularFileError", G_IO_ERROR_NOT_REGULAR_FILE, error);
        rbgio_define_error(RG_TARGET_NAMESPACE, "NotSymbolicLinkError", G_IO_ERROR_NOT_SYMBOLIC_LINK, error);
        rbgio_define_error(RG_TARGET_NAMESPACE, "NotMountableFileError", G_IO_ERROR_NOT_MOUNTABLE_FILE, error);
        rbgio_define_error(RG_TARGET_NAMESPACE, "FilenameTooLongError", G_IO_ERROR_FILENAME_TOO_LONG, error);
        rbgio_define_error(RG_TARGET_NAMESPACE, "InvalidFilenameError", G_IO_ERROR_INVALID_FILENAME, error);
        rbgio_define_error(RG_TARGET_NAMESPACE, "TooManyLinksError", G_IO_ERROR_TOO_MANY_LINKS, error);
        rbgio_define_error(RG_TARGET_NAMESPACE, "NoSpaceError", G_IO_ERROR_NO_SPACE, error);
        rbgio_define_error(RG_TARGET_NAMESPACE, "InvalidArgumentError", G_IO_ERROR_INVALID_ARGUMENT, error);
        rbgio_define_error(RG_TARGET_NAMESPACE, "PermissionDeniedError", G_IO_ERROR_PERMISSION_DENIED, error);
        rbgio_define_error(RG_TARGET_NAMESPACE, "NotSupportedError", G_IO_ERROR_NOT_SUPPORTED, error);
        rbgio_define_error(RG_TARGET_NAMESPACE, "NotMountedError", G_IO_ERROR_NOT_MOUNTED, error);
        rbgio_define_error(RG_TARGET_NAMESPACE, "AlreadyMountedError", G_IO_ERROR_ALREADY_MOUNTED, error);
        rbgio_define_error(RG_TARGET_NAMESPACE, "ClosedError", G_IO_ERROR_CLOSED, error);
        rbgio_define_error(RG_TARGET_NAMESPACE, "CancelledError", G_IO_ERROR_CANCELLED, error);
        rbgio_define_error(RG_TARGET_NAMESPACE, "PendingError", G_IO_ERROR_PENDING, error);
        rbgio_define_error(RG_TARGET_NAMESPACE, "ReadOnlyError", G_IO_ERROR_READ_ONLY, error);
        rbgio_define_error(RG_TARGET_NAMESPACE, "CantCreateBackupError", G_IO_ERROR_CANT_CREATE_BACKUP, error);
        rbgio_define_error(RG_TARGET_NAMESPACE, "WrongEtagError", G_IO_ERROR_WRONG_ETAG, error);
        rbgio_define_error(RG_TARGET_NAMESPACE, "TimedOutError", G_IO_ERROR_TIMED_OUT, error);
        rbgio_define_error(RG_TARGET_NAMESPACE, "WouldRecurseError", G_IO_ERROR_WOULD_RECURSE, error);
        rbgio_define_error(RG_TARGET_NAMESPACE, "BusyError", G_IO_ERROR_BUSY, error);
        rbgio_define_error(RG_TARGET_NAMESPACE, "WouldBlockError", G_IO_ERROR_WOULD_BLOCK, error);
        rbgio_define_error(RG_TARGET_NAMESPACE, "HostNotFoundError", G_IO_ERROR_HOST_NOT_FOUND, error);
        rbgio_define_error(RG_TARGET_NAMESPACE, "WouldMergeError", G_IO_ERROR_WOULD_MERGE, error);
        rbgio_define_error(RG_TARGET_NAMESPACE, "FailedHandledError", G_IO_ERROR_FAILED_HANDLED, error);
        rbgio_define_error(RG_TARGET_NAMESPACE, "TooManyOpenFilesError", G_IO_ERROR_TOO_MANY_OPEN_FILES, error);
        rbgio_define_error(RG_TARGET_NAMESPACE, "NotInitializedError", G_IO_ERROR_NOT_INITIALIZED, error);
        rbgio_define_error(RG_TARGET_NAMESPACE, "AddressInUseError", G_IO_ERROR_ADDRESS_IN_USE, error);
        rbgio_define_error(RG_TARGET_NAMESPACE, "PartialInputError", G_IO_ERROR_PARTIAL_INPUT, error);
        rbgio_define_error(RG_TARGET_NAMESPACE, "InvalidDataError", G_IO_ERROR_INVALID_DATA, error);
}
