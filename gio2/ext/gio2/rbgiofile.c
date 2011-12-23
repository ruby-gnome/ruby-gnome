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

#define RG_TARGET_NAMESPACE mFile
#define _SELF(value) RVAL2GFILE(value)

#define RVAL2GFILECOPYFLAGSDEFAULT(value) \
        RVAL2TYPE_WITH_DEFAULT((value), RVAL2GFILECOPYFLAGS, G_FILE_COPY_NONE)

#define RVAL2GFILECREATEFLAGSDEFAULT(value) \
        RVAL2TYPE_WITH_DEFAULT((value), \
                               RVAL2GFILECREATEFLAGS, \
                               G_FILE_CREATE_NONE)

#define RVAL2GFILEMONITORFLAGSDEFAULT(value) \
        RVAL2TYPE_WITH_DEFAULT((value), \
                               RVAL2GFILEMONITORFLAGS, \
                               G_FILE_MONITOR_NONE)

#define RVAL2GFILEQUERYINFOFLAGSDEFAULT(value) \
        RVAL2TYPE_WITH_DEFAULT((value), \
                               RVAL2GFILEQUERYINFOFLAGS, \
                               G_FILE_QUERY_INFO_NONE)

#define RVAL2FSATTRIBUTESDEFAULT(value) \
        RVAL2TYPE_WITH_DEFAULT((value), RVAL2CSTR, "fs::*")

static VALUE
rg_s_new_for_path(G_GNUC_UNUSED VALUE self, VALUE value)
{
        return GOBJ2RVAL_UNREF(g_file_new_for_path(RVAL2CSTR(value)));
}

static VALUE
rg_s_new_for_uri(G_GNUC_UNUSED VALUE self, VALUE value)
{
        return GOBJ2RVAL_UNREF(g_file_new_for_uri(RVAL2CSTR(value)));
}

static VALUE
rg_s_new_for_commandline_arg(G_GNUC_UNUSED VALUE self, VALUE value)
{
        return GOBJ2RVAL_UNREF(g_file_new_for_commandline_arg(RVAL2CSTR(value)));
}

static VALUE
rg_s_parse_name(G_GNUC_UNUSED VALUE self, VALUE value)
{
        return GOBJ2RVAL_UNREF(g_file_parse_name(RVAL2CSTR(value)));
}

static VALUE
rg_dup(VALUE self)
{
        return GOBJ2RVAL_UNREF(g_file_dup(_SELF(self)));
}

static VALUE
rg_hash(VALUE self)
{
        return GUINT2RVAL(g_file_hash(_SELF(self)));
}

static VALUE
rg_operator_file_equal(VALUE self, VALUE other)
{
        return CBOOL2RVAL(g_file_equal(_SELF(self), _SELF(other)));
}

static VALUE
rg_basename(VALUE self)
{
        return CSTR2RVAL_FREE(g_file_get_basename(_SELF(self)));
}

static VALUE
rg_path(VALUE self)
{
        return CSTR2RVAL_FREE(g_file_get_path(_SELF(self)));
}

static VALUE
rg_uri(VALUE self)
{
        return CSTR2RVAL_FREE(g_file_get_uri(_SELF(self)));
}

static VALUE
rg_parse_name(VALUE self)
{
        return CSTR2RVAL_FREE(g_file_get_parse_name(_SELF(self)));
}

static VALUE
rg_parent(VALUE self)
{
        return GOBJ2RVAL_UNREF(g_file_get_parent(_SELF(self)));
}

static VALUE
rg_has_parent_p(int argc, VALUE *argv, VALUE self)
{
        VALUE parent;

        rb_scan_args(argc, argv, "01", &parent);
        return CBOOL2RVAL(g_file_has_parent(_SELF(self), RVAL2GFILE(parent)));
}

static VALUE
rg_get_child(VALUE self, VALUE name)
{
        return GOBJ2RVAL_UNREF(g_file_get_child(_SELF(self), RVAL2CSTR(name)));
}

static VALUE
rg_get_child_for_display_name(VALUE self, VALUE name)
{
        GError *error = NULL;
        GFile *file = g_file_get_child_for_display_name(_SELF(self),
                                                        RVAL2CSTR(name),
                                                        &error);
        if (file == NULL)
                rbgio_raise_error(error);

        return GOBJ2RVAL_UNREF(file);
}

static VALUE
rg_has_prefix_p(VALUE self, VALUE prefix)
{
        return CBOOL2RVAL(g_file_has_prefix(_SELF(self), _SELF(prefix)));
}

static VALUE
rg_get_relative_path(VALUE self, VALUE other)
{
        return CSTR2RVAL_FREE(g_file_get_relative_path(_SELF(self), _SELF(other)));
}

static VALUE
rg_resolve_relative_path(VALUE self, VALUE path)
{
        return GOBJ2RVAL_UNREF(g_file_resolve_relative_path(_SELF(self), RVAL2CSTR(path)));
}

static VALUE
rg_native_p(VALUE self)
{
        return CBOOL2RVAL(g_file_is_native(_SELF(self)));
}

static VALUE
rg_has_uri_scheme_p(VALUE self, VALUE scheme)
{
        return CBOOL2RVAL(g_file_has_uri_scheme(_SELF(self), RVAL2CSTR(scheme)));
}

static VALUE
rg_uri_scheme(VALUE self)
{
        return CSTR2RVAL_FREE(g_file_get_uri_scheme(_SELF(self)));
}

struct file_input_stream_close_data
{
        GCancellable *cancellable;
        GFileInputStream *stream;
};

static VALUE
file_input_stream_close(VALUE data)
{
        struct file_input_stream_close_data *real;
        GError *error = NULL;

        real = (struct file_input_stream_close_data *)data;

        if (!g_input_stream_close(G_INPUT_STREAM(real->stream),
                                  real->cancellable,
                                  &error))
                rbgio_raise_error(error);

        return Qnil;
}

static VALUE
rg_read(int argc, VALUE *argv, VALUE self)
{
        VALUE cancellable;
        struct file_input_stream_close_data data;
        GError *error = NULL;

        rb_scan_args(argc, argv, "01", &cancellable);
        data.cancellable = RVAL2GCANCELLABLE(cancellable);
        data.stream = g_file_read(_SELF(self), data.cancellable, &error);
        if (data.stream == NULL)
                rbgio_raise_error(error);

        if (!rb_block_given_p())
                return GOBJ2RVAL_UNREF(data.stream);

        return rb_ensure(rb_yield, GOBJ2RVAL_UNREF(data.stream),
                         file_input_stream_close, (VALUE)&data);
}

static VALUE
rg_read_async(int argc, VALUE *argv, VALUE self)
{
        VALUE rbio_priority, rbcancellable, block;
        int io_priority;
        GCancellable *cancellable;

        rb_scan_args(argc, argv, "02&", &rbio_priority, &rbcancellable, &block);
        io_priority = RVAL2IOPRIORITYDEFAULT(rbio_priority);
        cancellable = RVAL2GCANCELLABLE(rbcancellable);
        SAVE_BLOCK(block);
        g_file_read_async(_SELF(self),
                          io_priority,
                          cancellable,
                          rbgio_async_ready_callback,
                          (gpointer)block);

        return self;
}

static VALUE
rg_read_finish(VALUE self, VALUE result)
{
        GError *error = NULL;
        GFileInputStream *stream;

        stream = g_file_read_finish(_SELF(self), RVAL2GASYNCRESULT(result), &error);
        if (stream == NULL)
                rbgio_raise_error(error);

        return GOBJ2RVAL_UNREF(stream);
}

typedef GFileOutputStream *(*CreateMethod)(GFile *,
                                           GFileCreateFlags,
                                           GCancellable *,
                                           GError **);

struct file_output_stream_close_data
{
        GCancellable *cancellable;
        GFileOutputStream *stream;
};

static VALUE
file_output_stream_close(VALUE data)
{
        struct file_output_stream_close_data *real;
        GError *error = NULL;

        real = (struct file_output_stream_close_data *)data;

        if (!g_output_stream_close(G_OUTPUT_STREAM(real->stream),
                                   real->cancellable,
                                   &error))
                rbgio_raise_error(error);

        return Qnil;
}

