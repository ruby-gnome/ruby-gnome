/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/* $Id: rbgnome-client.c,v 1.4 2002/10/19 16:36:25 tkubo Exp $ */
/* base on libgnomeui/gnome-client.h */

/* Gnome::Client - GNOME session management client support
 *
 * Copyright (C) 1998 Carsten Schaar <nhadcasc@fs-maphy.uni-hannover.de>
 * Copyright (C) 2001 Neil Conway <neilconway@rogers.com>
 * Copyright (C) 2002 KUBO Takehiro <kubo@jiubao.org>
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

static void
call_argv_command(func, self, args)
    void (*func)(GnomeClient *, gint, gchar **);
    VALUE self, args;
{
    gchar **argv;
    int i;

    Check_Type(args, T_ARRAY);
    argv = ALLOCA_N(gchar *, RARRAY(args)->len + 1);
    for (i = 0; i < RARRAY(args)->len; i++) {
        argv[i] = RVAL2CSTR(RARRAY(args)->ptr[i]);
    }
    argv[i] = NULL;
    (*func)(_SELF(self), RARRAY(args)->len, argv);
}

static VALUE
rbgnome_master_client(self)
    VALUE self;
{
    return GOBJ2RVAL(gnome_master_client());
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
    gnome_client_set_global_config_prefix(_SELF(self), RVAL2CSTR(prefix));
    return self;
}

static VALUE
client_get_flags(self)
    VALUE self;
{
    GnomeClientFlags result = gnome_client_get_flags(_SELF(self));
    return INT2NUM(result);
}

static VALUE
client_set_restart_style(self, style)
    VALUE self, style;
{
    gnome_client_set_restart_style(_SELF(self),
                                   NUM2INT(style)/* GnomeRestartStyle */);
    return self;
}

static VALUE
client_set_priority(self, priority)
    VALUE self, priority;
{
    gnome_client_set_priority(_SELF(self), NUM2UINT(priority));
    return self;
}

static VALUE
client_set_restart_command(self, args)
    VALUE self, args;
{
    call_argv_command(gnome_client_set_restart_command, self, args);
    return self;
}

#if 0
void         gnome_client_add_static_arg (GnomeClient *client, ...);
#endif

static VALUE
client_set_discard_command(self, args)
    VALUE self, args;
{
    call_argv_command(gnome_client_set_discard_command, self, args);
    return self;
}

static VALUE
client_set_resign_command(self, args)
    VALUE self, args;
{
    call_argv_command(gnome_client_set_resign_command, self, args);
    return self;
}

static VALUE
client_set_shutdown_command(self, args)
    VALUE self, args;
{
    call_argv_command(gnome_client_set_shutdown_command, self, args);
    return self;
}

static VALUE
client_set_current_directory(self, dir)
    VALUE self, dir;
{
    gnome_client_set_current_directory(_SELF(self), RVAL2CSTR(dir));
    return self;
}

static VALUE
client_set_environment(self, name, value)
    VALUE self, name, value;
{
    gnome_client_set_environment(_SELF(self), RVAL2CSTR(name), RVAL2CSTR(value));
    return self;
}

static VALUE
client_set_clone_command(self, args)
    VALUE self, args;
{
    call_argv_command(gnome_client_set_clone_command, self, args);
    return self;
}

static VALUE
client_set_process_id(self, pid)
    VALUE self, pid;
{
    gnome_client_set_process_id(_SELF(self), (pid_t)NUM2INT(pid));
    return self;
}

static VALUE
client_set_program(self, program)
    VALUE self, program;
{
    gnome_client_set_program(_SELF(self), RVAL2CSTR(program));
    return self;
}

static VALUE
client_set_user_id(self, id)
    VALUE self, id;
{
    gnome_client_set_user_id(_SELF(self), RVAL2CSTR(id));
    return self;
}

static VALUE
client_save_any_dialog(self, dialog)
    VALUE self, dialog;
{
    gnome_client_save_any_dialog(_SELF(self), GTK_DIALOG(RVAL2GOBJ(dialog)));
    return self;
}

