/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2007 Ruby-GNOME2 Project Team
 *  Copyright (C) 2003, 2004 Laurent Sansonetti <lrz@gnome.org>
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 *  MA  02110-1301  USA
 */

#include "rbgst-private.h"

#define RG_TARGET_NAMESPACE cIndexFactory
#define SELF(self) RVAL2GST_INDEX_FACTORY(self)

/* Class: Gst::IndexFactory
 * Create indexes from a factory.  
 */

static VALUE
rg_s_find(G_GNUC_UNUSED VALUE self, VALUE name)
{
    return GOBJ2RVAL(gst_index_factory_find(RVAL2CSTR(name)));
}

/* Method: to_s
 * Returns: a String representing the factory.
 */
static VALUE
rg_to_s (VALUE self)
{
    GstIndexFactory *factory = RGST_INDEX_FACTORY (self); 
    return rb_str_new_with_format ("Index: %s (%s)",
                                  GST_PLUGIN_FEATURE_NAME (factory),
                                  factory->longdesc);
}

static VALUE
rg_description(VALUE self)
{
    return CSTR2RVAL(SELF(self)->longdesc);
}

void
Init_gst_indexfactory (VALUE mGst)
{
    VALUE RG_TARGET_NAMESPACE;

    RG_TARGET_NAMESPACE = G_DEF_CLASS(GST_TYPE_INDEX_FACTORY,
                                      "IndexFactory", mGst);

    RG_DEF_SMETHOD(find, 1);

    RG_DEF_METHOD(to_s, 0);

    RG_DEF_METHOD(description, 0);
}
