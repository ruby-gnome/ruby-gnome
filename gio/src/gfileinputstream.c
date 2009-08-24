#include "gio2.h"

#define _SELF(value) \
        RVAL2GFILEINPUTSTREAM(value)

DECLARE_VARG(query_info)
{
        const char *attributes;
        GCancellable *cancellable;
        GError *error = NULL;
        GFileInfo *info;

        SCAN_2_ARGS("02",
                    attributes, RVAL2ATTRIBUTESDEFAULT,
                    cancellable, RVAL2GCANCELLABLE);

        info = g_file_input_stream_query_info(_SELF(self),
                                              attributes,
                                              cancellable,
                                              &error);
        if (info == NULL)
                rbgio_raise_io_error(error);

        return GOBJ2RVAL(info);
}

DECLARE_VARG(query_info_async)
{
        const char *attributes;
        int io_priority;
        GCancellable *cancellable;
        VALUE block;

        SCAN_3_ARGS_WITH_BLOCK("03",
                               attributes, RVAL2ATTRIBUTESDEFAULT,
                               io_priority, RVAL2IOPRIORITYDEFAULT,
                               cancellable, RVAL2GCANCELLABLE);

        g_file_input_stream_query_info_async(_SELF(self),
                                             attributes,
                                             io_priority,
                                             cancellable,
                                             rbgio_async_ready_callback,
                                             (gpointer)block);

        return self;
}

DECLARE(query_info_finish)(VALUE self, VALUE result)
{
        GError *error = NULL;
        GFileInfo *info;

        info = g_file_input_stream_query_info_finish(_SELF(self),
                                                     RVAL2GASYNCRESULT(result),
                                                     &error);
        if (info == NULL)
                rbgio_raise_io_error(error);

        return GOBJ2RVAL(info);
}

void
Init_gfileinputstream(VALUE glib)
{
        VALUE fileinputstream = G_DEF_CLASS(G_TYPE_FILE_INPUT_STREAM,
                                            "FileInputStream",
                                            glib);

        rb_undef_alloc_func(fileinputstream);

        DEF_METHOD(fileinputstream, query_info, -1);
        DEF_METHOD(fileinputstream, query_info_async, -1);
        DEF_METHOD(fileinputstream, query_info_finish, 1);
}
