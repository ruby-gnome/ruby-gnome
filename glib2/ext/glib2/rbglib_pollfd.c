/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011-2021  Ruby-GNOME Project Team
 *  Copyright (C) 2005  Masao Mutoh
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

#include "rbgprivate.h"

#define RG_TARGET_NAMESPACE cPollFD
#define _SELF(s) ((GPollFD*)RVAL2BOXED(s, G_TYPE_POLLFD))

static VALUE
rg_initialize(VALUE self, VALUE fd, VALUE events, VALUE revents)
{
    GPollFD gfd;
    gfd.fd = NUM2INT(fd);
    gfd.events = NUM2INT(events);
    gfd.revents = NUM2INT(revents);

    G_INITIALIZE(self, g_boxed_copy(G_TYPE_POLLFD, &gfd));
    return Qnil;
}

static VALUE
rg_set_fd(VALUE self, VALUE fd)
{
    _SELF(self)->fd = NUM2INT(fd);
    return self;
}
static VALUE
rg_fd(VALUE self)
{
    return INT2NUM(_SELF(self)->fd);
}

static VALUE
rg_set_events(VALUE self, VALUE events)
{
    _SELF(self)->events = events;
    return self;
}
static VALUE
rg_events(VALUE self)
{
    return INT2NUM(_SELF(self)->events);
}

static VALUE
rg_set_revents(VALUE self, VALUE revents)
{
    _SELF(self)->revents = revents;
    return self;
}
static VALUE
rg_revents(VALUE self)
{
    return INT2NUM(_SELF(self)->revents);
}

void
Init_glib_poll_fd(void)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(G_TYPE_POLLFD, "PollFD", mGLib);

    RG_DEF_METHOD(initialize, 3);

    RG_DEF_METHOD(set_fd, 1);
    RG_DEF_METHOD(fd, 0);
    RG_DEF_METHOD(set_events, 1);
    RG_DEF_METHOD(events, 0);
    RG_DEF_METHOD(set_revents, 1);
    RG_DEF_METHOD(revents, 0);
}
