/*****************************************************************************
 *
 * gnomevfs-mime.c: GnomeVFS::Mime class.
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
 * Latest Revision: 2003-08-08
 *
 *****************************************************************************/

/* Includes ******************************************************************/

#include "gnomevfs.h"
#include <libgnomevfs/gnome-vfs-application-registry.h>
#include <libgnomevfs/gnome-vfs-mime-info.h>

/* Defines *******************************************************************/

#define MIMEAPPGLIST2ARY(list)	(mimeapplist2ary(list))

/* Type Definitions **********************************************************/

/* Function Declarations *****************************************************/

/* Global Variables **********************************************************/

/* Function Implementations **************************************************/

static VALUE
mimeapplist2ary(list)
	GList *list;
{
	VALUE ary;
	
	ary = rb_ary_new();
	while (list) {
		rb_ary_push(ary, GVFSMIMEAPP2RVAL(list->data));
		list = list->next;
	}

	return ary;
}

static VALUE
mime_get_default_action_type(self, mime)
	VALUE self, mime;
{
	return INT2FIX(gnome_vfs_mime_get_default_action_type(
							RVAL2CSTR(mime)));
}

static VALUE
mime_get_default_application(self, mime)
	VALUE self, mime;
{
	return GVFSMIMEAPP2RVAL(gnome_vfs_mime_get_default_application(
							RVAL2CSTR(mime)));
}

/* XXX: gnome_vfs_mime_get_default_component */

static VALUE
mime_get_short_list_applications(self, mime)
	VALUE self, mime;
{
	return MIMEAPPGLIST2ARY(gnome_vfs_mime_get_short_list_applications(
							RVAL2CSTR(mime)));
}

/* XXX: gnome_vfs_get_short_list_components */

static VALUE
mime_get_all_applications(self, mime)
	VALUE self, mime;
{
	return MIMEAPPGLIST2ARY(gnome_vfs_mime_get_all_applications(
							RVAL2CSTR(mime)));
}

/* XXX: gnome_vfs_mime_get_all_components */

static VALUE
mime_set_default_action_type(self, mime, type)
	VALUE self, mime, type;
{
	return GVFSRESULT2RVAL(gnome_vfs_mime_set_default_action_type(
							RVAL2CSTR(mime),
							FIX2INT(type)));
}

static VALUE
mime_set_default_application(self, mime, id)
	VALUE self, mime, id;
{
	return GVFSRESULT2RVAL(gnome_vfs_mime_set_default_application(
							RVAL2CSTR(mime),
							RVAL2CSTR(id)));
}

static VALUE
mime_set_default_component(self, mime, iid)
	VALUE self, mime, iid;
{
	return GVFSRESULT2RVAL(gnome_vfs_mime_set_default_component(
							RVAL2CSTR(mime),
							RVAL2CSTR(iid)));
}

static VALUE
mime_get_icon(self, mime)
	VALUE self, mime;
{
	return CSTR2RVAL(gnome_vfs_mime_get_icon(RVAL2CSTR(mime)));
}

static VALUE
mime_set_icon(self, mime, path)
	VALUE self, mime, path;
{
	return GVFSRESULT2RVAL(gnome_vfs_mime_set_icon(RVAL2CSTR(mime),
						       RVAL2CSTR(path)));
}

static VALUE
mime_get_description(self, mime)
	VALUE self, mime;
{
	return CSTR2RVAL(gnome_vfs_mime_get_description(RVAL2CSTR(mime)));
}

static VALUE
mime_set_description(self, mime, path)
	VALUE self, mime, path;
{
	return GVFSRESULT2RVAL(gnome_vfs_mime_set_description(RVAL2CSTR(mime),
						       RVAL2CSTR(path)));
}

static VALUE
mime_can_be_executable(self, mime)
	VALUE self, mime;
{
	return CBOOL2RVAL(gnome_vfs_mime_can_be_executable(RVAL2CSTR(mime)));
}

