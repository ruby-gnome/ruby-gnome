
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
 *  Class: Gst::TypeFactory < Gst::PluginFeature
 *
 *  Add types to plugins. 
 */

/*
 *  Class method: find(aFactoryNameString) -> aTypeFactoryObject
 *
 *  Searches for a type factory of the given name.
 *  If founds, returns a Gst::TypeFactory object.  Otherwise, returns nil.
 */
static VALUE rb_gst_typefactory_find(self, name)
    VALUE self, name;
{
    GstTypeFactory *factory = gst_type_factory_find(RVAL2CSTR(name));
    return factory != NULL
        ? RGST_TYPE_FACTORY_NEW(factory)
        : Qnil;
}

/*
 *  Method: mime -> aString
 *
 *  Gets the mime type handled by the factory, as a String. 
 */
static VALUE rb_gst_typefactory_get_mime(self)
    VALUE self;
{
    GstTypeFactory *factory = RGST_TYPE_FACTORY(self);
    return CSTR2RVAL(factory->mime);
}

/*
 *  Method: exts -> aString
 *
 *  Gets the files extentions handled by the factory, as a String. 
 */
static VALUE rb_gst_typefactory_get_exts(self)
    VALUE self;
{
    GstTypeFactory *factory = RGST_TYPE_FACTORY(self);
    return CSTR2RVAL(factory->exts);
}

/*
 *  Method: to_s -> aString
 *
 *  Gets a String representing the factory.
 */
static VALUE rb_gst_typefactory_to_s(self)
    VALUE self;
{
    GstTypeFactory *factory = RGST_TYPE_FACTORY(self); 
    return rb_str_format("Type: %s (mime: '%s', file(s): '%s')",
                         GST_PLUGIN_FEATURE_NAME(factory),
                         factory->mime, 
                         factory->exts);
}

void Init_gst_typefactory(void) {
    VALUE c = G_DEF_CLASS(GST_TYPE_TYPE_FACTORY, "TypeFactory", mGst); 

    rb_define_singleton_method(c, "find", rb_gst_typefactory_find, 1);
   
    rb_define_method(c, "mime", rb_gst_typefactory_get_mime, 0);
    rb_define_method(c, "exts", rb_gst_typefactory_get_exts, 0);
    rb_define_method(c, "to_s", rb_gst_typefactory_to_s, 0);
}

