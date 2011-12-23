/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2008-2009  Ruby-GNOME2 Project Team
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 *  MA  02110-1301  USA
 */

#include "rbgio2private.h"

#define RG_TARGET_NAMESPACE cCancellable
#define _SELF(value) RVAL2GCANCELLABLE(value)

static VALUE
rg_initialize(VALUE self)
{
        G_INITIALIZE(self, g_cancellable_new());

        return Qnil;
}

static VALUE
rg_cancelled_p(VALUE self)
{
        return CBOOL2RVAL(g_cancellable_is_cancelled(_SELF(self)));
}

static VALUE
rg_raise_error_if_cancelled(VALUE self)
{
        GError *error = NULL;

        if (g_cancellable_set_error_if_cancelled(_SELF(self), &error))
                rbgio_raise_error(error);

        return self;
}

static VALUE
rg_fd(VALUE self)
{
        return FD2RVAL(g_cancellable_get_fd(_SELF(self)));
}

static VALUE
rg_make_pollfd(VALUE self)
{
        GCancellable *cancellable = _SELF(self);
        GPollFD *gfd = g_new(GPollFD, 1);

        g_cancellable_make_pollfd(cancellable, gfd);

        return BOXED2RVAL(gfd, G_TYPE_POLL_FD);
}

G_GNUC_NORETURN static VALUE
rg_release_fd(VALUE self)
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
rg_s_current(G_GNUC_UNUSED VALUE self)
{
        return GOBJ2RVAL(g_cancellable_get_current());
}

static VALUE
rg_pop_current(VALUE self)
{
        g_cancellable_pop_current(_SELF(self));

        return self;
}

static VALUE
rg_push_current(VALUE self)
{
        g_cancellable_push_current(_SELF(self));

        return self;
}

static VALUE
rg_reset(VALUE self)
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
rg_connect(VALUE self)
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
rg_disconnect(VALUE self, VALUE handler_id)
{
        g_cancellable_disconnect(_SELF(self), RVAL2GULONG(handler_id));

        return self;
}

static VALUE
rg_cancel(VALUE self)
{
        g_cancellable_cancel(_SELF(self));

        return self;
}

void
Init_gcancellable(VALUE mGio)
{
        VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(G_TYPE_CANCELLABLE, "Cancellable", mGio);

        RG_DEF_SMETHOD(current, 0);

        RG_DEF_METHOD(initialize, 0);
        RG_DEF_METHOD_P(cancelled, 0);
        RG_DEF_METHOD(raise_error_if_cancelled, 0);
        RG_DEF_METHOD(fd, 0);
        RG_DEF_METHOD(make_pollfd, 0);
        RG_DEF_METHOD(release_fd, 0);
        RG_DEF_METHOD(pop_current, 0);
        RG_DEF_METHOD(push_current, 0);
        RG_DEF_METHOD(reset, 0);
        RG_DEF_METHOD(connect, 0);
        RG_DEF_METHOD(disconnect, 1);
        RG_DEF_METHOD(cancel, 0);
}