static VALUE
mime_set_can_be_executable(self, mime, executable)
	VALUE self, mime, executable;
{
	return GVFSRESULT2RVAL(
		gnome_vfs_mime_set_can_be_executable(RVAL2CSTR(mime),
						     RVAL2CBOOL(executable)));
}

static VALUE
mime_set_short_list_applications(self, mime, ids)
	VALUE self, mime, ids;
{
	GList *list;
	GnomeVFSResult result;

	list = STRARY2GLIST(ids);
	result = gnome_vfs_mime_set_short_list_applications(RVAL2CSTR(mime), 
							    list);
	g_list_free(list);
	return GVFSRESULT2RVAL(result);
}

static VALUE
mime_set_short_list_components(self, mime, iids)
	VALUE self, mime, iids;
{
	GList *list;
	GnomeVFSResult result;

	list = STRARY2GLIST(iids);
	result = gnome_vfs_mime_set_short_list_components(RVAL2CSTR(mime),
							  list);
	g_list_free(list);
	return GVFSRESULT2RVAL(result);
}

static VALUE
mime_add_application_to_short_list(self, mime, id)
	VALUE self, mime, id;
{
	return GVFSRESULT2RVAL(gnome_vfs_mime_add_application_to_short_list(
							RVAL2CSTR(mime),
							RVAL2CSTR(id)));
}

static VALUE
mime_remove_application_to_short_list(self, mime, id)
	VALUE self, mime, id;
{
	return GVFSRESULT2RVAL(
		gnome_vfs_mime_remove_application_from_short_list(
							RVAL2CSTR(mime),
							RVAL2CSTR(id)));
}

static VALUE
mime_add_component_to_short_list(self, mime, id)
	VALUE self, mime, id;
{
	return GVFSRESULT2RVAL(gnome_vfs_mime_add_component_to_short_list(
							RVAL2CSTR(mime),
							RVAL2CSTR(id)));
}

static VALUE
mime_remove_component_to_short_list(self, mime, id)
	VALUE self, mime, id;
{
	return GVFSRESULT2RVAL(
		gnome_vfs_mime_remove_component_from_short_list(
							RVAL2CSTR(mime),
							RVAL2CSTR(id)));
}

static VALUE
mime_add_extension(self, mime, ext)
	VALUE self, mime, ext;
{
	return GVFSRESULT2RVAL(gnome_vfs_mime_add_extension(RVAL2CSTR(mime),
							    RVAL2CSTR(ext)));
}

static VALUE
mime_remove_extension(self, mime, ext)
	VALUE self, mime, ext;
{
	return GVFSRESULT2RVAL(gnome_vfs_mime_remove_extension(RVAL2CSTR(mime),
							    RVAL2CSTR(ext)));
}

static VALUE
mime_extend_all_applications(self, mime, ids)
	VALUE self, mime, ids;
{
	GList *list;
	GnomeVFSResult result;

	list = STRARY2GLIST(ids);
	result = gnome_vfs_mime_extend_all_applications(RVAL2CSTR(mime), list);
	g_list_free(list);
	return GVFSRESULT2RVAL(result);
}

static VALUE
mime_remove_from_all_applications(self, mime, ids)
	VALUE self, mime, ids;
{
	GList *list;
	GnomeVFSResult result;

	list = STRARY2GLIST(ids);
	result = gnome_vfs_mime_remove_from_all_applications(RVAL2CSTR(mime),
							     list);
	g_list_free(list);
	return GVFSRESULT2RVAL(result);
}

static VALUE
mime_freeze(self)
	VALUE self;
{
	gnome_vfs_mime_freeze();
	return Qnil;
}

static VALUE
mime_thaw(self)
	VALUE self;
{
	gnome_vfs_mime_thaw();
	return Qnil;
}

static VALUE
mime_info_reload(self)
	VALUE self;
{
	gnome_vfs_mime_info_reload();
	return Qnil;
}

