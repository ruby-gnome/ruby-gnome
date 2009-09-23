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

#define _SELF(value) \
        RVAL2GFILE(value)

static VALUE s_id_call;

#define DEFONEARGSINGLETONMETHOD(name, arg_type, type) \
        DECLARE(name)(UNUSED(VALUE self), VALUE value) \
        { \
                return type(g_file_##name(arg_type(value))); \
        }

DEFONEARGSINGLETONMETHOD(new_for_path, RVAL2CSTR, GOBJ2RVAL)
DEFONEARGSINGLETONMETHOD(new_for_uri, RVAL2CSTR, GOBJ2RVAL)
DEFONEARGSINGLETONMETHOD(new_for_commandline_arg, RVAL2CSTR, GOBJ2RVAL)
DEFONEARGSINGLETONMETHOD(parse_name, RVAL2CSTR, GOBJ2RVAL)

#define DEFSIMPLEMETHOD(name, type) \
        DECLARE(name)(VALUE self){ return type(g_file_##name(_SELF(self))); }

DEFSIMPLEMETHOD(dup, GOBJ2RVAL)
DEFSIMPLEMETHOD(hash, UINT2NUM)

DECLARE(equal)(VALUE self, VALUE other)
{
        return CBOOL2RVAL(g_file_equal(_SELF(self), _SELF(other)));
}

#define DEFGETMETHOD(name, type) \
        DECLARE(name)(VALUE self) { return type(g_file_get_##name(_SELF(self))); }

DEFGETMETHOD(basename, CSTR2RVAL_FREE)
DEFGETMETHOD(path, CSTR2RVAL_FREE)
DEFGETMETHOD(uri, CSTR2RVAL_FREE)

DECLARE(get_parse_name)(VALUE self)
{
        return CSTR2RVAL_FREE(g_file_get_parse_name(_SELF(self)));
}

DEFGETMETHOD(parent, GOBJ2RVAL)

DECLARE(get_child)(VALUE self, VALUE name)
{
        return GOBJ2RVAL(g_file_get_child(_SELF(self), RVAL2CSTR(name)));
}

DECLARE(get_child_for_display_name)(VALUE self, VALUE name)
{
        GError *error = NULL;
        GFile *file = g_file_get_child_for_display_name(_SELF(self),
                                                        RVAL2CSTR(name),
                                                        &error);
        if (file == NULL)
                rbgio_raise_io_error(error);

        return GOBJ2RVAL(file);
}

#define DEFONEARGMETHOD(name, arg_type, type) \
        DECLARE(name)(VALUE self, VALUE arg) \
        { \
                return type(g_file_##name(_SELF(self), arg_type(arg))); \
        }

DEFONEARGMETHOD(has_prefix, _SELF, CBOOL2RVAL)
DEFONEARGMETHOD(get_relative_path, _SELF, CSTR2RVAL_FREE)
DEFONEARGMETHOD(resolve_relative_path, RVAL2CSTR, GOBJ2RVAL)

DEFSIMPLEMETHOD(is_native, CBOOL2RVAL)

DEFONEARGMETHOD(has_uri_scheme, RVAL2CSTR, CBOOL2RVAL)

DEFGETMETHOD(uri_scheme, CSTR2RVAL_FREE)

DECLARE_VARG(read)
{
        GCancellable *cancellable;
        GError *error = NULL;
        VALUE result;

        SCAN_1_ARG("01", cancellable, RVAL2GCANCELLABLE);

        GFileInputStream *stream = g_file_read(_SELF(self),
                                               cancellable,
                                               &error);
        if (stream == NULL)
                rbgio_raise_io_error(error);

        if (!rb_block_given_p())
                return GOBJ2RVAL(stream);

        result = rb_yield(GOBJ2RVAL(stream));

        if (!g_input_stream_close(G_INPUT_STREAM(stream), cancellable, &error))
                rbgio_raise_io_error(error);

        return result;
}

#define DEFOBJECTFINISH(name, type) \
        DECLARE(name##_finish)(VALUE self, VALUE result) \
        { \
                GError *error = NULL; \
                type *object = g_file_##name##_finish(_SELF(self), \
                                                      RVAL2GASYNCRESULT(result), \
                                                      &error); \
                if (object == NULL) \
                        rbgio_raise_io_error(error); \
                return GOBJ2RVAL(object); \
        }

DECLARE_VARG(read_async)
{
        int io_priority;
        GCancellable *cancellable;
        VALUE block;

        SCAN_2_ARGS_WITH_BLOCK("02",
                               io_priority, RVAL2IOPRIORITYDEFAULT,
                               cancellable, RVAL2GCANCELLABLE);

        g_file_read_async(_SELF(self),
                          io_priority,
                          cancellable,
                          rbgio_async_ready_callback,
                          (gpointer)block);

        return self;
}

DEFOBJECTFINISH(read, GFileInputStream)

DECLARE_VARG(append_to)
{
        GFileCreateFlags flags;
        GCancellable *cancellable;
        GError *error = NULL;
        GFileOutputStream *stream;
        VALUE result;

        SCAN_2_ARGS("02",
                    flags, RVAL2GFILECREATEFLAGSDEFAULT,
                    cancellable, RVAL2GCANCELLABLE);

        stream = g_file_append_to(_SELF(self), flags, cancellable, &error);
        if (stream == NULL)
                rbgio_raise_io_error(error);

        if (!rb_block_given_p())
                return GOBJ2RVAL(stream);

        result = rb_yield(GOBJ2RVAL(stream));

        if (!g_output_stream_close(G_OUTPUT_STREAM(stream),
                                   cancellable,
                                   &error))
                rbgio_raise_io_error(error);

        return result;
}

DECLARE_VARG(create)
{
        GFileCreateFlags flags;
        GCancellable *cancellable;
        GError *error = NULL;
        GFileOutputStream *stream;
        VALUE result;

        SCAN_2_ARGS("02",
                    flags, RVAL2GFILECREATEFLAGSDEFAULT,
                    cancellable, RVAL2GCANCELLABLE);

        stream = g_file_create(_SELF(self), flags, cancellable, &error);
        if (stream == NULL)
                rbgio_raise_io_error(error);

        if (!rb_block_given_p())
                return GOBJ2RVAL(stream);

        result = rb_yield(GOBJ2RVAL(stream));

        if (!g_output_stream_close(G_OUTPUT_STREAM(stream),
                                   cancellable,
                                   &error))
                rbgio_raise_io_error(error);

        return result;
}

DECLARE_VARG(replace)
{
        const char *etag;
        gboolean make_backup;
        GFileCreateFlags flags;
        GCancellable *cancellable;
        GError *error = NULL;
        GFileOutputStream *stream;
        VALUE result;

        SCAN_4_ARGS("04",
                    etag, RVAL2CSTR_ACCEPT_NIL,
                    make_backup, RVAL2CBOOL,
                    flags, RVAL2GFILECREATEFLAGSDEFAULT,
                    cancellable, RVAL2GCANCELLABLE);

        stream = g_file_replace(_SELF(self),
                                etag,
                                make_backup,
                                flags,
                                cancellable,
                                &error);
        if (stream == NULL)
                rbgio_raise_io_error(error);

        if (!rb_block_given_p())
                return GOBJ2RVAL(stream);

        result = rb_yield(GOBJ2RVAL(stream));

        if (!g_output_stream_close(G_OUTPUT_STREAM(stream),
                                   cancellable,
                                   &error))
                rbgio_raise_io_error(error);

        return result;
}

DECLARE_VARG(append_to_async)
{
        GFileCreateFlags flags;
        int io_priority;
        GCancellable *cancellable;
        VALUE block;

        SCAN_3_ARGS_WITH_BLOCK("03",
                               flags, RVAL2GFILECREATEFLAGSDEFAULT,
                               io_priority, RVAL2IOPRIORITYDEFAULT,
                               cancellable, RVAL2GCANCELLABLE);

        g_file_append_to_async(_SELF(self),
                               flags,
                               io_priority,
                               cancellable,
                               rbgio_async_ready_callback,
                               (gpointer)block);

        return self;
}

DEFOBJECTFINISH(append_to, GFileOutputStream)

DECLARE_VARG(create_async)
{
        GFileCreateFlags flags;
        int io_priority;
        GCancellable *cancellable;
        VALUE block;

        SCAN_3_ARGS_WITH_BLOCK("03",
                               flags, RVAL2GFILECREATEFLAGSDEFAULT,
                               io_priority, RVAL2IOPRIORITYDEFAULT,
                               cancellable, RVAL2GCANCELLABLE);

        g_file_create_async(_SELF(self),
                            flags,
                            io_priority,
                            cancellable,
                            rbgio_async_ready_callback,
                            (gpointer)block);

        return self;
}

DEFOBJECTFINISH(create, GFileOutputStream)

DECLARE_VARG(replace_async)
{
        const char *etag;
        gboolean make_backup;
        GFileCreateFlags flags;
        int io_priority;
        GCancellable *cancellable;
        VALUE block;

        SCAN_5_ARGS_WITH_BLOCK("05",
                               etag, RVAL2CSTR_ACCEPT_NIL,
                               make_backup, RVAL2CBOOL,
                               flags, RVAL2GFILECREATEFLAGSDEFAULT,
                               io_priority, RVAL2IOPRIORITYDEFAULT,
                               cancellable, RVAL2GCANCELLABLE);

        g_file_replace_async(_SELF(self),
                             etag,
                             make_backup,
                             flags,
                             io_priority,
                             cancellable,
                             rbgio_async_ready_callback,
                             (gpointer)block);

        return self;
}

DEFOBJECTFINISH(replace, GFileOutputStream)

DECLARE_VARG(query_info)
{
        const char *attributes;
        GFileQueryInfoFlags flags;
        GCancellable *cancellable;
        GError *error = NULL;
        GFileInfo *info;

        SCAN_3_ARGS("03",
                    attributes, RVAL2ATTRIBUTESDEFAULT,
                    flags, RVAL2GFILEQUERYINFOFLAGSDEFAULT,
                    cancellable, RVAL2GCANCELLABLE);

        info = g_file_query_info(_SELF(self),
                                 attributes,
                                 flags,
                                 cancellable,
                                 &error);
        if (info == NULL)
                rbgio_raise_io_error(error);

        return GOBJ2RVAL(info);
}

DECLARE_VARG(query_info_async)
{
        const char *attributes;
        GFileQueryInfoFlags flags;
        int io_priority;
        GCancellable *cancellable;
        VALUE block;

        SCAN_4_ARGS_WITH_BLOCK("04",
                               attributes, RVAL2ATTRIBUTESDEFAULT,
                               flags, RVAL2GFILEQUERYINFOFLAGSDEFAULT,
                               io_priority, RVAL2IOPRIORITYDEFAULT,
                               cancellable, RVAL2GCANCELLABLE);

        g_file_query_info_async(_SELF(self),
                                attributes,
                                flags,
                                io_priority,
                                cancellable,
                                rbgio_async_ready_callback,
                                (gpointer)block);

        return self;
}

DEFOBJECTFINISH(query_info, GFileInfo)

DECLARE_VARG(query_exists)
{
        GCancellable *cancellable;

        SCAN_1_ARG("01", cancellable, RVAL2GCANCELLABLE);

        return CBOOL2RVAL(g_file_query_exists(_SELF(self), cancellable));
}

DECLARE_VARG(query_file_type)
{
        GFileQueryInfoFlags flags;
        GCancellable *cancellable;

        SCAN_2_ARGS("02",
                    flags, RVAL2GFILEQUERYINFOFLAGSDEFAULT,
                    cancellable, RVAL2GCANCELLABLE);

        return GFILETYPE2RVAL(g_file_query_file_type(_SELF(self),
                                                     flags,
                                                     cancellable));
}

DECLARE_VARG(query_filesystem_info)
{
        const char *attributes;
        GCancellable *cancellable;
        GError *error = NULL;
        GFileInfo *info;

        SCAN_2_ARGS("02",
                    attributes, RVAL2FSATTRIBUTESDEFAULT,
                    cancellable, RVAL2GCANCELLABLE);

        info = g_file_query_filesystem_info(_SELF(self),
                                            attributes,
                                            cancellable,
                                            &error);
        if (info == NULL)
                rbgio_raise_io_error(error);

        return GOBJ2RVAL(info);
}

DECLARE_VARG(query_filesystem_info_async)
{
        const char *attributes;
        int io_priority;
        GCancellable *cancellable;
        VALUE block;

        SCAN_3_ARGS_WITH_BLOCK("03",
                               attributes, RVAL2FSATTRIBUTESDEFAULT,
                               io_priority, RVAL2IOPRIORITYDEFAULT,
                               cancellable, RVAL2GCANCELLABLE);

        g_file_query_filesystem_info_async(_SELF(self),
                                           attributes,
                                           io_priority,
                                           cancellable,
                                           rbgio_async_ready_callback,
                                           (gpointer)block);

        return self;
}

DEFOBJECTFINISH(query_filesystem_info, GFileInfo)

DECLARE_VARG(query_default_handler)
{
        GCancellable *cancellable;
        GError *error = NULL;
        GAppInfo *info;

        SCAN_1_ARG("01", cancellable, RVAL2GCANCELLABLE);

        info = g_file_query_default_handler(_SELF(self), cancellable, &error);
        if (info == NULL)
                rbgio_raise_io_error(error);

        return GOBJ2RVAL(info);
}

DECLARE_VARG(find_enclosing_mount)
{
        GCancellable *cancellable;
        GError *error = NULL;
        GMount *mount;

        SCAN_1_ARG("01", cancellable, RVAL2GCANCELLABLE);

        mount = g_file_find_enclosing_mount(_SELF(self), cancellable, &error);
        if (mount == NULL)
                rbgio_raise_io_error(error);

        return GOBJ2RVAL(mount);
}

DECLARE_VARG(find_enclosing_mount_async)
{
        int io_priority;
        GCancellable *cancellable;
        VALUE block;

        SCAN_2_ARGS_WITH_BLOCK("02",
                               io_priority, RVAL2IOPRIORITYDEFAULT,
                               cancellable, RVAL2GCANCELLABLE);

        g_file_find_enclosing_mount_async(_SELF(self),
                                          io_priority,
                                          cancellable,
                                          rbgio_async_ready_callback,
                                          (gpointer)block);

        return self;
}

DEFOBJECTFINISH(find_enclosing_mount, GMount)

DECLARE_VARG(enumerate_children)
{
        const char *attributes;
        GFileQueryInfoFlags flags;
        GCancellable *cancellable;
        GError *error = NULL;
        GFileEnumerator *enumerator;

        SCAN_3_ARGS("03",
                    attributes, RVAL2ATTRIBUTESDEFAULT,
                    flags, RVAL2GFILEQUERYINFOFLAGSDEFAULT,
                    cancellable, RVAL2GCANCELLABLE);

        enumerator = g_file_enumerate_children(_SELF(self),
                                               attributes,
                                               flags,
                                               cancellable,
                                               &error);
        if (enumerator == NULL)
                rbgio_raise_io_error(error);

        if (!rb_block_given_p())
                return GOBJ2RVAL(enumerator);

        while (TRUE) {
                GFileInfo *info = g_file_enumerator_next_file(enumerator,
                                                              cancellable,
                                                              &error);
                if (error != NULL) {
                        g_file_enumerator_close(enumerator, cancellable, NULL);
                        g_object_unref(enumerator);
                        rbgio_raise_io_error(error);
                }

                if (info == NULL)
                        break;

                rb_yield(GOBJ2RVAL(info));
        }

        if (!g_file_enumerator_close(enumerator, cancellable, &error)) {
                g_object_unref(enumerator);
                rbgio_raise_io_error(error);
        }

        g_object_unref(enumerator);

        return self;
}

DECLARE_VARG(enumerate_children_async)
{
        const char *attributes;
        GFileQueryInfoFlags flags;
        int io_priority;
        GCancellable *cancellable;
        VALUE block;

        SCAN_4_ARGS_WITH_BLOCK("04",
                               attributes, RVAL2ATTRIBUTESDEFAULT,
                               flags, RVAL2GFILEQUERYINFOFLAGSDEFAULT,
                               io_priority, RVAL2IOPRIORITYDEFAULT,
                               cancellable, RVAL2GCANCELLABLE);

        g_file_enumerate_children_async(_SELF(self),
                                        attributes,
                                        flags,
                                        io_priority,
                                        cancellable,
                                        rbgio_async_ready_callback,
                                        (gpointer)block);

        return self;
}

DEFOBJECTFINISH(enumerate_children, GFileEnumerator)

DECLARE_VARG(set_display_name)
{
        const char *display_name;
        GCancellable *cancellable;
        GError *error = NULL;
        GFile *file;

        SCAN_2_ARGS("11",
                    display_name, RVAL2CSTR,
                    cancellable, RVAL2GCANCELLABLE);

        file = g_file_set_display_name(_SELF(self),
                                       display_name,
                                       cancellable,
                                       &error);
        if (file == NULL)
                rbgio_raise_io_error(error);

        return GOBJ2RVAL(file);
}

DECLARE_VARG(set_display_name_async)
{
        const char *display_name;
        int io_priority;
        GCancellable *cancellable;
        VALUE block;

        SCAN_3_ARGS_WITH_BLOCK("12",
                               display_name, RVAL2CSTR,
                               io_priority, RVAL2IOPRIORITYDEFAULT,
                               cancellable, RVAL2GCANCELLABLE);

        g_file_set_display_name_async(_SELF(self),
                                      display_name,
                                      io_priority,
                                      cancellable,
                                      rbgio_async_ready_callback,
                                      (gpointer)block);

        return self;
}

DEFOBJECTFINISH(set_display_name, GFile)

typedef gboolean (*CancellableMethod)(GFile *, GCancellable *, GError **);

static VALUE
cancellable_method(CancellableMethod method, int argc, VALUE *argv, VALUE self)
{
        GCancellable *cancellable;
        GError *error = NULL;

        SCAN_1_ARG("01", cancellable, RVAL2GCANCELLABLE);

        if (!method(_SELF(self), cancellable, &error))
                rbgio_raise_io_error(error);

        return self;
}

#define DEFCANCELLABLEMETHOD(name) \
        DECLARE_VARG(name) \
        { \
                return cancellable_method(g_file_##name, argc, argv, self); \
        }

DEFCANCELLABLEMETHOD(delete)
DEFCANCELLABLEMETHOD(trash)

static void
progress_callback(goffset current_num_bytes,
                  goffset total_num_bytes,
                  gpointer data)
{
        VALUE block = USE_BLOCK_AND_SAVE(data);

        /* TODO: Should we G_PROTECT_CALLBACK here? */
        if (!NIL_P(block))
                rb_funcall(block, s_id_call, 2,
                           rbglib_int64_to_num(current_num_bytes),
                           rbglib_int64_to_num(total_num_bytes));
}

DECLARE_VARG(copy)
{
        GFile *destination;
        GFileCopyFlags flags;
        GCancellable *cancellable;
        GFileProgressCallback callback = NULL;
        GError *error = NULL;
        VALUE block;

        SCAN_3_ARGS_WITH_BLOCK("12",
                               destination, _SELF,
                               flags, RVAL2GFILECOPYFLAGSDEFAULT,
                               cancellable, RVAL2GCANCELLABLE);

        if (!NIL_P(block))
                callback = progress_callback;

        if (!g_file_copy(_SELF(self),
                         destination,
                         flags,
                         cancellable,
                         callback,
                         (gpointer)block,
                         &error))
                rbgio_raise_io_error(error);

        return self;
}

DECLARE_VARG(copy_async)
{
        GFile *destination;
        GFileCopyFlags flags;
        int io_priority;
        GCancellable *cancellable;
        GFileProgressCallback callback = NULL;
        VALUE block;

        SCAN_4_ARGS_WITH_BLOCK("13",
                               destination, _SELF,
                               flags, RVAL2GFILECOPYFLAGSDEFAULT,
                               io_priority, RVAL2IOPRIORITYDEFAULT,
                               cancellable, RVAL2GCANCELLABLE);

        if (block != Qnil)
                callback = progress_callback;

        /* NOTE: This requires the block to take a variable number of
         * arguments. It will be two integer values for the progress part and
         * a GLib::AsyncResult when done. */
        g_file_copy_async(_SELF(self),
                          destination,
                          flags,
                          io_priority,
                          cancellable,
                          callback,
                          (gpointer)block,
                          rbgio_async_ready_callback,
                          (gpointer)block);

        return self;
}

DECLARE(copy_finish)(VALUE self, VALUE result)
{
        GError *error = NULL;
        if (!g_file_copy_finish(_SELF(self), RVAL2GASYNCRESULT(result), &error))
                rbgio_raise_io_error(error);
        return self;
}

DECLARE_VARG(move)
{
        GFile *destination;
        GFileCopyFlags flags;
        GCancellable *cancellable;
        GFileProgressCallback callback = NULL;
        GError *error = NULL;
        VALUE block;

        SCAN_3_ARGS_WITH_BLOCK("12",
                               destination, _SELF,
                               flags, RVAL2GFILECOPYFLAGSDEFAULT,
                               cancellable, RVAL2GCANCELLABLE);

        if (!NIL_P(block))
                callback = progress_callback;

        if (!g_file_move(_SELF(self),
                         destination,
                         flags,
                         cancellable,
                         callback,
                         (gpointer)block,
                         &error))
                rbgio_raise_io_error(error);

        return self;
}

DEFCANCELLABLEMETHOD(make_directory)
DEFCANCELLABLEMETHOD(make_directory_with_parents)

DECLARE_VARG(make_symbolic_link)
{
        const char *symlink_value;
        GCancellable *cancellable;
        GError *error = NULL;

        SCAN_2_ARGS("11",
                    symlink_value, RVAL2CSTR,
                    cancellable, RVAL2GCANCELLABLE);

        if (!g_file_make_symbolic_link(_SELF(self),
                                       symlink_value,
                                       cancellable,
                                       &error))
                rbgio_raise_io_error(error);

        return self;
}

DECLARE_VARG(query_settable_attributes)
{
        GCancellable *cancellable;
        GError *error = NULL;
        GFileAttributeInfoList *list;

        SCAN_1_ARG("01", cancellable, RVAL2GCANCELLABLE);

        list = g_file_query_settable_attributes(_SELF(self),
                                                cancellable,
                                                &error);
        if (list == NULL)
                rbgio_raise_io_error(error);

        return GFILEATTRIBUTEINFOLIST2RVAL(list);
}

DECLARE_VARG(query_writable_namespaces)
{
        GCancellable *cancellable;
        GError *error = NULL;
        GFileAttributeInfoList *list;

        SCAN_1_ARG("01", cancellable, RVAL2GCANCELLABLE);

        list = g_file_query_writable_namespaces(_SELF(self),
                                                cancellable,
                                                &error);
        if (list == NULL)
                rbgio_raise_io_error(error);

        return GFILEATTRIBUTEINFOLIST2RVAL(list);
}

DECLARE_VARG(set_attribute)
{
        VALUE rbattribute,
              rbtype,
              rbvalue,
              rbflags,
              rbcancellable;
        const char *attribute;
        GFileAttributeType type;
        gpointer value;
        gboolean gboolean_value;
        guint32 guint32_value;
        gint32 gint32_value;
        guint64 guint64_value;
        gint64 gint64_value;
        GFileQueryInfoFlags flags;
        GCancellable *cancellable;
        GError *error = NULL;

        rb_scan_args(argc, argv, "32",
                     &rbattribute, &rbtype, &rbvalue, &rbflags, &rbcancellable);
        attribute = RVAL2CSTR(rbattribute);
        type = RVAL2GFILEATTRIBUTETYPE(rbtype);

        switch (type) {
        case G_FILE_ATTRIBUTE_TYPE_STRING:
        case G_FILE_ATTRIBUTE_TYPE_BYTE_STRING:
                value = RVAL2CSTR(rbvalue);
                break;
        case G_FILE_ATTRIBUTE_TYPE_BOOLEAN:
                gboolean_value = RVAL2CBOOL(rbvalue);
                value = &gboolean_value;
                break;
        case G_FILE_ATTRIBUTE_TYPE_UINT32:
                guint32_value = NUM2UINT(rbvalue);
                value = &guint32_value;
                break;
        case G_FILE_ATTRIBUTE_TYPE_INT32:
                gint32_value = NUM2INT(rbvalue);
                value = &gint32_value;
                break;
        case G_FILE_ATTRIBUTE_TYPE_UINT64:
                guint64_value = rbglib_num_to_uint64(rbvalue);
                value = &guint64_value;
                break;
        case G_FILE_ATTRIBUTE_TYPE_INT64:
                gint64_value = rbglib_num_to_int64(rbvalue);
                value = &gint64_value;
                break;
        case G_FILE_ATTRIBUTE_TYPE_OBJECT:
                value = RVAL2GOBJ(rbvalue);
                break;
        case G_FILE_ATTRIBUTE_TYPE_INVALID:
        default:
                rb_raise(rb_eArgError, "Unknown file attribute type: %d", type);
        }

        flags = RVAL2GFILEQUERYINFOFLAGSDEFAULT(rbflags);
        cancellable = RVAL2GCANCELLABLE(rbcancellable);

        if (!g_file_set_attribute(_SELF(self),
                                  attribute,
                                  type,
                                  value,
                                  flags,
                                  cancellable,
                                  &error))
                rbgio_raise_io_error(error);

        return self;
}

DECLARE_VARG(set_attributes_from_info)
{
        GFileInfo *info;
        GFileQueryInfoFlags flags;
        GCancellable *cancellable;
        GError *error = NULL;

        SCAN_3_ARGS("12",
                    info, RVAL2GFILEINFO,
                    flags, RVAL2GFILEQUERYINFOFLAGSDEFAULT,
                    cancellable, RVAL2GCANCELLABLE);

        if (!g_file_set_attributes_from_info(_SELF(self),
                                             info,
                                             flags,
                                             cancellable,
                                             &error))
                rbgio_raise_io_error(error);

        return self;
}

DECLARE_VARG(set_attributes_async)
{
        GFileInfo *info;
        GFileQueryInfoFlags flags;
        int io_priority;
        GCancellable *cancellable;
        VALUE block;

        SCAN_4_ARGS_WITH_BLOCK("13",
                               info, RVAL2GFILEINFO,
                               flags, RVAL2GFILEQUERYINFOFLAGSDEFAULT,
                               io_priority, RVAL2IOPRIORITYDEFAULT,
                               cancellable, RVAL2GCANCELLABLE);

        g_file_set_attributes_async(_SELF(self),
                                    info,
                                    flags,
                                    io_priority,
                                    cancellable,
                                    rbgio_async_ready_callback,
                                    (gpointer)block);

        return self;
}

DECLARE(set_attributes_finish)(VALUE self, VALUE result)
{
        GFileInfo *info;
        GError *error = NULL;
        if (!g_file_set_attributes_finish(_SELF(self),
                                          RVAL2GASYNCRESULT(result),
                                          &info,
                                          &error))
                rbgio_raise_io_error(error);

        return GOBJ2RVAL(info);
}

#define DEFSETATTRMETHOD(name, c_type, type) \
        DECLARE_VARG(name) \
        { \
                const char *attribute; \
                c_type value; \
                GFileQueryInfoFlags flags; \
                GCancellable *cancellable; \
                GError *error = NULL; \
                SCAN_4_ARGS("22", \
                            attribute, RVAL2CSTR, \
                            value, type, \
                            flags, RVAL2GFILEQUERYINFOFLAGSDEFAULT, \
                            cancellable, RVAL2GCANCELLABLE); \
                if (!g_file_##name(_SELF(self), \
                                   attribute, \
                                   value, \
                                   flags, \
                                   cancellable, \
                                   &error)) \
                        rbgio_raise_io_error(error); \
                return self; \
        }

DEFSETATTRMETHOD(set_attribute_string, const char *, RVAL2CSTR)
DEFSETATTRMETHOD(set_attribute_byte_string, const char *, RVAL2CSTR)
DEFSETATTRMETHOD(set_attribute_uint32, guint32, NUM2UINT)
DEFSETATTRMETHOD(set_attribute_int32, gint32, NUM2INT)
DEFSETATTRMETHOD(set_attribute_uint64, guint64, rbglib_num_to_uint64)
DEFSETATTRMETHOD(set_attribute_int64, gint64, rbglib_num_to_int64)

#if 0
static VALUE
set_attribute_value(int argc, VALUE *argv, VALUE self)
{
        VALUE rbattribute,
              rbvalue,
              rbflags,
              rbcancellable;
        const char *attribute;
        GFileAttributeType type;
        gpointer value;
        GFileQueryInfoFlags flags;
        GCancellable *cancellable;
        GError *error = NULL;

        rb_scan_args(argc, argv, "22", &rbattribute, &rbvalue, &rbflags,
                     &rbcancellable);
        attribute = RVAL2CSTR(rbattribute);
        flags = NIL_P(rbflags) ? G_FILE_QUERY_INFO_NONE : RVAL2GFILEQUERYINFOFLAGS(rbflags);
        cancellable = RVAL2GCANCELLABLE(rbcancellable);

        switch (TYPE(rbvalue)) {
        case T_STRING:
                /* TODO: How do we decide if it’s a TYPE_BYTE_STRING or a
                 * TYPE_STRING? */
                type = G_FILE_ATTRIBUTE_TYPE_STRING;
                value = RVAL2CSTR(rbvalue);
                break;
        case T_TRUE:
        case T_FALSE:
                type = G_FILE_ATTRIBUTE_TYPE_BOOLEAN:
                value = &RVAL2CBOOL(rbvalue);
                break;
                case T_
        case G_FILE_ATTRIBUTE_TYPE_UINT32:
                value = &NUM2UINT(rbvalue);
                break;
        case G_FILE_ATTRIBUTE_TYPE_INT32:
                value = &NUM2INT(rbvalue);
                break;
        case G_FILE_ATTRIBUTE_TYPE_UINT64:
                value = &rbglib_num_to_uint64(rbvalue);
                break;
        case G_FILE_ATTRIBUTE_TYPE_INT64:
                value = &rbglib_num_to_int64(rbvalue);
                break;
        default:
                rb_raise(rb_eArgError, "Cannot determine file attribute type");
        }

        if (!g_file_set_attribute(_SELF(self), attribute, type, value, flags,
                                  cancellable, &error))
                rbgio_raise_io_error(error);

        return self;
}
#endif

DECLARE_VARG(mount_mountable)
{
        GMountMountFlags flags;
        GMountOperation *mount_operation;
        GCancellable *cancellable;
        VALUE block;

        SCAN_3_ARGS_WITH_BLOCK("03",
                               flags, RVAL2GMOUNTMOUNTFLAGSDEFAULT,
                               mount_operation, RVAL2GMOUNTOPERATION,
                               cancellable, RVAL2GCANCELLABLE);

        g_file_mount_mountable(_SELF(self),
                               flags,
                               mount_operation,
                               cancellable,
                               rbgio_async_ready_callback,
                               (gpointer)block);

        return self;
}

DEFOBJECTFINISH(mount_mountable, GFile)

#define DEFTHUNKFINISH(name) \
        DECLARE(name##_finish)(VALUE self, VALUE result) \
        { \
                GError *error = NULL; \
                if (!g_file_##name##_finish(_SELF(self), \
                                            RVAL2GASYNCRESULT(result), \
                                            &error)) \
                        rbgio_raise_io_error(error); \
                return self; \
        }

DECLARE_VARG(unmount_mountable)
{
        GMountUnmountFlags flags;
        GCancellable *cancellable;
        VALUE block;

        SCAN_2_ARGS_WITH_BLOCK("02",
                               flags, RVAL2GMOUNTUNMOUNTFLAGSDEFAULT,
                               cancellable, RVAL2GCANCELLABLE);

        g_file_unmount_mountable(_SELF(self),
                                 flags,
                                 cancellable,
                                 rbgio_async_ready_callback,
                                 (gpointer)block);

        return self;
}

DEFTHUNKFINISH(unmount_mountable)

DECLARE_VARG(eject_mountable)
{
        GMountUnmountFlags flags;
        GCancellable *cancellable;
        VALUE block;

        SCAN_2_ARGS_WITH_BLOCK("02",
                               flags, RVAL2GMOUNTUNMOUNTFLAGSDEFAULT,
                               cancellable, RVAL2GCANCELLABLE);

        g_file_eject_mountable(_SELF(self),
                               flags,
                               cancellable,
                               rbgio_async_ready_callback,
                               (gpointer)block);

        return self;
}

DEFTHUNKFINISH(eject_mountable)

DECLARE_VARG(mount_enclosing_volume)
{
        GMountMountFlags flags;
        GMountOperation *mount_operation;
        GCancellable *cancellable;
        VALUE block;

        SCAN_3_ARGS_WITH_BLOCK("03",
                               flags, RVAL2GMOUNTMOUNTFLAGSDEFAULT,
                               mount_operation, RVAL2GMOUNTOPERATION,
                               cancellable, RVAL2GCANCELLABLE);

        g_file_mount_enclosing_volume(_SELF(self),
                                      flags,
                                      mount_operation,
                                      cancellable,
                                      rbgio_async_ready_callback,
                                      (gpointer)block);

        return self;
}

DEFTHUNKFINISH(mount_enclosing_volume)

DECLARE_VARG(monitor_directory)
{
        GFileMonitorFlags flags;
        GCancellable *cancellable;
        GError *error = NULL;
        GFileMonitor *monitor;

        SCAN_2_ARGS("02",
                    flags, RVAL2GFILEMONITORFLAGSDEFAULT,
                    cancellable, RVAL2GCANCELLABLE);

        monitor = g_file_monitor_directory(_SELF(self),
                                           flags,
                                           cancellable,
                                           &error);
        if (monitor == NULL)
                rbgio_raise_io_error(error);

        return GOBJ2RVAL(monitor);
}

DECLARE_VARG(monitor_file)
{
        GFileMonitorFlags flags;
        GCancellable *cancellable;
        GError *error = NULL;
        GFileMonitor *monitor;

        SCAN_2_ARGS("02",
                    flags, RVAL2GFILEMONITORFLAGSDEFAULT,
                    cancellable, RVAL2GCANCELLABLE);

        monitor = g_file_monitor_file(_SELF(self), flags, cancellable, &error);
        if (monitor == NULL)
                rbgio_raise_io_error(error);

        return GOBJ2RVAL(monitor);
}

DECLARE_VARG(monitor)
{
        GFileMonitorFlags flags;
        GCancellable *cancellable;
        GError *error = NULL;
        GFileMonitor *monitor;

        SCAN_2_ARGS("02",
                    flags, RVAL2GFILEMONITORFLAGSDEFAULT,
                    cancellable, RVAL2GCANCELLABLE);

        monitor = g_file_monitor(_SELF(self), flags, cancellable, &error);
        if (monitor == NULL)
                rbgio_raise_io_error(error);

        return GOBJ2RVAL(monitor);
}

DECLARE_VARG(load_contents)
{
        GCancellable *cancellable;
        char *contents;
        gsize length;
        char *etag_out;
        GError *error = NULL;
        VALUE rbcontents,
              rbetag;

        SCAN_1_ARG("01", cancellable, RVAL2GCANCELLABLE);

        if (!g_file_load_contents(_SELF(self),
                                  cancellable,
                                  &contents,
                                  &length,
                                  &etag_out,
                                  &error))
                rbgio_raise_io_error(error);

        rbcontents = rb_str_new(contents, length);
        g_free(contents);
        rbetag = CSTR2RVAL_FREE(etag_out);

        return rb_assoc_new(rbcontents, rbetag);
}

DECLARE_VARG(load_contents_async)
{
        GCancellable *cancellable;
        VALUE block;

        SCAN_1_ARG_WITH_BLOCK("01", cancellable, RVAL2GCANCELLABLE);

        g_file_load_contents_async(_SELF(self),
                                   cancellable,
                                   rbgio_async_ready_callback,
                                   (gpointer)block);

        return self;
}

DECLARE(load_contents_finish)(VALUE self, VALUE result)
{
        char *contents;
        gsize length;
        char *etag_out;
        GError *error = NULL;

        if (!g_file_load_contents_finish(_SELF(self),
                                         RVAL2GASYNCRESULT(result),
                                         &contents,
                                         &length,
                                         &etag_out,
                                         &error))
                rbgio_raise_io_error(error);

        return rb_assoc_new(CSTR2RVAL_OWN(contents, length),
                            CSTR2RVAL_FREE(etag_out));
}

static gboolean
load_contents_async_read_more_callback(const char *file_contents,
                                       goffset file_size,
                                       gpointer data)
{
        VALUE block = USE_BLOCK_AND_SAVE(data);

        if (!NIL_P(block))
                /* TODO: Should we G_PROTECT_CALLBACK here? */
                return RVAL2CBOOL(rb_funcall(block, s_id_call, 1,
                                             CSTR2RVAL_OWN_FROZEN(file_contents,
                                                                  file_size)));

        return TRUE;
}

DECLARE_VARG(load_contents_partial_async)
{
        GCancellable *cancellable;
        gboolean use_read_more_callback;
        VALUE block;

        SCAN_2_ARGS_WITH_BLOCK("02",
                               cancellable, RVAL2GCANCELLABLE,
                               use_read_more_callback, RVAL2CBOOL);

        /* TODO: Should we force a block if use_read_more_callback is true? */
        g_file_load_partial_contents_async(_SELF(self),
                                           cancellable,
                                           use_read_more_callback ?
                                                load_contents_async_read_more_callback :
                                                NULL,
                                           rbgio_async_ready_callback,
                                           (gpointer)block);

        return self;
}

DECLARE(load_contents_partial_finish)(VALUE self, VALUE result)
{
        char *contents;
        gsize length;
        char *etag_out;
        GError *error = NULL;

        if (!g_file_load_partial_contents_finish(_SELF(self),
                                                 RVAL2GASYNCRESULT(result),
                                                 &contents,
                                                 &length,
                                                 &etag_out,
                                                 &error))
                rbgio_raise_io_error(error);

        return rb_assoc_new(CSTR2RVAL_OWN(contents, length),
                            CSTR2RVAL_FREE(etag_out));
}

DECLARE_VARG(replace_contents)
{
        VALUE rbcontents;
        const char *contents;
        gsize length;
        const char *etag;
        gboolean make_backup;
        GFileCreateFlags flags;
        char *new_etag;
        GCancellable *cancellable;
        GError *error = NULL;

        SCAN_5_ARGS("14",
                    rbcontents, (VALUE),
                    etag, RVAL2CSTR_ACCEPT_NIL,
                    make_backup, RVAL2CBOOL,
                    flags, RVAL2GFILECREATEFLAGSDEFAULT,
                    cancellable, RVAL2GCANCELLABLE);
        contents = RVAL2CSTR(rbcontents);
        length = RSTRING_LEN(rbcontents);

        if (!g_file_replace_contents(_SELF(self),
                                     contents,
                                     length,
                                     etag,
                                     make_backup,
                                     flags,
                                     &new_etag,
                                     cancellable,
                                     &error))
                rbgio_raise_io_error(error);

        return CSTR2RVAL_FREE(new_etag);
}

DECLARE_VARG(replace_contents_async)
{
        VALUE rbcontents;
        const char *contents;
        gsize length;
        const char *etag;
        gboolean make_backup;
        GFileCreateFlags flags;
        GCancellable *cancellable;
        VALUE block;

        SCAN_5_ARGS_WITH_BLOCK("14",
                               rbcontents, (VALUE),
                               etag, RVAL2CSTR_ACCEPT_NIL,
                               make_backup, RVAL2CBOOL,
                               flags, RVAL2GFILECREATEFLAGSDEFAULT,
                               cancellable, RVAL2GCANCELLABLE);
        contents = RVAL2CSTR(rbcontents);
        length = RSTRING_LEN(rbcontents);

        g_file_replace_contents_async(_SELF(self),
                                      contents,
                                      length,
                                      etag,
                                      make_backup,
                                      flags,
                                      cancellable,
                                      rbgio_async_ready_callback,
                                      (gpointer)block);

        return self;
}

DECLARE(replace_contents_finish)(VALUE self, VALUE result)
{
        char *new_etag;
        GError *error = NULL;

        if (!g_file_replace_contents_finish(_SELF(self),
                                            RVAL2GASYNCRESULT(result),
                                            &new_etag,
                                            &error))
                rbgio_raise_io_error(error);

        return CSTR2RVAL_FREE(new_etag);
}

DECLARE_VARG(copy_attributes)
{
        GFile *destination;
        GFileCopyFlags flags;
        GCancellable *cancellable;
        GError *error = NULL;

        SCAN_3_ARGS("12",
                    destination, _SELF,
                    flags, RVAL2GFILECOPYFLAGSDEFAULT,
                    cancellable, RVAL2GCANCELLABLE);

        if (!g_file_copy_attributes(_SELF(self),
                                    destination,
                                    flags,
                                    cancellable,
                                    &error))
                rbgio_raise_io_error(error);

        return self;
}

void
Init_gfile(VALUE glib)
{
        VALUE file = G_DEF_INTERFACE(G_TYPE_FILE, "File", glib);

        s_id_call = rb_intern("call");

        DEF_CONSTANTS_CLASS(G_TYPE_FILE_QUERY_INFO_FLAGS,
                            "QueryInfoFlags",
                            "G_FILE_",
                            file);
        DEF_CONSTANTS_CLASS(G_TYPE_FILE_CREATE_FLAGS,
                            "CreateFlags",
                            "G_FILE_",
                            file);
        DEF_CONSTANTS_CLASS(G_TYPE_FILE_COPY_FLAGS,
                            "CopyFlags",
                            "G_FILE_",
                            file);
        DEF_CONSTANTS_CLASS(G_TYPE_FILE_MONITOR_FLAGS,
                            "MonitorFlags",
                            "G_FILE_",
                            file);

        /* TODO: Should this be under GLib, perhaps? */
        DEF_CONSTANTS_CLASS(G_TYPE_FILE_TYPE, "Type", "G_FILE_", file);

        DEF_SINGLETON_METHOD(file, new_for_path, 1);
        DEF_SINGLETON_METHOD(file, new_for_uri, 1);
        DEF_SINGLETON_METHOD(file, new_for_commandline_arg, 1);
        DEF_SINGLETON_METHOD(file, parse_name, 1);

        DEF_METHOD(file, dup, 0);
        DEF_METHOD(file, hash, 0);
        rb_define_method(file, "==", INTERFACE_METHOD(equal), 1);
        /* TODO: Should we really define these?
        rb_define_alias(file, "===", "==");
        rb_define_alias(file, "eql?", "==");
        */
        DEF_METHOD(file, basename, 0);
        DEF_METHOD(file, path, 0);
        DEF_METHOD(file, uri, 0);
        rb_define_method(file, "parse_name", INTERFACE_METHOD(get_parse_name), 0);
        DEF_METHOD(file, parent, 0);
        DEF_METHOD(file, get_child, 1);
        DEF_METHOD(file, get_child_for_display_name, 1);
        DEF_BOOL_METHOD(file, has_prefix, 1);
        DEF_METHOD(file, get_relative_path, 1);
        DEF_METHOD(file, resolve_relative_path, 1);
        DEF_IS_METHOD(file, native, 0);
        DEF_BOOL_METHOD(file, has_uri_scheme, 1);
        DEF_METHOD(file, uri_scheme, 0);
        DEF_METHOD(file, read, -1);
        DEF_METHOD(file, read_async, -1);
        DEF_METHOD(file, read_finish, 1);
        DEF_METHOD(file, append_to, -1);
        DEF_METHOD(file, create, -1);
        DEF_METHOD(file, replace, -1);
        DEF_METHOD(file, append_to_async, -1);
        DEF_METHOD(file, append_to_finish, 1);
        DEF_METHOD(file, create_async, -1);
        DEF_METHOD(file, create_finish, 1);
        DEF_METHOD(file, replace_async, -1);
        DEF_METHOD(file, replace_finish, 1);
        DEF_METHOD(file, query_info, -1);
        DEF_METHOD(file, query_info_async, -1);
        DEF_METHOD(file, query_info_finish, 1);
        /* TODO: Do we want both #query_exists? and #exists?? */
        DEF_BOOL_METHOD(file, query_exists, -1);
        rb_define_alias(file, "exists?", "query_exists?");
        DEF_METHOD(file, query_file_type, -1);
        DEF_METHOD(file, query_filesystem_info, -1);
        DEF_METHOD(file, query_filesystem_info_async, -1);
        DEF_METHOD(file, query_filesystem_info_finish, 1);
        DEF_METHOD(file, query_default_handler, -1);
        DEF_METHOD(file, find_enclosing_mount, -1);
        DEF_METHOD(file, find_enclosing_mount_async, -1);
        DEF_METHOD(file, find_enclosing_mount_finish, 1);
        DEF_METHOD(file, enumerate_children, -1);
        DEF_METHOD(file, enumerate_children_async, -1);
        DEF_METHOD(file, enumerate_children_finish, 1);
        DEF_METHOD(file, set_display_name, -1);
        DEF_METHOD(file, set_display_name_async, -1);
        DEF_METHOD(file, set_display_name_finish, 1);
        /* TODO:
         * def display_name=(name)
         *   set_display_name name
         * end
         *
         * def display_name_async=(name)
         *   set_display_name_async name
         * end
         */
        DEF_METHOD(file, delete, -1);
        DEF_METHOD(file, trash, -1);
        DEF_METHOD(file, copy, -1);
        DEF_METHOD(file, copy_async, -1);
        DEF_METHOD(file, copy_finish, 1);
        DEF_METHOD(file, move, -1);
        DEF_METHOD(file, make_directory, -1);
        DEF_METHOD(file, make_directory_with_parents, -1);
        DEF_METHOD(file, make_symbolic_link, -1);
        DEF_METHOD(file, query_settable_attributes, -1);
        DEF_METHOD(file, query_writable_namespaces, -1);
        DEF_METHOD(file, set_attribute, -1);
        DEF_METHOD(file, set_attributes_from_info, -1);
        DEF_METHOD(file, set_attributes_async, -1);
        DEF_METHOD(file, set_attributes_finish, 1);
        DEF_METHOD(file, set_attribute_string, -1);
        DEF_METHOD(file, set_attribute_byte_string, -1);
        DEF_METHOD(file, set_attribute_uint32, -1);
        DEF_METHOD(file, set_attribute_int32, -1);
        DEF_METHOD(file, set_attribute_uint64, -1);
        DEF_METHOD(file, set_attribute_int64, -1);
        DEF_METHOD(file, mount_mountable, -1);
        DEF_METHOD(file, mount_mountable_finish, 1);
        DEF_METHOD(file, unmount_mountable, -1);
        DEF_METHOD(file, unmount_mountable_finish, 1);
        DEF_METHOD(file, eject_mountable, -1);
        DEF_METHOD(file, eject_mountable_finish, 1);
        DEF_METHOD(file, mount_enclosing_volume, -1);
        DEF_METHOD(file, mount_enclosing_volume_finish, 1);
        DEF_METHOD(file, monitor_directory, -1);
        DEF_METHOD(file, monitor_file, -1);
        DEF_METHOD(file, monitor, -1);
        DEF_METHOD(file, load_contents, -1);
        DEF_METHOD(file, load_contents_async, -1);
        DEF_METHOD(file, load_contents_finish, 1);
        DEF_METHOD(file, load_contents_partial_async, -1);
        DEF_METHOD(file, load_contents_partial_finish, 1);
        DEF_METHOD(file, replace_contents, -1);
        DEF_METHOD(file, replace_contents_async, -1);
        DEF_METHOD(file, replace_contents_finish, 1);
        DEF_METHOD(file, copy_attributes, -1);
}
