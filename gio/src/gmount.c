#include "gio2.h"

#define _SELF(value) \
        RVAL2GMOUNT(value)

#define DEFGETMETHOD(name, type) \
        DECLARE(name)(VALUE self){ return type(g_mount_get_##name(_SELF(self))); }

DEFGETMETHOD(name, CSTR2RVAL2)
DEFGETMETHOD(uuid, CSTR2RVAL2)
DEFGETMETHOD(icon, GOBJ2RVAL)
DEFGETMETHOD(drive, GOBJ2RVAL)
DEFGETMETHOD(root, GOBJ2RVAL)
DEFGETMETHOD(volume, GOBJ2RVAL)

#define DEFSIMPLEMETHOD(name, type) \
        DECLARE(name)(VALUE self){ return type(g_mount_##name(_SELF(self))); }

DEFSIMPLEMETHOD(can_unmount, CBOOL2RVAL)

DECLARE_VARG(unmount)
{
        GMountUnmountFlags flags;
        GCancellable *cancellable;
        VALUE block;

        SCAN_2_ARGS_WITH_BLOCK("02",
                               flags, RVAL2GMOUNTUNMOUNTFLAGSDEFAULT,
                               cancellable, RVAL2GCANCELLABLE);

        g_mount_unmount(_SELF(self),
                        flags,
                        cancellable,
                        rbgio_async_ready_callback,
                        (gpointer)block);

        return self;
}

DECLARE(unmount_finish)(VALUE self, VALUE result)
{
        GError *error = NULL;
        if (!g_mount_unmount_finish(_SELF(self),
                                    RVAL2GASYNCRESULT(result),
                                    &error))
                rbgio_raise_io_error(error);

        return self;
}

DECLARE_VARG(remount)
{
        GMountMountFlags flags;
        GMountOperation *mount_operation;
        GCancellable *cancellable;
        VALUE block;

        SCAN_3_ARGS_WITH_BLOCK("03",
                               flags, RVAL2GMOUNTMOUNTFLAGSDEFAULT,
                               mount_operation, RVAL2GMOUNTOPERATION,
                               cancellable, RVAL2GCANCELLABLE);

        g_mount_remount(_SELF(self),
                        flags,
                        mount_operation,
                        cancellable,
                        rbgio_async_ready_callback,
                        (gpointer)block);

        return self;
}

DECLARE(remount_finish)(VALUE self, VALUE result)
{
        GError *error = NULL;
        if (!g_mount_remount_finish(_SELF(self),
                                    RVAL2GASYNCRESULT(result),
                                    &error))
                rbgio_raise_io_error(error);

        return self;
}

DEFSIMPLEMETHOD(can_eject, CBOOL2RVAL)

DECLARE_VARG(eject)
{
        GMountUnmountFlags flags;
        GCancellable *cancellable;
        VALUE block;

        SCAN_2_ARGS_WITH_BLOCK("02",
                               flags, RVAL2GMOUNTUNMOUNTFLAGSDEFAULT,
                               cancellable, RVAL2GCANCELLABLE);

        g_mount_eject(_SELF(self),
                      flags,
                      cancellable,
                      rbgio_async_ready_callback,
                      (gpointer)block);

        return self;
}

DECLARE(eject_finish)(VALUE self, VALUE result)
{
        GError *error = NULL;
        if (!g_mount_eject_finish(_SELF(self),
                                  RVAL2GASYNCRESULT(result),
                                  &error))
                rbgio_raise_io_error(error);

        return self;
}

DECLARE_VARG(guess_content_type)
{
        gboolean force_rescan;
        GCancellable *cancellable;
        VALUE block;

        SCAN_2_ARGS_WITH_BLOCK("02",
                               force_rescan, RVAL2CBOOL,
                               cancellable, RVAL2GCANCELLABLE);

        g_mount_guess_content_type(_SELF(self),
                                   force_rescan,
                                   cancellable,
                                   rbgio_async_ready_callback,
                                   (gpointer)block);

        return self;
}

DECLARE(guess_content_type_finish)(VALUE self, VALUE result)
{
        GError *error = NULL;
        gchar **content_types;

        content_types = g_mount_guess_content_type_finish(_SELF(self),
                                                          RVAL2GASYNCRESULT(result),
                                                          &error);
        if (content_types == NULL)
                rbgio_raise_io_error(error);

        return str_vector_to_ary(content_types);
}

DECLARE_VARG(guess_content_type_sync)
{
        gboolean force_rescan;
        GCancellable *cancellable;
        GError *error = NULL;
        gchar **content_types;

        SCAN_2_ARGS("02",
                    force_rescan, RVAL2CBOOL,
                    cancellable, RVAL2GCANCELLABLE);

        content_types = g_mount_guess_content_type_sync(_SELF(self),
                                                        force_rescan,
                                                        cancellable,
                                                        &error);
        if (content_types == NULL)
                rbgio_raise_io_error(error);

        return str_vector_to_ary(content_types);
}

DEFSIMPLEMETHOD(is_shadowed, CBOOL2RVAL)

#define DEFSIMPLESELFMETHOD(name) \
       DECLARE(name)(VALUE self){ g_mount_##name(_SELF(self)); return self; }

DEFSIMPLESELFMETHOD(shadow)
DEFSIMPLESELFMETHOD(unshadow)

void
Init_gmount(VALUE glib)
{
        VALUE mount = G_DEF_INTERFACE(G_TYPE_MOUNT, "Mount", glib);

        DEF_CONSTANTS_CLASS(G_TYPE_MOUNT_MOUNT_FLAGS,
                            "MountFlags",
                            "G_MOUNT_",
                            mount);
        DEF_CONSTANTS_CLASS(G_TYPE_MOUNT_UNMOUNT_FLAGS,
                            "UnmountFlags",
                            "G_MOUNT_",
                            mount);

        DEF_METHOD(mount, name, 0);
        DEF_METHOD(mount, uuid, 0);
        DEF_METHOD(mount, icon, 0);
        DEF_METHOD(mount, drive, 0);
        DEF_METHOD(mount, root, 0);
        DEF_METHOD(mount, volume, 0);
        DEF_BOOL_METHOD(mount, can_unmount, 0);
        DEF_METHOD(mount, unmount, -1);
        DEF_METHOD(mount, unmount_finish, 1);
        DEF_METHOD(mount, remount, -1);
        DEF_METHOD(mount, remount_finish, 1);
        DEF_BOOL_METHOD(mount, can_eject, 0);
        DEF_METHOD(mount, eject, -1);
        DEF_METHOD(mount, eject_finish, 1);
        DEF_METHOD(mount, guess_content_type, -1);
        DEF_METHOD(mount, guess_content_type_finish, 1);
        DEF_METHOD(mount, guess_content_type_sync, -1);
        DEF_IS_METHOD(mount, shadowed, 0);
        DEF_METHOD(mount, shadow, 0);
        DEF_METHOD(mount, unshadow, 0);
}
