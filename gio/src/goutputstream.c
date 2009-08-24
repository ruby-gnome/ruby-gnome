#include "gio2.h"

#define _SELF(value) \
        RVAL2GOUTPUTSTREAM(value)

DECLARE_VARG(write)
{
        VALUE rbbuffer,
              rbcancellable;
        const char *buffer;
        gsize count;
        GCancellable *cancellable;
        GError *error = NULL;
        gssize bytes_written;

        rb_scan_args(argc, argv, "11", &rbbuffer, &rbcancellable);
        buffer = RVAL2CSTR(rbbuffer);
        count = RSTRING_LEN(rbbuffer);
        cancellable = RVAL2GCANCELLABLE(rbcancellable);

        bytes_written = g_output_stream_write(_SELF(self),
                                              buffer,
                                              count,
                                              cancellable,
                                              &error);
        if (bytes_written == -1)
                rbgio_raise_io_error(error);

        return INT2NUM(bytes_written);
}

DECLARE_VARG(write_all)
{
        VALUE rbbuffer,
              rbcancellable;
        const char *buffer;
        gsize count;
        gsize bytes_written;
        GCancellable *cancellable;
        GError *error = NULL;

        rb_scan_args(argc, argv, "11", &rbbuffer, &rbcancellable);
        buffer = RVAL2CSTR(rbbuffer);
        count = RSTRING_LEN(rbbuffer);
        cancellable = RVAL2GCANCELLABLE(rbcancellable);

        if (!g_output_stream_write_all(_SELF(self),
                                       buffer,
                                       count,
                                       &bytes_written,
                                       cancellable,
                                       &error))
                rbgio_raise_io_error(error);

        return UINT2NUM(bytes_written);
}

DECLARE_VARG(splice)
{
        GInputStream *source;
        GOutputStreamSpliceFlags flags;
        GCancellable *cancellable;
        GError *error = NULL;
        gssize bytes_spliced;

        SCAN_3_ARGS("12",
                    source, RVAL2GINPUTSTREAM,
                    flags, RVAL2GOUTPUTSTREAMSPLICEFLAGSDEFAULT,
                    cancellable, RVAL2GCANCELLABLE);

        bytes_spliced = g_output_stream_splice(_SELF(self),
                                               source,
                                               flags,
                                               cancellable,
                                               &error);
        if (bytes_spliced == -1)
                rbgio_raise_io_error(error);

        return INT2NUM(bytes_spliced);
}

void
Init_goutputstream(VALUE glib)
{
        VALUE outputstream = G_DEF_CLASS(G_TYPE_OUTPUT_STREAM,
                                         "OutputStream",
                                         glib);

        DEF_CONSTANTS_CLASS(G_TYPE_OUTPUT_STREAM_SPLICE_FLAGS,
                            "SpliceFlags",
                            "G_OUTPUT_STREAM_",
                            outputstream);

        DEF_METHOD(outputstream, write, -1);
        DEF_METHOD(outputstream, write_all, -1);
        DEF_METHOD(outputstream, splice, -1);
}
