/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011-2021  Ruby-GNOME Project Team
 *  Copyright (C) 2004  Masao Mutoh
 *  Copyright (C) 2004  Kazuhiro NISHIYAMA
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 *  MA  02110-1301  USA
 */

#include "rbgprivate.h"
#include "rbglib.h"

#define RG_TARGET_NAMESPACE mSpawn

static ID id_call;
static ID id_new;

static void
child_setup(gpointer data)
{
    VALUE func = POINTER2RVAL(data);
    if (!NIL_P(func)) {
        rb_funcall(func, id_call, 0);
    }
}

static VALUE
rg_s_async_with_pipes(VALUE self, VALUE working_directory, VALUE argv, VALUE envp, VALUE flags)
{
    GError *err = NULL;
    gboolean ret;
    GPid child_pid;
    VALUE func = Qnil;
    gchar **gargv;
    gchar **genvp;
    gint standard_input, standard_output, standard_error;

    if (rb_block_given_p()) {
        func = rb_block_proc();
        G_RELATIVE(self, func);
    }

    gargv = (gchar **)RVAL2STRV(argv);
    genvp = (gchar **)RVAL2STRV_ACCEPT_NIL(envp);
    ret = g_spawn_async_with_pipes(RVAL2CSTR_ACCEPT_NIL(working_directory),
                                   gargv, genvp, NUM2INT(flags),
                                   (GSpawnChildSetupFunc)child_setup, 
                                   (gpointer)func,
                                   &child_pid, 
                                   &standard_input, &standard_output,
                                   &standard_error, &err);
    g_free(gargv);
    g_free(genvp);
    if (!ret)
        RAISE_GERROR(err);

    return rb_ary_new3(4, INT2NUM((gint)child_pid), 
                       rb_funcall(rb_cIO, id_new, 1, INT2NUM(standard_input)),
                       rb_funcall(rb_cIO, id_new, 1, INT2NUM(standard_output)),
                       rb_funcall(rb_cIO, id_new, 1, INT2NUM(standard_error)));
}

static VALUE
rg_s_async(VALUE self, VALUE working_directory, VALUE argv, VALUE envp, VALUE flags)
{
    GError *err = NULL;
    gboolean ret;
    GPid child_pid;
    VALUE func = Qnil;
    gchar **gargv;
    gchar **genvp;

    if (rb_block_given_p()) {
        func = rb_block_proc();
        G_RELATIVE(self, func);
    }

    gargv = (gchar **)RVAL2STRV(argv);
    genvp = (gchar **)RVAL2STRV_ACCEPT_NIL(envp);
    ret = g_spawn_async(RVAL2CSTR_ACCEPT_NIL(working_directory),
                        gargv, genvp, NUM2INT(flags),
                        (GSpawnChildSetupFunc)child_setup, (gpointer)func,
                        &child_pid, &err);
    g_free(gargv);
    g_free(genvp);
    if (!ret)
        RAISE_GERROR(err);

    return INT2NUM((int)child_pid);
}

static VALUE
rg_s_sync(VALUE self, VALUE working_directory, VALUE argv, VALUE envp, VALUE flags)
{
    GError *err = NULL;
    gboolean ret;
    VALUE func = Qnil;
    gchar** gargv;
    gchar** genvp;
    gchar *standard_output = NULL, *standard_error = NULL;
    gint exit_status;
    VALUE std_out, std_err;

    if (rb_block_given_p()) {
        func = rb_block_proc();
        G_RELATIVE(self, func);
    }

    gargv = (gchar **)RVAL2STRV(argv);
    genvp = (gchar **)RVAL2STRV_ACCEPT_NIL(envp);
    ret = g_spawn_sync(RVAL2CSTR_ACCEPT_NIL(working_directory),
                       gargv, genvp, NUM2INT(flags),
                       (GSpawnChildSetupFunc)child_setup, (gpointer)func,
                       &standard_output, &standard_error,
                       &exit_status, &err);
    g_free(gargv);
    g_free(genvp);
    if (!ret)
        RAISE_GERROR(err);

    if (standard_output) {
        std_out = CSTR2RVAL(standard_output);
        g_free(standard_output);
    } else {
        std_out = Qnil;
        standard_output = NULL;
    }
    if (standard_error) {
        std_err = CSTR2RVAL(standard_error);
        g_free(standard_error);
        standard_error = NULL;
    } else {
        std_err = Qnil;
    }

    if (! ret)
        RAISE_GERROR(err);

    return rb_ary_new3(3, std_out, std_err, INT2FIX(exit_status));

}

