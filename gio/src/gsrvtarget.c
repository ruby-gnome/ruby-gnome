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

#define RVAL2GSRVTARGET(object) \
        ((GSrvTarget *)(RVAL2BOXED(object, G_TYPE_SRV_TARGET)))

#define _SELF(value) RVAL2GSRVTARGET(value)

static VALUE
srvtarget_initialize(VALUE self, VALUE hostname, VALUE port, VALUE priority, VALUE weight)
{
        /* TODO: Does this work with boxed? */
        /* TODO: hostname should be infected when returned. */
        G_INITIALIZE(self, g_srv_target_new(RVAL2CSTR(hostname),
                                            RVAL2GUINT16(port),
                                            RVAL2GUINT16(priority),
                                            RVAL2GUINT16(weight)));

        return Qnil;
}

static VALUE
srvtarget_get_hostname(VALUE self)
{
        return CSTR2RVAL(g_srv_target_get_hostname(_SELF(self)));
}

static VALUE
srvtarget_get_port(VALUE self)
{
        return GUINT162RVAL(g_srv_target_get_port(_SELF(self)));
}

static VALUE
srvtarget_get_priority(VALUE self)
{
        return GUINT162RVAL(g_srv_target_get_priority(_SELF(self)));
}

static VALUE
srvtarget_get_weight(VALUE self)
{
        return GUINT162RVAL(g_srv_target_get_weight(_SELF(self)));
}

/* TODO: srvtarget_list_sort */

void
Init_gsrvtarget(VALUE glib)
{
        VALUE srvtarget = G_DEF_CLASS(G_TYPE_SRV_TARGET, "SrvTarget", glib);

        rb_define_method(srvtarget, "initialize", srvtarget_initialize, 4);
        rb_define_method(srvtarget, "hostname", srvtarget_get_hostname, 0);
        rb_define_method(srvtarget, "port", srvtarget_get_port, 0);
        rb_define_method(srvtarget, "priority", srvtarget_get_priority, 0);
        rb_define_method(srvtarget, "weight", srvtarget_get_weight, 0);
}
