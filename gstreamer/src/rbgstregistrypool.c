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

/* FIXME: Document me */

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

static VALUE
rb_gst_rp_add (VALUE self, VALUE registry, VALUE priority)
{
	gst_registry_pool_add (RGST_REGISTRY (registry),
			       FIX2INT (priority));
	return self;
}

static VALUE
rb_gst_rp_remove (VALUE self, VALUE registry)
{
	gst_registry_pool_remove (RGST_REGISTRY (registry));
	return self;
}

static VALUE
rb_gst_rp_add_plugin (VALUE self, VALUE plugin)
{
	gst_registry_pool_add_plugin (RGST_PLUGIN (plugin));
	return self;
}
			
static VALUE
rb_gst_rp_load_all (VALUE self)
{
	gst_registry_pool_load_all ();
	return self;
}

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

static VALUE
rb_gst_rp_find_plugin (VALUE self, VALUE name)
{
	GstPlugin *plugin;
	
	plugin = gst_registry_pool_find_plugin (RVAL2CSTR (name));
	return plugin != NULL
		? RGST_PLUGIN_NEW (plugin)
		: Qnil;
}

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
	rb_define_alias (m, "registries", "list");
	rb_define_module_function (m, "add", rb_gst_rp_add, 2);
	rb_define_module_function (m, "remove", rb_gst_rp_remove, 1);
	rb_define_module_function (m, "add_plugin", rb_gst_rp_add_plugin, 1);
	rb_define_module_function (m, "load_all", rb_gst_rp_load_all, 0);
	rb_define_module_function (m, "plugin_list", rb_gst_rp_plugin_list, 0);
	rb_define_alias (m, "plugins", "plugin_list");
	rb_define_module_function (m, "feature_list", rb_gst_rp_feature_list, 1);
	rb_define_alias (m, "features", "feature_list");
	rb_define_module_function (m, "find_plugin", rb_gst_rp_find_plugin, 1);
	rb_define_module_function (m, "find_feature", rb_gst_rp_find_feature, 2);
	rb_define_module_function (m, "get_prefered", rb_gst_rp_get_prefered, 1);
}
