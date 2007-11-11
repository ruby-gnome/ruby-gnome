/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
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

#define SELF(self) RVAL2GST_TYPE_FIND_FACTORY(self)

/* Class: Gst::TypeFindFactory
 * Information about registered type find functions.
 */

/*
 * Class method: list
 *
 * Gets the list of all registered type find factories.
 *
 * Returns: an Array of Gst::TypeFindFactory objects.
 */
static VALUE
rb_gst_type_find_factory_get_list (void)
{
	GList *list;
	GList *orig;
	VALUE arr;

	arr = rb_ary_new ();
	orig = gst_type_find_factory_get_list();
	for (list = orig; list != NULL; list = g_list_next (list)) {
		GstTypeFindFactory *tff = GST_TYPE_FIND_FACTORY (list->data);
		rb_ary_push (arr, RGST_TYPE_FIND_FACTORY_NEW (tff));
	}
	if (orig != NULL) {
		g_list_free (orig);
	}
	return arr;
}

/*
 * Method: extensions
 *
 * Gets the extensions associated with a type find factory.
 *
 * Returns: an Array of String objects, or nil if no extensions are 
 * associated with the type find factory.
 */
static VALUE
rb_gst_type_find_factory_get_extensions(VALUE self)
{
    VALUE ary;
    gchar **extensions;

    ary = rb_ary_new ();
    extensions = gst_type_find_factory_get_extensions(SELF(self));

    if (!extensions)
        return ary;

    while (*extensions) {
        rb_ary_push(ary, CSTR2RVAL(*extensions));
        extensions++;
    }

    return ary;
}

/*
 * Method: caps
 *
 * Gets the caps associated with a type find factory.
 *
 * Returns: the Gst::Caps object associated with this factory.
 */
static VALUE
rb_gst_type_find_factory_get_caps (VALUE self)
{
	return RGST_CAPS_NEW (gst_type_find_factory_get_caps (RGST_TYPE_FIND_FACTORY (self)));
}

#if 0   /* requires Gst::TypeFind */
static VALUE 
rb_gst_type_find_factory_call_function (VALUE self, VALUE find)
{
    gst_type_find_factory_call_function(RGST_TYPE_FIND_FACTORY (self), );
    return Qnil;
}
#endif

static VALUE
rb_gst_type_find_factory_to_s (VALUE self)
{
	GstTypeFindFactory *factory = RGST_TYPE_FIND_FACTORY (self);
	return rb_str_new_with_format ("TypeFind: %s", GST_PLUGIN_FEATURE_NAME (factory));
}

void
Init_gst_typefindfactory (void)
{
	VALUE c = G_DEF_CLASS (GST_TYPE_TYPE_FIND_FACTORY, "TypeFindFactory", mGst);

	rb_define_singleton_method(c, "list", rb_gst_type_find_factory_get_list, 0);
	rb_define_method(c, "extensions", rb_gst_type_find_factory_get_extensions, 0);
	rb_define_method(c, "caps", rb_gst_type_find_factory_get_caps, 0);
	/*rb_define_method(c, "call_function", rb_gst_type_find_factory_call_function, 1);*/
	rb_define_method(c, "to_s", rb_gst_type_find_factory_to_s, 0);
}