static VALUE
create_method(CreateMethod method, int argc, VALUE *argv, VALUE self)
{
        VALUE flags, cancellable;
        struct file_output_stream_close_data data;
        GError *error = NULL;

        rb_scan_args(argc, argv, "02", &flags, &cancellable);
        data.cancellable = RVAL2GCANCELLABLE(cancellable);
        data.stream = method(_SELF(self),
                             RVAL2GFILECREATEFLAGSDEFAULT(flags),
                             data.cancellable,
                             &error);
        if (data.stream == NULL)
                rbgio_raise_error(error);

        if (!rb_block_given_p())
                return GOBJ2RVAL_UNREF(data.stream);

        return rb_ensure(rb_yield, GOBJ2RVAL_UNREF(data.stream),
                         file_output_stream_close, (VALUE)&data);
}

static VALUE
rg_append_to(int argc, VALUE *argv, VALUE self)
{
        return create_method(g_file_append_to, argc, argv, self);
}

static VALUE
rg_create(int argc, VALUE *argv, VALUE self)
{
        return create_method(g_file_create, argc, argv, self);
}

static VALUE
rg_replace(int argc, VALUE *argv, VALUE self)
{
        VALUE etag, make_backup, flags, cancellable;
        struct file_output_stream_close_data data;
        GError *error = NULL;

        rb_scan_args(argc, argv, "04", &etag, &make_backup, &flags, &cancellable);
        data.cancellable = RVAL2GCANCELLABLE(cancellable);
        data.stream = g_file_replace(_SELF(self),
                                     RVAL2CSTR_ACCEPT_NIL(etag),
                                     RVAL2CBOOL(make_backup),
                                     RVAL2GFILECREATEFLAGSDEFAULT(flags),
                                     data.cancellable,
                                     &error);
        if (data.stream == NULL)
                rbgio_raise_error(error);

        if (!rb_block_given_p())
                return GOBJ2RVAL_UNREF(data.stream);

        return rb_ensure(rb_yield, GOBJ2RVAL_UNREF(data.stream),
                         file_output_stream_close, (VALUE)&data);
}

typedef void (*CreateAsyncMethod)(GFile *, GFileCreateFlags, int,
                                  GCancellable *, GAsyncReadyCallback,
                                  gpointer);

static VALUE
create_async_method(CreateAsyncMethod method, int argc, VALUE *argv, VALUE self)
{
        VALUE rbflags, rbio_priority, rbcancellable, block;
        GFileCreateFlags flags;
        int io_priority;
        GCancellable *cancellable;

        rb_scan_args(argc, argv, "03&", &rbflags, &rbio_priority, &rbcancellable, &block);
        flags = RVAL2GFILECREATEFLAGSDEFAULT(rbflags);
        io_priority = RVAL2IOPRIORITYDEFAULT(rbio_priority);
        cancellable = RVAL2GCANCELLABLE(rbcancellable);
        SAVE_BLOCK(block);
        method(_SELF(self),
               flags,
               io_priority,
               cancellable,
               rbgio_async_ready_callback,
               (gpointer)block);

        return self;
}

static VALUE
rg_append_to_async(int argc, VALUE *argv, VALUE self)
{
        return create_async_method(g_file_append_to_async, argc, argv, self);
}

typedef GFileOutputStream *(*WriteFinishMethod)(GFile *, GAsyncResult *, GError **);

static VALUE
write_finish_method(WriteFinishMethod method, VALUE self, VALUE result)
{
        GError *error = NULL;
        GFileOutputStream *stream;

        stream = method(_SELF(self), RVAL2GASYNCRESULT(result), &error);
        if (stream == NULL)
                rbgio_raise_error(error);

        return GOBJ2RVAL_UNREF(stream);
}

static VALUE
rg_append_to_finish(VALUE self, VALUE result)
{
        return write_finish_method(g_file_append_to_finish, self, result);
}

static VALUE
rg_create_async(int argc, VALUE *argv, VALUE self)
{
        return create_async_method(g_file_create_async, argc, argv, self);
}

static VALUE
rg_create_finish(VALUE self, VALUE result)
{
        return write_finish_method(g_file_create_finish, self, result);
}

typedef void (*ReplaceAsyncMethod)(GFile *,
                                   const char *,
                                   gboolean,
                                   GFileCreateFlags,
                                   int,
                                   GCancellable *,
                                   GAsyncReadyCallback,
                                   gpointer);

static VALUE
replace_async_method(ReplaceAsyncMethod method, int argc, VALUE *argv, VALUE self)
{
        VALUE rbetag, rbmake_backup, rbflags, rbio_priority, rbcancellable, block;
        const char *etag;
        gboolean make_backup;
        GFileCreateFlags flags;
        int io_priority;
        GCancellable *cancellable;

        rb_scan_args(argc, argv, "05&", &rbetag, &rbmake_backup, &rbflags, &rbio_priority, &rbcancellable, &block);
        etag = RVAL2CSTR_ACCEPT_NIL(rbetag);
        make_backup = RVAL2CBOOL(rbmake_backup);
        flags = RVAL2GFILECREATEFLAGSDEFAULT(rbflags);
        io_priority = RVAL2IOPRIORITYDEFAULT(rbio_priority);
        cancellable = RVAL2GCANCELLABLE(rbcancellable);
        SAVE_BLOCK(block);
        method(_SELF(self),
               etag,
               make_backup,
               flags,
               io_priority,
               cancellable,
               rbgio_async_ready_callback,
               (gpointer)block);

        return self;
}

static VALUE
rg_replace_async(int argc, VALUE *argv, VALUE self)
{
        return replace_async_method(g_file_replace_async, argc, argv, self);
}

static VALUE
rg_replace_finish(VALUE self, VALUE result)
{
        return write_finish_method(g_file_replace_finish, self, result);
}

static VALUE
rg_query_info(int argc, VALUE *argv, VALUE self)
{
        VALUE attributes, flags, cancellable;
        GError *error = NULL;
        GFileInfo *info;

        rb_scan_args(argc, argv, "03", &attributes, &flags, &cancellable);
        info = g_file_query_info(_SELF(self),
                                 RVAL2ATTRIBUTESDEFAULT(attributes),
                                 RVAL2GFILEQUERYINFOFLAGSDEFAULT(flags),
                                 RVAL2GCANCELLABLE(cancellable),
                                 &error);
        if (info == NULL)
                rbgio_raise_error(error);

        return GOBJ2RVAL_UNREF(info);
}

static VALUE
rg_query_info_async(int argc, VALUE *argv, VALUE self)
{
        VALUE rbattributes, rbflags, rbio_priority, rbcancellable, block;
        const char *attributes;
        GFileQueryInfoFlags flags;
        int io_priority;
        GCancellable *cancellable;

        rb_scan_args(argc, argv, "04&", &rbattributes, &rbflags, &rbio_priority, &rbcancellable, &block);
        attributes = RVAL2ATTRIBUTESDEFAULT(rbattributes);
        flags = RVAL2GFILEQUERYINFOFLAGSDEFAULT(rbflags);
        io_priority = RVAL2IOPRIORITYDEFAULT(rbio_priority);
        cancellable = RVAL2GCANCELLABLE(rbcancellable);
        SAVE_BLOCK(block);
        g_file_query_info_async(_SELF(self),
                                attributes,
                                flags,
                                io_priority,
                                cancellable,
                                rbgio_async_ready_callback,
                                (gpointer)block);

        return self;
}

static VALUE
rg_query_info_finish(VALUE self, VALUE result)
{
        GError *error = NULL;

        GFileInfo *info = g_file_query_info_finish(_SELF(self), RVAL2GASYNCRESULT(result), &error);
        if (info == NULL)
                rbgio_raise_error(error);

        return GOBJ2RVAL_UNREF(info);
}

static VALUE
rg_query_exists_p(int argc, VALUE *argv, VALUE self)
{
        VALUE cancellable;

        rb_scan_args(argc, argv, "01", &cancellable);

        return CBOOL2RVAL(g_file_query_exists(_SELF(self), RVAL2GCANCELLABLE(cancellable)));
}

static VALUE
rg_query_file_type(int argc, VALUE *argv, VALUE self)
{
        VALUE flags, cancellable;

        rb_scan_args(argc, argv, "02", &flags, &cancellable);

        return GFILETYPE2RVAL(g_file_query_file_type(_SELF(self),
                                                     RVAL2GFILEQUERYINFOFLAGSDEFAULT(flags),
                                                     RVAL2GCANCELLABLE(cancellable)));
}

static VALUE
rg_query_filesystem_info(int argc, VALUE *argv, VALUE self)
{
        VALUE attributes, cancellable;
        GError *error = NULL;
        GFileInfo *info;

        rb_scan_args(argc, argv, "02", &attributes, &cancellable);
        info = g_file_query_filesystem_info(_SELF(self),
                                            RVAL2FSATTRIBUTESDEFAULT(attributes),
                                            RVAL2GCANCELLABLE(cancellable),
                                            &error);
        if (info == NULL)
                rbgio_raise_error(error);

        return GOBJ2RVAL_UNREF(info);
}

