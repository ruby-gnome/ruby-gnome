#include "gio2.h"

#define _SELF(value) \
        RVAL2GINPUTSTREAM(value)

static VALUE s_id_call;

DECLARE_VARG(read)
{
        gsize count;
        VALUE buffer;
        GCancellable *cancellable;
        GError *error = NULL;
        gssize bytes_read;

        SCAN_2_ARGS("11", count, NUM2UINT, cancellable, RVAL2GCANCELLABLE);

        /* TODO: Correct? */
        buffer = rb_str_buf_new(count);

        bytes_read = g_input_stream_read(_SELF(self),
                                         RSTRING_PTR(buffer),
                                         count,
                                         cancellable,
                                         &error);
        if (bytes_read == -1)
                /* TODO: Should we free buffer? */
                rbgio_raise_io_error(error);

        return buffer;
}

DECLARE_VARG(read_all)
{
        gsize count;
        VALUE buffer;
        GCancellable *cancellable;
        GError *error = NULL;
        gsize bytes_read;

        SCAN_2_ARGS("11", count, NUM2UINT, cancellable, RVAL2GCANCELLABLE);

        buffer = rb_str_buf_new(count);

        if (!g_input_stream_read_all(_SELF(self),
                                     RSTRING_PTR(buffer),
                                     count,
                                     &bytes_read,
                                     cancellable,
                                     &error))
                /* TODO: Should we include information about how many bytes
                 * were read (bytes_read)? */
                /* TODO: Should we free buffer? */
                rbgio_raise_io_error(error);

        return buffer;
}

DECLARE_VARG(skip)
{
        gsize count;
        GCancellable *cancellable;
        GError *error = NULL;
        gssize bytes_skipped;

        SCAN_2_ARGS("11", count, NUM2UINT, cancellable, RVAL2GCANCELLABLE);

        bytes_skipped = g_input_stream_skip(_SELF(self),
                                            count,
                                            cancellable,
                                            &error);
        if (bytes_skipped == -1)
                rbgio_raise_io_error(error);

        return INT2NUM(bytes_skipped);
}

DECLARE_VARG(close)
{
        GCancellable *cancellable;
        GError *error = NULL;

        SCAN_1_ARG("01", cancellable, RVAL2GCANCELLABLE);

        if (!g_input_stream_close(_SELF(self), cancellable, &error))
                rbgio_raise_io_error(error);

        return self;
}

static void
read_async_callback(UNUSED(GObject *source),
                    GAsyncResult *result,
                    gpointer user_data)
{
        VALUE data = (VALUE)user_data;

        if (NIL_P(data))
                return;

        G_CHILD_REMOVE(mGLib, data);

        /* TODO: I guess we should probably implement our own result object
         * here that gives you data[0] on #finish. */
        rb_funcall(RARRAY_PTR(data)[1], s_id_call, 2,
                   GOBJ2RVAL(result), RARRAY_PTR(data)[0]);
}

DECLARE_VARG(read_async)
{
        gsize count;
        int io_priority;
        GCancellable *cancellable;
        VALUE data;

        SCAN_3_ARGS("12",
                    count, NUM2UINT,
                    io_priority, RVAL2IOPRIORITYDEFAULT,
                    cancellable, RVAL2GCANCELLABLE);

        if (rb_block_given_p()) {
                data = rb_assoc_new(rb_str_buf_new(count), rb_block_proc());
                G_CHILD_ADD(mGLib, data);
        } else {
                data = Qnil;
        }

        g_input_stream_read_async(_SELF(self),
                                  RSTRING_PTR(RARRAY_PTR(data)[0]),
                                  count,
                                  io_priority,
                                  cancellable,
                                  read_async_callback,
                                  (gpointer)data);

        return self;
}

DECLARE(read_finish)(VALUE self, VALUE result)
{
        GError *error = NULL;
        gssize read_bytes;

        read_bytes = g_input_stream_read_finish(_SELF(self),
                                                RVAL2GASYNCRESULT(result),
                                                &error);
        if (read_bytes == -1)
                rbgio_raise_io_error(error);

        /* TODO: Return self instead? */
        return INT2NUM(read_bytes);
}

DECLARE_VARG(skip_async)
{
        gsize count;
        int io_priority;
        GCancellable *cancellable;
        VALUE block;

        SCAN_3_ARGS_WITH_BLOCK("12",
                               count, NUM2UINT,
                               io_priority, RVAL2IOPRIORITYDEFAULT,
                               cancellable, RVAL2GCANCELLABLE);

        g_input_stream_skip_async(_SELF(self),
                                  count,
                                  io_priority,
                                  cancellable,
                                  rbgio_async_ready_callback,
                                  (gpointer)block);

        return self;
}

DECLARE(skip_finish)(VALUE self, VALUE result)
{
        GError *error = NULL;
        gssize skipped_bytes;

        skipped_bytes = g_input_stream_skip_finish(_SELF(self),
                                                   RVAL2GASYNCRESULT(result),
                                                   &error);
        if (skipped_bytes == -1)
                rbgio_raise_io_error(error);

        return INT2NUM(skipped_bytes);
}

DECLARE_VARG(close_async)
{
        int io_priority;
        GCancellable *cancellable;
        VALUE block;

        SCAN_2_ARGS_WITH_BLOCK("02",
                               io_priority, RVAL2IOPRIORITYDEFAULT,
                               cancellable, RVAL2GCANCELLABLE);

        g_input_stream_close_async(_SELF(self),
                                   io_priority,
                                   cancellable,
                                   rbgio_async_ready_callback,
                                   (gpointer)block);

        return self;
}

DECLARE(close_finish)(VALUE self, VALUE result)
{
        GError *error = NULL;
        if (!g_input_stream_skip_finish(_SELF(self),
                                        RVAL2GASYNCRESULT(result),
                                        &error))
                rbgio_raise_io_error(error);

        return self;
}

#define DEFSIMPLEMETHOD(name, type) \
        DECLARE(name)(VALUE self){ return type(g_input_stream_##name(_SELF(self))); }

DEFSIMPLEMETHOD(is_closed, CBOOL2RVAL)
DEFSIMPLEMETHOD(has_pending, CBOOL2RVAL)

DECLARE(set_pending)(VALUE self)
{
        GError *error = NULL;

        if (!g_input_stream_set_pending(_SELF(self), &error))
                rbgio_raise_io_error(error);

        return self;
}

DECLARE(clear_pending)(VALUE self)
{
        g_input_stream_clear_pending(_SELF(self));

        return self;
}

void
Init_ginputstream(VALUE glib)
{
        VALUE inputstream = G_DEF_CLASS(G_TYPE_INPUT_STREAM,
                                        "InputStream",
                                        glib);

        s_id_call = rb_intern("call");

        /* TODO: Undef alloct func? */

        DEF_METHOD(inputstream, read, -1);
        DEF_METHOD(inputstream, read_all, -1);
        DEF_METHOD(inputstream, skip, -1);
        DEF_METHOD(inputstream, close, -1);
        DEF_METHOD(inputstream, read_async, -1);
        DEF_METHOD(inputstream, read_finish, 1);
        DEF_METHOD(inputstream, skip_async, -1);
        DEF_METHOD(inputstream, skip_finish, 1);
        DEF_METHOD(inputstream, close_async, -1);
        DEF_METHOD(inputstream, close_finish, 1);
        DEF_IS_METHOD(inputstream, closed, 0);
        DEF_BOOL_METHOD(inputstream, has_pending, 0);
        DEF_METHOD(inputstream, set_pending, 0);
        DEF_METHOD(inputstream, clear_pending, 0);
}
