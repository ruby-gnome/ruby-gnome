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

#ifndef __RBGIO2_H__
#define __RBGIO2_H__

#include <gio/gio.h>

#include "rbgio2conversions.h"

G_BEGIN_DECLS

/* Block Handling */
#define SAVE_BLOCK(block) G_STMT_START { \
        if (!NIL_P(block)) \
                G_CHILD_ADD(mGLib, (block)); \
} G_STMT_END

#define USE_BLOCK(data) \
        rbgio_child_remove_and_return(mGLib, (VALUE)(data))

#define USE_BLOCK_AND_SAVE(data) ((VALUE)data)

#define G_TYPE_FILE_ATTRIBUTE_MATCHER (g_file_attribute_matcher_get_type())
GType g_file_attribute_matcher_get_type(void);

#define G_TYPE_FILE_ATTRIBUTE_INFO (g_file_attribute_info_get_type())
GType g_file_attribute_info_get_type(void);

#define G_TYPE_FILE_ATTRIBUTE_INFO_LIST (g_file_attribute_info_list_get_type())
GType g_file_attribute_info_list_get_type(void);

/* Conversion Macros */
#define RVAL2TYPE_WITH_DEFAULT(value, type, default_value) \
        (NIL_P(value) ? (default_value) : type(value))

#define RVAL2ATTRIBUTESDEFAULT(value) \
        RVAL2TYPE_WITH_DEFAULT((value), RVAL2CSTR, "standard::*")

#define RVAL2GDRIVESTARTFLAGSDEFAULT(value) \
        RVAL2TYPE_WITH_DEFAULT((value), \
                               RVAL2GDRIVESTARTFLAGS, \
                               G_DRIVE_START_NONE)

#define RVAL2GMOUNTMOUNTFLAGSDEFAULT(value) \
        RVAL2TYPE_WITH_DEFAULT((value), \
                               RVAL2GMOUNTMOUNTFLAGS, \
                               G_MOUNT_MOUNT_NONE)

#define RVAL2GMOUNTUNMOUNTFLAGSDEFAULT(value) \
        RVAL2TYPE_WITH_DEFAULT((value), \
                               RVAL2GMOUNTUNMOUNTFLAGS, \
                               G_MOUNT_UNMOUNT_NONE)

#define RVAL2GSOCKETPROTOCOLDEFAULT(value) \
        RVAL2TYPE_WITH_DEFAULT((value), \
                               RVAL2GSOCKETPROTOCOL, \
                               G_SOCKET_PROTOCOL_DEFAULT)

#define GTIMEVAL2RVAL(value) rb_time_new((value)->tv_sec, (value)->tv_usec)

#define RVAL2GZLIBCOMPRESSORFORMATDEFAULT(value) \
        RVAL2TYPE_WITH_DEFAULT((value), \
                               RVAL2GZLIBCOMPRESSORFORMAT, \
                               G_ZLIB_COMPRESSOR_FORMAT_ZLIB)

#define RVAL2IOPRIORITYDEFAULT(value) \
        RVAL2TYPE_WITH_DEFAULT((value), NUM2INT, G_PRIORITY_DEFAULT)

/* Integer Type Conversion Macros */
#define FD2RVAL(value) INT2FIX(value)
#define RVAL2FD(value) FIX2INT(value)
#define RVAL2FDS(value, n) RVAL2GINTS(value, n)

#define GINT2RVAL(value) INT2NUM(value)
#define RVAL2GINT(value) NUM2INT(value)

#define GINT162RVAL(value) INT2FIX(value)
#define RVAL2GINT16(value) FIX2INT(value)

#define GINT322RVAL(value) INT2NUM(value)
#define RVAL2GINT32(value) NUM2INT(value)

#define GINT642RVAL(value) rbglib_int64_to_num(value)
#define RVAL2GINT64(value) rbglib_num_to_int64(value)

#define GSIZE2RVAL(value) ULONG2NUM(value)
#define RVAL2GSIZE(value) NUM2ULONG(value)

#define GLONG2RVAL(value) LONG2NUM(value)
#define RVAL2GLONG(value) NUM2LONG(value)

#define GOFFSET2RVAL(value) rbglib_int64_to_num(value)
#define RVAL2GOFFSET(value) rbglib_num_to_int64(value)

#define GSSIZE2RVAL(value) LONG2NUM(value)
#define RVAL2GSSIZE(value) NUM2LONG(value)

/* TODO: Should we be zeroing out bits? */
#define GUCHAR2RVAL(value) INT2FIX(value)
#define RVAL2GUCHAR(value) FIX2INT(value)

#define GUINT2RVAL(value) UINT2NUM(value)
#define RVAL2GUINT(value) NUM2UINT(value)

/* TODO: Should we be zeroing out bits? */
#define GUINT162RVAL(value) INT2FIX(value)
#define RVAL2GUINT16(value) FIX2INT(value)

#define GUINT322RVAL(value) UINT2NUM(value)
#define RVAL2GUINT32(value) NUM2UINT(value)

#define GUINT642RVAL(value) rbglib_uint64_to_num(value)
#define RVAL2GUINT64(value) rbglib_num_to_uint64(value)

#define GULONG2RVAL(value) ULONG2NUM(value)
#define RVAL2GULONG(value) NUM2ULONG(value)

VALUE rbgio_define_domain_error(VALUE module,
                                const char *name,
                                GQuark domain,
                                const char *enum_name,
                                VALUE parent);
VALUE rbgio_define_error(VALUE module,
                         const char *name,
                         gint code,
                         VALUE domain_error);
NORETURN(void rbgio_raise_error(GError *error));
GObject *rbgio_ginitable_new(GType type, VALUE parameters, VALUE cancellable);
void rbgio_gasyncinitable_new_async(GType type,
                                    VALUE parameters,
                                    VALUE io_priority,
                                    VALUE cancellable,
                                    VALUE block);

G_END_DECLS

#endif /* __RBGIO2_H__ */