static VALUE
rg_query_filesystem_info_async(int argc, VALUE *argv, VALUE self)
{
        VALUE rbattributes, rbio_priority, rbcancellable, block;
        const char *attributes;
        int io_priority;
        GCancellable *cancellable;

        rb_scan_args(argc, argv, "03&", &rbattributes, &rbio_priority, &rbcancellable, &block);
        attributes = RVAL2FSATTRIBUTESDEFAULT(rbattributes);
        io_priority = RVAL2IOPRIORITYDEFAULT(rbio_priority);
        cancellable = RVAL2GCANCELLABLE(rbcancellable);
        SAVE_BLOCK(block);
        g_file_query_filesystem_info_async(_SELF(self),
                                           attributes,
                                           io_priority,
                                           cancellable,
                                           rbgio_async_ready_callback,
                                           (gpointer)block);

        return self;
}

static VALUE
rg_query_filesystem_info_finish(VALUE self, VALUE result)
{
        GError *error = NULL;
        GFileInfo *info;

        info = g_file_query_filesystem_info_finish(_SELF(self),
                                                   RVAL2GASYNCRESULT(result),
                                                   &error);
        if (info == NULL)
                rbgio_raise_error(error);

        return GOBJ2RVAL_UNREF(info);
}

static VALUE
rg_query_default_handler(int argc, VALUE *argv, VALUE self)
{
        VALUE cancellable;
        GError *error = NULL;
        GAppInfo *info;

        rb_scan_args(argc, argv, "01", &cancellable);
        info = g_file_query_default_handler(_SELF(self),
                                            RVAL2GCANCELLABLE(cancellable),
                                            &error);
        if (info == NULL)
                rbgio_raise_error(error);

        return GOBJ2RVAL_UNREF(info);
}

static VALUE
rg_find_enclosing_mount(int argc, VALUE *argv, VALUE self)
{
        VALUE cancellable;
        GError *error = NULL;
        GMount *mount;

        rb_scan_args(argc, argv, "01", &cancellable);
        mount = g_file_find_enclosing_mount(_SELF(self),
                                            RVAL2GCANCELLABLE(cancellable),
                                            &error);
        if (mount == NULL)
                rbgio_raise_error(error);

        return GOBJ2RVAL_UNREF(mount);
}

static VALUE
rg_find_enclosing_mount_async(int argc, VALUE *argv, VALUE self)
{
        VALUE rbio_priority, rbcancellable, block;
        int io_priority;
        GCancellable *cancellable;

        rb_scan_args(argc, argv, "02&", &rbio_priority, &rbcancellable, &block);
        io_priority = RVAL2IOPRIORITYDEFAULT(rbio_priority);
        cancellable = RVAL2GCANCELLABLE(rbcancellable);
        SAVE_BLOCK(block);
        g_file_find_enclosing_mount_async(_SELF(self),
                                          io_priority,
                                          cancellable,
                                          rbgio_async_ready_callback,
                                          (gpointer)block);

        return self;
}

static VALUE
rg_find_enclosing_mount_finish(VALUE self, VALUE result)
{
        GError *error = NULL;

        GMount *mount = g_file_find_enclosing_mount_finish(_SELF(self), RVAL2GASYNCRESULT(result), &error);
        if (mount == NULL)
                rbgio_raise_error(error);

        return GOBJ2RVAL_UNREF(mount);
}

struct file_enumerator_each_data
{
        GCancellable *cancellable;
        GFileEnumerator *enumerator;
};

static VALUE
file_enumerator_each(VALUE data)
{
        struct file_enumerator_each_data *real;
        GError *error = NULL;

        real = (struct file_enumerator_each_data *)data;

        while (TRUE) {
                GFileInfo *info = g_file_enumerator_next_file(real->enumerator,
                                                              real->cancellable,
                                                              &error);
                if (error != NULL)
                        rbgio_raise_error(error);

                if (info == NULL)
                        break;

                rb_yield(GOBJ2RVAL_UNREF(info));
        }

        return Qnil;
}

static VALUE
file_enumerator_each_ensure(VALUE data)
{
        struct file_enumerator_each_data *real;
        GError *error = NULL;

        real = (struct file_enumerator_each_data *)data;

        if (!g_file_enumerator_close(real->enumerator,
                                     real->cancellable,
                                     &error)) {
                g_object_unref(real->enumerator);
                rbgio_raise_error(error);
        }

        g_object_unref(real->enumerator);

        return Qnil;
}

static VALUE
rg_enumerate_children(int argc, VALUE *argv, VALUE self)
{
        VALUE attributes, flags, cancellable;
        struct file_enumerator_each_data data;
        GError *error = NULL;

        rb_scan_args(argc, argv, "03", &attributes, &flags, &cancellable);
        data.cancellable = RVAL2GCANCELLABLE(cancellable);
        data.enumerator = g_file_enumerate_children(_SELF(self),
                                                    RVAL2ATTRIBUTESDEFAULT(attributes),
                                                    RVAL2GFILEQUERYINFOFLAGSDEFAULT(flags),
                                                    data.cancellable,
                                                    &error);
        if (data.enumerator == NULL)
                rbgio_raise_error(error);

        if (!rb_block_given_p())
                return GOBJ2RVAL_UNREF(data.enumerator);

        rb_ensure(file_enumerator_each, (VALUE)&data,
                  file_enumerator_each_ensure, (VALUE)&data);

        return self;
}

static VALUE
rg_enumerate_children_async(int argc, VALUE *argv, VALUE self)
{
        VALUE rbattributes, rbflags, rbio_priority, rbcancellable, block;
        const char *attributes;
        GFileQueryInfoFlags flags;
        int io_priority;
        GCancellable *cancellable;

        rb_scan_args(argc, argv, "04&", &rbattributes, &rbflags, &rbio_priority, &rbcancellable, &block);
        attributes = RVAL2ATTRIBUTESDEFAULT(rbattributes);
        flags = RVAL2GFILEQUERYINFOFLAGSDEFAULT(rbflags);
        io_priority = RVAL2IOPRIORITYDEFAULT(rbio_priority);
        cancellable = RVAL2GCANCELLABLE(rbcancellable);
        SAVE_BLOCK(block);
        g_file_enumerate_children_async(_SELF(self),
                                        attributes,
                                        flags,
                                        io_priority,
                                        cancellable,
                                        rbgio_async_ready_callback,
                                        (gpointer)block);

        return self;
}

static VALUE
rg_enumerate_children_finish(VALUE self, VALUE result)
{
        GError *error = NULL;
        GFileEnumerator *enumerator;

        enumerator = g_file_enumerate_children_finish(_SELF(self),
                                                      RVAL2GASYNCRESULT(result),
                                                      &error);
        if (enumerator == NULL)
                rbgio_raise_error(error);

        return GOBJ2RVAL_UNREF(enumerator);
}

static VALUE
rg_set_display_name(int argc, VALUE *argv, VALUE self)
{
        VALUE display_name, cancellable;
        GError *error = NULL;
        GFile *file;

        rb_scan_args(argc, argv, "11", &display_name, &cancellable);

        file = g_file_set_display_name(_SELF(self),
                                       RVAL2CSTR(display_name),
                                       RVAL2GCANCELLABLE(cancellable),
                                       &error);
        if (file == NULL)
                rbgio_raise_error(error);

        return GOBJ2RVAL_UNREF(file);
}

static VALUE
rg_set_display_name_async(int argc, VALUE *argv, VALUE self)
{
        VALUE rbdisplay_name, rbio_priority, rbcancellable, block;
        const char *display_name;
        int io_priority;
        GCancellable *cancellable;

        rb_scan_args(argc, argv, "03&", &rbdisplay_name, &rbio_priority, &rbcancellable, &block);
        display_name = RVAL2CSTR(rbdisplay_name);
        io_priority = RVAL2IOPRIORITYDEFAULT(rbio_priority);
        cancellable = RVAL2GCANCELLABLE(rbcancellable);
        SAVE_BLOCK(block);
        g_file_set_display_name_async(_SELF(self),
                                      display_name,
                                      io_priority,
                                      cancellable,
                                      rbgio_async_ready_callback,
                                      (gpointer)block);

        return self;
}

