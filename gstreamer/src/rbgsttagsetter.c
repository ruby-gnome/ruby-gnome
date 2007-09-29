/*
 * Copyright (C) 2003-2005 Laurent Sansonetti <lrz@gnome.org>
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
rb_gst_tag_setter_merge (VALUE self, VALUE structure, VALUE merge_mode)
{
    gst_tag_setter_merge_tags(RGST_TAG_SETTER(self),
                              ruby_hash_to_gst_structure_with_name(structure, GST_TAG_LIST_STRUCTURE_NAME),
                              RVAL2GENUM(merge_mode, GST_TYPE_TAG_MERGE_MODE));
    return self;
}

void
Init_gst_tag_setter (void)
{
	VALUE c = G_DEF_INTERFACE (GST_TYPE_TAG_SETTER, "TagSetter", mGst);
	rb_define_method (c, "merge", rb_gst_tag_setter_merge, 2);	
}
