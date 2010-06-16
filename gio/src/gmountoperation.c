/* -*- c-file-style: "ruby" -*- */
/*
 * Ruby/GIO: a Ruby binding of gio-2.0.x.
 * Copyright (C) 2008-2009  Ruby-GNOME2 Project Team
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "gio2.h"

#define _SELF(value) RVAL2GMOUNTOPERATION(value)

static VALUE
mountoperation_initialize(VALUE self)
{
        G_INITIALIZE(self, g_mount_operation_new());

        return Qnil;
}

static VALUE
mountoperation_username(VALUE self)
{
	return CSTR2RVAL(g_mount_operation_get_username(_SELF(self)));
}

static VALUE
mountoperation_set_username(VALUE self, VALUE value)
{
	g_mount_operation_set_username(_SELF(self), RVAL2CSTR(value));

	return self;
}

static VALUE
mountoperation_password(VALUE self)
{
	return CSTR2RVAL(g_mount_operation_get_password(_SELF(self)));
}

static VALUE
mountoperation_set_password(VALUE self, VALUE value)
{
	g_mount_operation_set_password(_SELF(self), RVAL2CSTR(value));

	return self;
}

static VALUE
mountoperation_anonymous(VALUE self)
{
	return CBOOL2RVAL(g_mount_operation_get_anonymous(_SELF(self)));
}

static VALUE
mountoperation_set_anonymous(VALUE self, VALUE value)
{
	g_mount_operation_set_anonymous(_SELF(self), RVAL2CBOOL(value));

	return self;
}

static VALUE
mountoperation_domain(VALUE self)
{
	return CSTR2RVAL(g_mount_operation_get_domain(_SELF(self)));
}

static VALUE
mountoperation_set_domain(VALUE self, VALUE value)
{
	g_mount_operation_set_domain(_SELF(self), RVAL2CSTR(value));

	return self;
}

static VALUE
mountoperation_password_save(VALUE self)
{
	return GPASSWORDSAVE2RVAL(g_mount_operation_get_password_save(_SELF(self)));
}

static VALUE
mountoperation_set_password_save(VALUE self, VALUE value)
{
	g_mount_operation_set_password_save(_SELF(self), RVAL2GPASSWORDSAVE(value));

	return self;
}

static VALUE
mountoperation_choice(VALUE self)
{
	return INT2NUM(g_mount_operation_get_choice(_SELF(self)));
}

static VALUE
mountoperation_set_choice(VALUE self, VALUE value)
{
	g_mount_operation_set_choice(_SELF(self), NUM2INT(value));

	return self;
}

static VALUE
mountoperation_reply(VALUE self, VALUE value)
{
	g_mount_operation_reply(_SELF(self), RVAL2GMOUNTOPERATIONRESULT(value));

	return self;
}

void
Init_gmountoperation(VALUE glib)
{
        VALUE mountoperation = G_DEF_CLASS(G_TYPE_MOUNT_OPERATION, "MountOperation", glib);

        /* TODO: Are these necessary? */
        G_DEF_CLASS(G_TYPE_MOUNT_OPERATION_RESULT, "Result", mountoperation);
	G_DEF_CONSTANTS(mountoperation, G_TYPE_MOUNT_OPERATION_RESULT, "G_MOUNT_OPERATION_");

        G_DEF_CLASS(G_TYPE_ASK_PASSWORD_FLAGS, "AskPasswordFlags", glib);
	G_DEF_CONSTANTS(glib, G_TYPE_ASK_PASSWORD_FLAGS, "G_");
        G_DEF_CLASS(G_TYPE_PASSWORD_SAVE, "PasswordSave", glib);
	G_DEF_CONSTANTS(glib, G_TYPE_PASSWORD_SAVE, "G_");

        rb_define_method(mountoperation, "initialize", mountoperation_initialize, 0);
        rb_define_method(mountoperation, "username", mountoperation_username, 0);
	rb_define_method(mountoperation, "set_username", mountoperation_set_username, 1);
	G_DEF_SETTER(mountoperation, "username");
        rb_define_method(mountoperation, "password", mountoperation_password, 0);
	rb_define_method(mountoperation, "set_password", mountoperation_set_password, 1);
	G_DEF_SETTER(mountoperation, "password");
        rb_define_method(mountoperation, "anonymous", mountoperation_anonymous, 0);
	rb_define_method(mountoperation, "set_anonymous", mountoperation_set_anonymous, 1);
	G_DEF_SETTER(mountoperation, "anonymous");
        rb_define_method(mountoperation, "domain", mountoperation_domain, 0);
	rb_define_method(mountoperation, "set_domain", mountoperation_set_domain, 1);
	G_DEF_SETTER(mountoperation, "domain");
        rb_define_method(mountoperation, "password_save", mountoperation_password_save, 0);
	rb_define_method(mountoperation, "set_password_save", mountoperation_set_password_save, 1);
	G_DEF_SETTER(mountoperation, "password_save");
        rb_define_method(mountoperation, "choice", mountoperation_choice, 0);
	rb_define_method(mountoperation, "set_choice", mountoperation_set_choice, 1);
	G_DEF_SETTER(mountoperation, "choice");
        rb_define_method(mountoperation, "reply", mountoperation_reply, 1);
}
