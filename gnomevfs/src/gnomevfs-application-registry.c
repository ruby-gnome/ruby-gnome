/*****************************************************************************
 *
 * gnomevfs-application-registry.c: GnomeVFS::ApplicationRegistry module.
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
 * Author: Nikolai :: lone-star :: Weibull <lone-star@home.se>
 *
 * Latest Revision: 2003-08-05
 *
 *****************************************************************************/

/* Includes ******************************************************************/

#include "gnomevfs.h"

/* Defines *******************************************************************/

/* Type Definitions **********************************************************/

/* Function Declarations *****************************************************/

/* Global Variables **********************************************************/

/* Function Implementations **************************************************/

static VALUE
application_registry_exists(self, id)
	VALUE self, id;
{
	return CBOOL2RVAL(
			gnome_vfs_application_registry_exists(RVAL2CSTR(id)));
}

static VALUE
application_registry_get_keys(self, id)
	VALUE self, id;
{
	GList *list;
	VALUE ary;

	list = gnome_vfs_application_registry_get_keys(RVAL2CSTR(id));
	ary = GLIST2STRARY(list);
	gnome_vfs_application_registry_keys_list_free(list);
	return ary;
}

/* XXX: can these two be joined? */
static VALUE
application_registry_peek_value(self, id, key)
	VALUE self, id, key;
{
	return CSTR2RVAL(gnome_vfs_application_registry_peek_value(
							RVAL2CSTR(id),
							RVAL2CSTR(key)));
}

static VALUE
application_registry_get_bool_value(self, id, key)
	VALUE self, id, key;
{
	gboolean set, got_key;

	set = gnome_vfs_application_registry_get_bool_value(RVAL2CSTR(id),
							    RVAL2CSTR(key),
							    &got_key);
	return got_key ? CBOOL2RVAL(set) : Qnil;
}

static VALUE
application_registry_remove_application(self, id)
	VALUE self, id;
{
	gnome_vfs_application_registry_remove_application(RVAL2CSTR(id));
	return Qnil;
}

static VALUE
application_registry_set_value(self, id, key, value)
	VALUE self, id, key, value;
{
	if (TYPE(value) == T_STRING) {
		gnome_vfs_application_registry_set_value(RVAL2CSTR(id),
							 RVAL2CSTR(key),
							 RVAL2CSTR(value));
	} else {
		gnome_vfs_application_registry_set_bool_value(RVAL2CSTR(id),
							RVAL2CSTR(key),
							RVAL2CBOOL(value));
	}
	return Qnil;
}

static VALUE
application_registry_unset_key(self, id, key)
	VALUE self, id, key;
{
	gnome_vfs_application_registry_unset_key(RVAL2CSTR(id),
						 RVAL2CSTR(key));
	return Qnil;
}

static VALUE
application_registry_get_applications(self, mime)
	VALUE self, mime;
{
	return GLIST2ARY3(application_registry_get_applications(
							RVAL2CSTR(mime)));
}

static VALUE
application_registry_get_mime_types(self, id)
	VALUE self, id;
{
	return GLIST2ARY3(application_registry_get_mime_types(RVAL2CSTR(id)));
}

static VALUE
application_registry_supports_mime_type(self, id, mime)
	VALUE self, id, mime;
{
	return CBOOL2RVAL(gnome_vfs_application_registry_supports_mime_type(
							RVAL2CSTR(id),
							RVAL2CSTR(mime)));
}

static VALUE
application_registry_supports_uri_scheme(self, id, scheme)
	VALUE self, id, scheme;
{
	return CBOOL2RVAL(gnome_vfs_application_registry_supports_uri_scheme(
							RVAL2CSTR(id),
							RVAL2CSTR(scheme)));
}

static VALUE
application_registry_clear_mime_types(self, id)
	VALUE self, id;
{
	gnome_vfs_application_registry_clear_mime_types(RVAL2CSTR(id));
	return Qnil;
}

static VALUE
application_registry_add_mime_type(self, id, mime)
	VALUE self, id, mime;
{
	gnome_vfs_application_registry_add_mime_type(RVAL2CSTR(id),
						     RVAL2CSTR(mime));
	return Qnil;
}

static VALUE
application_registry_remove_mime_type(self, id, mime)
	VALUE self, id, mime;
{
	gnome_vfs_application_registry_remove_mime_type(RVAL2CSTR(id),
							RVAL2CSTR(mime));
	return Qnil;
}

static VALUE
application_registry_sync(self)
	VALUE self;
{
	gnome_vfs_application_registry_sync();
	return Qnil;
}

static VALUE
application_registry_shutdown(self)
	VALUE self;
{
	gnome_vfs_application_registry_shutdown();
	return Qnil;
}

static VALUE
application_registry_reload(self)
	VALUE self;
{
	gnome_vfs_application_registry_reload();
	return Qnil;
}

static VALUE
application_registry_get_mime_application(self, id)
	VALUE self, id;
{
	return GVFSMIMEAPP2RVAL(
		gnome_vfs_application_registry_get_mime_application(
							RVAL2CSTR(id)));
}

void
Init_gnomevfs_application_registry(m_gvfs)
	VALUE m_gvfs;
{
	VALUE gvfs_appreg = rb_define_module_under(m_gvfs,
						 "ApplicationRegistry");

	rb_define_module_function(gvfs_appreg, "exists?",
				  application_registry_exists, 1);
	rb_define_module_function(gvfs_appreg, "get_keys",
				  application_registry_get_keys, 1);
	rb_define_module_function(gvfs_appreg, "peek_value",
				  application_registry_peek_value, 2);
	rb_define_module_function(gvfs_appreg, "get_bool_value",
				  application_registry_get_bool_value, 2);
	rb_define_module_function(gvfs_appreg, "remove_application",
				  application_registry_remove_application, 1);
	rb_define_module_function(gvfs_appreg, "set_value",
				  application_registry_set_value, 3);
	rb_define_module_function(gvfs_appreg, "set_bool_value",
				  application_registry_set_value, 3);
	rb_define_module_function(gvfs_appreg, "unset_key",
				  application_registry_unset_key, 2);
	rb_define_module_function(gvfs_appreg, "get_applications",
				  application_registry_get_applications, 1);
	rb_define_module_function(gvfs_appreg, "get_mime_types",
				  application_registry_get_mime_types, 1);
	rb_define_module_function(gvfs_appreg, "supports_mime_type",
				  application_registry_supports_mime_type, 2);
	rb_define_module_function(gvfs_appreg, "supports_uri_scheme",
				  application_registry_supports_uri_scheme, 2);
	rb_define_module_function(gvfs_appreg, "clear_mime_types",
				  application_registry_clear_mime_types, 1);
	rb_define_module_function(gvfs_appreg, "add_mime_type",
				  application_registry_add_mime_type, 2);
	rb_define_module_function(gvfs_appreg, "remove_mime_type",
				  application_registry_remove_mime_type, 2);
	rb_define_module_function(gvfs_appreg, "sync",
				  application_registry_sync, 0);
	rb_define_module_function(gvfs_appreg, "shutdown",
				  application_registry_shutdown, 0);
	rb_define_module_function(gvfs_appreg, "reload",
				  application_registry_reload, 0);
	rb_define_module_function(gvfs_appreg, "get_mime_application",
				  application_registry_get_mime_application,
				  1);
}

/* vim: set sts=0 sw=8 ts=8: *************************************************/
