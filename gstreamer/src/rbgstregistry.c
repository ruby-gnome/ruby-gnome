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

/* Class: Gst::Registry
 * Abstract class for managing plugins. 
 */

static VALUE
rb_gst_registry_get_default(VALUE self)
{
    return RGST_REGISTRY_NEW(gst_registry_get_default());
}

/*
 * Method: plugins
 *
 * Gets a list of all plugins in the pool.
 *
 * Returns: an array of Gst::Plugin objects.
 */
static VALUE
rb_gst_registry_get_plugins(VALUE self)
{
    GList *list, *node;
    VALUE arr;

    arr = rb_ary_new();
    list = gst_registry_get_plugin_list(RGST_REGISTRY(self));
    for (node = list;
         node != NULL;
         node = g_list_next(node)) {
        GstPlugin *plugin = (GstPlugin *)node->data;
        rb_ary_push(arr, RGST_PLUGIN_NEW(plugin));
    }
    g_list_free(list);
    return arr;
}

/*
 * Method: each_plugin { |plugin| ... }
 *
 * Calls the block for each plugin in the pool, passing a reference to
 * the Gst::Plugin as parameter.
 *
 * Returns: always nil.
 */
static VALUE
rb_gst_registry_each_plugin (VALUE self)
{
	return rb_ary_yield (rb_gst_registry_get_plugins (self));
}

/*
 * Method: get_features(feature_type_or_plugin_name)
 * feature_type_or_plugin_name: a feature type or plugin name.
 *
 * Gets a list of all plugin features of the given type in the pool.
 *
 * Valid features types are Gst::AutoplugFactory, Gst::ElementFactory,
 * Gst::IndexFactory, Gst::SchedulerFactory and Gst::TypeFactory.
 *
 * Returns: an array of Gst::PluginFeature objects.
 */
static VALUE
rb_gst_registry_get_features(VALUE self, VALUE type_or_plugin_name)
{
    GList *list, *node;
    GType gtype;
    VALUE arr;

    if (RVAL2CBOOL(rb_obj_is_kind_of(type_or_plugin_name, rb_cString))) {
        list = gst_registry_get_feature_list_by_plugin(RGST_REGISTRY(self),
                                                       RVAL2CSTR(type_or_plugin_name));
    } else {
        gtype = CLASS2GTYPE(type_or_plugin_name);
        if (!is_valid_pluginfeature_type(gtype))
            rb_raise (rb_eArgError, "Invalid feature type.");

        list = gst_registry_get_feature_list(RGST_REGISTRY(self), gtype);
    }

    arr = rb_ary_new();
    for (node = list; node != NULL; node = g_list_next(node)) {
        rb_ary_push(arr, instanciate_pluginfeature(GST_PLUGIN_FEATURE(node->data)));
    }
    g_list_free (list);

    return arr;
}

/*
 * Method: each_feature(feature_type) { |plugin_feature| ... }
 *
 * Calls the block for each plugin features of the given type in the pool, 
 * passing a reference to the Gst::PluginFeature as parameter.
 *
 * Valid features types are Gst::AutoplugFactory, Gst::ElementFactory,
 * Gst::IndexFactory, Gst::SchedulerFactory and Gst::TypeFactory.
 *
 * Returns: always nil.
 */
static VALUE
rb_gst_registry_each_feature (VALUE self, VALUE type)
{
	return rb_ary_yield (rb_gst_registry_get_features (self, type));
}

/*
 * Method: paths
 *
 * Gets a list of paths of the registry.
 *
 * Returns: an array of strings.
 */
static VALUE
rb_gst_registry_get_paths (VALUE self)
{
	GList *list;
	VALUE arr;

	arr = rb_ary_new ();
	for (list = gst_registry_get_path_list (RGST_REGISTRY (self));
	     list != NULL;
	     list = g_list_next (list))
		rb_ary_push (arr, CSTR2RVAL ((gchar *) list->data)); 
	g_list_free (list);
	return arr;
}

/*
 * Method: each_path { |path| ... } 
 *
 * Calls the block for each path of the registry, passing a reference to
 * the path (a String object) as parameter.
 *
 * Returns: always nil.
 */
static VALUE
rb_gst_registry_each_path (VALUE self)
{
	return rb_ary_yield (rb_gst_registry_get_paths (self));
}

