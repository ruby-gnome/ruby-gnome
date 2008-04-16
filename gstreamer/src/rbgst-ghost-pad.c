/*
 * Copyright (C) 2006,2008 Sjoerd Simons <sjoerd@luon.net>
 * Copyright (C) 2003, 2004 Laurent Sansonetti <lrz@gnome.org>
 *
 * This file is part of Ruby/GStreamer.
 *
 * Ruby/GStreamer is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * Ruby/GStreamer is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with Ruby/GStreamer; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307  USA
 */

#include "rbgst.h"

static VALUE
rb_gst_ghost_pad_new(VALUE self, VALUE name, VALUE pad) 
{
    GstPad *gpad;
    
    gpad = gst_ghost_pad_new(RVAL2CSTR(name), RGST_PAD(pad));

    G_INITIALIZE(self, gpad);

    return Qnil;
}

void
Init_gst_ghost_pad (void)
{
    VALUE c = G_DEF_CLASS (GST_TYPE_GHOST_PAD, "GhostPad", mGst);

    rb_define_method(c, "initialize", rb_gst_ghost_pad_new, 2);
}
