
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
 *  Class: Gst::Registry
 *
 *  Abstract class for managing plugins. 
 */

/*
 *  Class method: plugins -> anArray
 *
 *  Get a list of all plugins in the pool, in an array of Gst::Plugin objects.
 */
static VALUE rb_gst_registry_get_plugins(self)
    VALUE self;
{
    GList *list;
    VALUE arr;

    arr  = rb_ary_new();
    for (list = gst_registry_pool_plugin_list();
         list != NULL;
         list = g_list_next(list))
    {     
        GstPlugin *plugin = (GstPlugin *) list->data;
        rb_ary_push(arr, RGST_PLUGIN_NEW(plugin));
    }
    g_list_free(list);
    return arr;
} 

/*
 *  Class method: each_plugin { |aPlugin| block } -> nil
 *
 *  Calls the block for each plugin in the pool, passing a reference to
 *  the Gst::Plugin as parameter.
 *
 *  Always returns nil.
 */
static VALUE rb_gst_registry_each_plugin(self)
    VALUE self;
{
    return rb_ary_yield(rb_gst_registry_get_plugins(self));
}

/*
 *  Class method: find_plugin(aNameString) -> aGstPluginObject
 *
 *  Find the plugin with the given name in the registry. 
 *  If founds, returns a reference to the Gst::Plugin object, otherwise
 *  returns nil.
 */
static VALUE rb_gst_registry_find_plugin(self, name)
    VALUE self, name;
{
    GstPlugin *plugin = gst_registry_pool_find_plugin(RVAL2CSTR(name));
    return plugin != NULL
        ? RGST_PLUGIN_NEW(plugin)
        : Qnil; 
}

/*
 *  Class method: registries -> anArray
 *
 *  Gets a list of all registries in the pool, in an Array
 *  of Gst::Registry objects.
 */
static VALUE rb_gst_registry_get_registries(self)
    VALUE self;
{
    GList *list;
    VALUE arr;

    arr = rb_ary_new();
    for (list = gst_registry_pool_list();
         list != NULL;
         list = g_list_next(list))
    {
        GstRegistry *reg = (GstRegistry *) list->data;
        rb_ary_push(arr, RGST_REGISTRY_NEW(reg)); 
    }
    g_list_free(list);
    return arr;
}

/*
 *  Class method: each { |aRegistryObject| block } -> nil
 *
 *  Calls the block for each registries in the pool, passing a reference to
 *  the Gst::Registry as parameter.
 *
 *  Always returns nil.
 */
static VALUE rb_gst_registry_each(self)
    VALUE self;
{
    return rb_ary_yield(rb_gst_registry_get_registries(self));
}

/*
 *  Method: name -> aString
 *
 *  Gets the name of the registry, as a String.
 */
static VALUE rb_gst_registry_get_name(self)
    VALUE self;
{
    GstRegistry *reg = RGST_REGISTRY(self);
    return CSTR2RVAL(reg->name);
}

/*
 *  Method: details -> aString
 *
 *  Gets some details about the registry, as a String.  
 *  Returns nil if the registry has no details.
 */
static VALUE rb_gst_registry_get_details(self)
    VALUE self;
{
    GstRegistry *reg = RGST_REGISTRY(self);
    return CSTR2RVAL(reg->details);
}

/*
 *  Method: loaded? -> aBoolean
 *
 *  Checks if the registry is currently loaded.
 */
static VALUE rb_gst_registry_is_loaded(self)
    VALUE self;
{
    GstRegistry *reg = RGST_REGISTRY(self);
    return CBOOL2RVAL(gst_registry_is_loaded(reg));
}

/*
 *  Method: paths -> anArray
 *
 *  Gets a list of paths of the registry, in an Array of String objects.
 */
static VALUE rb_gst_registry_get_paths(self)
    VALUE self;
{
    GstRegistry *reg;
    GList *list;
    VALUE arr;

    reg = RGST_REGISTRY(self);
    arr = rb_ary_new();
    for (list = gst_registry_get_path_list(reg);
         list != NULL;
         list = g_list_next(list))
    {
        rb_ary_push(arr, CSTR2RVAL((gchar *) list->data)); 
    }
    g_list_free(list);
    return arr;
}

/*
 *  Method: each_path { |aString| block } -> nil
 *
 *  Calls the block for each path of the registry, passing a reference to
 *  the path String as parameter.
 *
 *  Always returns nil.
 */
static VALUE rb_gst_registry_each_path(self)
    VALUE self;
{
    return rb_ary_yield(rb_gst_registry_get_paths(self));
}

void Init_gst_registry(void) {
    VALUE c = G_DEF_CLASS(GST_TYPE_REGISTRY, "Registry", mGst); 

    rb_define_singleton_method(c, "plugins",     rb_gst_registry_get_plugins, 0);
    rb_define_singleton_method(c, "each_plugin", rb_gst_registry_each_plugin, 0);
    rb_define_singleton_method(c, "find_plugin", rb_gst_registry_find_plugin, 1);
    
    rb_define_singleton_method(c, "each", rb_gst_registry_each, 0);
    rb_define_singleton_method(c, "registries", rb_gst_registry_get_registries, 0);
   
    rb_define_method(c, "name",    rb_gst_registry_get_name,    0);
    rb_define_method(c, "details", rb_gst_registry_get_details, 0);
    rb_define_method(c, "loaded?", rb_gst_registry_is_loaded,   0);

    rb_define_method(c, "paths",     rb_gst_registry_get_paths, 0);
    rb_define_method(c, "each_path", rb_gst_registry_each_path, 0);
}

