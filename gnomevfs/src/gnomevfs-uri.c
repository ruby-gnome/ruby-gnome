/*****************************************************************************
 *
 * gnomevfs-uri.c: GnomeVFS URI class.
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
 * Latest Revision: 2003-08-04
 *
 *****************************************************************************/

/* Includes ******************************************************************/

#include "gnomevfs.h"

/* Defines *******************************************************************/

#define _SELF(s)	((GnomeVFSURI *)RVAL2BOXED(s, GNOMEVFS_TYPE_URI))

/* Type Definitions **********************************************************/

/* Function Declarations *****************************************************/

/* Global Variables **********************************************************/

/* Function Implementations **************************************************/

GType
gnome_vfs_uri_get_type(void)
{
	static GType our_type = 0;

	if (our_type == 0) {
		our_type = g_boxed_type_register_static("GnomeVFSURI",
				(GBoxedCopyFunc)gnome_vfs_uri_ref,
				(GBoxedFreeFunc)gnome_vfs_uri_unref);
	}

	return our_type;
}

static VALUE
uri_initialize(self, uri)
	VALUE self, uri;
{
	GnomeVFSURI *gvfs_uri;

	if (RTEST(rb_obj_is_kind_of(uri, g_gvfs_uri))) {
		gvfs_uri = gnome_vfs_uri_dup(RVAL2GVFSURI(uri));
	} else {
		gvfs_uri = gnome_vfs_uri_new(RVAL2CSTR(uri));
	}

	G_INITIALIZE(self, gvfs_uri);
	return Qnil;
}

static VALUE
uri_resolve_relative(self, ref)
	VALUE self, ref;
{
	return GVFSURI2RVAL(gnome_vfs_uri_resolve_relative(_SELF(self),
							   RVAL2CSTR(ref)));
}

static VALUE
uri_append_string(self, str)
	VALUE self, str;
{
	GnomeVFSURI *uri;
	GnomeVFSURI *new_uri;

	uri = _SELF(self);
	new_uri = gnome_vfs_uri_append_string(uri, RVAL2CSTR(str));
	gnome_vfs_uri_unref(uri);
	gnome_vfs_uri_ref(new_uri);
	*uri = *new_uri;
	return self;
}

static VALUE
uri_append_path(self, str)
	VALUE self, str;
{
	GnomeVFSURI *uri;
	GnomeVFSURI *new_uri;

	uri = _SELF(self);
	new_uri = gnome_vfs_uri_append_path(uri, RVAL2CSTR(str));
	gnome_vfs_uri_unref(uri);
	gnome_vfs_uri_ref(new_uri);
	*uri = *new_uri;
	return self;
}

static VALUE
uri_append_file_name(self, str)
	VALUE self, str;
{
	GnomeVFSURI *uri;
	GnomeVFSURI *new_uri;

	uri = _SELF(self);
	new_uri = gnome_vfs_uri_append_file_name(uri, RVAL2CSTR(str));
	gnome_vfs_uri_unref(uri);
	gnome_vfs_uri_ref(new_uri);
	*uri = *new_uri;
	return self;
}

static VALUE
uri_append_string_ret(self, str)
	VALUE self, str;
{
	return GVFSURI2RVAL(gnome_vfs_uri_append_string(_SELF(self), 
							RVAL2CSTR(str)));
}

static VALUE
uri_append_path_ret(self, str)
	VALUE self, str;
{
	return GVFSURI2RVAL(gnome_vfs_uri_append_path(_SELF(self), 
						      RVAL2CSTR(str)));
}

static VALUE
uri_append_file_name_ret(self, str)
	VALUE self, str;
{
	return GVFSURI2RVAL(gnome_vfs_uri_append_file_name(_SELF(self),
							   RVAL2CSTR(str)));
}

static VALUE
uri_to_string(argc, argv, self)
	int argc;
	VALUE *argv;
	VALUE self;
{
	VALUE v_hide_opts;
	int hide_opts;
	gchar *str;
	VALUE v_str;

	if (rb_scan_args(argc, argv, "01", &v_hide_opts) == 1) {
		hide_opts = NUM2INT(v_hide_opts);
	} else {
		hide_opts = GNOME_VFS_URI_HIDE_NONE;
	}

	str = gnome_vfs_uri_to_string(_SELF(self), hide_opts);
	v_str = CSTR2RVAL(str);
	g_free(str);
	return v_str;
}

static VALUE
uri_is_local(self)
	VALUE self;
{
	return CBOOL2RVAL(gnome_vfs_uri_is_local(_SELF(self)));
}

static VALUE
uri_has_parent(self)
	VALUE self;
{
	return CBOOL2RVAL(gnome_vfs_uri_has_parent(_SELF(self)));
}

static VALUE
uri_get_parent(self)
	VALUE self;
{
	return GVFSURI2RVAL(gnome_vfs_uri_get_parent(_SELF(self)));
}

