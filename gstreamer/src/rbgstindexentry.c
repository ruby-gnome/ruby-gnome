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

/* Class: Gst::IndexEntry
 * An index's entry. 
 */

/*
 * Method: data
 *
 * Gets the internal data stored in the entry.  The data depends of the type
 * of the entry:
 *	* Gst::IndexEntry::ID: the description of the ID, as a String. 
 *	* Gst::IndexEntry::ASSOCIATION: an Array that contains the number of associations, a Gst::Format, the value of the format and the association flags (see Gst::Index::AssocFlags).
 *	* Gst::IndexEntry::FORMAT: an Array that contains a Gst::Format and its value.
 *	* Gst::IndexEntry::OBJECt: not yet implemented.
 *
 * Returns: the internal data of the entry. 
 */
static VALUE
rb_gst_index_entry_get_data (VALUE self)
{
    GstIndexEntry *entry;
    VALUE data;

    entry = RGST_INDEX_ENTRY (self);
    switch (entry->type) {
    case GST_INDEX_ENTRY_ID:
        data = CSTR2RVAL (entry->data.id.description);
        break;

    case GST_INDEX_ENTRY_ASSOCIATION:
        data = rb_ary_new ();
        rb_ary_push (data, INT2FIX (entry->data.assoc.nassocs));
        rb_ary_push (data, RGST_FORMAT_NEW (entry->data.assoc.assocs->format));
        rb_ary_push (data, ULL2NUM (entry->data.assoc.assocs->value));
        rb_ary_push (data,
                     GFLAGS2RVAL (entry->data.assoc.flags,
                                  GST_TYPE_ASSOC_FLAGS));
        break;

    case GST_INDEX_ENTRY_FORMAT:
        data = rb_ary_new ();
        rb_ary_push (data, RGST_FORMAT_NEW (entry->data.format.format));
        rb_ary_push (data, CSTR2RVAL (entry->data.format.key));
        break;

    case GST_INDEX_ENTRY_OBJECT:   /* TODO */
    default:
        data = Qnil;
    }
    return data;
}

/* Method: entry_type
 * Returns: the type of the entry (see Gst::IndexEntry::Types).
 */
static VALUE
rb_gst_index_entry_get_type (VALUE self)
{
    GstIndexEntry *entry = RGST_INDEX_ENTRY (self);

    return GENUM2RVAL (entry->type, GST_TYPE_INDEX_ENTRY_TYPE);
}

void
Init_gst_indexentry (void)
{
    VALUE c = G_DEF_CLASS (GST_TYPE_INDEX_ENTRY, "IndexEntry", mGst);

    rb_undef_method (c, "initialize");
    rb_define_method (c, "data", rb_gst_index_entry_get_data, 0);
    rb_define_method (c, "entry_type", rb_gst_index_entry_get_type, 0);

    G_DEF_CLASS (GST_TYPE_INDEX_ENTRY_TYPE, "Types", c);
    G_DEF_CONSTANTS (c, GST_TYPE_INDEX_ENTRY_TYPE, "GST_INDEX_ENTRY_");
}
