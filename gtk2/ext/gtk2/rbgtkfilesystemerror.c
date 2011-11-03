/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2005,2006 Ruby-GNOME2 Project Team
 *  Copyright (C) 2004  Seiya Nishizawa, Masao Mutoh
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

#include "global.h"

#define RG_TARGET_NAMESPACE cFileSystemError

void 
Init_gtk_filesystemerror(VALUE mGtk)
{
#if GTK_CHECK_VERSION(2,4,0)
#ifdef GTK_FILE_SYSTEM_ERROR
    VALUE RG_TARGET_NAMESPACE;

    /* GtkFileSystemError */
    RG_TARGET_NAMESPACE = G_DEF_ERROR2(GTK_FILE_SYSTEM_ERROR, "FileSystemError",
                                        mGtk, rb_eRuntimeError);
    rb_define_const(RG_TARGET_NAMESPACE, "NONEXISTENT",
                    INT2NUM(GTK_FILE_SYSTEM_ERROR_NONEXISTENT));
    rb_define_const(RG_TARGET_NAMESPACE, "NOT_FOLDER",
                    INT2NUM(GTK_FILE_SYSTEM_ERROR_NOT_FOLDER));
    rb_define_const(RG_TARGET_NAMESPACE, "INVALID_URI",
                    INT2NUM(GTK_FILE_SYSTEM_ERROR_INVALID_URI));
    rb_define_const(RG_TARGET_NAMESPACE, "BAD_FILENAME",
                    INT2NUM(GTK_FILE_SYSTEM_ERROR_BAD_FILENAME));
    rb_define_const(RG_TARGET_NAMESPACE, "FAILED",
                    INT2NUM(GTK_FILE_SYSTEM_ERROR_FAILED));
    rb_define_const(RG_TARGET_NAMESPACE, "ALREADY_EXSITS",
                    INT2NUM(GTK_FILE_SYSTEM_ERROR_ALREADY_EXISTS));
#endif
#endif
}
