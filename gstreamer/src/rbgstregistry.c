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

/* Class: Gst::Registry
 * Abstract class for managing plugins. 
 */

/*
 * Class method: plugins
 *
 * Gets a list of all plugins in the pool.
 *
 * Returns: an array of Gst::Plugin objects.
 */
static VALUE
rb_gst_registry_get_plugins (VALUE self)
{
	GList *list;
	VALUE arr;

	arr = rb_ary_new ();
	for (list = gst_registry_pool_plugin_list ();
	     list != NULL;
	     list = g_list_next (list)) {	 
		GstPlugin *plugin = (GstPlugin *) list->data;
		rb_ary_push (arr, RGST_PLUGIN_NEW (plugin));
	}
	g_list_free (list);
	return arr;
} 

/*
 * Class method: each_plugin { |plugin| ... }
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
 * Class method: find_plugin(name)
 * name: a plugin name.
 *
 * Finds the plugin with the given name in the registry. 
 *
 * Returns: a reference to the Gst::Plugin object if found, otherwise
 * returns nil.
 */
static VALUE 
rb_gst_registry_find_plugin (VALUE self, VALUE name)
{
	GstPlugin *plugin = gst_registry_pool_find_plugin (RVAL2CSTR (name));
	return plugin != NULL
		? RGST_PLUGIN_NEW (plugin)
		: Qnil; 
}

/*
 * Class method: registries
 *
 * Gets a list of all registries in the pool.
 *
 * Returns: an Array of Gst::Registry objects.
 */
static VALUE
rb_gst_registry_get_registries (VALUE self)
{
	GList *list;
	VALUE arr;

	arr = rb_ary_new ();
	for (list = gst_registry_pool_list ();
	     list != NULL;
	     list = g_list_next (list)) {
		GstRegistry *reg = (GstRegistry *) list->data;
		rb_ary_push (arr, RGST_REGISTRY_NEW (reg)); 
	}
	g_list_free (list);
	return arr;
}

/*
 * Class method: each { |registry| ... } -> nil
 *
 * Calls the block for each registries in the pool, passing a reference to
 * the Gst::Registry as parameter.
 *
 * Returns: always nil.
 */
static VALUE
rb_gst_registry_each (VALUE self)
{
	return rb_ary_yield (rb_gst_registry_get_registries (self));
}

/*
 * Class method: get_features(feature_type)
 * feature_type: a feature type.
 *
 * Gets a list of all plugin features of the given type in the pool.
 *
 * Valid features types are Gst::AutoplugFactory, Gst::ElementFactory,
 * Gst::IndexFactory, Gst::SchedulerFactory and Gst::TypeFactory.
 *
 * Returns: an array of Gst::PluginFeature objects.
 */
static VALUE
rb_gst_registry_get_features (VALUE self, VALUE type)
{
	GList *list;
	GType gtype;
	VALUE arr;

	arr = rb_ary_new ();
	gtype = CLASS2GTYPE (type);
	if (!is_valid_pluginfeature_type (gtype))
		rb_raise (rb_eArgError, "Invalid feature type.");
	else {
		for (list = gst_registry_pool_feature_list (gtype);
		     list != NULL;
		     list = g_list_next (list))
			rb_ary_push (arr, instanciate_pluginfeature (GST_PLUGIN_FEATURE (list->data)));
		g_list_free (list);
	}
	return arr;
}

/*
 * Class method: each_feature(feature_type) { |plugin_feature| ... }
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
 * Class method: find_feature(feature_name, feature_type)
 * feature_name: a name of a feature.
 * feature_type: a type of a feature.
 *
 * Finds the plugin feature with the given name and type from the pool of 
 * registries, as a Gst::PluginFeature reference.  
 *
 * Valid features types are Gst::AutoplugFactory, Gst::ElementFactory,
 * Gst::IndexFactory, Gst::SchedulerFactory and Gst::TypeFactory.
 *
 * Returns: a Gst::PluginFeature reference, or nil if the named plugin
 * feature is not found.
 */
static VALUE
rb_gst_registry_find_feature (VALUE self, VALUE name, VALUE type)
{
	GstPluginFeature *feature;
	GType gtype;
	
	gtype = CLASS2GTYPE (type);
	if (!is_valid_pluginfeature_type (gtype))
		rb_raise (rb_eArgError, "Invalid feature type.");
	feature = gst_registry_pool_find_feature (RVAL2CSTR (name),
						  gtype);
	return feature != NULL
		? instanciate_pluginfeature (feature)
		: Qnil;
}

/*
 * Class method: get_prefered(flags)
 * flags: registry flags (see Gst::Registry::Flags).
 *
 * Returns: the prefered registry with the given flags.
 */
