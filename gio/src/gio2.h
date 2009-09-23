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

#if defined(__GNUC__)
#  define UNUSED(u)   \
        u __attribute__((__unused__))
#  define HIDDEN   \
        __attribute__((visibility("hidden")))
#else
#  define UNUSED(u)   \
        u
#  define HIDDEN
#endif  

#define GIO2_DEFAULT_ATTRIBUTES "standard::*"

#define SCAN_1_ARG(format, var_1, type_1) G_STMT_START { \
        VALUE value_1; \
        rb_scan_args(argc, argv, format, &value_1); \
        var_1 = type_1(value_1); \
} G_STMT_END

#define SCAN_1_ARG_WITH_BLOCK(format, var_1, type_1) G_STMT_START { \
        VALUE value_1; \
        rb_scan_args(argc, argv, format"&", &value_1, &block); \
        var_1 = type_1(value_1); \
        SAVE_BLOCK(block); \
} G_STMT_END

#define SCAN_2_ARGS(format, var_1, type_1, var_2, type_2) G_STMT_START { \
        VALUE value_1, value_2; \
        rb_scan_args(argc, argv, format, &value_1, &value_2); \
        var_1 = type_1(value_1); \
        var_2 = type_2(value_2); \
} G_STMT_END

#define SCAN_2_ARGS_WITH_BLOCK(format, var_1, type_1, var_2, type_2) G_STMT_START { \
        VALUE value_1, value_2; \
        rb_scan_args(argc, argv, format"&", &value_1, &value_2, &block); \
        var_1 = type_1(value_1); \
        var_2 = type_2(value_2); \
        SAVE_BLOCK(block); \
} G_STMT_END

#define SCAN_3_ARGS(format, var_1, type_1, var_2, type_2, var_3, type_3) G_STMT_START { \
        VALUE value_1, value_2, value_3; \
        rb_scan_args(argc, argv, format, &value_1, &value_2, &value_3); \
        var_1 = type_1(value_1); \
        var_2 = type_2(value_2); \
        var_3 = type_3(value_3); \
} G_STMT_END

#define SCAN_3_ARGS_WITH_BLOCK(format, var_1, type_1, var_2, type_2, var_3, type_3) G_STMT_START { \
        VALUE value_1, value_2, value_3; \
        rb_scan_args(argc, argv, format"&", &value_1, &value_2, &value_3, &block); \
        var_1 = type_1(value_1); \
        var_2 = type_2(value_2); \
        var_3 = type_3(value_3); \
        SAVE_BLOCK(block); \
} G_STMT_END

#define SCAN_4_ARGS(format, var_1, type_1, var_2, type_2, var_3, type_3, var_4, type_4) G_STMT_START { \
        VALUE value_1, value_2, value_3, value_4; \
        rb_scan_args(argc, argv, format, &value_1, &value_2, &value_3, &value_4); \
        var_1 = type_1(value_1); \
        var_2 = type_2(value_2); \
        var_3 = type_3(value_3); \
        var_4 = type_4(value_4); \
} G_STMT_END

#define SCAN_4_ARGS_WITH_BLOCK(format, var_1, type_1, var_2, type_2, \
                               var_3, type_3, var_4, type_4) G_STMT_START { \
        VALUE value_1, value_2, value_3, value_4; \
        rb_scan_args(argc, argv, format"&", &value_1, &value_2, &value_3, \
                     &value_4, &block); \
        var_1 = type_1(value_1); \
        var_2 = type_2(value_2); \
        var_3 = type_3(value_3); \
        var_4 = type_4(value_4); \
        SAVE_BLOCK(block); \
} G_STMT_END

#define SCAN_5_ARGS(format, var_1, type_1, var_2, type_2, var_3, type_3, \
                    var_4, type_4, var_5, type_5) G_STMT_START { \
        VALUE value_1, value_2, value_3, value_4, value_5; \
        rb_scan_args(argc, argv, format, &value_1, &value_2, &value_3, \
                     &value_4, &value_5); \
        var_1 = type_1(value_1); \
        var_2 = type_2(value_2); \
        var_3 = type_3(value_3); \
        var_4 = type_4(value_4); \
        var_5 = type_5(value_5); \
} G_STMT_END

