/*
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

/*  Class: Gst::Seek
 */

void
Init_gst_seek (void)
{
    VALUE c = rb_define_module_under(mGst, "Seek");

    G_DEF_CLASS(GST_TYPE_SEEK_TYPE, "SeekType", mGst);
    G_DEF_CONSTANTS(c, GST_TYPE_SEEK_TYPE, "GST_SEEK_");

    G_DEF_CLASS(GST_TYPE_SEEK_FLAGS, "SeekFlags", mGst);
    G_DEF_CONSTANTS(c, GST_TYPE_SEEK_FLAGS, "GST_SEEK_");
}