static VALUE
rb_gst_registry_get_prefered (VALUE self, VALUE flags)
{
	GstRegistry *registry = gst_registry_pool_get_prefered (RVAL2GENUM (flags, GST_TYPE_REGISTRY_FLAGS));
	return registry != NULL
		? RGST_REGISTRY_NEW (registry)
		: Qnil;
}

/*
 * Class method: add(registry, priority)
 * registry: a Gst::Registry.
 * priority: the priority (as a Fixnum).
 *
 * Adds the registry to the pool with the given priority.
 *
 * Returns: always nil.
 */
static VALUE
rb_gst_registry_add_registry (VALUE self, VALUE registry, VALUE priority)
{
	gst_registry_pool_add (RGST_REGISTRY (registry), FIX2INT (priority));
	return Qnil;
}

/*
 * Class method: remove(registry)
 * registry: a Gst::Registry.
 *
 * Removes the registry from the pool.
 *
 * Returns: always nil.
 */
static VALUE
rb_gst_registry_remove_registry (VALUE self, VALUE registry)
{
	gst_registry_pool_remove (RGST_REGISTRY (registry));
	return Qnil;
}

/*
 * Class method: add_plugin(plugin)
 * plugin: a Gst::Plugin.
 *
 * Adds the plugin to the global pool of plugins.
 *
 * Returns: always nil.
 */
static VALUE
rb_gst_registry_add_plugin (VALUE self, VALUE plugin)
{
	gst_registry_pool_add_plugin (RGST_PLUGIN (plugin));
	return Qnil;
}

/*
 * Class method: load_all
 *
 * Loads all the registries in the pool. 
 * Registries with the Gst::Registry::DELAYED_LOADING flag on will not be loaded.
 *
 * Returns: always nil.
 */
static VALUE
rb_gst_registry_load_all (VALUE self)
{
	gst_registry_pool_load_all ();
	return Qnil;
}

/* Method: name
 * Returns: the name of the registry.
 */
static VALUE
rb_gst_registry_get_name (VALUE self)
{
	GstRegistry *reg = RGST_REGISTRY (self);
	return CSTR2RVAL (reg->name);
}

/* Method: details
 * Returns: some details about the registry, or nil
 * if the registry has no details.
 */
static VALUE
rb_gst_registry_get_details (VALUE self)
{
	GstRegistry *reg = RGST_REGISTRY (self);
	return CSTR2RVAL (reg->details);
}

/* Method: loaded?
 * Returns: true if the registry is currently loaded, false otherwise.
 */
static VALUE
rb_gst_registry_is_loaded (VALUE self)
{
	GstRegistry *reg = RGST_REGISTRY (self);
	return CBOOL2RVAL (gst_registry_is_loaded (reg));
}

/*
 * Method: load
 *
 * Loads the registry.
 *
 * Returns: true on success, otherwise false.
 */
static VALUE
rb_gst_registry_load (VALUE self)
{
	return CBOOL2RVAL (gst_registry_load (RGST_REGISTRY (self)));
}

/*
 * Method: save
 *
 * Saves the contents of the registry.
 *
 * Returns: true on success, otherwise false.
 */
static VALUE
rb_gst_registry_save (VALUE self)
{
	return CBOOL2RVAL (gst_registry_save (RGST_REGISTRY (self)));
}

/*
 * Method: rebuild
 *
 * Rebuilds the registry.
 *
 * Returns: true on success, otherwise false.
 */
static VALUE
rb_gst_registry_rebuild (VALUE self)
{
	return CBOOL2RVAL (gst_registry_rebuild (RGST_REGISTRY (self)));
}

/*
 * Method: unload
 *
 * Unloads the registry.
 *
 * Returns: true on success, otherwise false.
 */
