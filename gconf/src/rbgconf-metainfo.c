/*****************************************************************************
 *
 * rbgconf-metainfo.c: GConfMetaInfo wrapper.
 *
 * Copyright (C) 2003 Nikolai :: lone-star :: Weibull <lone-star@home.se>.
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
 * $Author: mutoh $
 *
 * $Date: 2003/05/24 13:23:34 $
 *
 *****************************************************************************/

/* Includes ******************************************************************/

#include "rbgconf.h"

/* Defines *******************************************************************/

#define GCONF_TYPE_METAINFO	(metainfo_get_gobject_type())
#define _SELF(s)  	((GConfMetaInfo *)RVAL2BOXED(s, GCONF_TYPE_METAINFO))

/* Type Definitions **********************************************************/

/* Function Declarations *****************************************************/

/* Global Variables **********************************************************/

/* Function Implementations **************************************************/

gpointer
metainfo_copy(gpointer boxed)
{
	/* if this gets called befoure the below sets our_type we loop */
	return g_boxed_copy(GCONF_TYPE_METAINFO, boxed);
}

GType
metainfo_get_gobject_type(void)
{
	static GType our_type = 0;

	if (our_type == 0) {
		our_type = g_boxed_type_register_static("GConfMetaInfo",
				(GBoxedCopyFunc)metainfo_copy,
				(GBoxedFreeFunc)gconf_meta_info_free);
	}

	return our_type;
}

static VALUE
metainfo_initialize(self)
	VALUE self;
{
	GConfMetaInfo *metainfo;

	metainfo = gconf_meta_info_new();
	G_INITIALIZE(self, metainfo);
	return Qnil;
}

static VALUE
metainfo_get_schema(self)
	VALUE self;
{
	return CSTR2RVAL(gconf_meta_info_get_schema(_SELF(self)));	
}

static VALUE
metainfo_get_mod_user(self)
	VALUE self;
{
	return CSTR2RVAL(gconf_meta_info_get_mod_user(_SELF(self)));	
}

static VALUE
metainfo_get_mod_time(self)
	VALUE self;
{
	return INT2NUM(gconf_meta_info_mod_time(_SELF(self)));	
}

static VALUE
metainfo_set_schema(self, schema)
	VALUE self, schema;
{
	gconf_meta_info_set_schema(_SELF(self), RVAL2CSTR(schema));
	return self;
}

static VALUE
metainfo_set_mod_user(self, user)
	VALUE self, user;
{
	gconf_meta_info_set_mod_user(_SELF(self), RVAL2CSTR(user));
	return self;
}

static VALUE
metainfo_set_mod_time(self, time)
	VALUE self, time;
{
	gconf_meta_info_set_mod_time(_SELF(self), NUM2INT(time));
	return self;
}

void
Init_gconf_metainfo(m_gconf)
	VALUE m_gconf;
{
	VALUE gmi = G_DEF_CLASS(GCONF_TYPE_METAINFO, "MetaInfo", m_gconf);

	rb_define_method(gmi, "initialize", metainfo_initialize, 0);
	rb_define_method(gmi, "schema", metainfo_get_schema, 0);
	rb_define_method(gmi, "mod_user", metainfo_get_mod_user, 0);
	rb_define_method(gmi, "mod_time", metainfo_get_mod_time, 0);
	rb_define_method(gmi, "set_schema", metainfo_set_schema, 1);
	rb_define_method(gmi, "set_mod_user", metainfo_set_mod_user, 1);
	rb_define_method(gmi, "set_mod_time", metainfo_set_mod_time, 1);

	G_DEF_SETTERS(gmi);
}

/* vim: set sts=0 sw=8 ts=8: *************************************************/
