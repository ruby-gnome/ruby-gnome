#include "gio2.h"

#define _SELF(value) \
        G_FILTER_OUTPUT_STREAM(RVAL2GOBJ(value))

#define DEFGETMETHOD(name, type) \
        DECLARE(name)(VALUE self) \
        { \
                return type(g_filter_output_stream_get_##name(_SELF(self))); \
        }

DEFGETMETHOD(base_stream, GOBJ2RVAL)
DEFGETMETHOD(close_base_stream, CBOOL2RVAL)

DECLARE(set_close_base_stream)(VALUE self, VALUE close_base)
{
        g_filter_output_stream_set_close_base_stream(_SELF(self),
                                                     RVAL2CBOOL(close_base));

        return self;
}

void
Init_gfilteroutputstream(VALUE glib)
{
        VALUE filteroutputstream = G_DEF_CLASS(G_TYPE_FILTER_OUTPUT_STREAM,
                                               "FilterOutputStream",
                                               glib);

        rb_undef_alloc_func(filteroutputstream);

        DEF_METHOD(filteroutputstream, base_stream, 0);
        DEF_BOOL_METHOD(filteroutputstream, close_base_stream, 0);
        DEF_SET_METHOD(filteroutputstream, close_base_stream);
}