static VALUE
rg_set_display_name_finish(VALUE self, VALUE result)
{
        GError *error = NULL;
        GFileInfo *info;

        info = g_file_query_filesystem_info_finish(_SELF(self),
                                                   RVAL2GASYNCRESULT(result),
                                                   &error);
        if (info == NULL)
                rbgio_raise_error(error);

        return GOBJ2RVAL_UNREF(info);
}

typedef gboolean (*CancellableMethod)(GFile *, GCancellable *, GError **);

static VALUE
cancellable_method(CancellableMethod method, int argc, VALUE *argv, VALUE self)
{
        VALUE cancellable;
        GError *error = NULL;

        rb_scan_args(argc, argv, "01", &cancellable);
        if (!method(_SELF(self), RVAL2GCANCELLABLE(cancellable), &error))
                rbgio_raise_error(error);

        return self;
}

static VALUE
rg_delete(int argc, VALUE *argv, VALUE self)
{
        return cancellable_method(g_file_delete, argc, argv, self);
}

static VALUE
rg_trash(int argc, VALUE *argv, VALUE self)
{
        return cancellable_method(g_file_trash, argc, argv, self);
}

struct progress_callback_data
{
        goffset current_num_bytes;
        goffset total_num_bytes;
        gpointer data;
};

static VALUE
progress_callback_call(VALUE data)
{
        static ID s_id_call;
        struct progress_callback_data *real;
        VALUE block;

        if (s_id_call == 0)
                s_id_call = rb_intern("call");

        real = (struct progress_callback_data *)data;
        block = USE_BLOCK_AND_SAVE(real->data);
        if (!NIL_P(block))
                rb_funcall(block, s_id_call, 2,
                           GOFFSET2RVAL(real->current_num_bytes),
                           GOFFSET2RVAL(real->total_num_bytes));

        return Qnil;
}

static void
progress_callback(goffset current_num_bytes, goffset total_num_bytes, gpointer data)
{
        struct progress_callback_data real;

        real.current_num_bytes = current_num_bytes;
        real.total_num_bytes = total_num_bytes;
        real.data = data;

        G_PROTECT_CALLBACK(progress_callback_call, &real);
}

typedef gboolean (*CopyMoveMethod)(GFile *,
                                   GFile *,
                                   GFileCopyFlags,
                                   GCancellable *,
                                   GFileProgressCallback,
                                   gpointer,
                                   GError **);

static VALUE
copy_move_method(CopyMoveMethod method, int argc, VALUE *argv, VALUE self)
{
        VALUE rbdestination, rbflags, rbcancellable, block;
        GFile *destination;
        GFileCopyFlags flags;
        GCancellable *cancellable;
        GError *error = NULL;

        rb_scan_args(argc, argv, "12&", &rbdestination, &rbflags, &rbcancellable, &block);
        destination = _SELF(rbdestination);
        flags = RVAL2GFILECOPYFLAGSDEFAULT(rbflags);
        cancellable = RVAL2GCANCELLABLE(rbcancellable);
        SAVE_BLOCK(block);
        if (!method(_SELF(self),
                    destination,
                    flags,
                    cancellable,
                    NIL_P(block) ? NULL : progress_callback,
                    (gpointer)block,
                    &error))
                rbgio_raise_error(error);

        return self;
}

static VALUE
rg_copy(int argc, VALUE *argv, VALUE self)
{
        return copy_move_method(g_file_copy, argc, argv, self);
}

static VALUE
rg_copy_async(int argc, VALUE *argv, VALUE self)
{
        VALUE rbdestination, rbflags, rbio_priority, rbcancellable, block;
        GFile *destination;
        GFileCopyFlags flags;
        int io_priority;
        GCancellable *cancellable;

        rb_scan_args(argc, argv, "13&", &rbdestination, &rbflags, &rbio_priority, &rbcancellable, &block);
        destination = _SELF(rbdestination);
        flags = RVAL2GFILECOPYFLAGSDEFAULT(rbflags);
        io_priority = RVAL2IOPRIORITYDEFAULT(rbio_priority);
        cancellable = RVAL2GCANCELLABLE(rbcancellable);
        SAVE_BLOCK(block);
        /* NOTE: This requires the block to take a variable number of
         * arguments. It will be two integer values for the progress part and
         * a GLib::AsyncResult when done. */
        g_file_copy_async(_SELF(self),
                          destination,
                          flags,
                          io_priority,
                          cancellable,
                          NIL_P(block) ? NULL : progress_callback,
                          (gpointer)block,
                          rbgio_async_ready_callback,
                          (gpointer)block);

        return self;
}

typedef gboolean (*BooleanFinishMethod)(GFile *, GAsyncResult *, GError **);

static VALUE
boolean_finish_method(BooleanFinishMethod method, VALUE self, VALUE result)
{
        GError *error = NULL;

        if (!method(_SELF(self), RVAL2GASYNCRESULT(result), &error))
                rbgio_raise_error(error);

        return self;
}

static VALUE
rg_copy_finish(VALUE self, VALUE result)
{
        return boolean_finish_method(g_file_copy_finish, self, result);
}

static VALUE
rg_move(int argc, VALUE *argv, VALUE self)
{
        return copy_move_method(g_file_move, argc, argv, self);
}

static VALUE
rg_make_directory(int argc, VALUE *argv, VALUE self)
{
        return cancellable_method(g_file_make_directory, argc, argv, self);
}

static VALUE
rg_make_directory_with_parents(int argc, VALUE *argv, VALUE self)
{
        return cancellable_method(g_file_make_directory_with_parents, argc, argv, self);
}

static VALUE
rg_make_symbolic_link(int argc, VALUE *argv, VALUE self)
{
        VALUE symlink_value, cancellable;
        GError *error = NULL;

        rb_scan_args(argc, argv, "11", &symlink_value, &cancellable);
        if (!g_file_make_symbolic_link(_SELF(self),
                                       RVAL2CSTR(symlink_value),
                                       RVAL2GCANCELLABLE(cancellable),
                                       &error))
                rbgio_raise_error(error);

        return self;
}

typedef GFileAttributeInfoList *(*QueryAttributesMethod)(GFile *,
                                                         GCancellable *,
                                                         GError **);

static VALUE
query_attributes_method(QueryAttributesMethod method,
                        int argc, VALUE *argv, VALUE self)
{
        VALUE cancellable;
        GError *error = NULL;
        GFileAttributeInfoList *list;

        rb_scan_args(argc, argv, "01", &cancellable);
        list = method(_SELF(self), RVAL2GCANCELLABLE(cancellable), &error);
        if (list == NULL)
                rbgio_raise_error(error);

        return GFILEATTRIBUTEINFOLIST2RVAL(list);
}

static VALUE
rg_query_settable_attributes(int argc, VALUE *argv, VALUE self)
{
        return query_attributes_method(g_file_query_settable_attributes,
                                       argc, argv, self);
}

static VALUE
rg_query_writable_namespaces(int argc, VALUE *argv, VALUE self)
{
        return query_attributes_method(g_file_query_writable_namespaces,
                                        argc, argv, self);
}

static VALUE
rg_set_attribute(int argc, VALUE *argv, VALUE self)
{
        VALUE rbattribute, rbtype, rbvalue, rbflags, rbcancellable;
        const char *attribute;
        GFileAttributeType type;
        gpointer value;
        gboolean gboolean_value;
        guint32 guint32_value;
        gint32 gint32_value;
        guint64 guint64_value;
        gint64 gint64_value;
        const char **stringv_value = NULL;
        GFileQueryInfoFlags flags;
        GCancellable *cancellable;
        GError *error = NULL;

        rb_scan_args(argc, argv, "32", &rbattribute, &rbtype, &rbvalue, &rbflags, &rbcancellable);
        attribute = RVAL2CSTR(rbattribute);
        type = RVAL2GFILEATTRIBUTETYPE(rbtype);
        flags = RVAL2GFILEQUERYINFOFLAGSDEFAULT(rbflags);
        cancellable = RVAL2GCANCELLABLE(rbcancellable);

        switch (type) {
        case G_FILE_ATTRIBUTE_TYPE_STRING:
        case G_FILE_ATTRIBUTE_TYPE_BYTE_STRING:
                value = (gpointer)RVAL2CSTR(rbvalue);
                break;
        case G_FILE_ATTRIBUTE_TYPE_BOOLEAN:
                gboolean_value = RVAL2CBOOL(rbvalue);
                value = &gboolean_value;
                break;
        case G_FILE_ATTRIBUTE_TYPE_UINT32:
                guint32_value = RVAL2GUINT32(rbvalue);
                value = &guint32_value;
                break;
        case G_FILE_ATTRIBUTE_TYPE_INT32:
                gint32_value = RVAL2GINT32(rbvalue);
                value = &gint32_value;
                break;
        case G_FILE_ATTRIBUTE_TYPE_UINT64:
                guint64_value = RVAL2GUINT64(rbvalue);
                value = &guint64_value;
                break;
        case G_FILE_ATTRIBUTE_TYPE_INT64:
                gint64_value = RVAL2GINT64(rbvalue);
                value = &gint64_value;
                break;
        case G_FILE_ATTRIBUTE_TYPE_OBJECT:
                value = RVAL2GOBJ(rbvalue);
                break;
        case G_FILE_ATTRIBUTE_TYPE_STRINGV:
                stringv_value = RVAL2STRV(rbvalue);
                value = (gpointer)stringv_value;
                break;
        case G_FILE_ATTRIBUTE_TYPE_INVALID:
        default:
                rb_raise(rb_eArgError, "Unknown file attribute type: %d", type);
        }

        g_file_set_attribute(_SELF(self),
                             attribute,
                             type,
                             value,
                             flags,
                             cancellable,
                             &error);
        g_free(stringv_value);
        if (error != NULL)
                rbgio_raise_error(error);

        return self;
}

