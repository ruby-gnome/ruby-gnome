/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2012  Ruby-GNOME2 Project Team
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

#include "rbvlcprivate.h"

static libvlc_event_t *
vlc_event_copy(const libvlc_event_t *instance)
{
    libvlc_event_t *result = g_new(libvlc_event_t, 1);
    *result = *instance;
    return result;
}

static void
vlc_event_free(libvlc_event_t *instance)
{
    g_free(instance);
}

GType
vlc_event_get_type(void)
{
    static GType our_type = 0;
    if (our_type == 0)
        our_type = g_boxed_type_register_static("VLCEvent",
                                                (GBoxedCopyFunc)vlc_event_copy,
                                                (GBoxedFreeFunc)vlc_event_free);
    return our_type;
}

#define RG_TARGET_NAMESPACE cEvent
#define _SELF(self) (RVAL2VLCEVENT(self))

static VALUE
rg_type(VALUE self)
{
    return VLCEVENTTYPE2RVAL(_SELF(self)->type);
}

void
Init_vlc_event(VALUE mVLC)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(VLC_TYPE_EVENT, "Event", mVLC);

    RG_DEF_METHOD(type, 0);
}
