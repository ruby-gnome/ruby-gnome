
/*
 * Copyright (C) 2003, 2004 Laurent Sansonetti <lrz@gnome.org>
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

/* Class: Gst::TypeFindFactory
 * I have no idea so don't ask.
 */

/*
 * Class method: list
 *
 * Returns: array of all the registered typefind factories.
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
 */
static VALUE 
rb_gst_type_find_factory_get_extensions (VALUE self) 
{
  // TODO Implement this!
  return Qnil;
}



/*
 * Method: caps
 *
 * Returns: the capabilities of this TypeFindFactory
 */
static VALUE
rb_gst_type_find_factory_get_caps (VALUE self)
{
	return RGST_CAPS_NEW( gst_type_find_factory_get_caps (RGST_TYPE_FIND_FACTORY (self)));
}



/*
 * Method: call_function
 *
 * Returns: nil
 */
static VALUE 
rb_gst_type_find_factory_call_function (VALUE self, VALUE find)
{
	// TODO Implement this!!! (it really does return nil though)
	// gst_type_find_factory_call_function(RGST_TYPE_FIND_FACTORY (self), );
  return Qnil;
}



static VALUE
rb_gst_type_find_factory_to_s (VALUE self)
{
	GstTypeFindFactory *factory = RGST_TYPE_FIND_FACTORY (self);
	return rb_str_format ("TypeFind: %s", GST_PLUGIN_FEATURE_NAME(factory));
}

void
Init_gst_typefindfactory (void)
{
	VALUE c = G_DEF_CLASS (GST_TYPE_TYPE_FIND_FACTORY, "TypeFindFactory", mGst);

	rb_define_singleton_method(c, "list", rb_gst_type_find_factory_get_list, 0);
	rb_define_method(c, "extensions", rb_gst_type_find_factory_get_extensions, 0);
	rb_define_method(c, "caps", rb_gst_type_find_factory_get_caps, 0);
	rb_define_method(c, "call_function", rb_gst_type_find_factory_call_function, 1);
	rb_define_method(c, "to_s", rb_gst_type_find_factory_to_s, 0);
}
