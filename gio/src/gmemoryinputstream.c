#include "gio2.h"

#define _SELF(value) \
        G_MEMORY_INPUT_STREAM(RVAL2GOBJ(value))

DECLARE(new_from_data)(VALUE self, VALUE data)
{
        StringValue(data);

        G_CHILD_ADD(self, data);

        return GOBJ2RVAL(g_memory_input_stream_new_from_data(RSTRING_PTR(data),
                                                             RSTRING_LEN(data),
                                                             NULL));
}

DECLARE(initialize)(VALUE self)
{
        G_INITIALIZE(self, g_memory_input_stream_new());

        return Qnil;
}

DECLARE(add_data)(VALUE self, VALUE data)
{
        StringValue(data);

        G_CHILD_ADD(self, data);

        g_memory_input_stream_add_data(_SELF(self),
                                       RSTRING_PTR(data),
                                       RSTRING_LEN(data),
                                       NULL);

        return self;
}

void
Init_gmemoryinputstream(VALUE glib)
{
        VALUE memoryinputstream = G_DEF_CLASS(G_TYPE_MEMORY_INPUT_STREAM,
                                              "MemoryInputStream",
                                              glib);

        DEF_SINGLETON_METHOD(memoryinputstream, new_from_data, 1);

        DEF_METHOD(memoryinputstream, initialize, 0);
        DEF_METHOD(memoryinputstream, add_data, 1);
}
