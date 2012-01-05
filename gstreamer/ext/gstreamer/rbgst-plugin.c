/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2007  Ruby-GNOME2 Project Team
 *  Copyright (C) 2003, 2004 Laurent Sansonetti <lrz@gnome.org>
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

#define RG_TARGET_NAMESPACE cPlugin
#define SELF(self) RVAL2GST_PLUGIN(self)

/* Class: Gst::Plugin
 * Dynamically loadable Elements 
 */

/* Method: loaded?
 * Returns: true if the plugin is loaded into memory, false otherwise.
 */
static VALUE
rg_loaded_p (VALUE self)
{
    GstPlugin *plugin = RGST_PLUGIN (self);
    return CBOOL2RVAL (gst_plugin_is_loaded (plugin));
}

/* Method: filename
 * Returns: the filename where this plugin comes from.
 */
static VALUE
rg_filename (VALUE self)
{
    return CSTR2RVAL (gst_plugin_get_filename (RGST_PLUGIN (self)));
}

/* Method: description 
 * Returns: a long description of the plugin.
 */
static VALUE
rg_description (VALUE self)
{
    return CSTR2RVAL (gst_plugin_get_description (RGST_PLUGIN (self)));
}

/* Method: package 
 * Returns: the name of the package this plugin belongs to.
 */
static VALUE
rg_package (VALUE self)
{
    return CSTR2RVAL (gst_plugin_get_package (RGST_PLUGIN (self)));
}

/* Method: license 
 * Returns: the effective license of the plugin.
 */
static VALUE
rg_license (VALUE self)
{
    return CSTR2RVAL (gst_plugin_get_license (RGST_PLUGIN (self)));
}

static VALUE
rg_source (VALUE self)
{
    return CSTR2RVAL(gst_plugin_get_source(SELF(self)));
}

/* Method: origin
 * Returns: the URL to the provider of the plugin.
 */
static VALUE
rg_origin (VALUE self)
{
    return CSTR2RVAL (gst_plugin_get_origin ( RGST_PLUGIN (self)));
}

/* Method: name 
 * Returns: the unique name of the plugin.
 */
static VALUE
rg_name (VALUE self)
{
    return CSTR2RVAL (gst_plugin_get_name (RGST_PLUGIN (self)));
}

/* Method: minor_version 
 * Returns: the minor version of the core this plugin was compiled for.
 */
static VALUE
rg_minor_version (VALUE self)
{
    return INT2FIX (RGST_PLUGIN (self)->desc.minor_version);
}

/* Method: major_version 
 * Returns: the major version of the core this plugin was compiled for.
 */
static VALUE
rg_major_version (VALUE self)
{
    return INT2FIX (RGST_PLUGIN (self)->desc.major_version);
}

/* Method: version
 * Returns: the version of the plugin (note: as a string).
 */
static VALUE
rg_version (VALUE self)
{
    return CSTR2RVAL (RGST_PLUGIN (self)->desc.version);
}

/* Method: ==(plugin)
 * Returns: true if two Gst::Plugin objects are refered by the same file,
 * false otherwise.
 */
static VALUE
rg_operator_is_equal (VALUE self, VALUE other_plugin)
{
    return NIL_P (other_plugin)
                ? Qfalse
                : rb_equal (rg_filename (self),
                        rg_filename (other_plugin));
}

void
Init_gst_plugin (VALUE mGst)
{
    VALUE RG_TARGET_NAMESPACE;

    RG_TARGET_NAMESPACE = G_DEF_CLASS(GST_TYPE_PLUGIN, "Plugin", mGst);

    RG_DEF_METHOD_P(loaded, 0);
    RG_DEF_METHOD_OPERATOR("==", is_equal, 1);

    RG_DEF_METHOD(filename, 0);
    RG_DEF_METHOD(major_version, 0);
    RG_DEF_METHOD(minor_version, 0);
    RG_DEF_METHOD(name, 0);
    RG_DEF_METHOD(description, 0);
    RG_DEF_METHOD(version, 0);
    RG_DEF_METHOD(license, 0);
    RG_DEF_METHOD(source, 0);
    RG_DEF_METHOD(package, 0);
    RG_DEF_METHOD(origin, 0);
}