/* XXX: gnome_vfs_uri_get_toplevel */

static VALUE
uri_get_host_name(self)
	VALUE self;
{
	return CSTR2RVAL(gnome_vfs_uri_get_host_name(_SELF(self)));
}

static VALUE
uri_get_scheme(self)
	VALUE self;
{
	return CSTR2RVAL(gnome_vfs_uri_get_scheme(_SELF(self)));
}

static VALUE
uri_get_host_port(self)
	VALUE self;
{
	return INT2FIX(gnome_vfs_uri_get_host_port(_SELF(self)));
}

static VALUE
uri_get_user_name(self)
	VALUE self;
{
	return CSTR2RVAL(gnome_vfs_uri_get_user_name(_SELF(self)));
}

static VALUE
uri_get_password(self)
	VALUE self;
{
	return CSTR2RVAL(gnome_vfs_uri_get_password(_SELF(self)));
}

static VALUE
uri_set_host_name(self, host_name)
	VALUE self, host_name;
{
	gnome_vfs_uri_set_host_name(_SELF(self), RVAL2CSTR(host_name));
	return self;
}

static VALUE
uri_set_host_port(self, host_port)
	VALUE self, host_port;
{
	gnome_vfs_uri_set_host_port(_SELF(self), FIX2UINT(host_port));
	return self;
}

static VALUE
uri_set_user_name(self, user_name)
	VALUE self, user_name;
{
	gnome_vfs_uri_set_user_name(_SELF(self), RVAL2CSTR(user_name));
	return self;
}

static VALUE
uri_set_password(self, password)
	VALUE self, password;
{
	gnome_vfs_uri_set_password(_SELF(self), RVAL2CSTR(password));
	return self;
}

static VALUE
uri_equal(self, other)
	VALUE self, other;
{
	return CBOOL2RVAL(gnome_vfs_uri_equal(RVAL2GVFSURI(self),
					      RVAL2GVFSURI(other)));
}

/* XXX: should recursive be optional? */
static VALUE
uri_is_parent(self, child, recursive)
	VALUE self, child, recursive;
{
	return CBOOL2RVAL(gnome_vfs_uri_is_parent(RVAL2GVFSURI(self),
						  RVAL2GVFSURI(child),
						  RTEST(recursive)));
}

static VALUE
uri_get_path(self)
	VALUE self;
{
	return CSTR2RVAL(gnome_vfs_uri_get_path(_SELF(self)));
}

static VALUE
uri_get_fragment_identifier(self)
	VALUE self;
{
	return CSTR2RVAL(gnome_vfs_uri_get_fragment_identifier(_SELF(self)));
}

static VALUE
uri_extract_dirname(self)
	VALUE self;
{
	gchar *str;
	VALUE v_str;

	str = gnome_vfs_uri_extract_dirname(_SELF(self));
	v_str = CSTR2RVAL(str);
	g_free(str);
	return v_str;
}

static VALUE
uri_extract_short_name(self)
	VALUE self;
{
	gchar *str;
	VALUE v_str;

	str = gnome_vfs_uri_extract_short_name(_SELF(self));
	v_str = CSTR2RVAL(str);
	g_free(str);
	return v_str;
}

static VALUE
uri_extract_short_path_name(self)
	VALUE self;
{
	gchar *str;
	VALUE v_str;

	str = gnome_vfs_uri_extract_short_path_name(_SELF(self));
	v_str = CSTR2RVAL(str);
	g_free(str);
	return v_str;
}

static VALUE
uri_same_fs(self, other)
	VALUE self, other;
{
	GnomeVFSResult result;
	gboolean same_fs;
	gchar *text_uri;

	if (RTEST(rb_obj_is_kind_of(other, g_gvfs_uri))) {
		result = gnome_vfs_check_same_fs_uris(_SELF(self),
						      RVAL2GVFSURI(other),
						      &same_fs);
	} else {
		text_uri = gnome_vfs_uri_to_string(_SELF(self),
						   GNOME_VFS_URI_HIDE_NONE);
		result = gnome_vfs_check_same_fs(text_uri,
						 RVAL2CSTR(other),
						 &same_fs);
		g_free(text_uri);
	}

	if (result == GNOME_VFS_OK) {
		return CBOOL2RVAL(same_fs);
	} else {
		return GVFSRESULT2RVAL(result);
	}
}

static VALUE
uri_exists(self)
	VALUE self;
{
	return CBOOL2RVAL(gnome_vfs_uri_exists(_SELF(self)));
}

/*
 * XXX: gnome_vfs_uri_hequal
 * XXX: gnome_vfs_uri_hash
 */

/* XXX: gnome_vfs_uri_list_* */

/* XXX: gnome_vfs_uri_make_full_from_relative */

