
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
 *  Class method: get_features(aFeatureType) -> anArray
 *
 *  Get a list of all plugin features of the given type in the pool, as
 *  an array of Gst::PluginFeature objects.
 *
 *  Valid class types are:
 *      * Gst::AutoplugFactory;
 *      * Gst::ElementFactory;
 *      * Gst::IndexFactory;
 *      * Gst::SchedulerFactory;
 *	    * Gst::TypeFactory.
 */
static VALUE rb_gst_registry_get_features(self, type)
    VALUE self, type;
{
    GList *list;
    GType gtype;
    VALUE arr;

    arr = rb_ary_new();
    gtype = CLASS2GTYPE(type);
    if (!is_valid_pluginfeature_type(gtype)) {
        rb_raise(rb_eArgError, "Invalid feature type.");
    }
    else {
        for (list = gst_registry_pool_feature_list(gtype);
             list != NULL;
            list = g_list_next(list))
        {
            rb_ary_push(arr, instanciate_pluginfeature(GST_PLUGIN_FEATURE(list->data)));
        }
        g_list_free(list);
    }
    return arr;
}

/*
 *  Class method: each_feature(aFeatureType) { |aPluginFeature| block } -> nil
 *
 *  Calls the block for each plugin features of the given type in the pool, 
 *  passing a reference to the Gst::PluginFeature as parameter.
 *
 *  Valid class types are:
 *      * Gst::AutoplugFactory;
 *      * Gst::ElementFactory;
 *      * Gst::IndexFactory;
 *      * Gst::SchedulerFactory;
 *	    * Gst::TypeFactory.
 *
 *  Always returns nil.
 */
static VALUE rb_gst_registry_each_feature(self, type)
    VALUE self, type;
{
    return rb_ary_yield(rb_gst_registry_get_features(self, type));
}

/*
 *  Class method: find_feature(aName, aFeatureType) -> aPluginFeature
 *
 *  Find the plugin feature with the given name and type from the pool of 
 *  registries, as a Gst::PluginFeature reference.  
 *
 *  Returns nil if the named plugin feature is not found.
 *
 *  Valid class types are:
 *      * Gst::AutoplugFactory;
 *      * Gst::ElementFactory;
 *      * Gst::IndexFactory;
 *      * Gst::SchedulerFactory;
 *	    * Gst::TypeFactory.
 */
static VALUE rb_gst_registry_find_feature(self, name, type)
    VALUE self, name, type;
{
    GstPluginFeature *feature;
    GType gtype;
    
    gtype = CLASS2GTYPE(type);
    if (!is_valid_pluginfeature_type(gtype)) {
        rb_raise(rb_eArgError, "Invalid feature type.");
    }
    feature = gst_registry_pool_find_feature(RVAL2CSTR(name),
                                             gtype);
    return feature != NULL
        ? instanciate_pluginfeature(feature)
        : Qnil;
}

/*
 *  Class method: get_prefered(someFlags) -> aRegistry
 *
 *  Get the prefered registry with the given flags.
 *  Valid flags are:
 *      * Gst::Registry::READABLE;
 *      * Gst::Registry::WRITABLE;
 *      * Gst::Registry::EXISTS;
 *      * Gst::Registry::REMOVE;
 *      * Gst::Registry::DELAYED_LOADING.
 */
static VALUE rb_gst_registry_get_prefered(self, flags)
    VALUE self, flags;
{
    GstRegistry *registry = gst_registry_pool_get_prefered(FIX2INT(flags));
    return registry != NULL
        ? RGST_REGISTRY_NEW(registry)
        : Qnil;
}

/*
 *  Class method: add(aRegistry, aPriority) -> nil
 *
 *  Add the registry to the pool with the given priority (as a Fixnum).
 */
static VALUE rb_gst_registry_add_registry(self, registry, priority)
    VALUE self, registry, priority;
{
    gst_registry_pool_add(RGST_REGISTRY(registry), FIX2INT(priority));
    return Qnil;
}

/*
 *  Class method: remove(aRegistry) -> nil
 *
 *  Remove the registry from the pool.
 */
static VALUE rb_gst_registry_remove_registry(self, registry)
    VALUE self, registry;
{
    gst_registry_pool_remove(RGST_REGISTRY(registry));
    return Qnil;
}

/*
 *  Class method: add_plugin(aPlugin) -> nil
 *
 *  Add the plugin to the global pool of plugins.
 */