static VALUE
rg_set_attributes_from_info(int argc, VALUE *argv, VALUE self)
{
        VALUE info, flags, cancellable;
        GError *error = NULL;

        rb_scan_args(argc, argv, "12", &info, &flags, &cancellable);
        if (!g_file_set_attributes_from_info(_SELF(self),
                                             RVAL2GFILEINFO(info),
                                             RVAL2GFILEQUERYINFOFLAGSDEFAULT(flags),
                                             RVAL2GCANCELLABLE(cancellable),
                                             &error))
                rbgio_raise_error(error);

        return self;
}

static VALUE
rg_set_attributes_async(int argc, VALUE *argv, VALUE self)
{
        VALUE rbinfo, rbflags, rbio_priority, rbcancellable, block;
        GFileInfo *info;
        GFileQueryInfoFlags flags;
        int io_priority;
        GCancellable *cancellable;

        rb_scan_args(argc, argv, "13&", &rbinfo, &rbflags, &rbio_priority, &rbcancellable, &block);
        info = RVAL2GFILEINFO(rbinfo);
        flags = RVAL2GFILEQUERYINFOFLAGSDEFAULT(rbflags);
        io_priority = RVAL2IOPRIORITYDEFAULT(rbio_priority);
        cancellable = RVAL2GCANCELLABLE(rbcancellable);
        SAVE_BLOCK(block);
        g_file_set_attributes_async(_SELF(self),
                                    info,
                                    flags,
                                    io_priority,
                                    cancellable,
                                    rbgio_async_ready_callback,
                                    (gpointer)block);

        return self;
}

static VALUE
rg_set_attributes_finish(VALUE self, VALUE result)
{
        GFileInfo *info;
        GError *error = NULL;

        if (!g_file_set_attributes_finish(_SELF(self),
                                          RVAL2GASYNCRESULT(result),
                                          &info,
                                          &error))
                rbgio_raise_error(error);

        return GOBJ2RVAL_UNREF(info);
}

static VALUE
rg_set_attribute_string(int argc, VALUE *argv, VALUE self)
{
        VALUE attribute, value, flags, cancellable;
        GError *error = NULL;

        rb_scan_args(argc, argv, "22", &attribute, &value, &flags, &cancellable);
        if (!g_file_set_attribute_string(_SELF(self),
                                         RVAL2CSTR(attribute),
                                         RVAL2CSTR(value),
                                         RVAL2GFILEQUERYINFOFLAGSDEFAULT(flags),
                                         RVAL2GCANCELLABLE(cancellable),
                                         &error))
                rbgio_raise_error(error);

        return self;
}

static VALUE
rg_set_attribute_byte_string(int argc, VALUE *argv, VALUE self)
{
        VALUE attribute, value, flags, cancellable;
        GError *error = NULL;

        rb_scan_args(argc, argv, "22", &attribute, &value, &flags, &cancellable);
        if (!g_file_set_attribute_byte_string(_SELF(self),
                                              RVAL2CSTR(attribute),
                                              RVAL2CSTR(value),
                                              RVAL2GFILEQUERYINFOFLAGSDEFAULT(flags),
                                              RVAL2GCANCELLABLE(cancellable),
                                              &error))
                rbgio_raise_error(error);

        return self;
}

static VALUE
rg_set_attribute_uint32(int argc, VALUE *argv, VALUE self)
{
        VALUE attribute, value, flags, cancellable;
        GError *error = NULL;

        rb_scan_args(argc, argv, "22", &attribute, &value, &flags, &cancellable);
        if (!g_file_set_attribute_uint32(_SELF(self),
                                         RVAL2CSTR(attribute),
                                         RVAL2GUINT32(value),
                                         RVAL2GFILEQUERYINFOFLAGSDEFAULT(flags),
                                         RVAL2GCANCELLABLE(cancellable),
                                         &error))
                rbgio_raise_error(error);

        return self;
}

static VALUE
rg_set_attribute_int32(int argc, VALUE *argv, VALUE self)
{
        VALUE attribute, value, flags, cancellable;
        GError *error = NULL;

        rb_scan_args(argc, argv, "22", &attribute, &value, &flags, &cancellable);
        if (!g_file_set_attribute_int32(_SELF(self),
                                        RVAL2CSTR(attribute),
                                        RVAL2GINT32(value),
                                        RVAL2GFILEQUERYINFOFLAGSDEFAULT(flags),
                                        RVAL2GCANCELLABLE(cancellable),
                                        &error))
                rbgio_raise_error(error);

        return self;
}

static VALUE
rg_set_attribute_uint64(int argc, VALUE *argv, VALUE self)
{
        VALUE attribute, value, flags, cancellable;
        GError *error = NULL;

        rb_scan_args(argc, argv, "22", &attribute, &value, &flags, &cancellable);
        if (!g_file_set_attribute_uint64(_SELF(self),
                                         RVAL2CSTR(attribute),
                                         RVAL2GUINT64(value),
                                         RVAL2GFILEQUERYINFOFLAGSDEFAULT(flags),
                                         RVAL2GCANCELLABLE(cancellable),
                                         &error))
                rbgio_raise_error(error);

        return self;
}

static VALUE
rg_set_attribute_int64(int argc, VALUE *argv, VALUE self)
{
        VALUE attribute, value, flags, cancellable;
        GError *error = NULL;

        rb_scan_args(argc, argv, "22", &attribute, &value, &flags, &cancellable);
        if (!g_file_set_attribute_int64(_SELF(self),
                                        RVAL2CSTR(attribute),
                                        RVAL2GINT64(value),
                                        RVAL2GFILEQUERYINFOFLAGSDEFAULT(flags),
                                        RVAL2GCANCELLABLE(cancellable),
                                        &error))
                rbgio_raise_error(error);

        return self;
}

typedef void (*MountMethod)(GFile *,
                            GMountMountFlags,
                            GMountOperation *,
                            GCancellable *,
                            GAsyncReadyCallback,
                            gpointer);

static VALUE
mount_method(MountMethod method, int argc, VALUE *argv, VALUE self)
{
        VALUE rbflags, rbmount_operation, rbcancellable, block;
        GMountMountFlags flags;
        GMountOperation *mount_operation;
        GCancellable *cancellable;

        rb_scan_args(argc, argv, "03&", &rbflags, &rbmount_operation, &rbcancellable, &block);
        flags = RVAL2GMOUNTMOUNTFLAGSDEFAULT(rbflags);
        mount_operation = RVAL2GMOUNTOPERATION(rbmount_operation);
        cancellable = RVAL2GCANCELLABLE(rbcancellable);
        SAVE_BLOCK(block);
        method(_SELF(self),
               flags,
               mount_operation,
               cancellable,
               rbgio_async_ready_callback,
               (gpointer)block);

        return self;
}

static VALUE
rg_mount_mountable(int argc, VALUE *argv, VALUE self)
{
        return mount_method(g_file_mount_mountable, argc, argv, self);
}

static VALUE
rg_mount_mountable_finish(VALUE self, VALUE result)
{
        GError *error = NULL;
        GFile *file;

        file = g_file_mount_mountable_finish(_SELF(self),
                                             RVAL2GASYNCRESULT(result),
                                             &error);
        if (file == NULL)
                rbgio_raise_error(error);

        return GOBJ2RVAL_UNREF(file);
}

typedef void (*UnmountEjectStopMountableMethod)(GFile *,
                                                GMountUnmountFlags,
                                                GMountOperation *,
                                                GCancellable *,
                                                GAsyncReadyCallback,
                                                gpointer);

