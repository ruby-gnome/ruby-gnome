/*****************************************************************************
 *
 * rbgconf-schema.c: GConf::Schema class.
 *
 * Copyright (C) 2002 Nikolai :: lone-star :: Weibull <lone-star@home.se>.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Nikolai :: lone-star :: Weibull <lone-star@home.se>
 *
 * Latest Revision: 2002-11-12
 *
 *****************************************************************************/

/* Includes ******************************************************************/

#include "rbgconf.h"

/* Defines *******************************************************************/

#define _SELF(s)	((GConfSchema *)RVAL2BOXED(s, GCONF_TYPE_SCHEMA))

/* Type Definitions **********************************************************/

/* Function Declarations *****************************************************/

/* Global Variables **********************************************************/

/* Function Implementations **************************************************/

GType
gconf_schema_get_gobject_type(void)
{
	static GType our_type = 0;

	if (our_type == 0) {
		our_type = g_boxed_type_register_static("GConfSchema",
				(GBoxedCopyFunc)gconf_schema_copy,
				(GBoxedFreeFunc)gconf_engine_unref);
	}

	return our_type;
}

static VALUE
schema_set_locale(self, locale)
	VALUE self, locale;
{
	gconf_schema_set_locale(_SELF(self), RVAL2CSTR(locale));
	return self;
}

static VALUE
schema_set_short_desc(self, desc)
	VALUE self, desc;
{
	gconf_schema_set_short_desc(_SELF(self), RVAL2CSTR(desc));
	return self;
}

static VALUE
schema_set_long_desc(self, desc)
	VALUE self, desc;
{
	gconf_schema_set_long_desc(_SELF(self), RVAL2CSTR(desc));
	return self;
}

static VALUE
schema_set_owner(self, owner)
	VALUE self, owner;
{
	gconf_schema_set_owner(_SELF(self), RVAL2CSTR(owner));
	return self;
}

static VALUE
schema_set_default_value(self, val)
	VALUE self, val;
{
	gconf_schema_set_default_value_nocopy(_SELF(self), RVAL2GCVAL(val));
	return self;
}

static VALUE
schema_get_locale(self)
	VALUE self;
{
	return CSTR2RVAL(gconf_schema_get_locale(_SELF(self)));
}

static VALUE
schema_get_short_desc(self)
	VALUE self;
{
	return CSTR2RVAL(gconf_schema_get_short_desc(_SELF(self)));
}

static VALUE
schema_get_long_desc(self)
	VALUE self;
{
	return CSTR2RVAL(gconf_schema_get_long_desc(_SELF(self)));
}

static VALUE
schema_get_owner(self)
	VALUE self;
{
	return CSTR2RVAL(gconf_schema_get_owner(_SELF(self)));
}

static VALUE
schema_get_default_value(self)
	VALUE self;
{
	return GCVAL2RVAL(gconf_schema_get_default_value(_SELF(self)));
}

void
Init_gconf_schema(m_gconf)
	VALUE m_gconf;
{
	VALUE gconf_schema = G_DEF_CLASS(GCONF_TYPE_SCHEMA, "Schema", m_gconf);

	rb_define_method(gconf_schema, "set_locale", schema_set_locale,  1);
	rb_define_method(gconf_schema, "set_short_desc", schema_set_short_desc,
			 1);
	rb_define_method(gconf_schema, "set_long_desc", schema_set_long_desc,
			 1);
	rb_define_method(gconf_schema, "set_owner", schema_set_owner, 1);
	rb_define_method(gconf_schema, "set_default_value",
			 schema_set_default_value, 1);
	rb_define_method(gconf_schema, "locale", schema_get_locale, 0);
	rb_define_method(gconf_schema, "short_desc", schema_get_short_desc, 0);
	rb_define_method(gconf_schema, "long_desc", schema_get_long_desc, 0);
	rb_define_method(gconf_schema, "owner", schema_get_owner, 0);
	rb_define_method(gconf_schema, "default_value",
			 schema_get_default_value, 0);

	G_DEF_SETTERS(gconf_schema);
}


/* vim: set sts=0 sw=8 ts=8: *************************************************/
