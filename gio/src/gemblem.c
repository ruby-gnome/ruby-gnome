#include "gio2.h"

#define _SELF(value) \
        RVAL2GEMBLEM(value)

DECLARE_VARG(initialize)
{
        VALUE rbicon,
              origin;
        GIcon *icon;

        rb_scan_args(argc, argv, "11", &rbicon, &origin);
        icon = RVAL2GICON(rbicon);

        if (!NIL_P(origin))
                G_INITIALIZE(self,
                             g_emblem_new_with_origin(icon,
                                                      RVAL2GEMBLEMORIGIN(origin)));
        else
                G_INITIALIZE(self, g_emblem_new(icon));

        return Qnil;
}

#define DEFGETMETHOD(name, type) \
        DECLARE(name)(VALUE self){ return type(g_emblem_get_##name(_SELF(self))); }

DEFGETMETHOD(icon, GOBJ2RVAL)
DEFGETMETHOD(origin, GEMBLEMORIGIN2RVAL)

void
Init_gemblem(VALUE glib)
{
        VALUE emblem = G_DEF_INTERFACE(G_TYPE_EMBLEM, "Emblem", glib);

        DEF_CONSTANTS_CLASS(G_TYPE_EMBLEM_ORIGIN,
                            "Origin",
                            "G_EMBLEM_",
                            emblem);

        DEF_METHOD(emblem, initialize, -1);
        DEF_METHOD(emblem, icon, 0);
        DEF_METHOD(emblem, origin, 0);
}
