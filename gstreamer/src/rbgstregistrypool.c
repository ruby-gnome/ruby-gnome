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

/* Module: Gst::RegistryPool
 * The registry pool manages the available registries and plugins in the 
 * system.
 */

/* 
 * Class method: list
 * 
 * Gets a list of all registries in the pool.
 * 
 * Returns: an Array of Gst::Registry objects.
 */
static VALUE
rb_gst_rp_list (VALUE self)
{
	GList *list;
	VALUE ary;
	
	list = gst_registry_pool_list ();
	ary = GLIST2ARY (list);
	g_list_free (list);
	return ary;
}

/*
 * Class method: add(registry, priority)
 * registry: a Gst::Registry object to add.
 * priority: an integer value representing the priority of the registry.
 *
 * Adds the given registry to the pool with the given priority.
 *
 * Returns: self.
 */
static VALUE
rb_gst_rp_add (VALUE self, VALUE registry, VALUE priority)
{
	gst_registry_pool_add (RGST_REGISTRY (registry),
			       FIX2INT (priority));
	return self;
}

/*
 * Class method: remove(registry)
 * registry: a Gst::Registry object to remove.
 *
 * Removes the given registry from the pool.
 * 
 * Returns: self.
 */
static VALUE
rb_gst_rp_remove (VALUE self, VALUE registry)
{
	gst_registry_pool_remove (RGST_REGISTRY (registry));
	return self;
}

/*
 * Class method: add_plugin(plugin)
 * plugin: a Gst::Plugin object to add.
 *
 * Adds the given plugin to the global pool of plugins.
 * 
 * Returns: self.
 */
static VALUE
rb_gst_rp_add_plugin (VALUE self, VALUE plugin)
{
	gst_registry_pool_add_plugin (RGST_PLUGIN (plugin));
	return self;
}
			
/*
 * Class method: load_all
 *
 * Loads all the registries in the pool.  Registries with the
 * Gst::Registry::DELAYED_LOADING flag will not be loaded.
 *
 * Returns: self.
 */
static VALUE
rb_gst_rp_load_all (VALUE self)
{
	gst_registry_pool_load_all ();
	return self;
}

/*
 * Class method: plugin_list
 *
 * Gets a list of all plugins in the pool.
 *
 * Returns: an Array of Gst::Plugin objects.
 */
static VALUE
rb_gst_rp_plugin_list (VALUE self)
{
	GList *list;
	VALUE ary;
	
	list = gst_registry_pool_plugin_list ();
	ary = GLIST2ARY (list);
	g_list_free (list);
	return ary;
}

/*
 * Class method: feature_list(klass)
 * klass: a GTyped class object.
 *
 * Gets a list of all plugins features of the given type in the pool.
 *
 * Returns: an Array of Gst::PluginFeature objects.
 */
static VALUE
rb_gst_rp_feature_list (VALUE self, VALUE klass)
{
	GList *list;
	VALUE ary;
	
	list = gst_registry_pool_feature_list (CLASS2GTYPE (klass));
	ary = GLIST2ARY (list);
	g_list_free (list);
	return ary;
}

/*
 * Class method: find_plugin(name)
 * name: the name of the plugin to find.
 *
 * Gets the named plugin from the registry pool.
 *
 * Returns: the Gst::Plugin with the given name, or nil if the plugin was
 * not found.
 */
static VALUE
rb_gst_rp_find_plugin (VALUE self, VALUE name)
{
	GstPlugin *plugin;
	
	plugin = gst_registry_pool_find_plugin (RVAL2CSTR (name));
	return plugin != NULL
		? RGST_PLUGIN_NEW (plugin)
		: Qnil;
}

/*
 * Class method; find_feature(name, klass)
 * name: the name of the plugin feature to find.
 * klass: a GTyped class object.
 *
 * Gets the plugin feature with the given name of type from the pool of
 * registries.
 *
 * Returns: a Gst::PluginFeature with the given name and type, or nil
 * if the plugin feature was not found.
 */
static VALUE
rb_gst_rp_find_feature (VALUE self, VALUE name, VALUE klass)
{
	GstPluginFeature *feature;
	
	feature = gst_registry_pool_find_feature (RVAL2CSTR (name),
						  CLASS2GTYPE (klass));
	return feature != NULL
		? RGST_PLUGIN_FEATURE_NEW (feature)
		: Qnil;
}

/*
 * Class method: get_prefered(flags)
 * flags: the flags for the prefered registry (see Gst::Registry::Flags).
 *
 * Returns: the prefered Gst::Registry with the given flags.
 */
static VALUE
rb_gst_rp_get_prefered (VALUE self, VALUE flags)
{
	GstRegistry *registry;

	registry = gst_registry_pool_get_prefered (RVAL2GFLAGS (flags,
								GST_TYPE_REGISTRY_FLAGS));
	return registry != NULL
		? RGST_REGISTRY_NEW (registry)
		: Qnil;
}

void
Init_gst_registry_pool (void)
{
	VALUE m = rb_define_module_under (mGst, "RegistryPool");

	rb_define_module_function (m, "list", rb_gst_rp_list, 0);
	rb_define_module_function (m, "add", rb_gst_rp_add, 2);
	rb_define_module_function (m, "remove", rb_gst_rp_remove, 1);
	rb_define_module_function (m, "add_plugin", rb_gst_rp_add_plugin, 1);
	rb_define_module_function (m, "load_all", rb_gst_rp_load_all, 0);
	rb_define_module_function (m, "plugin_list", rb_gst_rp_plugin_list, 0);
	rb_define_module_function (m, "feature_list", rb_gst_rp_feature_list, 1);
	rb_define_module_function (m, "find_plugin", rb_gst_rp_find_plugin, 1);
	rb_define_module_function (m, "find_feature", rb_gst_rp_find_feature, 2);
	rb_define_module_function (m, "get_prefered", rb_gst_rp_get_prefered, 1);
}