static VALUE
rg_s_command_line_sync(G_GNUC_UNUSED VALUE self, VALUE str)
{
    GError *err = NULL;
    const gchar *command_line;
    gchar *standard_output = NULL, *standard_error = NULL;
    gint exit_status;
    VALUE std_out, std_err;
    gboolean ret;

    command_line = RVAL2CSTR(str);
    ret = g_spawn_command_line_sync(command_line,
                                               &standard_output,
                                               &standard_error,
                                               &exit_status,
                                               &err);
    if (standard_output) {
        std_out = CSTR2RVAL(standard_output);
        g_free(standard_output);
    } else {
        std_out = Qnil;
        standard_output = NULL;
    }
    if (standard_error) {
        std_err = CSTR2RVAL(standard_error);
        g_free(standard_error);
        standard_error = NULL;
    } else {
        std_err = Qnil;
    }

    if (! ret)
        RAISE_GERROR(err);

    return rb_ary_new3(3, std_out, std_err, INT2FIX(exit_status));
}

static VALUE
rg_s_command_line_async(G_GNUC_UNUSED VALUE self, VALUE str)
{
    GError *err = NULL;
    const gchar *command_line;
    VALUE ret;

    command_line = StringValuePtr(str);
    ret = CBOOL2RVAL(g_spawn_command_line_async(command_line, &err));
    if (err != NULL)
        RAISE_GERROR(err);

    return ret;
}

#define RVAL2GPID(value) ((GPid)NUM2INT(pid))

static VALUE
rg_s_close_pid(G_GNUC_UNUSED VALUE self, VALUE pid)
{
    g_spawn_close_pid(RVAL2GPID(pid));
    return Qnil;
}

void
Init_glib_spawn(void)
{
    VALUE RG_TARGET_NAMESPACE = rb_define_module_under(mGLib, "Spawn");

    id_call = rb_intern("call");
    id_new = rb_intern("new");

    /* glib/gspawn.h */
    RG_DEF_SMETHOD(async_with_pipes, 4);
    RG_DEF_SMETHOD(async, 4);
    RG_DEF_SMETHOD(sync, 4);
    RG_DEF_SMETHOD(command_line_sync, 1);
    RG_DEF_SMETHOD(command_line_async, 1);
    RG_DEF_SMETHOD(close_pid, 1);

    rb_define_const(RG_TARGET_NAMESPACE, "LEAVE_DESCRIPTORS_OPEN", INT2NUM(G_SPAWN_LEAVE_DESCRIPTORS_OPEN));
    rb_define_const(RG_TARGET_NAMESPACE, "DO_NOT_REAP_CHILD", INT2NUM(G_SPAWN_DO_NOT_REAP_CHILD));
    rb_define_const(RG_TARGET_NAMESPACE, "SEARCH_PATH", INT2NUM(G_SPAWN_SEARCH_PATH));
    rb_define_const(RG_TARGET_NAMESPACE, "STDOUT_TO_DEV_NULL", INT2NUM(G_SPAWN_STDOUT_TO_DEV_NULL));
    rb_define_const(RG_TARGET_NAMESPACE, "STDERR_TO_DEV_NULL", INT2NUM(G_SPAWN_STDERR_TO_DEV_NULL));
    rb_define_const(RG_TARGET_NAMESPACE, "CHILD_INHERITS_STDIN", INT2NUM(G_SPAWN_CHILD_INHERITS_STDIN));
    rb_define_const(RG_TARGET_NAMESPACE, "FILE_AND_ARGV_ZERO", INT2NUM(G_SPAWN_FILE_AND_ARGV_ZERO));
}
