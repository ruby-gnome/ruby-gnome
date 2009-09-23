/* -*- c-file-style: "ruby" -*- */
/*
 * Ruby/GIO: a Ruby binding of gio-2.0.x.
 * Copyright (C) 2008-2009  Ruby-GNOME2 Project Team
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "gio2.h"

#define _SELF(value) \
        G_THEMED_ICON(RVAL2GOBJ(value))

DECLARE_VARG(initialize)
{
        VALUE rbiconnames,
              with_default_fallbacks;
        int n;
        char **iconnames;
        GIcon *icon;

        rb_scan_args(argc, argv, "11", &rbiconnames, &with_default_fallbacks);

        if (TYPE(rbiconnames) != T_ARRAY) {
                const char *iconname = RVAL2CSTR(rbiconnames);

                if (RVAL2CBOOL(with_default_fallbacks))
                        G_INITIALIZE(self,
                                     g_themed_icon_new_with_default_fallbacks(iconname));
                else
                        G_INITIALIZE(self, g_themed_icon_new(iconname));

                return Qnil;
        }

        if (argc > 1)
                /* TODO: Better error message or simply ignore that we were
                 * passed more than one argument? */
                rb_raise(rb_eArgError,
                         "Only one argument allowed when first argument is an Array");

        n = string_ary_to_char_p_ary(rbiconnames, &iconnames);

        icon = g_themed_icon_new_from_names(iconnames, n);

        g_free(iconnames);

        G_INITIALIZE(self, icon);

        return Qnil;
}

#define DEFSIMPLEMETHOD(name) \
        DECLARE(name)(VALUE self, VALUE iconname) \
        { \
                g_themed_icon_##name(_SELF(self), RVAL2CSTR(iconname)); \
                return self; \
        }

DEFSIMPLEMETHOD(prepend_name)
DEFSIMPLEMETHOD(append_name)

void
Init_gthemedicon(VALUE glib)
{
        VALUE themedicon = G_DEF_INTERFACE(G_TYPE_THEMED_ICON,
                                           "ThemedIcon",
                                           glib);

        DEF_METHOD(themedicon, initialize, 1);
        DEF_METHOD(themedicon, prepend_name, 1);
        DEF_METHOD(themedicon, append_name, 1);
}
