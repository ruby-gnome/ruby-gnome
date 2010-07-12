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

static VALUE
cancellable_raise_error_if_cancelled(VALUE self)
{
        GError *error = NULL;

        if (g_cancellable_set_error_if_cancelled(_SELF(self), &error))
                rbgio_raise_error(error);

        return self;
}

static VALUE
cancellable_get_fd(VALUE self)
{
        return FD2RVAL(g_cancellable_get_fd(_SELF(self)));
}

static VALUE
cancellable_make_pollfd(VALUE self)
{
        GPollFD *gfd = g_new(GPollFD, 1);

        g_cancellable_make_pollfd(_SELF(self), gfd);

        return BOXED2RVAL(gfd, G_TYPE_POLL_FD);
}

G_GNUC_NORETURN static VALUE
cancellable_release_fd(VALUE self)
{
        /* TODO: How do we implement this?  Maybe not at all? */
        self = self;
        rb_raise(rb_eNotImpError, "release_fd has not been implemented yet");
#if 0
        g_cancellable_release_fd(_SELF(self));

        return self;
#endif
}

static VALUE
cancellable_get_current(G_GNUC_UNUSED VALUE self)
{
        return GOBJ2RVAL(g_cancellable_get_current());
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
cancellable_connect_callback_call(VALUE block)
{
        static ID s_id_call;

        if (s_id_call == 0)
                s_id_call = rb_intern("call");

        rb_funcall(block, s_id_call, 0);

        return Qnil;
}

static void
cancellable_connect_callback(gpointer block)
{
        G_PROTECT_CALLBACK(cancellable_connect_callback_call, block);
}

static VALUE
cancellable_connect(VALUE self)
{
        VALUE block;

        block = rb_block_proc();
        G_RELATIVE(self, block);

        return GULONG2RVAL(g_cancellable_connect(_SELF(self),
                                                 G_CALLBACK(cancellable_connect_callback),
                                                 (gpointer)block,
                                                 NULL));
}

static VALUE
cancellable_disconnect(VALUE self, VALUE handler_id)
{
        g_cancellable_disconnect(_SELF(self), RVAL2GULONG(handler_id));

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

        rb_define_singleton_method(cancellable, "current", cancellable_get_current, 0);

        rb_define_method(cancellable, "initialize", cancellable_initialize, 0);
        rb_define_method(cancellable, "cancelled?", cancellable_is_cancelled, 0);
        rb_define_method(cancellable, "raise_error_if_cancelled", cancellable_raise_error_if_cancelled, 0);
        rb_define_method(cancellable, "fd", cancellable_get_fd, 0);
        rb_define_method(cancellable, "make_pollfd", cancellable_make_pollfd, 0);
        rb_define_method(cancellable, "release_fd", cancellable_release_fd, 0);
        rb_define_method(cancellable, "pop_current", cancellable_pop_current, 0);
        rb_define_method(cancellable, "push_current", cancellable_push_current, 0);
        rb_define_method(cancellable, "reset", cancellable_reset, 0);
        rb_define_method(cancellable, "connect", cancellable_connect, 0);
        rb_define_method(cancellable, "disconnect", cancellable_disconnect, 1);
        rb_define_method(cancellable, "cancel", cancellable_cancel, 0);
}
