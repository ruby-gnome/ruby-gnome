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

/* Class: Gst::SchedulerFactory
 * Create schedulers from a factory.  
 */

/* Class method: default_name
 * Returns: the default scheduler factory name.
 */
static VALUE
rb_gst_sf_default_name (VALUE self)
{
	return CSTR2RVAL (gst_scheduler_factory_get_default_name ());
}

/*
 * Class method: set_default_name(name)
 * name: the name of the default scheduler factory used as a default.
 *
 * Sets the default scheduler name.
 *
 * Returns: self.
 */
static VALUE
rb_gst_sf_set_default_name (VALUE self, VALUE name)
{
	gst_scheduler_factory_set_default_name (RVAL2CSTR (name));
	return self;
}

/* Method: to_s
 * Returns: a String representing the factory.
 */
static VALUE
rb_gst_schedulerfactory_to_s (VALUE self)
{
	GstSchedulerFactory *factory = RGST_SCHEDULER_FACTORY (self); 
	return rb_str_format ("Scheduler: %s (%s)",
			      GST_PLUGIN_FEATURE_NAME (factory),
			      factory->longdesc);
}

void
Init_gst_schedulerfactory (void)
{
	VALUE c = G_DEF_CLASS (GST_TYPE_SCHEDULER_FACTORY, "SchedulerFactory", 
			       mGst); 

	rb_define_singleton_method (c, "default_name", rb_gst_sf_default_name, 0);
	rb_define_singleton_method (c, "set_default_name", 
				    rb_gst_sf_set_default_name, 1);
	G_DEF_SETTERS (CLASS_OF (c));
	rb_define_method (c, "to_s", rb_gst_schedulerfactory_to_s, 0);
}
