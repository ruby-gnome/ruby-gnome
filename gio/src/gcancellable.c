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
        RVAL2GCANCELLABLE(value)

DECLARE(current)(UNUSED(VALUE self))
{
        return GOBJ2RVAL(g_cancellable_get_current());
}

DECLARE(initialize)(VALUE self)
{
        G_INITIALIZE(self, g_cancellable_new());

        return Qnil;
}

#define DEFSIMPLEMETHOD(name, type) \
        static VALUE \
        name(VALUE self){ return type(g_cancellable_##name(_SELF(self))); }

DECLARE(is_cancelled)(VALUE self)
{
        return CBOOL2RVAL(g_cancellable_is_cancelled(_SELF(self)));
}

/* TODO: What do we do about g_cancellable_set_error_if_cancelled? */

DECLARE(fd)(VALUE self)
{
        return INT2FIX(g_cancellable_get_fd(_SELF(self)));
}

DECLARE(make_pollfd)(VALUE self)
{
        GPollFD *gfd = g_new(GPollFD, 1);

        g_cancellable_make_pollfd(_SELF(self), gfd);

        return BOXED2RVAL(gfd, G_TYPE_POLL_FD);
}

#define DEFSIMPLESELFMETHOD(name) \
        DECLARE(name)(VALUE self){ g_cancellable_##name(_SELF(self)); return self; }

DEFSIMPLESELFMETHOD(pop_current)
DEFSIMPLESELFMETHOD(push_current)
DEFSIMPLESELFMETHOD(reset)
DEFSIMPLESELFMETHOD(cancel)

void
Init_gcancellable(VALUE glib)
{
        VALUE cancellable = G_DEF_CLASS(G_TYPE_CANCELLABLE,
                                        "Cancellable",
                                        glib);

        DEF_SINGLETON_METHOD(cancellable, current, 0);

        DEF_METHOD(cancellable, initialize, 0);
        DEF_IS_METHOD(cancellable, cancelled, 0);
        DEF_METHOD(cancellable, fd, 0);
        DEF_METHOD(cancellable, make_pollfd, 0);
        DEF_METHOD(cancellable, pop_current, 0);
        DEF_METHOD(cancellable, push_current, 0);
        DEF_METHOD(cancellable, reset, 0);
        DEF_METHOD(cancellable, cancel, 0);
}