#define SCAN_5_ARGS_WITH_BLOCK(format, var_1, type_1, var_2, type_2, \
                               var_3, type_3, var_4, type_4, \
                               var_5, type_5) G_STMT_START { \
        VALUE value_1, value_2, value_3, value_4, value_5; \
        rb_scan_args(argc, argv, format"&", &value_1, &value_2, &value_3, \
                     &value_4, &value_5, &block); \
        var_1 = type_1(value_1); \
        var_2 = type_2(value_2); \
        var_3 = type_3(value_3); \
        var_4 = type_4(value_4); \
        var_5 = type_5(value_5); \
        SAVE_BLOCK(block); \
} G_STMT_END

#define SAVE_BLOCK(block) G_STMT_START { \
        if ((block) != Qnil) \
                G_CHILD_ADD(mGLib, (block)); \
} G_STMT_END

#define USE_BLOCK(data) \
        rbgio_child_remove_and_return(mGLib, (VALUE)(data))

#define USE_BLOCK_AND_SAVE(data) \
        ((VALUE)data)

#define INTERFACE_METHOD(name) \
        rbgio_im_##name

#define DECLARE(name) \
        static VALUE \
        INTERFACE_METHOD(name)

#define DECLARE_VARG(name) \
        DECLARE(name)(int argc, VALUE *argv, VALUE self)

#define DEF_DEF_METHOD(def, module, name, function, args) \
        rb_define_##def((module), name, INTERFACE_METHOD(function), (args))

