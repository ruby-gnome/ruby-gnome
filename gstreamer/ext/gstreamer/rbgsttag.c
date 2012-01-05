/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011-2012  Ruby-GNOME2 Project Team
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

#define RG_TARGET_NAMESPACE mTag

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
rg_m_exists_p (VALUE self, VALUE tag)
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
rg_m_fixed_p (VALUE self, VALUE tag)
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
rg_m_get_nick (VALUE self, VALUE tag)
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
rg_m_get_description (VALUE self, VALUE tag)
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
rg_m_get_flag (VALUE self, VALUE tag)
{
    return GFLAGS2RVAL (gst_tag_get_flag (RVAL2CSTR (tag)),
                        GST_TYPE_TAG_FLAG);
}

void
Init_gst_tag (VALUE mGst)
{
    VALUE RG_TARGET_NAMESPACE = rb_define_module_under (mGst, "Tag");

    RG_DEF_MODFUNC_P(exists, 1);
    RG_DEF_MODFUNC(get_nick, 1);
    RG_DEF_MODFUNC(get_description, 1);
    RG_DEF_MODFUNC(get_flag, 1);
    RG_DEF_MODFUNC_P(fixed, 1);

    G_DEF_CLASS (GST_TYPE_TAG_FLAG, "Flag", RG_TARGET_NAMESPACE);
    G_DEF_CONSTANTS (RG_TARGET_NAMESPACE, GST_TYPE_TAG_FLAG, "GST_TAG_");
    G_DEF_CLASS (GST_TYPE_TAG_MERGE_MODE, "MergeMode", RG_TARGET_NAMESPACE);
    G_DEF_CONSTANTS (RG_TARGET_NAMESPACE, GST_TYPE_TAG_MERGE_MODE, "GST_TAG_");
}
