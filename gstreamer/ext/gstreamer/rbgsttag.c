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

/* Module: Gst::Tag
 * Helper module to the tagging interface.
 */

/*
 * Class method: exists?(tag)
 * tag: the name of the tag.
 *
 * Checks if the given tag is already registered.
 *
 * Returns: true if the tag is registered, false otherwise.
 */
static VALUE
rb_gst_tag_exists (VALUE self, VALUE tag)
{
    return CBOOL2RVAL (gst_tag_exists (RVAL2CSTR (tag)));
}

/*
 * Class method: fixed?(tag)
 * tag: the name of the tag.
 *
 * Checks if the given tag is fixed. A fixed tag can only contain one value. 
 * Unfixed tags can contain lists of values.
 *
 * Returns: true if the tag is fixed, false otherwise.
 */
static VALUE
rb_gst_tag_fixed (VALUE self, VALUE tag)
{
    return CBOOL2RVAL (gst_tag_is_fixed (RVAL2CSTR (tag)));
}

/*
 * Class method: get_nick(tag)
 * tag: the name of the tag.
 *
 * Returns: the human-readable name of this tag.
 */
static VALUE
rb_gst_tag_get_nick (VALUE self, VALUE tag)
{
    return CSTR2RVAL (gst_tag_get_nick (RVAL2CSTR (tag)));
}

/*
 * Class method: get_description(tag)
 * tag: the name of the tag.
 *
 * Returns: the human-readable description of this tag.
 */
static VALUE
rb_gst_tag_get_description (VALUE self, VALUE tag)
{
    return CSTR2RVAL (gst_tag_get_description (RVAL2CSTR (tag)));
}

/*
 * Class method: get_flag(tag)
 * tag: the name of the tag.
 *
 * Gets the flag of the given tag.
 *
 * Returns: the flag of this tag (see Gst::Tag::Flag).
 */
static VALUE
rb_gst_tag_get_flag (VALUE self, VALUE tag)
{
    return GFLAGS2RVAL (gst_tag_get_flag (RVAL2CSTR (tag)),
                        GST_TYPE_TAG_FLAG);
}

void
Init_gst_tag (void)
{
    VALUE m = rb_define_module_under (mGst, "Tag");

    rb_define_module_function (m, "exists?", rb_gst_tag_exists, 1);
    rb_define_module_function (m, "get_nick", rb_gst_tag_get_nick, 1);
    rb_define_module_function (m, "get_description", 
                                rb_gst_tag_get_description, 1);
    rb_define_module_function (m, "get_flag", rb_gst_tag_get_flag, 1);
    rb_define_module_function (m, "fixed?", rb_gst_tag_fixed, 1);

    G_DEF_CLASS (GST_TYPE_TAG_FLAG, "Flag", m);
    G_DEF_CONSTANTS (m, GST_TYPE_TAG_FLAG, "GST_TAG_");
    G_DEF_CLASS (GST_TYPE_TAG_MERGE_MODE, "MergeMode", m);
    G_DEF_CONSTANTS (m, GST_TYPE_TAG_MERGE_MODE, "GST_TAG_");
}
