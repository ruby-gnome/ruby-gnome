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

#define RVAL2GSRVTARGET(object) \
        ((GSrvTarget *)(RVAL2BOXED(object, G_TYPE_SRV_TARGET)))

#define RG_TARGET_NAMESPACE cSrvTarget
#define _SELF(value) RVAL2GSRVTARGET(value)

static VALUE
rg_initialize(VALUE self, VALUE hostname, VALUE port, VALUE priority, VALUE weight)
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
rg_hostname(VALUE self)
{
        return CSTR2RVAL(g_srv_target_get_hostname(_SELF(self)));
}

static VALUE
rg_port(VALUE self)
{
        return GUINT162RVAL(g_srv_target_get_port(_SELF(self)));
}

static VALUE
rg_priority(VALUE self)
{
        return GUINT162RVAL(g_srv_target_get_priority(_SELF(self)));
}

static VALUE
rg_weight(VALUE self)
{
        return GUINT162RVAL(g_srv_target_get_weight(_SELF(self)));
}

/* TODO: srvtarget_list_sort */

void
Init_gsrvtarget(VALUE mGio)
{
        VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(G_TYPE_SRV_TARGET, "SrvTarget", mGio);

        RG_DEF_METHOD(initialize, 4);
        RG_DEF_METHOD(hostname, 0);
        RG_DEF_METHOD(port, 0);
        RG_DEF_METHOD(priority, 0);
        RG_DEF_METHOD(weight, 0);
}
