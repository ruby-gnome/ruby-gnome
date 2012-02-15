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

#define RG_TARGET_NAMESPACE cType
#define _SELF(self) (RVAL2VLCEVENTTYPE(self))

/*
 * Get an event's type name.
 *
 * @return [String] type name
 * @todo fixme
 */
static VALUE
rg_type_name(VALUE self)
{
    return CSTR2RVAL(libvlc_event_type_name(_SELF(self)));
}

/*
 * Document-class: VLC::Event::Type
 *
 * @todo fixme
 */
void
Init_vlc_event_type(VALUE cEvent)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(VLC_TYPE_EVENT_TYPE, "Type", cEvent);

    RG_DEF_METHOD(type_name, 0);
}
