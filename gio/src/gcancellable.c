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

#define _SELF(value) RVAL2GCANCELLABLE(value)

static VALUE
cancellable_current(UNUSED(VALUE self))
{
        return GOBJ2RVAL(g_cancellable_get_current());
}

static VALUE
cancellable_initialize(VALUE self)
{
        G_INITIALIZE(self, g_cancellable_new());

        return Qnil;
}

static VALUE
cancellable_is_cancelled(VALUE self)
{
        return CBOOL2RVAL(g_cancellable_is_cancelled(_SELF(self)));
}

/* TODO: What do we do about g_cancellable_set_error_if_cancelled? */

static VALUE
cancellable_fd(VALUE self)
{
        return INT2FIX(g_cancellable_get_fd(_SELF(self)));
}

static VALUE
cancellable_make_pollfd(VALUE self)
{
        GPollFD *gfd = g_new(GPollFD, 1);

        g_cancellable_make_pollfd(_SELF(self), gfd);

        return BOXED2RVAL(gfd, G_TYPE_POLL_FD);
}

static VALUE
cancellable_pop_current(VALUE self)
{
        g_cancellable_pop_current(_SELF(self));

        return self;
}

static VALUE
cancellable_push_current(VALUE self)
{
        g_cancellable_push_current(_SELF(self));

        return self;
}

static VALUE
cancellable_reset(VALUE self)
{
        g_cancellable_reset(_SELF(self));

        return self;
}

static VALUE
cancellable_cancel(VALUE self)
{
        g_cancellable_cancel(_SELF(self));

        return self;
}

void
Init_gcancellable(VALUE glib)
{
        VALUE cancellable = G_DEF_CLASS(G_TYPE_CANCELLABLE, "Cancellable", glib);

        rb_define_singleton_method(cancellable, "current", cancellable_current, 0);

        rb_define_method(cancellable, "initialize", cancellable_initialize, 0);
        rb_define_method(cancellable, "cancelled?", cancellable_is_cancelled, 0);
        rb_define_method(cancellable, "fd", cancellable_fd, 0);
        rb_define_method(cancellable, "make_pollfd", cancellable_make_pollfd, 0);
        rb_define_method(cancellable, "pop_current", cancellable_pop_current, 0);
        rb_define_method(cancellable, "push_current", cancellable_push_current, 0);
        rb_define_method(cancellable, "reset", cancellable_reset, 0);
        rb_define_method(cancellable, "cancel", cancellable_cancel, 0);
}
