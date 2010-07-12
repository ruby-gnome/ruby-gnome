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

#ifndef GIO2_H
#define GIO2_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include <ruby.h>
#include <gio/gio.h>

#include <rbglib.h>
#include <rbgobject.h>

/* Block Handling */
#define SAVE_BLOCK(block) G_STMT_START { \
        if (!NIL_P(block)) \
                G_CHILD_ADD(mGLib, (block)); \
} G_STMT_END

#define USE_BLOCK(data) \
        rbgio_child_remove_and_return(mGLib, (VALUE)(data))

#define USE_BLOCK_AND_SAVE(data) ((VALUE)data)

/* Conversion Macros */
#define RVAL2TYPE_WITH_DEFAULT(value, type, default_value) \
        (NIL_P(value) ? (default_value) : type(value))

#define RVAL2ATTRIBUTESDEFAULT(value) \
        RVAL2TYPE_WITH_DEFAULT((value), RVAL2CSTR, "standard::*")

#define RVAL2GAPPINFO(value) G_APP_INFO(RVAL2GOBJ(value))

#define RVAL2GAPPLAUNCHCONTEXT(value) G_APP_LAUNCH_CONTEXT(RVAL2GOBJ(value))

#define RVAL2GASYNCRESULT(value) G_ASYNC_RESULT(RVAL2GOBJ(value))

#define RVAL2GCANCELLABLE(value) G_CANCELLABLE(RVAL2GOBJ(value))

#define RVAL2GCONVERTER(value) G_CONVERTER(RVAL2GOBJ(value))

#define GDATASTREAMBYTEORDER2RVAL(value) \
        RVAL2GENUM((value), G_TYPE_DATA_STREAM_BYTE_ORDER)

#define RVAL2GDATASTREAMBYTEORDER(value) \
        GENUM2RVAL((value), G_TYPE_DATA_STREAM_BYTE_ORDER)

#define RVAL2GDRIVESTARTFLAGS(value) \
        RVAL2GENUM((value), G_TYPE_DRIVE_START_FLAGS)
#define RVAL2GDRIVESTARTFLAGSDEFAULT(value) \
        RVAL2TYPE_WITH_DEFAULT((value), \
                               RVAL2GDRIVESTARTFLAGS, \
                               G_DRIVE_START_NONE)
#define RVAL2GEMBLEM(value) G_EMBLEM(RVAL2GOBJ(value))

#define RVAL2GFILE(value) G_FILE(RVAL2GOBJ(value))

#define G_TYPE_FILE_ATTRIBUTE_MATCHER (g_file_attribute_matcher_get_type())
GType g_file_attribute_matcher_get_type(void);

#define RVAL2GFILEATTRIBUTEMATCHER(value) \
        ((GFileAttributeMatcher *)(RVAL2BOXED(value, G_TYPE_FILE_ATTRIBUTE_MATCHER)))

#define GFILEATTRIBUTETYPE2RVAL(value) \
        GENUM2RVAL((value), G_TYPE_FILE_ATTRIBUTE_TYPE)

#define G_TYPE_FILE_ATTRIBUTE_INFO_LIST (g_file_attribute_info_list_get_type())
GType g_file_attribute_info_list_get_type();

#define GFILEATTRIBUTEINFOLIST2RVAL(object) \
        BOXED2RVAL(object, G_TYPE_FILE_ATTRIBUTE_INFO_LIST)

#define RVAL2GFILEATTRIBUTETYPE(value) \
        RVAL2GENUM((value), G_TYPE_FILE_ATTRIBUTE_TYPE)

#define RVAL2GFILEINFO(value) G_FILE_INFO(RVAL2GOBJ(value))

#define GFILETYPE2RVAL(value) GENUM2RVAL((value), G_TYPE_FILE_TYPE)
#define RVAL2GFILETYPE(value) RVAL2GENUM((value), G_TYPE_FILE_TYPE)

#define RVAL2GICON(value) G_ICON(RVAL2GOBJ(value))

#define RVAL2GINETADDRESS(value) G_INET_ADDRESS(RVAL2GOBJ(value))

#define RVAL2GINPUTSTREAM(value) G_INPUT_STREAM(RVAL2GOBJ(value))