static VALUE
unmount_eject_stop_mountable_method(UnmountEjectStopMountableMethod method,
                                    int argc, VALUE *argv, VALUE self)
{
        VALUE rbflags, rbmount_operation, rbcancellable, block;
        GMountUnmountFlags flags;
        GMountOperation *mount_operation;
        GCancellable *cancellable;

        rb_scan_args(argc, argv, "03&", &rbflags, &rbmount_operation, &rbcancellable, &block);
        flags = RVAL2GMOUNTUNMOUNTFLAGSDEFAULT(rbflags);
        mount_operation = RVAL2GMOUNTOPERATION(rbmount_operation);
        cancellable = RVAL2GCANCELLABLE(rbcancellable);
        SAVE_BLOCK(block);
        method(_SELF(self),
               flags,
               mount_operation,
               cancellable,
               rbgio_async_ready_callback,
               (gpointer)block);

        return self;
}

static VALUE
rg_unmount_mountable_with_operation(int argc, VALUE *argv, VALUE self)
{
        return unmount_eject_stop_mountable_method(g_file_unmount_mountable_with_operation,
                                                   argc, argv, self);
}

static VALUE
rg_unmount_mountable_with_operation_finish(VALUE self, VALUE result)
{
        return boolean_finish_method(g_file_unmount_mountable_with_operation_finish, self, result);
}

static VALUE
rg_eject_mountable_with_operation(int argc, VALUE *argv, VALUE self)
{
        return unmount_eject_stop_mountable_method(g_file_eject_mountable_with_operation,
                                                   argc, argv, self);
}

static VALUE
rg_eject_mountable_with_operation_finish(VALUE self, VALUE result)
{
        return boolean_finish_method(g_file_eject_mountable_with_operation_finish, self, result);
}

static VALUE
rg_start_mountable(int argc, VALUE *argv, VALUE self)
{
        VALUE rbflags, rbstart_operation, rbcancellable, block;
        GDriveStartFlags flags;
        GMountOperation *start_operation;
        GCancellable *cancellable;

        rb_scan_args(argc, argv, "03&", &rbflags, &rbstart_operation, &rbcancellable, &block);
        flags = RVAL2GDRIVESTARTFLAGSDEFAULT(rbflags);
        start_operation = RVAL2GMOUNTOPERATION(rbstart_operation);
        cancellable = RVAL2GCANCELLABLE(rbcancellable);
        SAVE_BLOCK(block);
        g_file_start_mountable(_SELF(self),
                               flags,
                               start_operation,
                               cancellable,
                               rbgio_async_ready_callback,
                               (gpointer)block);

        return self;
}

static VALUE
rg_start_mountable_finish(VALUE self, VALUE result)
{
        return boolean_finish_method(g_file_start_mountable_finish, self, result);
}

static VALUE
rg_stop_mountable(int argc, VALUE *argv, VALUE self)
{
        return unmount_eject_stop_mountable_method(g_file_stop_mountable,
                                                   argc, argv, self);
}

static VALUE
rg_stop_mountable_finish(VALUE self, VALUE result)
{
        return boolean_finish_method(g_file_stop_mountable_finish, self, result);
}

typedef void (*CancellableAsyncMethod)(GFile *,
                                       GCancellable *,
                                       GAsyncReadyCallback,
                                       gpointer);

static VALUE
cancellable_async_method(CancellableAsyncMethod method, int argc, VALUE *argv, VALUE self)
{
        VALUE rbcancellable, block;
        GCancellable *cancellable;

        rb_scan_args(argc, argv, "01&", &rbcancellable, &block);
        cancellable = RVAL2GCANCELLABLE(rbcancellable);
        SAVE_BLOCK(block);
        method(_SELF(self), cancellable, rbgio_async_ready_callback, (gpointer)block);

        return self;
}

static VALUE
rg_poll_mountable(int argc, VALUE *argv, VALUE self)
{
        return cancellable_async_method(g_file_poll_mountable, argc, argv, self);
}

static VALUE
rg_poll_mountable_finish(VALUE self, VALUE result)
{
        return boolean_finish_method(g_file_poll_mountable_finish, self, result);
}

static VALUE
rg_mount_enclosing_volume(int argc, VALUE *argv, VALUE self)
{
        return mount_method(g_file_mount_enclosing_volume, argc, argv, self);
}

static VALUE
rg_mount_enclosing_volume_finish(VALUE self, VALUE result)
{
        return boolean_finish_method(g_file_mount_enclosing_volume_finish, self, result);
}

typedef GFileMonitor *(*MonitorMethod)(GFile *,
                                       GFileMonitorFlags,
                                       GCancellable *,
                                       GError **);

static VALUE
monitor_method(MonitorMethod method, int argc, VALUE *argv, VALUE self)
{
        VALUE flags, cancellable;
        GError *error = NULL;
        GFileMonitor *monitor;

        rb_scan_args(argc, argv, "02", &flags, &cancellable);
        monitor = method(_SELF(self),
                         RVAL2GFILEMONITORFLAGSDEFAULT(flags),
                         RVAL2GCANCELLABLE(cancellable),
                         &error);
        if (monitor == NULL)
                rbgio_raise_error(error);

        return GOBJ2RVAL_UNREF(monitor);
}

static VALUE
rg_monitor_directory(int argc, VALUE *argv, VALUE self)
{
        return monitor_method(g_file_monitor_directory, argc, argv, self);
}

static VALUE
rg_monitor_file(int argc, VALUE *argv, VALUE self)
{
        return monitor_method(g_file_monitor_file, argc, argv, self);
}

static VALUE
rg_monitor(int argc, VALUE *argv, VALUE self)
{
        return monitor_method(g_file_monitor, argc, argv, self);
}

static VALUE
rg_load_contents(int argc, VALUE *argv, VALUE self)
{
        VALUE cancellable;
        char *contents;
        gsize length;
        char *etag_out;
        GError *error = NULL;

        rb_scan_args(argc, argv, "01", &cancellable);
        if (!g_file_load_contents(_SELF(self),
                                  RVAL2GCANCELLABLE(cancellable),
                                  &contents,
                                  &length,
                                  &etag_out,
                                  &error))
                rbgio_raise_error(error);

        return rb_assoc_new(CSTR2RVAL_TAINTED_FREE(contents, length),
                            CSTR2RVAL_FREE(etag_out));
}

static VALUE
rg_load_contents_async(int argc, VALUE *argv, VALUE self)
{
        return cancellable_async_method(g_file_load_contents_async, argc, argv, self);
}

static VALUE
rg_load_contents_finish(VALUE self, VALUE result)
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
                rbgio_raise_error(error);

        return rb_assoc_new(CSTR2RVAL_TAINTED_FREE(contents, length),
                            CSTR2RVAL_FREE(etag_out));
}

struct load_partial_contents_async_read_more_callback_data
{
        const char *file_contents;
        goffset file_size;
        gpointer data;
};

static VALUE
load_partial_contents_async_read_more_callback_call(VALUE data)
{
        static ID s_id_call;
        struct load_partial_contents_async_read_more_callback_data *real;
        VALUE block;

        if (s_id_call == 0)
                s_id_call = rb_intern("call");

        real = (struct load_partial_contents_async_read_more_callback_data *)data;
        block = USE_BLOCK_AND_SAVE(real->data);
        if (NIL_P(block))
                return Qtrue;

        return rb_funcall(block, s_id_call, 1,
                          CSTR2RVAL_TAINTED(real->file_contents,
                                            real->file_size));
}

static gboolean
load_partial_contents_async_read_more_callback(const char *file_contents,
                                               goffset file_size,
                                               gpointer data)
{
        struct load_partial_contents_async_read_more_callback_data real;

        real.file_contents = file_contents;
        real.file_size = file_size;
        real.data = data;

        return RVAL2CBOOL(G_PROTECT_CALLBACK(load_partial_contents_async_read_more_callback_call,
                                             &real));
}

static VALUE
rg_load_partial_contents_async(int argc, VALUE *argv, VALUE self)
{
        VALUE rbcancellable, rbuse_read_more_callback, block;
        GCancellable *cancellable;
        gboolean use_read_more_callback;

        rb_scan_args(argc, argv, "02&", &rbcancellable, &rbuse_read_more_callback, &block);
        cancellable = RVAL2GCANCELLABLE(rbcancellable);
        use_read_more_callback = RVAL2CBOOL(rbuse_read_more_callback);
        SAVE_BLOCK(block);
        /* TODO: Should we force a block if use_read_more_callback is true? */
        g_file_load_partial_contents_async(_SELF(self),
                                           cancellable,
                                           use_read_more_callback ?
                                                load_partial_contents_async_read_more_callback :
                                                NULL,
                                           rbgio_async_ready_callback,
                                           (gpointer)block);

        return self;
}