/*
 * Method: add_path(path)
 * path: a path (a String object).
 *
 * Add the given path to the registry. 
 * The syntax of the path is specific to the registry. 
 * If the path has already been added, do nothing.
 *
 * Returns: nil.
 */
static VALUE
rb_gst_registry_add_path(VALUE self, VALUE path)
{
    gst_registry_add_path(RGST_REGISTRY(self), RVAL2CSTR(path));
    return Qnil;
}

/*
 * Method: add_plugin(plugin)
 * plugin: a Gst::Plugin.
 *
 * Adds a plugin to the registry. The 'plugin-added' signal will be emitted.
 *
 * Returns: true on success, false otherwise.
 */
static VALUE
rb_gst_registry_add_plugin(VALUE self, VALUE plugin)
{
    return CBOOL2RVAL(gst_registry_add_plugin(RGST_REGISTRY (self),
                                              RGST_PLUGIN (plugin)));
}

/*
 * Method: remove_plugin(plugin)
 * plugin: a Gst::Plugin.
 *
 * Removes a plugin from the registry.
 *
 * Returns: self.
 */
static VALUE
rb_gst_registry_remove_plugin (VALUE self, VALUE plugin)
{
	gst_registry_remove_plugin (RGST_REGISTRY (self),
				    RGST_PLUGIN (plugin));
	return self;
}

/*
 * Method: find_plugin(name)
 * name: a plugin name.
 *  
 * Finds the plugin with the given name in the registry.
 *
 * Returns: a reference to a Gst::Plugin object if found,
 * otherwise returns nil.
 */
static VALUE
rb_gst_registry_find_plugin (VALUE self, VALUE name)
{
	GstPlugin *plugin = gst_registry_find_plugin (RGST_REGISTRY (self),
						      RVAL2CSTR (name));
	return plugin != NULL
		? RGST_PLUGIN_NEW (plugin)
		: Qnil;
}

/*
 * Method: find_feature(feature_name, feature_type)
 * feature_name: a feature name.
 * feature_type: a feature type.
 * 
 * Finds the plugin feature with the given name and type in the registry.
 *
 * Valid features types are Gst::AutoplugFactory, Gst::ElementFactory,
 * Gst::IndexFactory, Gst::SchedulerFactory and Gst::TypeFactory.
 *
 * Returns: a reference to a Gst::PluginFeature on success, or nil if the
 * named plugin feature is not found.
 */
static VALUE
rb_gst_registry_find_feature(VALUE self, VALUE name, VALUE type)
{
	GstPluginFeature *feature;
	GType gtype;
	
	gtype = CLASS2GTYPE (type);
	if (!is_valid_pluginfeature_type (gtype))
		rb_raise (rb_eArgError, "Invalid feature type.");
	feature = gst_registry_find_feature (RGST_REGISTRY (self),
					     RVAL2CSTR (name),
					     gtype);
	return feature != NULL
		? instanciate_pluginfeature (feature)
		: Qnil;
}

void
Init_gst_registry (void)
{
	VALUE c = G_DEF_CLASS (GST_TYPE_REGISTRY, "Registry", mGst); 

	rb_define_singleton_method(c, "default", rb_gst_registry_get_default, 0);
	
	rb_define_method(c, "plugins", rb_gst_registry_get_plugins, 0);
	rb_define_method(c, "each_plugin", rb_gst_registry_each_plugin, 0);
	rb_define_method(c, "get_features", rb_gst_registry_get_features, 1);
	rb_define_method(c, "each_feature", rb_gst_registry_each_feature, 1);
	rb_define_method(c, "paths", rb_gst_registry_get_paths, 0);
	rb_define_method(c, "each_path", rb_gst_registry_each_path, 0);
	rb_define_method(c, "add_path",	rb_gst_registry_add_path, 1);
	rb_define_method(c, "add_plugin", rb_gst_registry_add_plugin, 1);
	rb_define_method(c, "remove_plugin", rb_gst_registry_remove_plugin, 1);
	rb_define_method(c, "find_plugin", rb_gst_registry_find_plugin, 1);
	rb_define_method(c, "find_feature", rb_gst_registry_find_feature, 2);
}
