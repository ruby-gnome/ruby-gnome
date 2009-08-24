#include "gio2.h"

#define _SELF(value) \
        G_VOLUME_MONITOR(RVAL2GOBJ(value))

DECLARE(get)(UNUSED(VALUE self))
{
        return GOBJ2RVAL(g_volume_monitor_get());
}

#define DEFGETMETHOD(name, type) \
        DECLARE(name)(VALUE self){ return type(g_volume_monitor_get_##name(_SELF(self))); }

DEFGETMETHOD(connected_drives, GLIST2ARY_FREE)
DEFGETMETHOD(volumes, GLIST2ARY_FREE)
DEFGETMETHOD(mounts, GLIST2ARY_FREE)

#define DEFUUIDMETHOD(name) \
        static VALUE \
        name(VALUE self, VALUE uuid) \
        { \
                return GOBJ2RVAL(g_volume_monitor_##name(_SELF(self), \
                                                         RVAL2CSTR(uuid))); \
        }

#define DEFGETONEARGMETHOD(name, arg_type, type) \
        DECLARE(name)(VALUE self, VALUE value) \
        { \
                return type(g_volume_monitor_##name(_SELF(self), \
                                                    arg_type(value))); \
        }

DEFGETONEARGMETHOD(get_mount_for_uuid, RVAL2CSTR, GOBJ2RVAL)
DEFGETONEARGMETHOD(get_volume_for_uuid, RVAL2CSTR, GOBJ2RVAL)

void
Init_gvolumemonitor(VALUE glib)
{
        VALUE volumemonitor = G_DEF_CLASS(G_TYPE_VOLUME_MONITOR,
                                          "VolumeMonitor",
                                          glib);

        rb_define_const(volumemonitor,
                        "EXTENSION_POINT_NAME",
                        CSTR2RVAL(G_VOLUME_MONITOR_EXTENSION_POINT_NAME));

        rb_undef_alloc_func(volumemonitor);

        DEF_SINGLETON_METHOD(volumemonitor, get, 0);

        DEF_METHOD(volumemonitor, connected_drives, 0);
        DEF_METHOD(volumemonitor, volumes, 0);
        DEF_METHOD(volumemonitor, mounts, 0);
        DEF_METHOD(volumemonitor, get_mount_for_uuid, 0);
        DEF_METHOD(volumemonitor, get_volume_for_uuid, 0);
}