static VALUE
mime_type_is_known(self, mime)
	VALUE self, mime;
{
	return CBOOL2RVAL(gnome_vfs_mime_type_is_known(RVAL2CSTR(mime)));
}

static VALUE
mime_get_value(self, mime, key)
	VALUE self, mime, key;
{
	return CSTR2RVAL(gnome_vfs_mime_get_value(RVAL2CSTR(mime),
						  RVAL2CSTR(key)));
}

static VALUE
mime_set_value(self, mime, key, value)
	VALUE self, mime, key, value;
{
	return GVFSRESULT2RVAL(gnome_vfs_mime_set_value(RVAL2CSTR(mime),
							RVAL2CSTR(key),
							RVAL2CSTR(value)));
}

static VALUE
mime_get_key_list(self, mime)
	VALUE self, mime;
{
	GList *list;
	VALUE ary;

	list = gnome_vfs_mime_get_extensions_list(RVAL2CSTR(mime));
	ary = GLIST2STRARY(list);
	gnome_vfs_mime_keys_list_free(list);
	return ary;
}

static VALUE
mime_get_extensions_list(self, mime)
	VALUE self, mime;
{
	GList *list;
	VALUE ary;

	list = gnome_vfs_mime_get_extensions_list(RVAL2CSTR(mime));
	ary = GLIST2STRARY(list);
	gnome_vfs_mime_extensions_list_free(list);
	return ary;
}

static VALUE
mime_get_extensions_string(self, mime)
	VALUE self, mime;
{
	char *exts;
	VALUE str;

	exts = gnome_vfs_mime_get_extensions_string(RVAL2CSTR(mime));
	str = CSTR2RVAL(exts);
	g_free(exts);
	return str;
}

static VALUE
mime_get_extensions_pretty_string(self, mime)
	VALUE self, mime;
{
	char *exts;
	VALUE str;

	exts = gnome_vfs_mime_get_extensions_pretty_string(RVAL2CSTR(mime));
	str = CSTR2RVAL(exts);
	g_free(exts);
	return str;
}

static VALUE
mime_set_registered_type_key(self, mime, key, value)
	VALUE self, mime, key, value;
{
	return GVFSRESULT2RVAL(
		gnome_vfs_mime_set_registered_type_key(RVAL2CSTR(mime),
						       RVAL2CSTR(key),
						       RVAL2CSTR(value)));
}

static VALUE
mime_set_extensions_list(self, mime, exts)
	VALUE self, mime, exts;
{
	return GVFSRESULT2RVAL(gnome_vfs_mime_set_extensions_list(
						RVAL2CSTR(mime),
						RVAL2CSTR(exts)));
}

static VALUE
mime_registered_mime_type_delete(self, mime)
	VALUE self, mime;
{
	gnome_vfs_mime_registered_mime_type_delete(RVAL2CSTR(mime));
	return Qnil;
}

static VALUE
mime_reset(self)
	VALUE self;
{
	gnome_vfs_mime_reset();
	return Qnil;
}

