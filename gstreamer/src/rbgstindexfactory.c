
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
 *  Class: Gst::IndexFactory < Gst::PluginFeature
 *
 *  Create indexes from a factory.  
 */

/*
 *  Method: to_s -> aString
 *
 *  Gets a String representing the factory, as a String.
 */
static VALUE rb_gst_indexfactory_to_s(self)
    VALUE self;
{
    GstIndexFactory *factory = RGST_INDEX_FACTORY(self); 
    return rb_str_format("Index: %s (%s)",
                         GST_PLUGIN_FEATURE_NAME(factory),
                         factory->longdesc);
}

void Init_gst_indexfactory(void) {
    VALUE c = G_DEF_CLASS(GST_TYPE_INDEX_FACTORY, "IndexFactory", mGst);
    rb_define_method(c, "to_s", rb_gst_indexfactory_to_s, 0);
}