#define RVAL2GMOUNT(value) G_MOUNT(RVAL2GOBJ(value))

#define RVAL2GMOUNTMOUNTFLAGS(value) \
        RVAL2GFLAGS((value), G_TYPE_MOUNT_MOUNT_FLAGS)
#define RVAL2GMOUNTMOUNTFLAGSDEFAULT(value) \
        RVAL2TYPE_WITH_DEFAULT((value), \
                               RVAL2GMOUNTMOUNTFLAGS, \
                               G_MOUNT_MOUNT_NONE)

#define RVAL2GMOUNTUNMOUNTFLAGS(value) \
        RVAL2GFLAGS((value), G_TYPE_MOUNT_UNMOUNT_FLAGS)
#define RVAL2GMOUNTUNMOUNTFLAGSDEFAULT(value) \
        RVAL2TYPE_WITH_DEFAULT((value), \
                               RVAL2GMOUNTUNMOUNTFLAGS, \
                               G_MOUNT_UNMOUNT_NONE)

#define RVAL2GMOUNTOPERATION(value) G_MOUNT_OPERATION(RVAL2GOBJ(value))

#define RVAL2GOUTPUTSTREAM(value) G_OUTPUT_STREAM(RVAL2GOBJ(value))

#define RVAL2GSOCKET(value) G_SOCKET(RVAL2GOBJ(value))

#define RVAL2GSOCKETADDRESS(value) G_SOCKET_ADDRESS(RVAL2GOBJ(value))

#define RVAL2GSOCKETCONNECTABLE(value) G_SOCKET_CONNECTABLE(RVAL2GOBJ(value))

#define GSOCKETFAMILY2RVAL(value) GENUM2RVAL((value), G_TYPE_SOCKET_FAMILY)
#define RVAL2GSOCKETFAMILY(value) RVAL2GENUM((value), G_TYPE_SOCKET_FAMILY)

#define GSOCKETPROTOCOL2RVAL(value) GENUM2RVAL((value), G_TYPE_SOCKET_PROTOCOL)
#define RVAL2GSOCKETPROTOCOL(value) RVAL2GFLAGS((value), G_TYPE_SOCKET_PROTOCOL)
#define RVAL2GSOCKETPROTOCOLDEFAULT(value) \
        RVAL2TYPE_WITH_DEFAULT((value), \
                               RVAL2GSOCKETPROTOCOL, \
                               G_SOCKET_PROTOCOL_DEFAULT)

#define GSOCKETTYPE2RVAL(value) GENUM2RVAL((value), G_TYPE_SOCKET_TYPE)
#define RVAL2GSOCKETTYPE(value) RVAL2GFLAGS((value), G_TYPE_SOCKET_TYPE)

#define GTIMEVAL2RVAL(value) rb_time_new((value)->tv_sec, (value)->tv_usec)

#define RVAL2GZLIBCOMPRESSORFORMAT(value) \
        RVAL2GFLAGS((value), G_TYPE_ZLIB_COMPRESSOR_FORMAT)
#define RVAL2GZLIBCOMPRESSORFORMATDEFAULT(value) \
        RVAL2TYPE_WITH_DEFAULT((value), \
                               RVAL2GZLIBCOMPRESSORFORMAT, \
                               G_ZLIB_COMPRESSOR_FORMAT_ZLIB)

#define RVAL2IOPRIORITYDEFAULT(value) \
        RVAL2TYPE_WITH_DEFAULT((value), NUM2INT, G_PRIORITY_DEFAULT)

#ifdef HAVE_GIO_UNIX
#  define RVAL2GUNIXFDLIST(value) G_UNIX_FD_LIST(RVAL2GOBJ(value))
#endif

/* Integer Type Conversion Macros */
#define FD2RVAL(value) INT2FIX(value)
#define RVAL2FD(value) FIX2INT(value)

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

#define CSTR2RVAL_TAINTED(string, length) \
        rbgio_cstr_to_rval_tainted((string), (length))
#define CSTR2RVAL_TAINTED_FREE(string, length) \
        rbgio_cstr_to_rval_tainted_free((string), (length))

