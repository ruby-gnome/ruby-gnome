
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
 *  Class: Gst::Plugin
 *
 *  Dynamically loadable Elements 
 */

static GstPlugin* plugin_copy(const GstPlugin* plugin) {
    GstPlugin* new_plugin;
    g_return_val_if_fail (plugin != NULL, NULL);
    new_plugin = g_new(GstPlugin, 1);
    *new_plugin = *plugin;
    return new_plugin;
}

GType gst_plugin_get_type(void) {
    static GType our_type = 0;
    if (our_type == 0) {
        our_type = g_boxed_type_register_static ("GstPlugin",
            (GBoxedCopyFunc)plugin_copy,
            (GBoxedFreeFunc)g_free);
    }
    return our_type;
}

/*
 *  Method: name -> aString
 *
 *  Gets (short) name of the plugin, as a String.
 */
static VALUE rb_gst_plugin_get_name(self)
    VALUE self;
{
    GstPlugin *plugin = RGST_PLUGIN(self);
    return CSTR2RVAL(plugin->name);
}

/*
 *  Method: longname -> aString
 *
 *  Gets long name (descriptive) of the plugin, as a String.
 */
static VALUE rb_gst_plugin_get_longname(self)
    VALUE self;
{
    GstPlugin *plugin = RGST_PLUGIN(self);
    return CSTR2RVAL(plugin->longname);
}

/*
 *  Method: filename -> aString
 *
 *  Gets the file name where the plugin cames from, as a String.
 */
static VALUE rb_gst_plugin_get_filename(self)
    VALUE self;
{
    GstPlugin *plugin = RGST_PLUGIN(self);
    return CSTR2RVAL(plugin->filename);
}

/*
 *  Method: loaded? -> aBoolean
 *
 *  Checks if the plugin is loaded into memory.
 */
static VALUE rb_gst_plugin_is_loaded(self)
    VALUE self;
{
    GstPlugin *plugin = RGST_PLUGIN(self);
    return CBOOL2RVAL(gst_plugin_is_loaded(plugin));
}

/*
 *  Method: features -> anArray
 *
 *  Get a list of all the features that this plugin provides, in an array
 *  of Gst::PluginFeature objects. 
 */
static VALUE rb_gst_plugin_get_features(self)
    VALUE self;
{
    GstPlugin *plugin;
    GList *list;
    VALUE arr;

    arr = rb_ary_new();
    plugin = RGST_PLUGIN(self);

    for (list = gst_plugin_get_feature_list(plugin);
         list != NULL;
         list = g_list_next(list))
    {
        GstPluginFeature *feature = GST_PLUGIN_FEATURE(list->data);
        rb_ary_push(arr, instanciate_pluginfeature(feature));
    }
    return arr;
}

/*
 *  Method: each_feature { |aPluginFeature| block } -> nil 
 *
 *  Calls the block for each feature this plugin provides, passing
 *  a reference to the Gst::PluginFeature object as parameter.
 *
 *  Always returns nil.
 */
static VALUE rb_gst_plugin_each_feature(self)
    VALUE self;
{
    return rb_ary_yield(rb_gst_plugin_get_features(self));
}

/*
 *  Method: == anOtherPlugin -> aBoolean
 *
 *  Checks if two Gst::Plugin objects are refered by the same file.
 */
static VALUE rb_gst_plugin_is_equal(self, other_plugin)
    VALUE self, other_plugin;
{
   return NIL_P(other_plugin)
        ? Qfalse
        : rb_equal(rb_gst_plugin_get_filename(self),
                   rb_gst_plugin_get_filename(other_plugin));
}

void Init_gst_plugin(void) {
    VALUE c = G_DEF_CLASS(GST_TYPE_PLUGIN, "Plugin", mGst);

    rb_define_method(c, "name",     rb_gst_plugin_get_name, 0);
    rb_define_method(c, "longname", rb_gst_plugin_get_longname, 0);
    rb_define_method(c, "filename", rb_gst_plugin_get_filename, 0);

    rb_define_method(c, "loaded?", rb_gst_plugin_is_loaded, 0);

    rb_define_method(c, "features", rb_gst_plugin_get_features, 0);
    rb_define_method(c, "each_feature", rb_gst_plugin_each_feature, 0);
    
    rb_define_method(c, "==", rb_gst_plugin_is_equal, 1);
}

