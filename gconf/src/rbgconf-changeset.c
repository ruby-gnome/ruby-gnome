/*****************************************************************************
 *
 * rbgconf-changeset.c: GConf::ChangeSet class.
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
 * $Date: 2003/04/13 00:19:20 $
 *
 *****************************************************************************/

/* Includes ******************************************************************/

#include "rbgconf.h"

/* Defines *******************************************************************/

#define _SELF(s)	((GConfChangeSet *)RVAL2BOXED(s, GCONF_TYPE_CHANGESET))

/* Type Definitions **********************************************************/

/* Function Declarations *****************************************************/

/* Global Variables **********************************************************/

/* Function Implementations **************************************************/

static GConfChangeSet *
gconf_change_set_copy(changeset)
	GConfChangeSet *changeset;
{
	gconf_change_set_ref(changeset);
	return changeset;
}
	
GType
gconf_changeset_get_type(void)
{
	static GType our_type = 0;

	if (our_type == 0) {
		our_type = g_boxed_type_register_static("GConfChangeSet",
				(GBoxedCopyFunc)gconf_change_set_copy,
				(GBoxedFreeFunc)gconf_change_set_unref);
	}

	return our_type;
}

static VALUE
change_set_initialize(self)
	VALUE self;
{
	G_INITIALIZE(self, gconf_change_set_new());
	return Qnil;
}

static VALUE
change_set_clear(self)
	VALUE self;
{
	gconf_change_set_clear(_SELF(self));
	return self;
}

static VALUE
change_set_size(self)
	VALUE self;
{
	return INT2FIX(gconf_change_set_size(_SELF(self)));
}

static VALUE
change_set_remove(self, key)
	VALUE self, key;
{
	gconf_change_set_remove(_SELF(self), RVAL2CSTR(key));
	return self;
}

static void
change_set_foreach_func(GConfChangeSet *cs, const gchar *key,
			GConfValue *value, gpointer user_data)
{
	rb_funcall((VALUE)user_data, g_id_call, 2, CSTR2RVAL(key),
		   GCVAL2RVAL(value));
}

static VALUE
change_set_foreach(self)
{
	VALUE func;

	func = rb_f_lambda();
	G_RELATIVE(self, func);
	gconf_change_set_foreach(_SELF(self),
			(GConfChangeSetForeachFunc)change_set_foreach_func,
			(gpointer)func);
	return self;
}

static VALUE
change_set_set(self, key, value)
	VALUE self, key, value;
{
	gconf_change_set_set_nocopy(_SELF(self), RVAL2CSTR(key),
				    RVAL2GCVAL(value));
	return self;
}

static VALUE
change_set_unset(self, key)
	VALUE self, key;
{
	gconf_change_set_unset(_SELF(self), RVAL2CSTR(key));
	return self;
}

/* gconf_change_set_check_value() ??? */

void
Init_gconf_changeset(m_gconf)
	VALUE m_gconf;
{
	VALUE gconf_changeset;

	gconf_changeset = G_DEF_CLASS(GCONF_TYPE_CHANGESET, "ChangeSet",
				      m_gconf);

	rb_define_method(gconf_changeset, "initialize", change_set_initialize,
			 0);
	rb_define_method(gconf_changeset, "clear", change_set_clear, 0);
	rb_define_method(gconf_changeset, "size", change_set_size, 0);
	rb_define_method(gconf_changeset, "remove", change_set_remove, 1);
	rb_define_method(gconf_changeset, "each", change_set_foreach, 0);
	rb_define_method(gconf_changeset, "set", change_set_set, 2);
	rb_define_method(gconf_changeset, "unset", change_set_unset, 1);
}

/* vim: set sts=0 sw=8 ts=8: *************************************************/
