/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
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

#define RG_TARGET_NAMESPACE mSeek

/*  Class: Gst::Seek
 */

void
Init_gst_seek (VALUE mGst)
{
    VALUE RG_TARGET_NAMESPACE = rb_define_module_under(mGst, "Seek");

    G_DEF_CLASS(GST_TYPE_SEEK_TYPE, "SeekType", mGst);
    G_DEF_CONSTANTS(RG_TARGET_NAMESPACE, GST_TYPE_SEEK_TYPE, "GST_SEEK_");

    G_DEF_CLASS(GST_TYPE_SEEK_FLAGS, "SeekFlags", mGst);
    G_DEF_CONSTANTS(RG_TARGET_NAMESPACE, GST_TYPE_SEEK_FLAGS, "GST_SEEK_");
}