#define GFDS2ARY(fds) rbgio_fds_to_ary(fds)
#define GFDS2ARY_FREE(fds) rbgio_fds_to_ary_free(fds)

#define GFILEARY2GLIST(ary) rbgio_gfile_ary_to_glist(ary)
#define GFILEARY2GLIST_ACCEPT_NIL(ary) rbgio_gfile_ary_to_glist_accept_nil(ary)

#define STRVECTOR2ARY(vector) rbgio_str_vector_to_ary(vector)
#define STRVECTOR2ARY_FREE(vector) rbgio_str_vector_to_ary_free(vector)
#define ARY2STRVECTOR(ary) rbgio_ary_to_str_vector(ary)

G_GNUC_INTERNAL VALUE rbgio_cstr_to_rval_tainted(const char *string,
                                                 gsize length);
G_GNUC_INTERNAL VALUE rbgio_cstr_to_rval_tainted_free(char *string,
                                                      gsize length);
G_GNUC_INTERNAL VALUE rbgio_fds_to_ary(const gint *fds);
G_GNUC_INTERNAL VALUE rbgio_fds_to_ary_free(gint *fds);
G_GNUC_INTERNAL GList *rbgio_gfile_ary_to_glist(VALUE ary);
G_GNUC_INTERNAL GList *rbgio_gfile_ary_to_glist_accept_nil(VALUE ary);
G_GNUC_INTERNAL void rbgio_rval_to_gtimeval(VALUE value, GTimeVal *time);
G_GNUC_INTERNAL VALUE rbgio_str_vector_to_ary(const gchar * const *vector);
G_GNUC_INTERNAL VALUE rbgio_str_vector_to_ary_free(gchar **vector);
G_GNUC_INTERNAL char **rbgio_ary_to_str_vector(VALUE ary);
G_GNUC_INTERNAL void rbgio_async_ready_callback(GObject *source,
                                       GAsyncResult *result,
                                       gpointer data);
G_GNUC_INTERNAL VALUE rbgio_child_remove_and_return(VALUE parent, VALUE child);
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

