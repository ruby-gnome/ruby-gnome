/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/* $Id: rbgnome-client.c,v 1.3 2002/09/25 17:17:24 tkubo Exp $ */

/* Gnome::Client - GNOME session management client support
 *
 * Copyright (C) 1998 Carsten Schaar <nhadcasc@fs-maphy.uni-hannover.de>
 * Copyright (C) 2001 Neil Conway <neilconway@rogers.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Cambridge, MA 02139, USA.
 */

#include "rbgnome.h"

#define _SELF(self) GNOME_CLIENT(RVAL2GOBJ(self))

static VALUE
rbgnome_master_client(self)
    VALUE self;
{
    return GOBJ2RVAL(gnome_master_client());
}

static VALUE
client_initialize(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    GnomeClient* client;
    VALUE do_connect;
    rb_scan_args(argc, argv, "01", &do_connect);
    if (NIL_P(do_connect) || RTEST(do_connect)) {
        client = gnome_client_new();
    } else {
        client = gnome_client_new_without_connection();
    }
    RBGTK_INITIALIZE(self, client);
    return Qnil;
}

static VALUE
client_connect(self)
    VALUE self;
{
    gnome_client_connect(_SELF(self));
    return self;
}

static VALUE
client_disconnect(self)
    VALUE self;
{
    gnome_client_disconnect(_SELF(self));
    return self;
}

static VALUE
client_connected(self)
    VALUE self;
{
    return GNOME_CLIENT_CONNECTED(_SELF(self)) ? Qtrue : Qfalse;
}

static VALUE
client_get_id(self)
    VALUE self;
{
    const gchar *result = gnome_client_get_id(_SELF(self));
    return result ? rb_str_new2(result) : Qnil;
}

static VALUE
client_set_id(self, client_id)
    VALUE self, client_id;
{
    gnome_client_set_id(_SELF(self), RVAL2CSTR(client_id));
    return self;
}

static VALUE
client_get_previous_id(self)
    VALUE self;
{
    const gchar *result = gnome_client_get_previous_id(_SELF(self));
    return result ? rb_str_new2(result) : Qnil;
}

static VALUE
client_get_config_prefix(self)
    VALUE self;
{
    const gchar *result = gnome_client_get_config_prefix(_SELF(self));
    return result ? rb_str_new2(result) : Qnil;
}

static VALUE
client_get_global_config_prefix(self)
    VALUE self;
{
    const gchar *result = gnome_client_get_global_config_prefix(_SELF(self));
    return result ? rb_str_new2(result) : Qnil;
}

static VALUE
client_set_global_config_prefix(self, prefix)
    VALUE self, prefix;
{
    gnome_client_set_global_config_prefix(
            _SELF(self), RVAL2CSTR(prefix));
    return self;
}

static VALUE
client_set_current_directory(self, dir)
    VALUE self, dir;
{
    gnome_client_set_current_directory(_SELF(self),
                                       RVAL2CSTR(dir));
    return self;
}

static VALUE
client_set_environment(self, name, value)
    VALUE self, name, value;
{
    gnome_client_set_environment(_SELF(self),
                                 RVAL2CSTR(name),
                                 RVAL2CSTR(value));
    return self;
}

static VALUE
client_set_process_id(self, pid)
    VALUE self, pid;
{
    gnome_client_set_process_id(_SELF(self),
                                (pid_t)NUM2INT(pid));
    return self;
}

static VALUE
client_set_program(self, program)
    VALUE self, program;
{
    gnome_client_set_program(_SELF(self),
                             RVAL2CSTR(program));
    return self;
}

static VALUE
client_set_user_id(self, user_id)
    VALUE self, user_id;
{
    gnome_client_set_user_id(_SELF(self),
                             RVAL2CSTR(user_id));
    return self;
}

static VALUE
client_request_phase_2(self)
    VALUE self;
{
    gnome_client_request_phase_2(_SELF(self));
    return self;
}

static VALUE
client_save_any_dialog(self, dialog)
    VALUE self, dialog;
{
    gnome_client_save_any_dialog(_SELF(self),
                                 GTK_DIALOG(RVAL2GOBJ(dialog)));
    return self;
}

static VALUE
client_save_error_dialog(self, dialog)
    VALUE self, dialog;
{
    gnome_client_save_error_dialog(_SELF(self),
                                   GTK_DIALOG(RVAL2GOBJ(dialog)));
    return self;
}

static VALUE
client_flush(self)
    VALUE self;
{
    gnome_client_flush(_SELF(self));
    return self;
}

void
Init_gnome_client(mGnome)
    VALUE mGnome;
{
    VALUE gnoClient = G_DEF_CLASS(GNOME_TYPE_CLIENT, "Client", mGnome);

    rb_define_module_function(mGnome, "master_client", rbgnome_master_client, 0);

    rb_define_method(gnoClient, "initialize", client_initialize, -1);
    rb_define_method(gnoClient, "connect", client_connect, 0);
    rb_define_method(gnoClient, "disconnect", client_disconnect, 0);
    rb_define_method(gnoClient, "connected", client_connected, 0);
    rb_define_method(gnoClient, "get_id", client_get_id, 0);
    rb_define_method(gnoClient, "set_id", client_set_id, 1);
    rb_define_method(gnoClient, "get_previous_id", client_get_previous_id, 0);
    rb_define_method(gnoClient, "get_config_prefix", client_get_config_prefix, 0);
    rb_define_method(gnoClient, "get_global_config_prefix", client_get_global_config_prefix, 0);
    rb_define_method(gnoClient, "set_global_config_prefix", client_set_global_config_prefix, 1);
    rb_define_method(gnoClient, "set_current_directory", client_set_current_directory, 1);
    rb_define_method(gnoClient, "set_environment", client_set_environment, 2);
    rb_define_method(gnoClient, "set_process_id", client_set_process_id, 1);
    rb_define_method(gnoClient, "set_program", client_set_program, 1);
    rb_define_method(gnoClient, "set_user_id", client_set_user_id, 1);
    rb_define_method(gnoClient, "request_phase_2", client_request_phase_2, 0);
    rb_define_method(gnoClient, "save_any_dialog", client_save_any_dialog, 1);
    rb_define_method(gnoClient, "save_error_dialog", client_save_error_dialog, 1);
    rb_define_method(gnoClient, "flush", client_flush, 0);

    rb_define_alias(gnoClient, "connected?", "connected");
    rb_define_alias(gnoClient, "id", "get_id");
    rb_define_alias(gnoClient, "id=", "set_id");
    rb_define_alias(gnoClient, "previous_id", "get_previous_id");
    rb_define_alias(gnoClient, "config_prefix", "get_config_prefix");
    rb_define_alias(gnoClient, "global_config_prefix", "get_global_config_prefix");
    rb_define_alias(gnoClient, "global_config_prefix=", "set_global_config_prefix");
    rb_define_alias(gnoClient, "current_directory=", "set_current_directory");
    rb_define_alias(gnoClient, "process_id=", "set_process_id");
    rb_define_alias(gnoClient, "program=", "set_program");
    rb_define_alias(gnoClient, "user_id=", "set_user_id");
}