#define DEF_SINGLETON_METHOD(module, name, args) \
        DEF_DEF_METHOD(singleton_method, module, #name, name, args)

#define DEF_SINGLETON_BOOL_METHOD(module, name, args) \
        DEF_DEF_METHOD(singleton_method, module, #name"?", name, args)

#define DEF_SINGLETON_IS_METHOD(module, name, args) \
        DEF_DEF_METHOD(singleton_method, module, #name"?", is_##name, args)

#define DEF_METHOD(module, name, args) \
        DEF_DEF_METHOD(method, module, #name, name, args)

#define DEF_SET_METHOD(module, name) \
        DEF_METHOD((module), set_##name, 1); \
        G_DEF_SETTER((module), #name)

#define DEF_ATTRIBUTE(module, name) \
        DEF_METHOD((module), name, 0); \
        DEF_SET_METHOD((module), name)

#define DEF_BOOL_METHOD(module, name, args) \
        DEF_DEF_METHOD(method, module, #name"?", name, args)

#define DEF_IS_METHOD(module, name, args) \
        DEF_DEF_METHOD(method, module, #name"?", is_##name, args)

#define DEF_CONSTANTS_CLASS(type, name, prefix, module) \
        G_DEF_CLASS((type), (name), (module)); \
        G_DEF_CONSTANTS((module), (type), (prefix))

#define RVAL2TYPE_WITH_DEFAULT(value, type, default_value) \
        (NIL_P(value) ? (default_value) : type(value))

#define NUM2INT_DEFAULT_MINUS_1(value) \
        RVAL2TYPE_WITH_DEFAULT((value), NUM2INT, -1)

#define RVAL2ATTRIBUTESDEFAULT(value) \
        RVAL2TYPE_WITH_DEFAULT((value), RVAL2CSTR, GIO2_DEFAULT_ATTRIBUTES)

#define RVAL2FSATTRIBUTESDEFAULT(value) \
        RVAL2TYPE_WITH_DEFAULT((value), RVAL2CSTR, "fs::*")

#define RVAL2IOPRIORITYDEFAULT(value) \
        RVAL2TYPE_WITH_DEFAULT((value), NUM2INT, G_PRIORITY_DEFAULT)

#define RVAL2GAPPINFO(value) \
        G_APP_INFO(RVAL2GOBJ(value))

#define RVAL2GCANCELLABLE(value) \
        G_CANCELLABLE(RVAL2GOBJ(value))

#define RVAL2GINPUTSTREAM(value) \
        G_INPUT_STREAM(RVAL2GOBJ(value))

#define RVAL2GFILEINPUTSTREAM(value) \
        G_FILE_INPUT_STREAM(RVAL2GOBJ(value))

#define RVAL2GFILEOUTPUTSTREAM(value) \
        G_FILE_OUTPUT_STREAM(RVAL2GOBJ(value))

#define GFILEATTRIBUTETYPE2RVAL(value) \
        GENUM2RVAL((value), G_TYPE_FILE_ATTRIBUTE_TYPE)

#define RVAL2GFILEATTRIBUTETYPE(value) \
        RVAL2GENUM((value), G_TYPE_FILE_ATTRIBUTE_TYPE)

#define GFILEATTRIBUTEINFOFLAGS2RVAL(value) \
        GFLAGS2RVAL((value), G_TYPE_FILE_ATTRIBUTE_INFO_FLAGS)

#define RVAL2GFILEATTRIBUTEINFOFLAGS(value) \
        RVAL2GFLAGS((value), G_TYPE_FILE_ATTRIBUTE_INFO_FLAGS)

#define RVAL2GFILEATTRIBUTEINFOFLAGSDEFAULT(value) \
        RVAL2TYPE_WITH_DEFAULT((value), \
                               RVAL2GFILEATTRIBUTEINFOFLAGS, \
                               G_FILE_ATTRIBUTE_INFO_NONE)

#define GFILEATTRIBUTESTATUS2RVAL(value) \
        GENUM2RVAL((value), G_TYPE_FILE_ATTRIBUTE_STATUS)

#define RVAL2GFILEINFO(value) \
        G_FILE_INFO(RVAL2GOBJ(value))

#define GFILETYPE2RVAL(value) \
        GENUM2RVAL((value), G_TYPE_FILE_TYPE)

#define RVAL2GFILETYPE(value) \
        RVAL2GENUM((value), G_TYPE_FILE_TYPE)

#define GTIMEVAL2RVAL(value) \
        rb_time_new((value).tv_sec, (value).tv_usec)

#define RVAL2GFILEATTRIBUTEMATCHER(value) \
        ((GFileAttributeMatcher *)(RVAL2BOXED(value, G_TYPE_FILE_ATTRIBUTE_MATCHER)))

#define RVAL2GICON(value) \
        G_ICON(RVAL2GOBJ(value))

#define RVAL2GFILECREATEFLAGS(value) \
        RVAL2GFLAGS((value), G_TYPE_FILE_CREATE_FLAGS)

#define RVAL2GFILECREATEFLAGSDEFAULT(value) \
        RVAL2TYPE_WITH_DEFAULT((value), \
                               RVAL2GFILECREATEFLAGS, \
                               G_FILE_CREATE_NONE)

#define RVAL2GFILEQUERYINFOFLAGS(value) \
        RVAL2GFLAGS((value), G_TYPE_FILE_QUERY_INFO_FLAGS)

#define RVAL2GFILEQUERYINFOFLAGSDEFAULT(value) \
        RVAL2TYPE_WITH_DEFAULT((value), \
                               RVAL2GFILEQUERYINFOFLAGS, \
                               G_FILE_QUERY_INFO_NONE)

#define RVAL2GAPPINFOCREATEFLAGS(value) \
        RVAL2GFLAGS((value), G_TYPE_APP_INFO_CREATE_FLAGS)

#define RVAL2GAPPINFOCREATEFLAGSDEFAULT(value) \
        RVAL2TYPE_WITH_DEFAULT((value), \
                               RVAL2GAPPINFOCREATEFLAGS, \
                               G_APP_INFO_CREATE_NONE)

#define RVAL2GAPPLAUNCHCONTEXT(value) \
        G_APP_LAUNCH_CONTEXT(RVAL2GOBJ(self))

#define RVAL2GMOUNT(value) \
        G_MOUNT(RVAL2GOBJ(value))

#define RVAL2GMOUNTMOUNTFLAGS(value) \
        RVAL2GFLAGS((value), G_TYPE_MOUNT_MOUNT_FLAGS)

#define RVAL2GMOUNTMOUNTFLAGSDEFAULT(value) \
        RVAL2TYPE_WITH_DEFAULT((value), \
                               RVAL2GMOUNTMOUNTFLAGS, \
                               G_MOUNT_MOUNT_NONE)

#define GPASSWORDSAVE2RVAL(value) \
        GENUM2RVAL((value), G_TYPE_PASSWORD_SAVE)

#define RVAL2GPASSWORDSAVE(value) \
        RVAL2GENUM((value), G_TYPE_PASSWORD_SAVE)

#define RVAL2GMOUNTOPERATIONRESULT(value) \
        RVAL2GENUM((value), G_TYPE_MOUNT_OPERATION_RESULT)

#define RVAL2GMOUNTUNMOUNTFLAGS(value) \
        RVAL2GFLAGS((value), G_TYPE_MOUNT_UNMOUNT_FLAGS)

#define RVAL2GMOUNTUNMOUNTFLAGSDEFAULT(value) \
        RVAL2TYPE_WITH_DEFAULT((value), \
                               RVAL2GMOUNTUNMOUNTFLAGS, \
                               G_MOUNT_UNMOUNT_NONE)

#define RVAL2GFILECOPYFLAGS(value) \
        RVAL2GFLAGS((value), G_TYPE_FILE_COPY_FLAGS)

#define RVAL2GFILECOPYFLAGSDEFAULT(value) \
        RVAL2TYPE_WITH_DEFAULT((value), RVAL2GFILECOPYFLAGS, G_FILE_COPY_NONE)

#define G_TYPE_FILE_ATTRIBUTE_INFO_LIST (g_file_attribute_info_list_get_type())
GType g_file_attribute_info_list_get_type();

#define GFILEATTRIBUTEINFOLIST2RVAL(object) \
        BOXED2RVAL(object, G_TYPE_FILE_ATTRIBUTE_INFO_LIST)

#define RVAL2GFILEMONITORFLAGS(value) \
        RVAL2GFLAGS((value), G_TYPE_FILE_MONITOR_FLAGS)

#define RVAL2GFILEMONITORFLAGSDEFAULT(value) \
        RVAL2TYPE_WITH_DEFAULT((value), \
                               RVAL2GFILEMONITORFLAGS, \
                               G_FILE_MONITOR_NONE)

#define RVAL2GSEEKTYPEDEFAULT(value) \
        RVAL2TYPE_WITH_DEFAULT((value), NUM2INT, G_SEEK_CUR)

#define GDATASTREAMBYTEORDER2RVAL(value) \
        RVAL2GENUM((value), G_TYPE_DATA_STREAM_BYTE_ORDER)

#define RVAL2GDATASTREAMBYTEORDER(value) \
        GENUM2RVAL((value), G_TYPE_DATA_STREAM_BYTE_ORDER)

#define GDATASTREAMNEWLINETYPE2RVAL(value) \
        RVAL2GENUM((value), G_TYPE_DATA_STREAM_NEWLINE_TYPE)

#define RVAL2GDATASTREAMNEWLINETYPE(value) \
        GENUM2RVAL((value), G_TYPE_DATA_STREAM_NEWLINE_TYPE)

#define CSTR2RVAL_OWN(string, length) \
        cstr_to_rval_own((string), (length))

#define CSTR2RVAL_OWN_FROZEN(string, length) \
        cstr_to_rval_own_frozen((string), (length))

#define RVAL2GFILE(value) \
        G_FILE(RVAL2GOBJ(value))

#define RVAL2GEMBLEM(value) \
        G_EMBLEM(RVAL2GOBJ(value))

#define RVAL2GASYNCRESULT(value) \
        G_ASYNC_RESULT(RVAL2GOBJ(value))

#define RVAL2GOUTPUTSTREAM(value) \
        G_OUTPUT_STREAM(RVAL2GOBJ(value))

#define RVAL2GOUTPUTSTREAMSPLICEFLAGS(value) \
        RVAL2GFLAGS((value), G_TYPE_OUTPUT_STREAM_SPLICE_FLAGS)


#define RVAL2GOUTPUTSTREAMSPLICEFLAGSDEFAULT(value) \
        RVAL2TYPE_WITH_DEFAULT((value), \
                               RVAL2GFILECOPYFLAGS, \
                               G_OUTPUT_STREAM_SPLICE_NONE)

#define GEMBLEMORIGIN2RVAL(value) \
        GENUM2RVAL((value), G_TYPE_EMBLEM_ORIGIN)

#define RVAL2GEMBLEMORIGIN(value) \
        RVAL2GENUM((value), G_TYPE_EMBLEM_ORIGIN)

#define RVAL2GMOUNTOPERATION(value) \
        G_MOUNT_OPERATION(RVAL2GOBJ(value))

#define RVAL2FILEINFONAMESPACEDEFAULT(value) \
        RVAL2TYPE_WITH_DEFAULT((value), RVAL2CSTR, "*")

#define G_TYPE_FILE_ATTRIBUTE_MATCHER (g_file_attribute_matcher_get_type())
GType g_file_attribute_matcher_get_type(void);

void value_to_gtimeval(VALUE value, GTimeVal *time);
GList *g_file_ary_to_glist(VALUE ary);
int string_ary_to_char_p_ary(VALUE ary, char ***strings);
VALUE str_vector_to_ary(gchar **vector);
VALUE cstr_to_rval_own(char *string, gsize length);
VALUE cstr_to_rval_own_frozen(const char *string, gsize length);
void rbgio_async_ready_callback(GObject *source,
                                GAsyncResult *result,
                                gpointer data);
VALUE rbgio_child_remove_and_return(VALUE parent, VALUE child);
NORETURN(void rbgio_raise_io_error(GError *error));

void Init_gappinfo(VALUE glib);
void Init_gapplaunchcontext(VALUE glib);
void Init_gasyncresult(VALUE glib);
void Init_gbufferedinputstream(VALUE glib);
void Init_gbufferedoutputstream(VALUE glib);
void Init_gcancellable(VALUE glib);
void Init_gcontenttype(VALUE glib);
void Init_gdatainputstream(VALUE glib);
void Init_gdataoutputstream(VALUE glib);
void Init_gdrive(VALUE glib);
void Init_gemblem(VALUE glib);
void Init_gemblemedicon(VALUE glib);
void Init_gfile(VALUE glib);
void Init_gfileattribute(VALUE glib);
void Init_gfileattributematcher(VALUE glib);
void Init_gfileenumerator(VALUE glib);
void Init_gfileicon(VALUE glib);
void Init_gfileinfo(VALUE glib);
void Init_gfileinputstream(VALUE glib);
void Init_gfilemonitor(VALUE glib);
void Init_gfilenamecompleter(VALUE glib);
void Init_gfileoutputstream(VALUE glib);
void Init_gfilterinputstream(VALUE glib);
void Init_gfilteroutputstream(VALUE glib);
void Init_gicon(VALUE glib);
void Init_ginputstream(VALUE glib);
void Init_gioerror(VALUE glib);
void Init_gloadableicon(VALUE glib);
void Init_gmemoryinputstream(VALUE glib);
void Init_gmemoryoutputstream(VALUE glib);
void Init_gmount(VALUE glib);
void Init_gmountoperation(VALUE glib);
void Init_goutputstream(VALUE glib);
void Init_gseekable(VALUE glib);
void Init_gthemedicon(VALUE glib);
void Init_gvolume(VALUE glib);
void Init_gvolumemonitor(VALUE glib);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* GNOMEVFS_H */
