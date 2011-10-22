/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
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

/*****************************************/
static GPollFD*
pollfd_copy(const GPollFD* pollfd)
{
  GPollFD* new_pollfd;
  g_return_val_if_fail (pollfd != NULL, NULL);

  new_pollfd = g_new(GPollFD, 1);
  *new_pollfd = *pollfd;
  return new_pollfd;
}

GType
g_poll_fd_get_type(void)
{
  static GType our_type = 0;
  if (our_type == 0)
    our_type = g_boxed_type_register_static ("GPollFD",
                    (GBoxedCopyFunc)pollfd_copy,
                    (GBoxedFreeFunc)g_free);
  return our_type;
}
/*****************************************/

#define _SELF(s) ((GPollFD*)RVAL2BOXED(s, G_TYPE_POLL_FD))

static VALUE
poll_initialize(VALUE self, VALUE fd, VALUE events, VALUE revents)
{
    GPollFD gfd;
    gfd.fd = NUM2INT(fd);
    gfd.events = NUM2INT(events);
    gfd.revents = NUM2INT(revents);

    G_INITIALIZE(self, &gfd);
    return Qnil;
}

static VALUE
poll_set_fd(VALUE self, VALUE fd)
{
    _SELF(self)->fd = fd;
    return self;
}
static VALUE
poll_fd(VALUE self)
{
    return INT2NUM(_SELF(self)->fd);
}

static VALUE
poll_set_events(VALUE self, VALUE events)
{
    _SELF(self)->events = events;
    return self;
}
static VALUE
poll_events(VALUE self)
{
    return INT2NUM(_SELF(self)->events);
}

static VALUE
poll_set_revents(VALUE self, VALUE revents)
{
    _SELF(self)->revents = revents;
    return self;
}
static VALUE
poll_revents(VALUE self)
{
    return INT2NUM(_SELF(self)->revents);
}

void
Init_glib_poll_fd(void)
{
    VALUE fd = G_DEF_CLASS(G_TYPE_POLL_FD, "PollFD", mGLib); 

    rb_define_method(fd, "initialize", poll_initialize, 3);

    rb_define_method(fd, "set_fd", poll_set_fd, 1);
    rb_define_method(fd, "fd", poll_fd, 0);
    rb_define_method(fd, "set_events", poll_set_events, 1);
    rb_define_method(fd, "events", poll_events, 0);
    rb_define_method(fd, "set_revents", poll_set_revents, 1);
    rb_define_method(fd, "revents", poll_revents, 0);

    G_DEF_SETTERS(fd);

}