void
Init_gnomevfs_uri(m_gvfs)
	VALUE m_gvfs;
{
	/* FIXME: or? */
	rbgobj_boxed_not_copy_obj(GNOMEVFS_TYPE_URI);

	g_gvfs_uri = G_DEF_CLASS(GNOMEVFS_TYPE_URI, "URI", m_gvfs);

	rb_define_const(g_gvfs_uri, "HIDE_NONE",
			INT2FIX(GNOME_VFS_URI_HIDE_NONE));
	rb_define_const(g_gvfs_uri, "HIDE_USER_NAME",
			INT2FIX(GNOME_VFS_URI_HIDE_USER_NAME));
	rb_define_const(g_gvfs_uri, "HIDE_PASSWORD",
			INT2FIX(GNOME_VFS_URI_HIDE_PASSWORD));
	rb_define_const(g_gvfs_uri, "HIDE_HOST_NAME",
			INT2FIX(GNOME_VFS_URI_HIDE_HOST_NAME));
	rb_define_const(g_gvfs_uri, "HIDE_HOST_PORT",
			INT2FIX(GNOME_VFS_URI_HIDE_HOST_PORT));
	rb_define_const(g_gvfs_uri, "HIDE_TOPLEVEL_METHOD",
			INT2FIX(GNOME_VFS_URI_HIDE_TOPLEVEL_METHOD));
	rb_define_const(g_gvfs_uri, "HIDE_FRAGMENT_IDENTIFIER",
			INT2FIX(GNOME_VFS_URI_HIDE_FRAGMENT_IDENTIFIER));

	rb_define_const(g_gvfs_uri, "MAGIC_CHR",
			CHR2FIX(GNOME_VFS_URI_MAGIC_CHR));
	rb_define_const(g_gvfs_uri, "MAGIC_STR",
			CSTR2RVAL(GNOME_VFS_URI_MAGIC_STR));
	rb_define_const(g_gvfs_uri, "PATH_CHR",
			CHR2FIX(GNOME_VFS_URI_PATH_CHR));
	rb_define_const(g_gvfs_uri, "PATH_STR",
			CSTR2RVAL(GNOME_VFS_URI_PATH_STR));

	rb_define_method(g_gvfs_uri, "initialize", uri_initialize, 1);
	rb_define_method(g_gvfs_uri, "resolve_relative", uri_resolve_relative,
			 1);
	rb_define_method(g_gvfs_uri, "to_s", uri_to_string, -1);
	rb_define_method(g_gvfs_uri, "local?", uri_is_local, 0);
	rb_define_method(g_gvfs_uri, "has_parent?", uri_has_parent, 0);
	rb_define_method(g_gvfs_uri, "get_parent", uri_get_parent, 0);
	rb_define_method(g_gvfs_uri, "get_host_name", uri_get_host_name, 0);
	rb_define_method(g_gvfs_uri, "get_scheme", uri_get_scheme, 0);
	rb_define_method(g_gvfs_uri, "get_host_port", uri_get_host_port, 0);
	rb_define_method(g_gvfs_uri, "get_user_name", uri_get_user_name, 0);
	rb_define_method(g_gvfs_uri, "get_password", uri_get_password, 0);
	rb_define_method(g_gvfs_uri, "set_host_name", uri_set_host_name, 1);
	rb_define_method(g_gvfs_uri, "set_host_port", uri_set_host_port, 1);
	rb_define_method(g_gvfs_uri, "set_user_name", uri_set_user_name, 1);
	rb_define_method(g_gvfs_uri, "set_password", uri_set_password, 1);
	rb_define_method(g_gvfs_uri, "==", uri_equal, 1);
	rb_define_method(g_gvfs_uri, "parent?", uri_is_parent, 2);
	rb_define_method(g_gvfs_uri, "path", uri_get_path, 0);
	rb_define_method(g_gvfs_uri, "fragment_identifier", uri_get_fragment_identifier, 0);
	rb_define_method(g_gvfs_uri, "extract_dirname", uri_extract_dirname,
			 0);
	rb_define_method(g_gvfs_uri, "extract_short_name",
			 uri_extract_short_name, 0);
	rb_define_method(g_gvfs_uri, "extract_short_path_name",
			 uri_extract_short_path_name, 0);
	rb_define_method(g_gvfs_uri, "same_fs?", uri_same_fs, 1);
	rb_define_method(g_gvfs_uri, "exists?", uri_exists, 0);

	rb_define_method(g_gvfs_uri, "append_string!", uri_append_string, 1);
	rb_define_method(g_gvfs_uri, "append_path!", uri_append_path, 1);
	rb_define_method(g_gvfs_uri, "append_file_name!", uri_append_file_name,
			 1);
	rb_define_method(g_gvfs_uri, "append_string", uri_append_string_ret,
			 1);
	rb_define_method(g_gvfs_uri, "append_path", uri_append_path_ret, 1);
	rb_define_method(g_gvfs_uri, "append_file_name",
			 uri_append_file_name_ret, 1);

	G_DEF_SETTERS(g_gvfs_uri);
}

/* vim: set sts=0 sw=8 ts=8: *************************************************/
