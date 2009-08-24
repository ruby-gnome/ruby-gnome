#include "gio2.h"

#define _SELF(value) \
        RVAL2GASYNCRESULT(value)

DECLARE(source_object)(VALUE self)
{
        return GOBJ2RVAL(g_async_result_get_source_object(_SELF(self)));
}

void
Init_gasyncresult(VALUE glib)
{
        VALUE asyncresult = G_DEF_INTERFACE(G_TYPE_ASYNC_RESULT,
                                            "AsyncResult",
                                            glib);

        DEF_METHOD(asyncresult, source_object, 0);
}
