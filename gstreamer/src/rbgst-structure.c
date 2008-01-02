/*
 * Copyright (C) 2003, 2004 Laurent Sansonetti <lrz@gnome.org>
 * Copyright (C) 2007 Ruby-GNOME2 Project Team
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

#define SELF(obj) (RVAL2GST_STRUCT(obj))

static VALUE
initialize(int argc, VALUE *argv, VALUE self)
{
    VALUE name, fields;
    GstStructure *structure;

    rb_scan_args(argc, argv, "11", &name, &fields);

    structure = gst_structure_empty_new(RVAL2CSTR(name));
    if (!NIL_P(fields)) {
        /* set fields */
    }

    G_INITIALIZE(self, structure);

    return Qnil;
}

static VALUE
get_name(VALUE self)
{
    return CSTR2RVAL(gst_structure_get_name(SELF(self)));
}

static VALUE
set_name(VALUE self, VALUE name)
{
    gst_structure_set_name(SELF(self), RVAL2CSTR(name));
    return Qnil;
}

static VALUE
has_name_p(VALUE self, VALUE name)
{
    return CBOOL2RVAL(gst_structure_has_name(SELF(self), RVAL2CSTR(name)));
}

void
Init_gst_structure(void)
{
    VALUE rb_cGstStructure;

    rb_cGstStructure = G_DEF_CLASS(GST_TYPE_STRUCTURE, "Structure", mGst);

    rb_define_method(rb_cGstStructure, "initialize", initialize, -1);

    rb_define_method(rb_cGstStructure, "name", get_name, 0);
    rb_define_method(rb_cGstStructure, "set_name", set_name, 1);
    rb_define_method(rb_cGstStructure, "has_name?", has_name_p, 1);
    rb_define_alias(rb_cGstStructure, "have_name?", "has_name?");

    G_DEF_SETTERS(rb_cGstStructure);
}
