/*
 * Copyright (C) 2003 Laurent Sansonetti <lrz@gnome.org>
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

/* Class: Gst::AutoplugFactory
 * Create autopluggers from a factory.  
 */

/*
 * Class method: find(name)
 * name: the name of the autoplug factory to search for.
 *
 * Searches for a registered autoplug factory of the given
 * name/
 *
 * Returns: a reference to a Gst::AutoplugFactory object
 * if found, otherwise nil.
 */
static VALUE
rb_gst_autoplugfactory_find (VALUE self, VALUE name)
{
	GstAutoplugFactory *factory = gst_autoplug_factory_find (RVAL2CSTR (name));
	return factory != NULL
		? RGST_AUTOPLUG_FACTORY_NEW (factory)
		: Qnil;
}

/*
 * Class method: make(name)
 * name: the name of an autoplug factory.
 *
 * Creates a new Gst::Autoplug instance from the autoplug factory with the
 * given name.
 *
 *	auto1 = Gst::AutoplugFactory.make("staticrender")
 *	
 *	# This does the same
 *	auto2 = Gst::AutoplugFactory.find("staticrender").create
 *
 * Returns: a reference to a Gst::Autoplug object, or nil if there is no autoplug
 * factory registered with the given name or if the autoplug object could not be
 * constructed.
 */
static VALUE
rb_gst_autoplugfactory_make (VALUE self, VALUE name)
{
	GstAutoplug *autoplug = gst_autoplug_factory_make (RVAL2CSTR (name));
	return autoplug != NULL
		? RGST_AUTOPLUG_NEW (autoplug)
		: Qnil;
}

/*
 * Method: create
 *
 * Creates and returns a new Gst::Autoplug instance from the current autoplug 
 * factory. 
 *
 * Returns: a reference to a newly created Gst::Autoplug object, or nil if the
 * autoplug object could not be constructed.
 */
static VALUE
rb_gst_autoplugfactory_create (VALUE self)
{
	GstAutoplug *autoplug = gst_autoplug_factory_create (RGST_AUTOPLUG_FACTORY (self));
	return autoplug != NULL
		? RGST_AUTOPLUG_NEW (autoplug)
		: Qnil;
}

/* Method: to_s
 * Returns: a string representing the factory.
 */
static VALUE
rb_gst_autoplugfactory_to_s (VALUE self)
{
	GstAutoplugFactory *factory = RGST_AUTOPLUG_FACTORY (self);
	return rb_str_format ("Autoplug: %s (%s)",
			      GST_PLUGIN_FEATURE_NAME (factory),
			      factory->longdesc);
}

void
Init_gst_autoplugfactory (void)
{
	VALUE c = G_DEF_CLASS (GST_TYPE_AUTOPLUG_FACTORY, "AutoplugFactory", mGst); 
	rb_define_singleton_method (c, "make", rb_gst_autoplugfactory_make, 1);
	rb_define_singleton_method (c, "find", rb_gst_autoplugfactory_find, 1);
	rb_define_method (c, "create", rb_gst_autoplugfactory_create, 0);	 
	rb_define_method (c, "to_s", rb_gst_autoplugfactory_to_s, 0);	 
}