static VALUE rb_gst_registry_add_plugin(self, plugin)
    VALUE self, plugin;
{
    gst_registry_pool_add_plugin(RGST_PLUGIN(plugin));
    return Qnil;
}

/*
 *  Class method: load_all -> nil
 *
 *  Load all the registries in the pool. 
 *  Registries with the Gst::Registry::DELAYED_LOADING flag on will not be loaded.
 */
static VALUE rb_gst_registry_load_all(self)
    VALUE self;
{
    gst_registry_pool_load_all();
    return Qnil;
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
 *  Method: load -> aBoolean
 *
 *  Load the registry.  Returns true on success, otherwise false.
 */
static VALUE rb_gst_registry_load(self)
    VALUE self;
{
    return CBOOL2RVAL(gst_registry_load(RGST_REGISTRY(self)));
}

/*
 *  Method: save -> aBoolean
 *
 *  Save the contents of the registry.  Returns true on success, 
 *  otherwise false.
 */
static VALUE rb_gst_registry_save(self)
    VALUE self;
{
    return CBOOL2RVAL(gst_registry_save(RGST_REGISTRY(self)));
}

/*
 *  Method: rebuild -> aBoolean
 *
 *  Rebuild the registry.  Returns true on success, otherwise false.
 */
static VALUE rb_gst_registry_rebuild(self)
    VALUE self;
{
    return CBOOL2RVAL(gst_registry_rebuild(RGST_REGISTRY(self)));
}

/*
 *  Method: unload -> aBoolean
 *
 *  Unload the registry.  Returns true on success, otherwise false.
 */
static VALUE rb_gst_registry_unload(self)
    VALUE self;
{
    return CBOOL2RVAL(gst_registry_unload(RGST_REGISTRY(self)));
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

/*
 *  Method: add_path(aPath) -> self
 *
 *  Add the given path (as a String) to the registry. 
 *  The syntax of the path is specific to the registry. 
 *  If the path has already been added, do nothing.
 */
static VALUE rb_gst_registry_add_path(self, path)
    VALUE self, path;
{
    gst_registry_add_path(RGST_REGISTRY(self), RVAL2CSTR(path));
    return self;
}

/*
 *  Method: clear_paths -> self
 *
 *  Clear the paths of the registry.
 */
static VALUE rb_gst_registry_clear_paths(self)
    VALUE self;
{
    gst_registry_clear_paths(RGST_REGISTRY(self));
    return self;
}

/*
 *  Method: add_plugin(aPlugin) -> aBoolean
 *
 *  Add a plugin (as a Gst::Plugin reference) to the registry. 
 *  The 'plugin-added' signal will be emitted.
 *
 *  Returns true on success, false otherwise.
 */
static VALUE rb_gst_registry_add_plugin2(self, plugin)
    VALUE self, plugin;
{
    return CBOOL2RVAL(gst_registry_add_plugin(RGST_REGISTRY(self),
                                              RGST_PLUGIN(plugin)));
}

/*
 *  Method: remove_plugin(aPlugin) -> self
 *
 *  Remove a plugin (as a Gst::Plugin reference) from the registry.
 */
static VALUE rb_gst_registry_remove_plugin(self, plugin)
    VALUE self, plugin;
{
    gst_registry_remove_plugin(RGST_REGISTRY(self),
                               RGST_PLUGIN(plugin));
    return self;
}

/*
 *  Method: find_plugin(aName) -> aPlugin
 *  
 *  Find the plugin with the given name (as a String) in the registry.
 *  Returns a reference to a Gst::Plugin object if found, 
 *  otherwise returns nil.
 */
static VALUE rb_gst_registry_find_plugin2(self, name)
    VALUE self, name;
{
    GstPlugin *plugin = gst_registry_find_plugin(RGST_REGISTRY(self),
                                                 RVAL2CSTR(name));
    return plugin != NULL
        ? RGST_PLUGIN_NEW(plugin)
        : Qnil;
}

/*
 *  Method: load_plugin(aPlugin) -> aRetCode
 *
 *  Bring the plugin (as a Gst::Plugin reference) from the registry into 
 *  memory.
 *
 *  Returns a Fixnum code indicating the result, which may be:
 *      * Gst::Registry::OK;
 *      * Gst::Registry::LOAD_ERROR;
 *      * Gst::Registry::SAVE_ERROR;
 *      * Gst::Registry::PLUGIN_LOAD_ERROR;
 *      * Gst::Registry::PLUGIN_SIGNATURE_ERROR.
 */
static VALUE rb_gst_registry_load_plugin(self, plugin)
    VALUE self, plugin;
{
    return INT2FIX(gst_registry_load_plugin(RGST_REGISTRY(self),
                                            RGST_PLUGIN(plugin)));
}

/*
 *  Method: unload_plugin(aPlugin) -> aRetCode
 *
 *  Unload a plugin (as a Gst::Plugin reference) from the registry.
 *
 *  Returns a Fixnum code indicating the result, which may be:
 *      * Gst::Registry::OK;
 *      * Gst::Registry::LOAD_ERROR;
 *      * Gst::Registry::SAVE_ERROR;
 *      * Gst::Registry::PLUGIN_LOAD_ERROR;
 *      * Gst::Registry::PLUGIN_SIGNATURE_ERROR.
 */
static VALUE rb_gst_registry_unload_plugin(self, plugin)
    VALUE self, plugin;
{
    return INT2FIX(gst_registry_unload_plugin(RGST_REGISTRY(self),
                                              RGST_PLUGIN(plugin)));
}

/*
 *  Method: update_plugin(aPlugin) -> aRetCode
 *
 *  Unload a plugin (as a Gst::Plugin reference) in the registry.
 *
 *  Returns a Fixnum code indicating the result, which may be:
 *      * Gst::Registry::OK;
 *      * Gst::Registry::LOAD_ERROR;
 *      * Gst::Registry::SAVE_ERROR;
 *      * Gst::Registry::PLUGIN_LOAD_ERROR;
 *      * Gst::Registry::PLUGIN_SIGNATURE_ERROR.
 */
static VALUE rb_gst_registry_update_plugin(self, plugin)
    VALUE self, plugin;
{
    return INT2FIX(gst_registry_update_plugin(RGST_REGISTRY(self),
                                              RGST_PLUGIN(plugin)));
}

/*
 *  Method: find_feature(aName, aFeatureType) -> aPluginFeature
 *
 *  Find the plugin feature with the given name and type in the registry,
 *  as a Gst::PluginFeature reference.  Returns nil if the named plugin 
 *  feature is not found.
 *
 *  Valid class types are:
 *      * Gst::AutoplugFactory;
 *      * Gst::ElementFactory;
 *      * Gst::IndexFactory;
 *      * Gst::SchedulerFactory;
 *	    * Gst::TypeFactory.
 */
static VALUE rb_gst_registry_find_feature2(self, name, type)
    VALUE self, name, type;
{
    GstPluginFeature *feature;
    GType gtype;
    
    gtype = CLASS2GTYPE(type);
    if (!is_valid_pluginfeature_type(gtype)) {
        rb_raise(rb_eArgError, "Invalid feature type.");
    }
    feature = gst_registry_find_feature(RGST_REGISTRY(self),
                                        RVAL2CSTR(name),
                                        gtype);
    return feature != NULL
        ? instanciate_pluginfeature(feature)
        : Qnil;
}

/*
 *  Constant: READABLE
 *  The registry can be read.
 */
static VALUE constReadable = INT2FIX(GST_REGISTRY_READABLE);

/*
 *  Constant: WRITABLE
 *  The registry can be written to.
 */
static VALUE constWritable = INT2FIX(GST_REGISTRY_WRITABLE);

/*
 *  Constant: EXISTS
 *  The registry exists.
 */
static VALUE constExists = INT2FIX(GST_REGISTRY_EXISTS);

/*
 *  Constant: REMOTE
 *  The registry is remote and might be slower.
 */
static VALUE constRemote = INT2FIX(GST_REGISTRY_REMOTE);

/*
 *  Constant: DELAYED_LOADING
 *  The registry will be loaded on demand.
 */
static VALUE constDelayedLoading = INT2FIX(GST_REGISTRY_DELAYED_LOADING);

/*
 *  Constant: OK
 *  The registry reported no error.
 */
static VALUE constOk = INT2FIX(GST_REGISTRY_OK);

/*
 *  Constant: LOAD_ERROR 
 *  There was a load error.
 */
static VALUE constLoadError = INT2FIX(GST_REGISTRY_LOAD_ERROR);

/*
 *  Constant: SAVE_ERROR 
 *  There was an error saving the registry.
 */
static VALUE constSaveError = INT2FIX(GST_REGISTRY_SAVE_ERROR);

/*
 *  Constant: PLUGIN_LOAD_ERROR 
 *  There was an error loading a plugin.
 */
static VALUE constPluginLoadError = INT2FIX(GST_REGISTRY_PLUGIN_LOAD_ERROR);

/*
 *  Constant: PLUGIN_SIGNATURE_ERROR 
 *  There was an error saving a plugin.
 */
static VALUE constPluginSignatureError = INT2FIX(GST_REGISTRY_PLUGIN_SIGNATURE_ERROR);

void Init_gst_registry(void) {
    VALUE c = G_DEF_CLASS(GST_TYPE_REGISTRY, "Registry", mGst); 

    rb_define_singleton_method(c, "plugins",     rb_gst_registry_get_plugins, 0);
    rb_define_singleton_method(c, "each_plugin", rb_gst_registry_each_plugin, 0);
    rb_define_singleton_method(c, "find_plugin", rb_gst_registry_find_plugin, 1);
    rb_define_singleton_method(c, "add_plugin",  rb_gst_registry_add_plugin,  1);
    
    rb_define_singleton_method(c, "each",       rb_gst_registry_each,            0);
    rb_define_singleton_method(c, "registries", rb_gst_registry_get_registries,  0);
    rb_define_singleton_method(c, "add",        rb_gst_registry_add_registry,    2);
    rb_define_singleton_method(c, "remove",     rb_gst_registry_remove_registry, 1);
    rb_define_singleton_method(c, "load_all",   rb_gst_registry_load_all,        0);

    rb_define_singleton_method(c, "get_features", rb_gst_registry_get_features, 1);
    rb_define_singleton_method(c, "each_feature", rb_gst_registry_each_feature, 1);
    rb_define_singleton_method(c, "find_feature", rb_gst_registry_find_feature, 2);
    rb_define_singleton_method(c, "get_prefered", rb_gst_registry_get_prefered, 1);
    
    rb_define_method(c, "name",    rb_gst_registry_get_name,    0);
    rb_define_method(c, "details", rb_gst_registry_get_details, 0);
    rb_define_method(c, "loaded?", rb_gst_registry_is_loaded,   0);

    rb_define_method(c, "load",    rb_gst_registry_load,    0);
    rb_define_method(c, "save",    rb_gst_registry_save,    0);
    rb_define_method(c, "rebuild", rb_gst_registry_rebuild, 0);
    rb_define_method(c, "unload",  rb_gst_registry_unload,  0);

    rb_define_method(c, "paths",       rb_gst_registry_get_paths,   0);
    rb_define_method(c, "each_path",   rb_gst_registry_each_path,   0);
    rb_define_method(c, "add_path",    rb_gst_registry_add_path,    1);
    rb_define_method(c, "clear_paths", rb_gst_registry_clear_paths, 0);

    rb_define_method(c, "add_plugin",    rb_gst_registry_add_plugin2,   1);
    rb_define_method(c, "remove_plugin", rb_gst_registry_remove_plugin, 1);
    rb_define_method(c, "find_plugin",   rb_gst_registry_find_plugin2,  1);
    rb_define_method(c, "load_plugin",   rb_gst_registry_load_plugin,   1);
    rb_define_method(c, "unload_plugin", rb_gst_registry_unload_plugin, 1);
    rb_define_method(c, "update_plugin", rb_gst_registry_update_plugin, 1);

    rb_define_method(c, "find_feature", rb_gst_registry_find_feature2, 2);
    
    rb_define_const(c, "READABLE",        constReadable);
    rb_define_const(c, "WRITABLE",        constWritable);
    rb_define_const(c, "EXISTS",          constExists);
    rb_define_const(c, "REMOTE",          constRemote);
    rb_define_const(c, "DELAYED_LOADING", constDelayedLoading);

    rb_define_const(c, "OK",                     constOk);
    rb_define_const(c, "LOAD_ERROR",             constLoadError);
    rb_define_const(c, "SAVE_ERROR",             constSaveError);
    rb_define_const(c, "PLUGIN_LOAD_ERROR",      constPluginLoadError);
    rb_define_const(c, "PLUGIN_SIGNATURE_ERROR", constPluginSignatureError);
}

