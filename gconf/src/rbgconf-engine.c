/*****************************************************************************
 *
 * rbgconf-engine.c: GConfEngine wrapper.
 *
 * Copyright (C) 2002,2003 Nikolai :: lone-star :: Weibull <lone-star@home.se>.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307  USA
 *
 * $Author: pcppopper $
 *
 * $Date: 2003/02/04 21:28:45 $
 *
 *****************************************************************************/

/* Includes ******************************************************************/

#include "rbgconf.h"

/* Defines *******************************************************************/

/* Type Definitions **********************************************************/

/* Function Declarations *****************************************************/

/* Global Variables **********************************************************/

/* Function Implementations **************************************************/

static GConfEngine *
gconf_engine_copy(engine)
	GConfEngine *engine;
{
	g_return_val_if_fail(engine != NULL, NULL);
	gconf_engine_ref(engine);
	return engine;
}

GType
gconf_engine_get_type(void)
{
	static GType our_type = 0;

	if (our_type == 0) {
		our_type = g_boxed_type_register_static("GConfEngine",
				(GBoxedCopyFunc)gconf_engine_copy,
				(GBoxedFreeFunc)gconf_engine_unref);
	}

	return our_type;
}

static VALUE
engine_initialize(argc, argv, self)
	int argc;
	VALUE *argv;
	VALUE self;
{
	VALUE address;
	GConfEngine *engine;

	if (rb_scan_args(argc, argv, "01", &address) == 1) {
		engine = gconf_engine_get_for_address(RVAL2CSTR(address),
						      NULL);
	} else {
		engine = gconf_engine_get_default();
	}

	G_INITIALIZE(self, engine);
	return Qnil;
}

void
Init_gconf_engine(m_gconf)
	VALUE m_gconf;
{
	VALUE gconf_engine;
	
	gconf_engine = G_DEF_CLASS(GCONF_TYPE_ENGINE, "Engine", m_gconf);

	rb_define_method(gconf_engine, "initialize", engine_initialize, -1);
}

/* vim: set sts=0 sw=8 ts=8: *************************************************/
