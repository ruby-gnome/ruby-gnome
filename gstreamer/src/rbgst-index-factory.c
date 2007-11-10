/*
 * Copyright (C) 2003, 2004 Laurent Sansonetti <lrz@gnome.org>
 * Copyright (C) 2007 Ruby-GNOME2 Project Team
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

#define SELF(self) RVAL2GST_IDX_FACTORY(self)

/* Class: Gst::IndexFactory
 * Create indexes from a factory.  
 */

static VALUE
s_find(VALUE self, VALUE name)
{
    return GOBJ2RVAL(gst_index_factory_find(RVAL2CSTR(name)));
}

/* Method: to_s
 * Returns: a String representing the factory.
 */
static VALUE
rb_gst_indexfactory_to_s (VALUE self)
{
	GstIndexFactory *factory = RGST_INDEX_FACTORY (self); 
	return rb_str_new_with_format ("Index: %s (%s)",
			      GST_PLUGIN_FEATURE_NAME (factory),
			      factory->longdesc);
}

static VALUE
get_description(VALUE self)
{
    return CSTR2RVAL(SELF(self)->longdesc);
}

void
Init_gst_indexfactory (void)
{
    VALUE rb_cGstIndexFactory;

    rb_cGstIndexFactory = G_DEF_CLASS(GST_TYPE_INDEX_FACTORY,
                                      "IndexFactory", mGst);

    rb_define_singleton_method(rb_cGstIndexFactory, "find", s_find, 1);

    rb_define_method(rb_cGstIndexFactory, "to_s", rb_gst_indexfactory_to_s, 0);

    rb_define_method(rb_cGstIndexFactory, "description", get_description, 0);
}
