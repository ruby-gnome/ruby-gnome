#include "gio2.h"

#define _SELF(value) \
        RVAL2GMOUNTOPERATION(value)

DECLARE(initialize)(VALUE self)
{
        G_INITIALIZE(self, g_mount_operation_new());

        return Qnil;
}

#define DEFGETMETHOD(name, type) \
        DECLARE(name)(VALUE self) \
        { \
                return type(g_mount_operation_get_##name(_SELF(self))); \
        }

#define DEFSETMETHOD(name, type) \
        DECLARE(name)(VALUE self, VALUE value) \
        { \
                g_mount_operation_##name(_SELF(self), type(value)); \
                return self; \
        }

DEFGETMETHOD(username, CSTR2RVAL)
DEFSETMETHOD(set_username, RVAL2CSTR)
DEFGETMETHOD(password, CSTR2RVAL)
DEFSETMETHOD(set_password, RVAL2CSTR)
DEFGETMETHOD(anonymous, CBOOL2RVAL)
DEFSETMETHOD(set_anonymous, RVAL2CBOOL)
DEFGETMETHOD(domain, CSTR2RVAL)
DEFSETMETHOD(set_domain, RVAL2CSTR)
DEFGETMETHOD(password_save, GPASSWORDSAVE2RVAL)
DEFSETMETHOD(set_password_save, RVAL2GPASSWORDSAVE)
DEFGETMETHOD(choice, INT2NUM)
DEFSETMETHOD(set_choice, NUM2INT)

DEFSETMETHOD(reply, RVAL2GMOUNTOPERATIONRESULT)

void
Init_gmountoperation(VALUE glib)
{
        VALUE mountoperation = G_DEF_CLASS(G_TYPE_MOUNT_OPERATION,
                                           "MountOperation",
                                           glib);

        /* TODO: Are these necessary? */
        DEF_CONSTANTS_CLASS(G_TYPE_MOUNT_OPERATION_RESULT,
                            "Result",
                            "G_MOUNT_OPERATION_",
                            mountoperation);

        DEF_CONSTANTS_CLASS(G_TYPE_ASK_PASSWORD_FLAGS,
                            "AskPasswordFlags",
                            "G_",
                            glib);
        DEF_CONSTANTS_CLASS(G_TYPE_PASSWORD_SAVE,
                            "PasswordSave",
                            "G_",
                            glib);


        DEF_METHOD(mountoperation, initialize, 0);
        DEF_ATTRIBUTE(mountoperation, username);
        DEF_ATTRIBUTE(mountoperation, password);
        DEF_ATTRIBUTE(mountoperation, anonymous);
        DEF_ATTRIBUTE(mountoperation, domain);
        DEF_ATTRIBUTE(mountoperation, password_save);
        DEF_ATTRIBUTE(mountoperation, choice);
        DEF_METHOD(mountoperation, reply, 1);
}
