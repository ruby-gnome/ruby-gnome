
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
 *  Class: Gst::AutoplugFactory < Gst::PluginFeature
 *
 *  Create autopluggers from a factory.  
 */

/*
 *  Class method: find(aNameString) -> anAutoplugFactory
 *
 *  Searches for a registered autoplug factory of the given
 *  name, and returns a reference to a Gst::AutoplugFactory object
 *  if found.  Otherwise, returns nil.
 */
static VALUE rb_gst_autoplugfactory_find(self, name)
    VALUE self, name;
{
    GstAutoplugFactory *factory = gst_autoplug_factory_find(RVAL2CSTR(name));
    return factory != NULL
        ? RGST_AUTOPLUG_FACTORY_NEW(factory)
        : Qnil;
}

/*
 *  Class method: make(aNameString) -> anAutoplug
 *
 *  Creates a new Gst::Autoplug instance from the autoplug factory with the
 *  given name.
 *
 *  Returns nil if there is no autoplug factory registered with the given name
 *  or if the autoplug object could not be constructed.
 *
 *  Example:
 *
 *      auto1 = Gst::AutoplugFactory.make("staticrender")
 *      # This does the same
 *      auto2 = Gst::AutoplugFactory.find("staticrender").create
 */
static VALUE rb_gst_autoplugfactory_make(self, name)
    VALUE self, name;
{
    GstAutoplug *autoplug = gst_autoplug_factory_make(RVAL2CSTR(name));
    return autoplug != NULL
        ? RGST_AUTOPLUG_NEW(autoplug)
        : Qnil;
}

/*
 *  Method: create -> anAutoplug
 *
 *  Creates and returns a new Gst::Autoplug instance from the current autoplug 
 *  factory. 
 *
 *  Returns nil if the autoplug object could not be constructed.
 */
static VALUE rb_gst_autoplugfactory_create(self)
    VALUE self;
{
    GstAutoplug *autoplug = gst_autoplug_factory_create(RGST_AUTOPLUG_FACTORY(self));
    return autoplug != NULL
        ? RGST_AUTOPLUG_NEW(autoplug)
        : Qnil;
}

/*
 *  Method: to_s -> aString
 *
 *  Gets a String representing the factory.
 */
static VALUE rb_gst_autoplugfactory_to_s(self)
    VALUE self;
{
    GstAutoplugFactory *factory = RGST_AUTOPLUG_FACTORY(self);
    return rb_str_format("Autoplug: %s (%s)",
                         GST_PLUGIN_FEATURE_NAME(factory),
                         factory->longdesc);
}

void Init_gst_autoplugfactory(void) {
    VALUE c = G_DEF_CLASS(GST_TYPE_AUTOPLUG_FACTORY, "AutoplugFactory", mGst); 
    rb_define_singleton_method(c, "make", rb_gst_autoplugfactory_make, 1);
    rb_define_singleton_method(c, "find", rb_gst_autoplugfactory_find, 1);
    rb_define_method(c, "create", rb_gst_autoplugfactory_create, 0);     
    rb_define_method(c, "to_s", rb_gst_autoplugfactory_to_s, 0);     
}

