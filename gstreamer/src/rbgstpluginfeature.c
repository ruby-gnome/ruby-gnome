
/*
 * Copyright (C) 2003 Laurent Sansonetti <lrz@gnome.org>
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
 *  Class: Gst::PluginFeature
 *
 *  This is a base class for anything that can be added to a Gst::Plugin. 
 */

/*
 *  Method: name -> aString
 *
 *  Gets the name of the feature, as a String.
 */
static VALUE rb_gst_pluginfeature_get_name(self)
    VALUE self;
{
    GstPluginFeature *feature = RGST_PLUGIN_FEATURE(self);
    return CSTR2RVAL(GST_PLUGIN_FEATURE_NAME(feature));
}

/*
 *  Method: ensure_loaded -> aBoolean
 *
 *  Check if the plugin containing the feature is loaded.
 *  If not, the plugin will be loaded.
 *
 *  Returns a boolean indicating if the feature is loaded.
 */
static VALUE rb_gst_pluginfeature_ensure_loaded(self)
    VALUE self;
{
    return CBOOL2RVAL(gst_plugin_feature_ensure_loaded(
        RGST_PLUGIN_FEATURE(self))
    );
}

/*
 *  Method: unload_thyself -> self
 *
 *  Unload the feature. This will decrease the refcount in the 
 *  plugin and will eventually unload the plugin.
 */
static VALUE rb_gst_pluginfeature_unload_thyself(self)
    VALUE self;
{
    gst_plugin_feature_unload_thyself(RGST_PLUGIN_FEATURE(self));
    return self;
}

gboolean is_valid_pluginfeature_type(type)
    const GType type;
{
    return type == GST_TYPE_AUTOPLUG_FACTORY 
        || type == GST_TYPE_ELEMENT_FACTORY
        || type == GST_TYPE_INDEX_FACTORY
        || type == GST_TYPE_SCHEDULER_FACTORY
        || type == GST_TYPE_TYPE_FACTORY;
}

VALUE instanciate_pluginfeature(feature)
    GstPluginFeature *feature;
{
    VALUE obj = Qnil;

    if (GST_IS_ELEMENT_FACTORY(feature)) {
        obj = RGST_ELEMENT_FACTORY_NEW(feature);
    }
    else if (GST_IS_TYPE_FACTORY(feature)) {
        obj = RGST_TYPE_FACTORY_NEW(feature);
    }
    else if (GST_IS_SCHEDULER_FACTORY(feature)) {
        obj = RGST_SCHEDULER_FACTORY_NEW(feature);
    }
    else if (GST_IS_INDEX_FACTORY(feature)) {
        obj = RGST_INDEX_FACTORY_NEW(feature);
    }
    else if (GST_IS_AUTOPLUG_FACTORY(feature)) {
        obj = RGST_AUTOPLUG_FACTORY_NEW(feature);
    }
    else {
        rb_raise(rb_eArgError,
                 "Invalid plugin feature of type ``%s''",
                 g_type_name(G_OBJECT_TYPE(feature))); 
    }
    return obj;
}

void Init_gst_pluginfeature(void) {
    VALUE c = G_DEF_CLASS(GST_TYPE_PLUGIN_FEATURE, "PluginFeature", mGst);
    rb_define_method(c, "name", rb_gst_pluginfeature_get_name, 0);
    rb_define_method(c, "ensure_loaded", rb_gst_pluginfeature_ensure_loaded, 0);
    rb_define_method(c, "unload_thyself", rb_gst_pluginfeature_unload_thyself, 0);
}

