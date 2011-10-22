/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2004  Masao Mutoh
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

#include "rbgprivate.h"
#include "rbglib.h"

/* Use Ruby standard libraries.
enum        GFileTest;
GFileError  g_file_error_from_errno         (gint err_no);
gboolean    g_file_get_contents             (const gchar *filename,
                                             gchar **contents,
                                             gsize *length,
                                             GError **error);
gboolean    g_file_test                     (const gchar *filename,
                                             GFileTest test);
gint        g_mkstemp                       (gchar *tmpl);
gint        g_file_open_tmp                 (const gchar *tmpl,
                                             gchar **name_used,
                                             GError **error);
gchar*      g_file_read_link                (const gchar *filename,
                                             GError **error);

struct      GDir;
GDir*       g_dir_open                      (const gchar *path,
                                             guint flags,
                                             GError **error);
G_CONST_RETURN gchar* g_dir_read_name       (GDir *dir);
void        g_dir_rewind                    (GDir *dir);
void        g_dir_close                     (GDir *dir);
*/

#if GLIB_CHECK_VERSION(2, 16, 0)
static VALUE
rbglib_m_format_size_for_display(G_GNUC_UNUSED VALUE self, VALUE size)
{
    return CSTR2RVAL_FREE(g_format_size_for_display(NUM2INT(size)));
}
#endif

void
Init_glib_fileutils(void)
{
    VALUE cFileError = G_DEF_ERROR2(G_FILE_ERROR, "FileError", mGLib, rb_eIOError);

    rb_define_const(cFileError, "EXIST", INT2NUM(G_FILE_ERROR_EXIST));
    rb_define_const(cFileError, "ISDIR", INT2NUM(G_FILE_ERROR_ISDIR));
    rb_define_const(cFileError, "ACCES", INT2NUM(G_FILE_ERROR_ACCES));
    rb_define_const(cFileError, "NAMETOOLONG", INT2NUM(G_FILE_ERROR_NAMETOOLONG));
    rb_define_const(cFileError, "NOENT", INT2NUM(G_FILE_ERROR_NOENT));
    rb_define_const(cFileError, "NOTDIR", INT2NUM(G_FILE_ERROR_NOTDIR));
    rb_define_const(cFileError, "NXIO", INT2NUM(G_FILE_ERROR_NXIO));
    rb_define_const(cFileError, "NODEV", INT2NUM(G_FILE_ERROR_NODEV));
    rb_define_const(cFileError, "ROFS", INT2NUM(G_FILE_ERROR_ROFS));
    rb_define_const(cFileError, "TXTBSY", INT2NUM(G_FILE_ERROR_TXTBSY));
    rb_define_const(cFileError, "FAULT", INT2NUM(G_FILE_ERROR_FAULT));
    rb_define_const(cFileError, "LOOP", INT2NUM(G_FILE_ERROR_LOOP));
    rb_define_const(cFileError, "NOSPC", INT2NUM(G_FILE_ERROR_NOSPC));
    rb_define_const(cFileError, "NOMEM", INT2NUM(G_FILE_ERROR_NOMEM));
    rb_define_const(cFileError, "MFILE", INT2NUM(G_FILE_ERROR_MFILE));
    rb_define_const(cFileError, "NFILE", INT2NUM(G_FILE_ERROR_NFILE));
    rb_define_const(cFileError, "BADF", INT2NUM(G_FILE_ERROR_BADF));
    rb_define_const(cFileError, "INVAL", INT2NUM(G_FILE_ERROR_INVAL));
    rb_define_const(cFileError, "PIPE", INT2NUM(G_FILE_ERROR_PIPE));
    rb_define_const(cFileError, "AGAIN", INT2NUM(G_FILE_ERROR_AGAIN));
    rb_define_const(cFileError, "INTR", INT2NUM(G_FILE_ERROR_INTR));
    rb_define_const(cFileError, "IO", INT2NUM(G_FILE_ERROR_IO));
    rb_define_const(cFileError, "PERM", INT2NUM(G_FILE_ERROR_PERM));
    rb_define_const(cFileError, "FAILED", INT2NUM(G_FILE_ERROR_FAILED));

#if GLIB_CHECK_VERSION(2, 16, 0)
    rb_define_module_function(mGLib, "format_size_for_display",
			      rbglib_m_format_size_for_display, 1);
#endif
}
