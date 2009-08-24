#include "gio2.h"

#define _SELF(value) \
        G_FILE_ICON(RVAL2GOBJ(value))

DECLARE(initialize)(VALUE self, VALUE file)
{
        G_INITIALIZE(self, g_file_icon_new(RVAL2GFILE(file)));

        return Qnil;
}

DECLARE(file)(VALUE self)
{
        return GOBJ2RVAL(g_file_icon_get_file(_SELF(self)));
}

void
Init_gfileicon(VALUE glib)
{
        VALUE fileicon = G_DEF_CLASS(G_TYPE_FILE_ICON, "FileIcon", glib);

        DEF_METHOD(fileicon, initialize, 1);
        DEF_METHOD(fileicon, file, 0);
}
