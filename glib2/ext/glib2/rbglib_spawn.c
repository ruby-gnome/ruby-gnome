/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/**********************************************************************

  rbglib_spawn.c -

  $Author: sakai $
  $Date: 2007/07/08 02:40:12 $

  Copyright (C) 2004 Masao Mutoh
  Copyright (C) 2004 Kazuhiro NISHIYAMA

**********************************************************************/
#include "rbgprivate.h"
#include "rbglib.h"

static ID id_call;
static ID id_new;

static void
child_setup(gpointer func)
{
    if (! NIL_P(func)){
        rb_funcall((VALUE)func, id_call, 0);
    }
}

static VALUE
rbglib_m_spawn_async_with_pipes(VALUE self, VALUE working_directory, VALUE argv, VALUE envp, VALUE flags)
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
    ret = g_spawn_async_with_pipes(NIL_P(working_directory) ? NULL : RVAL2CSTR(working_directory),
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
rbglib_m_spawn_async(VALUE self, VALUE working_directory, VALUE argv, VALUE envp, VALUE flags)
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
    ret = g_spawn_async(NIL_P(working_directory) ? NULL : RVAL2CSTR(working_directory),
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
rbglib_m_spawn_sync(VALUE self, VALUE working_directory, VALUE argv, VALUE envp, VALUE flags)
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
    ret = g_spawn_sync(NIL_P(working_directory) ? NULL : RVAL2CSTR(working_directory),
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
rbglib_m_spawn_command_line_sync(VALUE self, VALUE str)
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
rbglib_m_spawn_command_line_async(VALUE self, VALUE str)
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

#ifdef HAVE_G_SPAWN_CLOSE_PID

#define RVAL2GPID(value) ((GPid)NUM2INT(pid))

static VALUE
rbglib_m_spawn_close_pid(VALUE self, VALUE pid)
{
    g_spawn_close_pid(RVAL2GPID(pid));
    return Qnil;
}

#endif

void
Init_glib_spawn()
{
    VALUE mGSpawn = rb_define_module_under(mGLib, "Spawn");
    VALUE cSpawnError = G_DEF_ERROR2(G_SPAWN_ERROR, "SpawnError", mGLib, rb_eIOError);

    id_call = rb_intern("call");
    id_new = rb_intern("new");

    /* glib/gspawn.h */
    rb_define_module_function(mGSpawn, "async_with_pipes", rbglib_m_spawn_async_with_pipes, 4);
    rb_define_module_function(mGSpawn, "async", rbglib_m_spawn_async, 4);
    rb_define_module_function(mGSpawn, "sync", rbglib_m_spawn_sync, 4);
    rb_define_module_function(mGSpawn, "command_line_sync", rbglib_m_spawn_command_line_sync, 1);
    rb_define_module_function(mGSpawn, "command_line_async", rbglib_m_spawn_command_line_async, 1);
#ifdef HAVE_G_SPAWN_CLOSE_PID
    rb_define_module_function(mGSpawn, "close_pid", rbglib_m_spawn_close_pid, 1);
#endif

    rb_define_const(mGSpawn, "LEAVE_DESCRIPTORS_OPEN", INT2NUM(G_SPAWN_LEAVE_DESCRIPTORS_OPEN));
    rb_define_const(mGSpawn, "DO_NOT_REAP_CHILD", INT2NUM(G_SPAWN_DO_NOT_REAP_CHILD));
    rb_define_const(mGSpawn, "SEARCH_PATH", INT2NUM(G_SPAWN_SEARCH_PATH));
    rb_define_const(mGSpawn, "STDOUT_TO_DEV_NULL", INT2NUM(G_SPAWN_STDOUT_TO_DEV_NULL));
    rb_define_const(mGSpawn, "STDERR_TO_DEV_NULL", INT2NUM(G_SPAWN_STDERR_TO_DEV_NULL));
    rb_define_const(mGSpawn, "CHILD_INHERITS_STDIN", INT2NUM(G_SPAWN_CHILD_INHERITS_STDIN));
    rb_define_const(mGSpawn, "FILE_AND_ARGV_ZERO", INT2NUM(G_SPAWN_FILE_AND_ARGV_ZERO));

    rb_define_const(cSpawnError, "FORK", INT2NUM(G_SPAWN_ERROR_FORK));
    rb_define_const(cSpawnError, "READ", INT2NUM(G_SPAWN_ERROR_READ));
    rb_define_const(cSpawnError, "CHDIR", INT2NUM(G_SPAWN_ERROR_CHDIR));
    rb_define_const(cSpawnError, "EACCES", INT2NUM(G_SPAWN_ERROR_ACCES));
    rb_define_const(cSpawnError, "EPERM", INT2NUM(G_SPAWN_ERROR_PERM));
    rb_define_const(cSpawnError, "E2BIG", INT2NUM(G_SPAWN_ERROR_2BIG));
    rb_define_const(cSpawnError, "ENOEXEC", INT2NUM(G_SPAWN_ERROR_NOEXEC));
    rb_define_const(cSpawnError, "ENAMETOOLONG", INT2NUM(G_SPAWN_ERROR_NAMETOOLONG));
    rb_define_const(cSpawnError, "ENOENT", INT2NUM(G_SPAWN_ERROR_NOENT));
    rb_define_const(cSpawnError, "ENOMEM", INT2NUM(G_SPAWN_ERROR_NOMEM));
    rb_define_const(cSpawnError, "ENOTDIR", INT2NUM(G_SPAWN_ERROR_NOTDIR));
    rb_define_const(cSpawnError, "ELOOP", INT2NUM(G_SPAWN_ERROR_LOOP));
    rb_define_const(cSpawnError, "ETXTBUSY", INT2NUM(G_SPAWN_ERROR_TXTBUSY));
    rb_define_const(cSpawnError, "EIO", INT2NUM(G_SPAWN_ERROR_IO));
    rb_define_const(cSpawnError, "ENFILE", INT2NUM(G_SPAWN_ERROR_NFILE));
    rb_define_const(cSpawnError, "EMFILE", INT2NUM(G_SPAWN_ERROR_MFILE));
    rb_define_const(cSpawnError, "EINVAL", INT2NUM(G_SPAWN_ERROR_INVAL));
    rb_define_const(cSpawnError, "EISDIR", INT2NUM(G_SPAWN_ERROR_ISDIR));
    rb_define_const(cSpawnError, "ELIBBAD", INT2NUM(G_SPAWN_ERROR_LIBBAD));
    rb_define_const(cSpawnError, "FAILED", INT2NUM(G_SPAWN_ERROR_FAILED));

}
