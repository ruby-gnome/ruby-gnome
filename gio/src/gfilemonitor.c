#include "gio2.h"

#define _SELF(value) \
        G_FILE_MONITOR(RVAL2GOBJ(value))

#define DEFSIMPLEMETHOD(name, type) \
        DECLARE(name)(VALUE self){ return type(g_file_monitor_##name(_SELF(self))); }

DEFSIMPLEMETHOD(cancel, CBOOL2RVAL)
DEFSIMPLEMETHOD(is_cancelled, CBOOL2RVAL)

DECLARE(set_rate_limit)(VALUE self, VALUE rate_limit)
{
        g_file_monitor_set_rate_limit(_SELF(self), NUM2INT(rate_limit));

        return self;
}

void
Init_gfilemonitor(VALUE glib)
{
        VALUE filemonitor = G_DEF_CLASS(G_TYPE_FILE_MONITOR,
                                        "FileMonitor",
                                        glib);

        DEF_CONSTANTS_CLASS(G_TYPE_FILE_MONITOR_FLAGS,
                            "Flags",
                            "G_FILE_MONITOR_",
                            filemonitor);
        DEF_CONSTANTS_CLASS(G_TYPE_FILE_MONITOR_EVENT,
                            "Event",
                            "G_FILE_MONITOR_",
                            filemonitor);

        rb_undef_alloc_func(filemonitor);

        DEF_METHOD(filemonitor, cancel, 0);
        DEF_IS_METHOD(filemonitor, cancelled, 0);
        DEF_SET_METHOD(filemonitor, rate_limit);
        /* TODO: Do we need #emit_event? */
}
