/*****************************************************************************
 *
 * gnomevfs-mime-application.c: GnomeVFS::MimeApplication class.
 *
 * Copyright (C) 2003 Nikolai Weibull <lone-star@home.se>.
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
 * $Date: 2006/06/22 16:50:30 $
 *
 *****************************************************************************/

/* Includes ******************************************************************/

#include "gnomevfs.h"
#include <libgnomevfs/gnome-vfs-mime-handlers.h>
#include <libgnomevfs/gnome-vfs-application-registry.h>

/* Defines *******************************************************************/

#define _SELF(s)	\
    ((GnomeVFSMimeApplication *)RVAL2BOXED(s, GNOMEVFS_TYPE_MIME_APPLICATION))

/* Type Definitions **********************************************************/

/* Function Declarations *****************************************************/

/* Global Variables **********************************************************/

/* Function Implementations **************************************************/

GType
gnome_vfs_mime_application_get_type(void)
{
	static GType our_type = 0;

	if (our_type == 0) {
		our_type =
			g_boxed_type_register_static("GnomeVFSMimeApplication",
			    (GBoxedCopyFunc)gnome_vfs_mime_application_copy,
			    (GBoxedFreeFunc)gnome_vfs_mime_application_free);
	}

	return our_type;
}

static VALUE
mime_application_initialize(self, id)
	VALUE self, id;
{
	G_INITIALIZE(self,
		     gnome_vfs_mime_application_new_from_id(RVAL2CSTR(id)));
	return Qnil;
}

static VALUE
mime_application_set_id(self, id)
	VALUE self, id;
{
	g_free(_SELF(self)->id);
	_SELF(self)->id = g_strdup(RVAL2CSTR(id));
	return self;
}

static VALUE
mime_application_get_id(self)
	VALUE self;
{
	return CSTR2RVAL(_SELF(self)->id);
}

static VALUE
mime_application_set_name(self, name)
	VALUE self, name;
{
	g_free(_SELF(self)->name);
	_SELF(self)->name = g_strdup(RVAL2CSTR(name));
	return self;
}

static VALUE
mime_application_get_name(self)
	VALUE self;
{
	return CSTR2RVAL(_SELF(self)->name);
}

static VALUE
mime_application_set_command(self, command)
	VALUE self, command;
{
	g_free(_SELF(self)->command);
	_SELF(self)->command = g_strdup(RVAL2CSTR(command));
	return self;
}

static VALUE
mime_application_get_command(self)
	VALUE self;
{
	return CSTR2RVAL(_SELF(self)->command);
}

static VALUE
mime_application_set_can_open_multiple_files(self, multiple)
	VALUE self, multiple;
{
	_SELF(self)->can_open_multiple_files = RVAL2CBOOL(multiple);
	return self;
}

static VALUE
mime_application_get_can_open_multiple_files(self)
	VALUE self;
{
	return CBOOL2RVAL(_SELF(self)->can_open_multiple_files);
}

static VALUE
mime_application_set_expected_uris(self, type)
	VALUE self, type;
{
	_SELF(self)->expects_uris = RVAL2GENUM(type, GNOME_VFS_TYPE_VFS_MIME_APPLICATION_ARGUMENT_TYPE);
	return self;
}

static VALUE
mime_application_get_expected_uris(self)
	VALUE self;
{
	return GENUM2RVAL(_SELF(self)->expects_uris, GNOME_VFS_TYPE_VFS_MIME_APPLICATION_ARGUMENT_TYPE);
}

static VALUE
mime_application_set_supported_uri_schemes(self, ary)
	VALUE self, ary;
{
	GList *list;

	list = _SELF(self)->supported_uri_schemes;
	g_list_foreach(list, (GFunc)g_free, NULL);
	g_list_free(list);
	*list = *STRARY2GLIST(ary);
	return Qnil;
}

static VALUE
mime_application_get_supported_uri_schemes(self)
	VALUE self;
{
	VALUE ary;

	ary = GLIST2STRARY(_SELF(self)->supported_uri_schemes);
	return ary;
}


static VALUE
mime_application_set_requires_terminal(self, requires)
	VALUE self, requires;
{
	_SELF(self)->requires_terminal = RVAL2CBOOL(requires);
	return self;
}

static VALUE
mime_application_get_requires_terminal(self)
	VALUE self;
{
	return CBOOL2RVAL(_SELF(self)->requires_terminal);
}

static VALUE
mime_application_is_user_owned(self)
	VALUE self;
{
	return CBOOL2RVAL(
		gnome_vfs_application_is_user_owned_application(_SELF(self)));
}

static VALUE
mime_application_save(self)
	VALUE self;
{
	gnome_vfs_application_registry_save_mime_application(_SELF(self));
	return self;
}

void
Init_gnomevfs_mime_application(m_gvfs)
	VALUE m_gvfs;
{
	VALUE gvfs_mime_app = G_DEF_CLASS(GNOMEVFS_TYPE_MIME_APPLICATION,
					  "MimeApplication", m_gvfs);

	rbgobj_boxed_not_copy_obj(GNOMEVFS_TYPE_MIME_APPLICATION);

	rb_define_singleton_method(gvfs_mime_app, "initialize",
				   mime_application_initialize, 1);
	rb_define_method(gvfs_mime_app, "set_id", mime_application_set_id, 1);
	rb_define_method(gvfs_mime_app, "id", mime_application_get_id, 0);
	rb_define_method(gvfs_mime_app, "set_name", mime_application_set_name,
			 1);
	rb_define_method(gvfs_mime_app, "name", mime_application_get_name, 0);
	rb_define_method(gvfs_mime_app, "set_command",
			 mime_application_set_command, 1);
	rb_define_method(gvfs_mime_app, "command",
			 mime_application_get_command, 0);
	rb_define_method(gvfs_mime_app, "set_can_open_multiple_files",
			 mime_application_set_can_open_multiple_files, 1);
	rb_define_method(gvfs_mime_app, "can_open_multiple_files?",
			 mime_application_get_can_open_multiple_files, 0);
	rb_define_method(gvfs_mime_app, "set_expected_uris",
			 mime_application_set_expected_uris, 1);
	rb_define_method(gvfs_mime_app, "expected_uris",
			 mime_application_get_expected_uris, 0);
	rb_define_method(gvfs_mime_app, "set_supported_uri_schemes",
			 mime_application_set_supported_uri_schemes, 1);
	rb_define_method(gvfs_mime_app, "supported_uri_schemes",
			 mime_application_get_supported_uri_schemes, 0);
	rb_define_method(gvfs_mime_app, "set_requires_terminal",
			 mime_application_set_requires_terminal, 1);
	rb_define_method(gvfs_mime_app, "requires_terminal",
			 mime_application_get_requires_terminal, 0);
	rb_define_method(gvfs_mime_app, "user_owned?",
			 mime_application_is_user_owned, 0);
	rb_define_method(gvfs_mime_app, "save",
			 mime_application_save, 0);

	G_DEF_SETTERS(gvfs_mime_app);

        /* GNOME_VFS_TYPE_VFS_MIME_APPLICATION_ARGUMENT_TYPE */
	G_DEF_CLASS(GNOME_VFS_TYPE_VFS_MIME_APPLICATION_ARGUMENT_TYPE, 
			"ArgumentType", gvfs_mime_app);
	G_DEF_CONSTANTS(gvfs_mime_app, 
			GNOME_VFS_TYPE_VFS_MIME_APPLICATION_ARGUMENT_TYPE, 
			"GNOME_VFS_MIME_APPLICATION_");
}

/* vim: set sts=0 sw=8 ts=8: *************************************************/
