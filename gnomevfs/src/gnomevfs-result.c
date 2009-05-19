/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*****************************************************************************
 *
 * gnomevfs-result.c: GnomeVFSResult wrapper.
 *
 * Copyright (C) 2004 Ruby-GNOME2 Project Team
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
 * $Date: 2004/08/26 16:27:28 $
 *
 *****************************************************************************/

#include "gnomevfs.h"

GQuark
gnome_vfs_error_quark(void)
{
  static GQuark quark = 0;
  if (quark == 0)
    quark = g_quark_from_static_string ("gnome-vfs-error-quark");
  return quark;
}

#define GNOME_VFS_ERROR gnome_vfs_error_quark()

VALUE
gnomevfs_result_to_rval(result)
    GnomeVFSResult result;
{
    if (result == GNOME_VFS_OK) {
        return Qtrue;
    } else {
        GError* error = g_error_new(GNOME_VFS_ERROR, result, "%s",
                                    gnome_vfs_result_to_string(result));
        RAISE_GERROR(error);
    }
    return Qnil;
}

void
Init_gnomevfs_result(m_gvfs)
    VALUE m_gvfs;
{
    VALUE gerror = G_DEF_ERROR2(GNOME_VFS_ERROR, "Error", m_gvfs, rb_eRuntimeError);
        
    rb_define_const(gerror, "NOT_FOUND", INT2NUM(GNOME_VFS_ERROR_NOT_FOUND));
    rb_define_const(gerror, "GENERIC", INT2NUM(GNOME_VFS_ERROR_GENERIC));
    rb_define_const(gerror, "INTERNAL", INT2NUM(GNOME_VFS_ERROR_INTERNAL));
    rb_define_const(gerror, "BAD_PARAMETERS", INT2NUM(GNOME_VFS_ERROR_BAD_PARAMETERS));
    rb_define_const(gerror, "NOT_SUPPORTED", INT2NUM(GNOME_VFS_ERROR_NOT_SUPPORTED));
    rb_define_const(gerror, "ERROR_IO", INT2NUM(GNOME_VFS_ERROR_IO));
    rb_define_const(gerror, "CORRUPTED_DATA", INT2NUM(GNOME_VFS_ERROR_CORRUPTED_DATA));
    rb_define_const(gerror, "WRONG_FORMAT", INT2NUM(GNOME_VFS_ERROR_WRONG_FORMAT));
    rb_define_const(gerror, "BAD_FILE", INT2NUM(GNOME_VFS_ERROR_BAD_FILE));
    rb_define_const(gerror, "TOO_BIG", INT2NUM(GNOME_VFS_ERROR_TOO_BIG));
    rb_define_const(gerror, "NO_SPACE", INT2NUM(GNOME_VFS_ERROR_NO_SPACE));
    rb_define_const(gerror, "READ_ONLY", INT2NUM(GNOME_VFS_ERROR_READ_ONLY));
    rb_define_const(gerror, "INVALID_URI", INT2NUM(GNOME_VFS_ERROR_INVALID_URI));
    rb_define_const(gerror, "NOT_OPEN", INT2NUM(GNOME_VFS_ERROR_NOT_OPEN));
    rb_define_const(gerror, "INVALID_OPEN_MODE", INT2NUM(GNOME_VFS_ERROR_INVALID_OPEN_MODE));
    rb_define_const(gerror, "ACCESS_DENIED", INT2NUM(GNOME_VFS_ERROR_ACCESS_DENIED));
    rb_define_const(gerror, "TOO_MANY_OPEN_FILES", INT2NUM(GNOME_VFS_ERROR_TOO_MANY_OPEN_FILES));
    rb_define_const(gerror, "EOF", INT2NUM(GNOME_VFS_ERROR_EOF));
    rb_define_const(gerror, "NOT_A_DIRECTORY", INT2NUM(GNOME_VFS_ERROR_NOT_A_DIRECTORY));
    rb_define_const(gerror, "IN_PROGRESS", INT2NUM(GNOME_VFS_ERROR_IN_PROGRESS));
    rb_define_const(gerror, "INTERRUPTED", INT2NUM(GNOME_VFS_ERROR_INTERRUPTED));
    rb_define_const(gerror, "FILE_EXISTS", INT2NUM(GNOME_VFS_ERROR_FILE_EXISTS));
    rb_define_const(gerror, "LOOP", INT2NUM(GNOME_VFS_ERROR_LOOP));
    rb_define_const(gerror, "NOT_PERMITTED", INT2NUM(GNOME_VFS_ERROR_NOT_PERMITTED));
    rb_define_const(gerror, "IS_DIRECTORY", INT2NUM(GNOME_VFS_ERROR_IS_DIRECTORY));
    rb_define_const(gerror, "NO_MEMORY", INT2NUM(GNOME_VFS_ERROR_NO_MEMORY));
    rb_define_const(gerror, "HOST_NOT_FOUND", INT2NUM(GNOME_VFS_ERROR_HOST_NOT_FOUND));
    rb_define_const(gerror, "INVALID_HOST_NAME", INT2NUM(GNOME_VFS_ERROR_INVALID_HOST_NAME));
    rb_define_const(gerror, "HOST_HAS_NO_ADDRESS", INT2NUM(GNOME_VFS_ERROR_HOST_HAS_NO_ADDRESS));
    rb_define_const(gerror, "LOGIN_FAILED", INT2NUM(GNOME_VFS_ERROR_LOGIN_FAILED));
    rb_define_const(gerror, "CANCELLED", INT2NUM(GNOME_VFS_ERROR_CANCELLED));
    rb_define_const(gerror, "DIRECTORY_BUSY", INT2NUM(GNOME_VFS_ERROR_DIRECTORY_BUSY));
    rb_define_const(gerror, "DIRECTORY_NOT_EMPTY", INT2NUM(GNOME_VFS_ERROR_DIRECTORY_NOT_EMPTY));
    rb_define_const(gerror, "TOO_MANY_LINKS", INT2NUM(GNOME_VFS_ERROR_TOO_MANY_LINKS));
    rb_define_const(gerror, "READ_ONLY_FILE_SYSTEM", INT2NUM(GNOME_VFS_ERROR_READ_ONLY_FILE_SYSTEM));
    rb_define_const(gerror, "NOT_SAME_FILE_SYSTEM", INT2NUM(GNOME_VFS_ERROR_NOT_SAME_FILE_SYSTEM));
    rb_define_const(gerror, "NAME_TOO_LONG", INT2NUM(GNOME_VFS_ERROR_NAME_TOO_LONG));
    rb_define_const(gerror, "SERVICE_NOT_AVAILABLE", INT2NUM(GNOME_VFS_ERROR_SERVICE_NOT_AVAILABLE));
    rb_define_const(gerror, "SERVICE_OBSOLETE", INT2NUM(GNOME_VFS_ERROR_SERVICE_OBSOLETE));
    rb_define_const(gerror, "PROTOCOL_ERROR", INT2NUM(GNOME_VFS_ERROR_PROTOCOL_ERROR));
#ifdef GNOME_VFS_ERROR_NO_MASTER_BROWSER
    rb_define_const(gerror, "NO_MASTER_BROWSER", INT2NUM(GNOME_VFS_ERROR_NO_MASTER_BROWSER));
#endif
}

