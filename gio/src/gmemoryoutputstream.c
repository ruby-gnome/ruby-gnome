#include "gio2.h"

#define _SELF(value) \
        G_MEMORY_OUTPUT_STREAM(RVAL2GOBJ(value))

#if 0
DECLARE_VARG(initialize)
{
        G_INITIALIZE(self, g_memory_output_stream_new(…));

        return Qnil;
}
#endif

void
Init_gmemoryoutputstream(VALUE glib)
{
        VALUE memoryoutputstream = G_DEF_CLASS(G_TYPE_MEMORY_OUTPUT_STREAM,
                                               "MemoryOutputStream",
                                               glib);

        /* TODO: This needs work. */
     /*   DEF_METHOD(memoryoutputstream, initialize, -1); */
}