G_GNUC_INTERNAL void Init_gappinfo(VALUE glib);
G_GNUC_INTERNAL void Init_gapplaunchcontext(VALUE glib);
G_GNUC_INTERNAL void Init_gasyncinitable(VALUE glib);
G_GNUC_INTERNAL void Init_gasyncresult(VALUE glib);
G_GNUC_INTERNAL void Init_gbufferedinputstream(VALUE glib);
G_GNUC_INTERNAL void Init_gbufferedoutputstream(VALUE glib);
G_GNUC_INTERNAL void Init_gcancellable(VALUE glib);
G_GNUC_INTERNAL void Init_gcharsetconverter(VALUE glib);
G_GNUC_INTERNAL void Init_gcontenttype(VALUE glib);
G_GNUC_INTERNAL void Init_gconverter(VALUE glib);
G_GNUC_INTERNAL void Init_gconverterinputstream(VALUE glib);
G_GNUC_INTERNAL void Init_gconverteroutputstream(VALUE glib);
G_GNUC_INTERNAL void Init_gdatainputstream(VALUE glib);
G_GNUC_INTERNAL void Init_gdataoutputstream(VALUE glib);
G_GNUC_INTERNAL void Init_gdesktopappinfo(VALUE glib);
G_GNUC_INTERNAL void Init_gdrive(VALUE glib);
G_GNUC_INTERNAL void Init_gemblem(VALUE glib);
G_GNUC_INTERNAL void Init_gemblemedicon(VALUE glib);
G_GNUC_INTERNAL void Init_gfile(VALUE glib);
G_GNUC_INTERNAL void Init_gfileattribute(VALUE glib);
G_GNUC_INTERNAL void Init_gfileattributematcher(VALUE glib);
G_GNUC_INTERNAL void Init_gfiledescriptorbased(VALUE glib);
G_GNUC_INTERNAL void Init_gfileenumerator(VALUE glib);
G_GNUC_INTERNAL void Init_gfileicon(VALUE glib);
G_GNUC_INTERNAL void Init_gfileinfo(VALUE glib);
G_GNUC_INTERNAL void Init_gfileinputstream(VALUE glib);
G_GNUC_INTERNAL void Init_gfileiostream(VALUE glib);
G_GNUC_INTERNAL void Init_gfilemonitor(VALUE glib);
G_GNUC_INTERNAL void Init_gfilenamecompleter(VALUE glib);
G_GNUC_INTERNAL void Init_gfileoutputstream(VALUE glib);
G_GNUC_INTERNAL void Init_gfilterinputstream(VALUE glib);
G_GNUC_INTERNAL void Init_gfilteroutputstream(VALUE glib);
G_GNUC_INTERNAL void Init_gicon(VALUE glib);
G_GNUC_INTERNAL void Init_ginetaddress(VALUE glib);
G_GNUC_INTERNAL void Init_ginetsocketaddress(VALUE glib);
G_GNUC_INTERNAL void Init_ginitable(VALUE glib);
G_GNUC_INTERNAL void Init_ginputstream(VALUE glib);
G_GNUC_INTERNAL void Init_gioerror(VALUE glib);
G_GNUC_INTERNAL void Init_giomodule(VALUE glib);
G_GNUC_INTERNAL void Init_gioscheduler(VALUE glib);
G_GNUC_INTERNAL void Init_giostream(VALUE glib);
G_GNUC_INTERNAL void Init_gloadableicon(VALUE glib);
G_GNUC_INTERNAL void Init_gmemoryinputstream(VALUE glib);
G_GNUC_INTERNAL void Init_gmemoryoutputstream(VALUE glib);
G_GNUC_INTERNAL void Init_gmount(VALUE glib);
G_GNUC_INTERNAL void Init_gmountoperation(VALUE glib);
G_GNUC_INTERNAL void Init_gnetworkaddress(VALUE glib);
G_GNUC_INTERNAL void Init_gnetworkservice(VALUE glib);
G_GNUC_INTERNAL void Init_goutputstream(VALUE glib);
G_GNUC_INTERNAL void Init_gresolver(VALUE glib);
G_GNUC_INTERNAL void Init_gseekable(VALUE glib);
G_GNUC_INTERNAL void Init_gsimpleasyncresult(VALUE glib);
G_GNUC_INTERNAL void Init_gsocket(VALUE glib);
G_GNUC_INTERNAL void Init_gsocketaddress(VALUE glib);
G_GNUC_INTERNAL void Init_gsocketclient(VALUE glib);
G_GNUC_INTERNAL void Init_gsocketconnectable(VALUE glib);
G_GNUC_INTERNAL void Init_gsocketconnection(VALUE glib);
G_GNUC_INTERNAL void Init_gsocketcontrolmessage(VALUE glib);
G_GNUC_INTERNAL void Init_gsocketlistener(VALUE glib);
G_GNUC_INTERNAL void Init_gsocketservice(VALUE glib);
G_GNUC_INTERNAL void Init_gsrvtarget(VALUE glib);
G_GNUC_INTERNAL void Init_gtcpconnection(VALUE glib);
G_GNUC_INTERNAL void Init_gthreadedsocketservice(VALUE glib);
G_GNUC_INTERNAL void Init_gthemedicon(VALUE glib);
G_GNUC_INTERNAL void Init_gunixconnection(VALUE glib);
G_GNUC_INTERNAL void Init_gunixfdlist(VALUE glib);
G_GNUC_INTERNAL void Init_gunixfdmessage(VALUE glib);
G_GNUC_INTERNAL void Init_gunixinputstream(VALUE glib);
G_GNUC_INTERNAL void Init_gunixmounts(VALUE glib);
G_GNUC_INTERNAL void Init_gunixoutputstream(VALUE glib);
G_GNUC_INTERNAL void Init_gunixsocketaddress(VALUE glib);
G_GNUC_INTERNAL void Init_gvfs(VALUE glib);
G_GNUC_INTERNAL void Init_gvolume(VALUE glib);
G_GNUC_INTERNAL void Init_gvolumemonitor(VALUE glib);
G_GNUC_INTERNAL void Init_gzlibcompressor(VALUE glib);
G_GNUC_INTERNAL void Init_gzlibdecompressor(VALUE glib);
G_GNUC_INTERNAL void Init_util(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* GNOMEVFS_H */