static VALUE
rb_gst_registry_unload (VALUE self)
{
	return CBOOL2RVAL (gst_registry_unload (RGST_REGISTRY (self)));
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
 * Returns: self.
 */
static VALUE
rb_gst_registry_add_path (VALUE self, VALUE path)
{
	gst_registry_add_path (RGST_REGISTRY (self), RVAL2CSTR (path));
	return self;
}

/*
 * Method: clear_paths
 *
 * Clears the paths of the registry.
 *
 * Returns: self.
 */
static VALUE
rb_gst_registry_clear_paths (VALUE self)
{
	gst_registry_clear_paths (RGST_REGISTRY (self));
	return self;
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
rb_gst_registry_add_plugin2 (VALUE self, VALUE plugin)
{
	return CBOOL2RVAL (gst_registry_add_plugin (RGST_REGISTRY (self),
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
rb_gst_registry_find_plugin2 (VALUE self, VALUE name)
{
	GstPlugin *plugin = gst_registry_find_plugin (RGST_REGISTRY (self),
						      RVAL2CSTR (name));
	return plugin != NULL
		? RGST_PLUGIN_NEW (plugin)
		: Qnil;
}

/*
 * Method: load_plugin(plugin)
 * plugin: a Gst::Plugin.
 *
 * Brings the specified plugin from the registry into memory.
 *
 * Returns: a code indicating the result (see Gst::Registry::Return).
 */
static VALUE
rb_gst_registry_load_plugin (VALUE self, VALUE plugin)
{
	return GENUM2RVAL (gst_registry_load_plugin (RGST_REGISTRY (self),
						     RGST_PLUGIN (plugin)),
			   GST_TYPE_REGISTRY_RETURN);
}

/*
 * Method: unload_plugin(plugin)
 * plugin: a Gst::Plugin.
 *
 * Unloads a plugin from the registry.
 *
 * Returns: a code indicating the result (see Gst::Registry::Return).
 */
static VALUE
rb_gst_registry_unload_plugin (VALUE self, VALUE plugin)
{
	return GENUM2RVAL (gst_registry_unload_plugin (RGST_REGISTRY (self),
						       RGST_PLUGIN (plugin)),
			   GST_TYPE_REGISTRY_RETURN);
}

/*
 * Method: update_plugin(plugin)
 * plugin: a Gst::Plugin.
 *
 * Updates a plugin in the registry.
 *
 * Returns: a code indicating the result (see Gst::Registry::Return).
 */
static VALUE
rb_gst_registry_update_plugin (VALUE self, VALUE plugin)
{
	return GENUM2RVAL (gst_registry_update_plugin (RGST_REGISTRY (self),
						       RGST_PLUGIN (plugin)),
			   GST_TYPE_REGISTRY_RETURN);
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
rb_gst_registry_find_feature2 (VALUE self, VALUE name, VALUE type)
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

	rb_define_singleton_method (c, "plugins", rb_gst_registry_get_plugins, 0);
	rb_define_singleton_method (c, "each_plugin", rb_gst_registry_each_plugin, 0);
	rb_define_singleton_method (c, "find_plugin", rb_gst_registry_find_plugin, 1);
	rb_define_singleton_method (c, "add_plugin", rb_gst_registry_add_plugin, 1);
	rb_define_singleton_method (c, "each", rb_gst_registry_each, 0);
	rb_define_singleton_method (c, "registries", rb_gst_registry_get_registries, 0);
	rb_define_singleton_method (c, "add", rb_gst_registry_add_registry, 2);
	rb_define_singleton_method (c, "remove", rb_gst_registry_remove_registry, 1);
	rb_define_singleton_method (c, "load_all", rb_gst_registry_load_all, 0);
	rb_define_singleton_method (c, "get_features", rb_gst_registry_get_features, 1);
	rb_define_singleton_method (c, "each_feature", rb_gst_registry_each_feature, 1);
	rb_define_singleton_method (c, "find_feature", rb_gst_registry_find_feature, 2);
	rb_define_singleton_method (c, "get_prefered", rb_gst_registry_get_prefered, 1);
	
	rb_define_method(c, "name", rb_gst_registry_get_name, 0);
	rb_define_method(c, "details", rb_gst_registry_get_details, 0);
	rb_define_method(c, "loaded?", rb_gst_registry_is_loaded, 0);
	rb_define_method(c, "load", rb_gst_registry_load, 0);
	rb_define_method(c, "save", rb_gst_registry_save, 0);
	rb_define_method(c, "rebuild", rb_gst_registry_rebuild, 0);
	rb_define_method(c, "unload", rb_gst_registry_unload, 0);
	rb_define_method(c, "paths", rb_gst_registry_get_paths, 0);
	rb_define_method(c, "each_path", rb_gst_registry_each_path, 0);
	rb_define_method(c, "add_path",	rb_gst_registry_add_path, 1);
	rb_define_method(c, "clear_paths", rb_gst_registry_clear_paths, 0);
	rb_define_method(c, "add_plugin", rb_gst_registry_add_plugin2, 1);
	rb_define_method(c, "remove_plugin", rb_gst_registry_remove_plugin, 1);
	rb_define_method(c, "find_plugin", rb_gst_registry_find_plugin2, 1);
	rb_define_method(c, "load_plugin", rb_gst_registry_load_plugin, 1);
	rb_define_method(c, "unload_plugin", rb_gst_registry_unload_plugin, 1);
	rb_define_method(c, "update_plugin", rb_gst_registry_update_plugin, 1);
	rb_define_method(c, "find_feature", rb_gst_registry_find_feature2, 2);

	G_DEF_CLASS (GST_TYPE_REGISTRY_FLAGS, "Flags", c);
	G_DEF_CONSTANTS (c, GST_TYPE_REGISTRY_FLAGS, "GST_REGISTRY_");
	G_DEF_CLASS (GST_TYPE_REGISTRY_RETURN, "Return", c);
	G_DEF_CONSTANTS (c, GST_TYPE_REGISTRY_RETURN, "GST_REGISTRY_");
}