void
Init_gnomevfs_mime(m_gvfs)
	VALUE m_gvfs;
{
	VALUE gvfs_mime = rb_define_module_under(m_gvfs, "Mime");

	rb_define_module_function(gvfs_mime, "get_default_action_type",
				  mime_get_default_application, 1);
	rb_define_module_function(gvfs_mime, "get_default_application",
				  mime_get_default_application, 1);
	rb_define_module_function(gvfs_mime, "get_short_list_applications",
				  mime_get_short_list_applications, 1);
	/* XXX: missing the two components functions above */
	rb_define_module_function(gvfs_mime, "get_all_applications",
				  mime_get_all_applications, 1);
	rb_define_module_function(gvfs_mime, "set_default_action_type",
				  mime_set_default_action_type, 2);
	rb_define_module_function(gvfs_mime, "set_default_application",
				  mime_set_default_application, 2);
	rb_define_module_function(gvfs_mime, "set_default_component",
				  mime_set_default_component, 2);
	rb_define_module_function(gvfs_mime, "get_icon", mime_get_icon, 1);
	rb_define_module_function(gvfs_mime, "set_icon", mime_set_icon, 2);
	rb_define_module_function(gvfs_mime, "get_description",
				  mime_get_description, 1);
	rb_define_module_function(gvfs_mime, "set_description",
				  mime_set_description, 2);
	/* XXX: these names suck! */
	rb_define_module_function(gvfs_mime, "can_be_executable?",
				  mime_can_be_executable, 1);
	rb_define_module_function(gvfs_mime, "set_can_be_executable",
				  mime_set_can_be_executable, 2);
	rb_define_module_function(gvfs_mime, "set_short_list_applications",
				  mime_set_short_list_applications, 2);
	rb_define_module_function(gvfs_mime, "set_short_list_components",
				  mime_set_short_list_components, 2);
	rb_define_module_function(gvfs_mime, "add_application_to_short_list",
				  mime_add_application_to_short_list, 2);
	rb_define_module_function(gvfs_mime,
				  "remove_application_to_short_list",
				  mime_remove_application_to_short_list, 2);
	rb_define_module_function(gvfs_mime, "add_component_to_short_list",
				  mime_add_component_to_short_list, 2);
	rb_define_module_function(gvfs_mime,
				  "remove_component_to_short_list",
				  mime_remove_component_to_short_list, 2);
	rb_define_module_function(gvfs_mime, "add_extension",
				  mime_add_extension, 2);
	rb_define_module_function(gvfs_mime, "remove_extension",
				  mime_remove_extension, 2);
	rb_define_module_function(gvfs_mime, "extend_all_applications",
				  mime_extend_all_applications, 2);
	rb_define_module_function(gvfs_mime, "remove_from_all_applications",
				  mime_remove_from_all_applications, 2);
	rb_define_module_function(gvfs_mime, "freeze", mime_freeze, 0);
	rb_define_module_function(gvfs_mime, "thaw", mime_thaw, 0);
	rb_define_module_function(gvfs_mime, "info_reload", mime_info_reload,
				  0);
	rb_define_module_function(gvfs_mime, "type_is_known?",
				  mime_type_is_known, 0);
	rb_define_module_function(gvfs_mime, "get_value", mime_get_value, 2);
	rb_define_module_function(gvfs_mime, "set_value", mime_set_value, 3);
	rb_define_module_function(gvfs_mime, "get_key_list",
				  mime_get_key_list, 1);
	rb_define_module_function(gvfs_mime, "get_extensions_list",
				  mime_get_extensions_list, 1);
	rb_define_module_function(gvfs_mime, "get_extensions_string",
				  mime_get_extensions_string, 1);
	rb_define_module_function(gvfs_mime, "get_extensions_pretty_string",
				  mime_get_extensions_pretty_string, 1);
	rb_define_module_function(gvfs_mime, "set_registered_type_key",
				  mime_set_registered_type_key, 3);
	rb_define_module_function(gvfs_mime, "set_extensions_list",
				  mime_set_extensions_list, 2);
	rb_define_module_function(gvfs_mime, "registered_mime_type_delete",
				  mime_registered_mime_type_delete, 1);
	rb_define_module_function(gvfs_mime, "reset", mime_reset, 0);

	rb_define_const(gvfs_mime, "ACTION_TYPE_NONE",
			INT2FIX(GNOME_VFS_MIME_ACTION_TYPE_NONE));
	rb_define_const(gvfs_mime, "ACTION_TYPE_APPLICATION",
			INT2FIX(GNOME_VFS_MIME_ACTION_TYPE_APPLICATION));
	rb_define_const(gvfs_mime, "ACTION_TYPE_COMPONENT",
			INT2FIX(GNOME_VFS_MIME_ACTION_TYPE_COMPONENT));
}

/* vim: set sts=0 sw=8 ts=8: *************************************************/
