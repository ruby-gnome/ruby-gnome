/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2003-2005 Laurent Sansonetti <lrz@gnome.org>
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

#define RG_TARGET_NAMESPACE mTagSetter

/* Class: Gst::TagSetter
 * Element interface that allows the setting and retrieval of media metadata.
 */

/*
 * Method: merge(structure, merge_mode)
 * structure: a Ruby hash representing the tag list of merge from.
 * merge_mode: the mode to merge with (see Gst::Tag::MergeMode).
 *
 * Merges the given tag list in the setter's list using the given mode.
 *
 * Returns: self.
 */
static VALUE
rg_merge (VALUE self, VALUE structure, VALUE merge_mode)
{
    gst_tag_setter_merge_tags(RGST_TAG_SETTER(self),
                              RVAL2GST_STRUCT(structure),
                              RVAL2GENUM(merge_mode, GST_TYPE_TAG_MERGE_MODE));
    return self;
}

void
Init_gst_tag_setter (VALUE mGst)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_INTERFACE (GST_TYPE_TAG_SETTER, "TagSetter", mGst);
    RG_DEF_METHOD(merge, 2);   
}
