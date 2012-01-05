/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2006,2008 Sjoerd Simons <sjoerd@luon.net>
 *  Copyright (C) 2003, 2004 Laurent Sansonetti <lrz@gnome.org>
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

#include "rbgst-private.h"

#define RG_TARGET_NAMESPACE cGhostPad

static VALUE
rg_initialize(VALUE self, VALUE name, VALUE pad)
{
    GstPad *gpad;

    gpad = gst_ghost_pad_new(RVAL2CSTR(name), RVAL2GST_PAD(pad));

    G_INITIALIZE(self, gpad);

    return Qnil;
}

void
Init_gst_ghost_pad(VALUE mGst)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GST_TYPE_GHOST_PAD, "GhostPad", mGst);

    RG_DEF_METHOD(initialize, 2);
}
