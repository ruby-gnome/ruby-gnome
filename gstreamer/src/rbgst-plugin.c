/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
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

#define SELF(self) RVAL2GST_PLUGIN(self)

/* Class: Gst::Plugin
 * Dynamically loadable Elements 
 */

/* Method: loaded?
 * Returns: true if the plugin is loaded into memory, false otherwise.
 */
static VALUE
rb_gst_plugin_is_loaded (VALUE self)
{
	GstPlugin *plugin = RGST_PLUGIN (self);
	return CBOOL2RVAL (gst_plugin_is_loaded (plugin));
}

/* Method: filename
 * Returns: the filename where this plugin comes from.
 */
static VALUE
rb_gst_plugin_get_filename (VALUE self)
{
	return CSTR2RVAL (gst_plugin_get_filename (RGST_PLUGIN (self)));
}

/* Method: description 
 * Returns: a long description of the plugin.
 */
static VALUE
rb_gst_plugin_get_description (VALUE self)
{
	return CSTR2RVAL (gst_plugin_get_description (RGST_PLUGIN (self)));
}

/* Method: package 
 * Returns: the name of the package this plugin belongs to.
 */
static VALUE
rb_gst_plugin_get_package (VALUE self)
{
	return CSTR2RVAL (gst_plugin_get_package (RGST_PLUGIN (self)));
}

/* Method: license 
 * Returns: the effective license of the plugin.
 */
static VALUE
rb_gst_plugin_get_license (VALUE self)
{
	return CSTR2RVAL (gst_plugin_get_license (RGST_PLUGIN (self)));
}

static VALUE
get_source (VALUE self)
{
    return CSTR2RVAL(gst_plugin_get_source(SELF(self)));
}

/* Method: origin
 * Returns: the URL to the provider of the plugin.
 */
static VALUE
rb_gst_plugin_get_origin (VALUE self)
{
	return CSTR2RVAL (gst_plugin_get_origin ( RGST_PLUGIN (self)));
}

/* Method: name 
 * Returns: the unique name of the plugin.
 */
static VALUE
rb_gst_plugin_get_name (VALUE self)
{
	return CSTR2RVAL (gst_plugin_get_name (RGST_PLUGIN (self)));
}

/* Method: minor_version 
 * Returns: the minor version of the core this plugin was compiled for.
 */
static VALUE
rb_gst_plugin_get_minor_version (VALUE self)
{
	return INT2FIX (RGST_PLUGIN (self)->desc.minor_version);
}

/* Method: major_version 
 * Returns: the major version of the core this plugin was compiled for.
 */
static VALUE
rb_gst_plugin_get_major_version (VALUE self)
{
	return INT2FIX (RGST_PLUGIN (self)->desc.major_version);
}

/* Method: version
 * Returns: the version of the plugin (note: as a string).
 */
static VALUE
rb_gst_plugin_get_version (VALUE self)
{
	return CSTR2RVAL (RGST_PLUGIN (self)->desc.version);
}

/* Method: ==(plugin)
 * Returns: true if two Gst::Plugin objects are refered by the same file,
 * false otherwise.
 */
static VALUE
rb_gst_plugin_is_equal (VALUE self, VALUE other_plugin)
{
	return NIL_P (other_plugin)
		? Qfalse
		: rb_equal (rb_gst_plugin_get_filename (self),
			    rb_gst_plugin_get_filename (other_plugin));
}

void
Init_gst_plugin (void)
{
    VALUE rb_cGstPlugin;

    rb_cGstPlugin = G_DEF_CLASS(GST_TYPE_PLUGIN, "Plugin", mGst);

    rb_define_method(rb_cGstPlugin, "loaded?", rb_gst_plugin_is_loaded, 0);
    rb_define_method(rb_cGstPlugin, "==", rb_gst_plugin_is_equal, 1);

    rb_define_method(rb_cGstPlugin, "filename", rb_gst_plugin_get_filename, 0);
    rb_define_method(rb_cGstPlugin, "major_version",
                     rb_gst_plugin_get_major_version, 0);
    rb_define_method(rb_cGstPlugin, "minor_version",
                     rb_gst_plugin_get_minor_version, 0);
    rb_define_method(rb_cGstPlugin, "name", rb_gst_plugin_get_name, 0);
    rb_define_method(rb_cGstPlugin, "description",
                     rb_gst_plugin_get_description, 0);
    rb_define_method(rb_cGstPlugin, "version", rb_gst_plugin_get_version, 0);
    rb_define_method(rb_cGstPlugin, "license", rb_gst_plugin_get_license, 0);
    rb_define_method(rb_cGstPlugin, "source", get_source, 0);
    rb_define_method(rb_cGstPlugin, "package", rb_gst_plugin_get_package, 0);
    rb_define_method(rb_cGstPlugin, "origin", rb_gst_plugin_get_origin, 0);
}
