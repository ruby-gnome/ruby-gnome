
/*
 * Copyright (C) 2003 Laurent Sansonetti <laurent@datarescue.be>
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

/*
 *  Class: Gst::EventMask
 *
 *  Event mask. 
 */

static GstEventMask* event_mask_copy(const GstEventMask* eventmask) {
    GstEventMask* new_eventmask;
    g_return_val_if_fail (eventmask != NULL, NULL);
    new_eventmask = g_new(GstEventMask, sizeof(GstEventMask));
    *new_eventmask = *eventmask;
    return new_eventmask;
}

GType gst_event_mask_get_type(void) {
    static GType our_type = 0;
    if (our_type == 0) {
        our_type = g_boxed_type_register_static ("GstEventMask",
            (GBoxedCopyFunc)event_mask_copy,
            (GBoxedFreeFunc)g_free);
    }
    return our_type;
}

/*
 *  Method: type_id -> aFixnum
 *
 *  Gets the type id of the Gst::Event type referred by this
 *  event mask.
 */
static VALUE rb_gst_eventmask_get_type_id(self)
    VALUE self;
{
    return INT2FIX(RGST_EVENT_MASK(self)->type);
}

/*
 *  Method: type_flags -> anArray 
 *
 *  Returns all the flags available as an array of Fixnums.
 */
static VALUE rb_gst_eventmask_get_flags(self)
    VALUE self;
{
    GFlagsClass *flags_class;
    GstEventMask *mask;
    gint flags, index;
    GEnumValue *value;
    VALUE arr;

    mask = RGST_EVENT_MASK(self);
    flags_class = NULL;
    flags = index = 0;

    switch (mask->type) {
        case GST_EVENT_SEEK:
            flags = mask->flags;
            flags_class = (GFlagsClass *) 
                g_type_class_ref(gst_seek_type_get_type());
            break;
        default:
            break;
    }

    value = g_enum_get_value((GEnumClass*)g_type_class_ref(gst_event_type_get_type()), 
                             mask->type);
    arr = rb_ary_new();

    while (flags) {
        GFlagsValue *value;
        if (flags & 1) {
            value = g_flags_get_first_value(flags_class, 1 << index);
            rb_ary_push(arr, INT2FIX(value->value));
        }
        flags >>= 1;
        index++;
    }

    return arr;
}

void Init_gst_eventmask(void) {
    VALUE c = G_DEF_CLASS(GST_TYPE_EVENT_MASK, "EventMask", mGst);
    rb_define_method(c, "type_id",    rb_gst_eventmask_get_type_id, 0);
    rb_define_method(c, "type_flags", rb_gst_eventmask_get_flags,   0);
}