static VALUE
client_save_error_dialog(self, dialog)
    VALUE self, dialog;
{
    gnome_client_save_error_dialog(_SELF(self), GTK_DIALOG(RVAL2GOBJ(dialog)));
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
client_request_save(self, save_style, shutdown, interact_style, fast, global)
    VALUE self, save_style, shutdown, interact_style, fast, global;
{
    gnome_client_request_save(_SELF(self),
                              NUM2INT(save_style), /* GnomeSaveStyle */
                              RTEST(shutdown),
                              NUM2INT(interact_style), /* GnomeInteractStyle */
                              RTEST(fast),
                              RTEST(global));
    return self;
}

static VALUE
client_flush(self)
    VALUE self;
{
    gnome_client_flush(_SELF(self));
    return self;
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
client_set_id(self, client_id)
    VALUE self, client_id;
{
    gnome_client_set_id(_SELF(self), RVAL2CSTR(client_id));
    return self;
}

static VALUE
client_get_id(self)
    VALUE self;
{
    const gchar *result = gnome_client_get_id(_SELF(self));
    return result ? rb_str_new2(result) : Qnil;
}

static VALUE
client_get_previous_id(self)
    VALUE self;
{
    const gchar *result = gnome_client_get_previous_id(_SELF(self));
    return result ? rb_str_new2(result) : Qnil;
}

static VALUE
client_get_desktop_id(self)
    VALUE self;
{
    const gchar *result = gnome_client_get_desktop_id(_SELF(self));
    return result ? rb_str_new2(result) : Qnil;
}

#if 0
void         gnome_client_request_interaction    (GnomeClient *client,
						  GnomeDialogType dialog_type,
						  GnomeInteractFunction function,
						  gpointer data);

void         gnome_client_request_interaction_interp (GnomeClient *client,
						      GnomeDialogType dialog_type,
						      GtkCallbackMarshal function,
						      gpointer data,
						      GtkDestroyNotify destroy);

void         gnome_interaction_key_return        (gint     key,
						  gboolean cancel_shutdown);
#endif

void
Init_gnome_client(mGnome)
    VALUE mGnome;
{
    VALUE gnoClient = G_DEF_CLASS(GNOME_TYPE_CLIENT, "Client", mGnome);

    /* GnomeInteractStyle */
    rb_define_const(mGnome, "INTERACT_NONE", INT2FIX(GNOME_INTERACT_NONE));
    rb_define_const(mGnome, "INTERACT_ERRORS", INT2FIX(GNOME_INTERACT_ERRORS));
    rb_define_const(mGnome, "INTERACT_ANY", INT2FIX(GNOME_INTERACT_ANY));

    /* GnomeDialogType */
    rb_define_const(mGnome, "DIALOG_ERROR", INT2FIX(GNOME_DIALOG_ERROR));
    rb_define_const(mGnome, "DIALOG_NORMAL", INT2FIX(GNOME_DIALOG_NORMAL));

    /* GnomeSaveStyle */
    rb_define_const(mGnome, "SAVE_GLOBAL", INT2FIX(GNOME_SAVE_GLOBAL));
    rb_define_const(mGnome, "SAVE_LOCAL", INT2FIX(GNOME_SAVE_LOCAL));
    rb_define_const(mGnome, "SAVE_BOTH", INT2FIX(GNOME_SAVE_BOTH));

    /* GnomeRestartStyle */
    rb_define_const(mGnome, "RESTART_IF_RUNNING", INT2FIX(GNOME_RESTART_IF_RUNNING));
    rb_define_const(mGnome, "RESTART_ANYWAY", INT2FIX(GNOME_RESTART_ANYWAY));
    rb_define_const(mGnome, "RESTART_IMMEDIATELY", INT2FIX(GNOME_RESTART_IMMEDIATELY));
    rb_define_const(mGnome, "RESTART_NEVER", INT2FIX(GNOME_RESTART_NEVER));

    /* GnomeClientState */
    rb_define_const(mGnome, "CLIENT_IDLE", INT2FIX(GNOME_CLIENT_IDLE));
    rb_define_const(mGnome, "CLIENT_SAVING_PHASE_1", INT2FIX(GNOME_CLIENT_SAVING_PHASE_1));
    rb_define_const(mGnome, "CLIENT_WAITING_FOR_PHASE_2", INT2FIX(GNOME_CLIENT_WAITING_FOR_PHASE_2));
    rb_define_const(mGnome, "CLIENT_SAVING_PHASE_2", INT2FIX(GNOME_CLIENT_SAVING_PHASE_2));
    rb_define_const(mGnome, "CLIENT_FROZEN", INT2FIX(GNOME_CLIENT_FROZEN));
    rb_define_const(mGnome, "CLIENT_DISCONNECTED", INT2FIX(GNOME_CLIENT_DISCONNECTED));
    rb_define_const(mGnome, "CLIENT_REGISTERING", INT2FIX(GNOME_CLIENT_REGISTERING));

    /* GnomeClientFlags */
    rb_define_const(mGnome, "CLIENT_IS_CONNECTED", INT2FIX(GNOME_CLIENT_IS_CONNECTED));
    rb_define_const(mGnome, "CLIENT_RESTARTED", INT2FIX(GNOME_CLIENT_RESTARTED));
    rb_define_const(mGnome, "CLIENT_RESTORED", INT2FIX(GNOME_CLIENT_RESTORED));

    rb_define_module_function(mGnome, "master_client", rbgnome_master_client, 0);
    rb_define_method(gnoClient, "config_prefix", client_get_config_prefix, 0);
    rb_define_method(gnoClient, "global_config_prefix", client_get_global_config_prefix, 0);
    rb_define_method(gnoClient, "set_global_config_prefix", client_set_global_config_prefix, 1);
    rb_define_method(gnoClient, "get_flags", client_get_flags, 1);
    rb_define_method(gnoClient, "set_restart_style", client_set_restart_style, 1);
    rb_define_method(gnoClient, "set_priority", client_set_priority, 1);
    rb_define_method(gnoClient, "set_restart_command", client_set_restart_command, 1);
    rb_define_method(gnoClient, "set_discard_command", client_set_discard_command, 1);
    rb_define_method(gnoClient, "set_resign_command", client_set_resign_command, 1);
    rb_define_method(gnoClient, "set_shutdown_command", client_set_shutdown_command, 1);
    rb_define_method(gnoClient, "set_current_directory", client_set_current_directory, 1);
    rb_define_method(gnoClient, "set_environment", client_set_environment, 2);
    rb_define_method(gnoClient, "set_clone_command", client_set_clone_command, 1);
    rb_define_method(gnoClient, "set_process_id", client_set_process_id, 1);
    rb_define_method(gnoClient, "set_program", client_set_program, 1);
    rb_define_method(gnoClient, "set_user_id", client_set_user_id, 1);
    rb_define_method(gnoClient, "save_any_dialog", client_save_any_dialog, 1);
    rb_define_method(gnoClient, "save_error_dialog", client_save_error_dialog, 1);
    rb_define_method(gnoClient, "request_phase_2", client_request_phase_2, 0);
    rb_define_method(gnoClient, "request_save", client_request_save, 5);
    rb_define_method(gnoClient, "flush", client_flush, 0);

    rb_define_method(gnoClient, "initialize", client_initialize, -1);
    rb_define_method(gnoClient, "connect", client_connect, 0);
    rb_define_method(gnoClient, "disconnect", client_disconnect, 0);
    rb_define_method(gnoClient, "connected", client_connected, 0);
    rb_define_method(gnoClient, "set_id", client_set_id, 1);
    rb_define_method(gnoClient, "id", client_get_id, 0);
    rb_define_method(gnoClient, "previous_id", client_get_previous_id, 0);
    rb_define_method(gnoClient, "desktop_id", client_get_desktop_id, 0);

    G_DEF_SETTERS(gnoClient);
}