static VALUE
rg_load_partial_contents_finish(VALUE self, VALUE result)
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
                rbgio_raise_error(error);

        return rb_assoc_new(CSTR2RVAL_TAINTED_FREE(contents, length),
                            CSTR2RVAL_FREE(etag_out));
}

static VALUE
rg_replace_contents(int argc, VALUE *argv, VALUE self)
{
        VALUE rbcontents, etag, make_backup, flags, cancellable;
        const char *contents;
        char *new_etag;
        GError *error = NULL;

        rb_scan_args(argc, argv, "14", &rbcontents, &etag, &make_backup, &flags, &cancellable);
        contents = RVAL2CSTR(rbcontents);
        if (!g_file_replace_contents(_SELF(self),
                                     contents,
                                     (gsize)RSTRING_LEN(rbcontents),
                                     RVAL2CSTR_ACCEPT_NIL(etag),
                                     RVAL2CBOOL(make_backup),
                                     RVAL2GFILECREATEFLAGSDEFAULT(flags),
                                     &new_etag,
                                     RVAL2GCANCELLABLE(cancellable),
                                     &error))
                rbgio_raise_error(error);

        return CSTR2RVAL_FREE(new_etag);
}

static VALUE
rg_replace_contents_async(int argc, VALUE *argv, VALUE self)
{
        VALUE rbcontents, rbetag, rbmake_backup, rbflags, rbcancellable, block;
        const char *contents;
        gsize length;
        const char *etag;
        gboolean make_backup;
        GFileCreateFlags flags;
        GCancellable *cancellable;

        rb_scan_args(argc, argv, "14&", &rbcontents, &rbetag, &rbmake_backup, &rbflags, &rbcancellable, &block);
        contents = RVAL2CSTR(rbcontents);
        length = RSTRING_LEN(rbcontents);
        etag = RVAL2CSTR_ACCEPT_NIL(rbetag);
        make_backup = RVAL2CBOOL(rbmake_backup);
        flags = RVAL2GFILECREATEFLAGSDEFAULT(rbflags);
        cancellable = RVAL2GCANCELLABLE(rbcancellable);
        SAVE_BLOCK(block);
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

static VALUE
rg_replace_contents_finish(VALUE self, VALUE result)
{
        char *new_etag;
        GError *error = NULL;

        if (!g_file_replace_contents_finish(_SELF(self),
                                            RVAL2GASYNCRESULT(result),
                                            &new_etag,
                                            &error))
                rbgio_raise_error(error);

        return CSTR2RVAL_FREE(new_etag);
}

static VALUE
rg_copy_attributes(int argc, VALUE *argv, VALUE self)
{
        VALUE destination, flags, cancellable;
        GError *error = NULL;

        rb_scan_args(argc, argv, "12", &destination, &flags, &cancellable);
        if (!g_file_copy_attributes(_SELF(self),
                                    _SELF(destination),
                                    RVAL2GFILECOPYFLAGSDEFAULT(flags),
                                    RVAL2GCANCELLABLE(cancellable),
                                    &error))
                rbgio_raise_error(error);

        return self;
}

struct file_io_stream_close_data
{
        GCancellable *cancellable;
        GFileIOStream *stream;
};

static VALUE
file_io_stream_close(VALUE data)
{
        struct file_io_stream_close_data *real;
        GError *error = NULL;

        real = (struct file_io_stream_close_data *)data;

        if (!g_io_stream_close(G_IO_STREAM(real->stream),
                               real->cancellable,
                               &error))
                rbgio_raise_error(error);

        return Qnil;
}

static VALUE
rg_create_readwrite(int argc, VALUE *argv, VALUE self)
{
        VALUE flags, cancellable;
        struct file_io_stream_close_data data;
        GError *error = NULL;

        rb_scan_args(argc, argv, "02", &flags, &cancellable);
        data.cancellable = RVAL2GCANCELLABLE(cancellable);
        data.stream = g_file_create_readwrite(_SELF(self),
                                              RVAL2GFILECREATEFLAGSDEFAULT(flags),
                                              data.cancellable,
                                              &error);
        if (data.stream == NULL)
                rbgio_raise_error(error);

        if (!rb_block_given_p())
                return GOBJ2RVAL_UNREF(data.stream);

        return rb_ensure(rb_yield, GOBJ2RVAL_UNREF(data.stream),
                         file_io_stream_close, (VALUE)&data);
}

static VALUE
rg_create_readwrite_async(int argc, VALUE *argv, VALUE self)
{
        return create_async_method(g_file_create_readwrite_async, argc, argv, self);
}

typedef GFileIOStream *(*ReadwriteFinishMethod)(GFile *, GAsyncResult *, GError **);

static VALUE
readwrite_finish_method(ReadwriteFinishMethod method, VALUE self, VALUE result)
{
        GError *error = NULL;

        GFileIOStream *stream = method(_SELF(self), RVAL2GASYNCRESULT(result), &error);
        if (stream == NULL)
                rbgio_raise_error(error);

        return GOBJ2RVAL_UNREF(stream);
}

static VALUE
rg_create_readwrite_finish(VALUE self, VALUE result)
{
        return readwrite_finish_method(g_file_create_readwrite_finish, self, result);
}

static VALUE
rg_open_readwrite(int argc, VALUE *argv, VALUE self)
{
        VALUE cancellable;
        struct file_io_stream_close_data data;
        GError *error = NULL;

        rb_scan_args(argc, argv, "01", &cancellable);
        data.cancellable = RVAL2GCANCELLABLE(cancellable);
        data.stream = g_file_open_readwrite(_SELF(self),
                                            data.cancellable,
                                            &error);
        if (data.stream == NULL)
                rbgio_raise_error(error);

        if (!rb_block_given_p())
                return GOBJ2RVAL_UNREF(data.stream);

        return rb_ensure(rb_yield, GOBJ2RVAL_UNREF(data.stream),
                         file_io_stream_close, (VALUE)&data);
}

static VALUE
rg_open_readwrite_async(int argc, VALUE *argv, VALUE self)
{
        VALUE rbio_priority, rbcancellable, block;
        int io_priority;
        GCancellable *cancellable;

        rb_scan_args(argc, argv, "02&", &rbio_priority, &rbcancellable, &block);
        io_priority = RVAL2IOPRIORITYDEFAULT(rbio_priority);
        cancellable = RVAL2GCANCELLABLE(rbcancellable);
        SAVE_BLOCK(block);
        g_file_open_readwrite_async(_SELF(self), io_priority, cancellable,
                                    rbgio_async_ready_callback, (gpointer)block);

        return self;
}

static VALUE
rg_open_readwrite_finish(VALUE self, VALUE result)
{
        return readwrite_finish_method(g_file_open_readwrite_finish, self, result);
}

static VALUE
rg_replace_readwrite(int argc, VALUE *argv, VALUE self)
{
        VALUE etag, make_backup, flags, cancellable;
        struct file_io_stream_close_data data;
        GError *error = NULL;

        rb_scan_args(argc, argv, "04", &etag, &make_backup, &flags, &cancellable);
        data.cancellable = RVAL2GCANCELLABLE(cancellable);
        data.stream = g_file_replace_readwrite(_SELF(self),
                                          RVAL2CSTR_ACCEPT_NIL(etag),
                                          RVAL2CBOOL(make_backup),
                                          RVAL2GFILECREATEFLAGSDEFAULT(flags),
                                          data.cancellable,
                                          &error);
        if (data.stream == NULL)
                rbgio_raise_error(error);

        if (!rb_block_given_p())
                return GOBJ2RVAL_UNREF(data.stream);

        return rb_ensure(rb_yield, GOBJ2RVAL_UNREF(data.stream),
                         file_io_stream_close, (VALUE)&data);
}

static VALUE
rg_replace_readwrite_async(int argc, VALUE *argv, VALUE self)
{
        return replace_async_method(g_file_replace_readwrite_async, argc, argv, self);
}

static VALUE
rg_replace_readwrite_finish(VALUE self, VALUE result)
{
        return readwrite_finish_method(g_file_replace_readwrite_finish, self, result);
}

static VALUE
rg_supports_thread_contexts_p(VALUE self)
{
        return CBOOL2RVAL(g_file_supports_thread_contexts(_SELF(self)));
}

void
Init_gfile(VALUE mGio)
{
        VALUE RG_TARGET_NAMESPACE = G_DEF_INTERFACE(G_TYPE_FILE, "File", mGio);

        G_DEF_CLASS(G_TYPE_FILE_QUERY_INFO_FLAGS, "QueryInfoFlags", RG_TARGET_NAMESPACE);
        G_DEF_CONSTANTS(RG_TARGET_NAMESPACE, G_TYPE_FILE_QUERY_INFO_FLAGS, "G_FILE_");

        G_DEF_CLASS(G_TYPE_FILE_CREATE_FLAGS, "CreateFlags", RG_TARGET_NAMESPACE);
        G_DEF_CONSTANTS(RG_TARGET_NAMESPACE, G_TYPE_FILE_CREATE_FLAGS, "G_FILE_");

        G_DEF_CLASS(G_TYPE_FILE_COPY_FLAGS, "CopyFlags", RG_TARGET_NAMESPACE);
        G_DEF_CONSTANTS(RG_TARGET_NAMESPACE, G_TYPE_FILE_COPY_FLAGS, "G_FILE_");

        G_DEF_CLASS(G_TYPE_FILE_MONITOR_FLAGS, "MonitorFlags", RG_TARGET_NAMESPACE);
        G_DEF_CONSTANTS(RG_TARGET_NAMESPACE, G_TYPE_FILE_MONITOR_FLAGS, "G_FILE_");

        G_DEF_CLASS(G_TYPE_FILE_TYPE, "Type", RG_TARGET_NAMESPACE);
        G_DEF_CONSTANTS(RG_TARGET_NAMESPACE, G_TYPE_FILE_TYPE, "G_FILE_");

        RG_DEF_SMETHOD(new_for_path, 1);
        RG_DEF_SMETHOD(new_for_uri, 1);
        RG_DEF_SMETHOD(new_for_commandline_arg, 1);
        RG_DEF_SMETHOD(parse_name, 1);

        RG_DEF_METHOD(dup, 0);
        RG_DEF_METHOD(hash, 0);
        RG_DEF_METHOD_OPERATOR("==", file_equal, 1);
        RG_DEF_METHOD(basename, 0);
        RG_DEF_METHOD(path, 0);
        RG_DEF_METHOD(uri, 0);
        RG_DEF_METHOD(parse_name, 0);
        RG_DEF_METHOD(parent, 0);
        RG_DEF_METHOD_P(has_parent, -1);
        RG_DEF_METHOD(get_child, 1);
        RG_DEF_METHOD(get_child_for_display_name, 1);
        RG_DEF_METHOD_P(has_prefix, 1);
        RG_DEF_METHOD(get_relative_path, 1);
        RG_DEF_METHOD(resolve_relative_path, 1);
        RG_DEF_METHOD_P(native, 0);
        RG_DEF_METHOD_P(has_uri_scheme, 1);
        RG_DEF_METHOD(uri_scheme, 0);
        RG_DEF_METHOD(read, -1);
        RG_DEF_METHOD(read_async, -1);
        RG_DEF_METHOD(read_finish, 1);
        RG_DEF_METHOD(append_to, -1);
        RG_DEF_METHOD(create, -1);
        RG_DEF_METHOD(replace, -1);
        RG_DEF_METHOD(append_to_async, -1);
        RG_DEF_METHOD(append_to_finish, 1);
        RG_DEF_METHOD(create_async, -1);
        RG_DEF_METHOD(create_finish, 1);
        RG_DEF_METHOD(replace_async, -1);
        RG_DEF_METHOD(replace_finish, 1);
        RG_DEF_METHOD(query_info, -1);
        RG_DEF_METHOD(query_info_async, -1);
        RG_DEF_METHOD(query_info_finish, 1);
        RG_DEF_METHOD_P(query_exists, -1);
        RG_DEF_ALIAS("exists?", "query_exists?");
        RG_DEF_METHOD(query_file_type, -1);
        RG_DEF_METHOD(query_filesystem_info, -1);
        RG_DEF_METHOD(query_filesystem_info_async, -1);
        RG_DEF_METHOD(query_filesystem_info_finish, 1);
        RG_DEF_METHOD(query_default_handler, -1);
        RG_DEF_METHOD(find_enclosing_mount, -1);
        RG_DEF_METHOD(find_enclosing_mount_async, -1);
        RG_DEF_METHOD(find_enclosing_mount_finish, 1);
        RG_DEF_METHOD(enumerate_children, -1);
        RG_DEF_METHOD(enumerate_children_async, -1);
        RG_DEF_METHOD(enumerate_children_finish, 1);
        RG_DEF_METHOD(set_display_name, -1);
        G_DEF_SETTER(RG_TARGET_NAMESPACE, "display_name");
        RG_DEF_METHOD(set_display_name_async, -1);
        RG_DEF_METHOD(set_display_name_finish, 1);
        RG_DEF_METHOD(delete, -1);
        RG_DEF_METHOD(trash, -1);
        RG_DEF_METHOD(copy, -1);
        RG_DEF_METHOD(copy_async, -1);
        RG_DEF_METHOD(copy_finish, 1);
        RG_DEF_METHOD(move, -1);
        RG_DEF_METHOD(make_directory, -1);
        RG_DEF_METHOD(make_directory_with_parents, -1);
        RG_DEF_METHOD(make_symbolic_link, -1);
        RG_DEF_METHOD(query_settable_attributes, -1);
        RG_DEF_METHOD(query_writable_namespaces, -1);
        RG_DEF_METHOD(set_attribute, -1);
        RG_DEF_METHOD(set_attributes_from_info, -1);
        G_DEF_SETTER(RG_TARGET_NAMESPACE, "attributes_from_info");
        RG_DEF_METHOD(set_attributes_async, -1);
        RG_DEF_METHOD(set_attributes_finish, 1);
        RG_DEF_METHOD(set_attribute_string, -1);
        RG_DEF_METHOD(set_attribute_byte_string, -1);
        RG_DEF_METHOD(set_attribute_uint32, -1);
        RG_DEF_METHOD(set_attribute_int32, -1);
        RG_DEF_METHOD(set_attribute_uint64, -1);
        RG_DEF_METHOD(set_attribute_int64, -1);
        RG_DEF_METHOD(mount_mountable, -1);
        RG_DEF_METHOD(mount_mountable_finish, 1);
        RG_DEF_METHOD(unmount_mountable_with_operation, -1);
        RG_DEF_METHOD(unmount_mountable_with_operation_finish, 1);
        RG_DEF_METHOD(eject_mountable_with_operation, -1);
        RG_DEF_METHOD(eject_mountable_with_operation_finish, 1);
        RG_DEF_METHOD(start_mountable, -1);
        RG_DEF_METHOD(start_mountable_finish, 1);
        RG_DEF_METHOD(stop_mountable, -1);
        RG_DEF_METHOD(stop_mountable_finish, 1);
        RG_DEF_METHOD(poll_mountable, -1);
        RG_DEF_METHOD(poll_mountable_finish, 1);
        RG_DEF_METHOD(mount_enclosing_volume, -1);
        RG_DEF_METHOD(mount_enclosing_volume_finish, 1);
        RG_DEF_METHOD(monitor_directory, -1);
        RG_DEF_METHOD(monitor_file, -1);
        RG_DEF_METHOD(monitor, -1);
        RG_DEF_METHOD(load_contents, -1);
        RG_DEF_METHOD(load_contents_async, -1);
        RG_DEF_METHOD(load_contents_finish, 1);
        RG_DEF_METHOD(load_partial_contents_async, -1);
        RG_DEF_METHOD(load_partial_contents_finish, 1);
        RG_DEF_METHOD(replace_contents, -1);
        RG_DEF_METHOD(replace_contents_async, -1);
        RG_DEF_METHOD(replace_contents_finish, 1);
        RG_DEF_METHOD(copy_attributes, -1);
        RG_DEF_METHOD(create_readwrite, -1);
        RG_DEF_METHOD(create_readwrite_async, -1);
        RG_DEF_METHOD(create_readwrite_finish, 1);
        RG_DEF_METHOD(open_readwrite, -1);
        RG_DEF_METHOD(open_readwrite_async, -1);
        RG_DEF_METHOD(open_readwrite_finish, 1);
        RG_DEF_METHOD(replace_readwrite, -1);
        RG_DEF_METHOD(replace_readwrite_async, -1);
        RG_DEF_METHOD(replace_readwrite_finish, 1);
        RG_DEF_METHOD_P(supports_thread_contexts, 0);
}
