/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/**********************************************************************

  rbglib_spawn.c -

  $Author: mutoh $
  $Date: 2004/08/01 16:36:23 $

  Copyright (C) 2004 Masao Mutoh
  Copyright (C) 2004 Kazuhiro NISHIYAMA

**********************************************************************/
#include "global.h"
#include "rbglib.h"
#include <errno.h>

static VALUE spawn_error[20];
static ID id_call;
static ID id_new;

void
rbglib_spawn_error(error)
    GError *error;
{
    VALUE exc = rb_funcall(spawn_error[error->code], id_new, 1, CSTR2RVAL(error->message));
    g_error_free(error);
    rb_exc_raise(exc);
}
static void
child_setup(func)
    gpointer func;
{
    if (! NIL_P(func)){
        rb_funcall((VALUE)func, id_call, 0);
    }
}

static VALUE
rbglib_m_spawn_async_with_pipes(self, working_directory, argv, envp, flags)
    VALUE self, working_directory, argv, envp, flags;
{
    GError *err = NULL;
    gboolean ret;
    GPid child_pid;
    VALUE func = Qnil;
    gint gargc, genc, i;
    gchar** gargv = (gchar**)NULL;
    gchar** genvp = (gchar**)NULL;
    gint standard_input, standard_output, standard_error;

    if (rb_block_given_p()) {
        func = G_BLOCK_PROC();
        G_RELATIVE(self, func);
    }

    if (! NIL_P(argv)){
        Check_Type(argv, T_ARRAY);
        gargc = RARRAY(argv)->len;
        gargv = ALLOCA_N(gchar*, gargc + 1);
        for (i = 0; i < gargc; i++) {
            if (TYPE(RARRAY(argv)->ptr[i]) == T_STRING) {
                gargv[i] = RVAL2CSTR(RARRAY(argv)->ptr[i]);
            }
            else {
                gargv[i] = "";
            }
        }
        gargv[gargc] = (gchar*)NULL;
    }

    if (! NIL_P(envp)){
        Check_Type(envp, T_ARRAY);
        genc = RARRAY(envp)->len;
        genvp = ALLOCA_N(gchar*, genc + 1);
        for (i = 0; i < genc; i++) {
            if (TYPE(RARRAY(envp)->ptr[i]) == T_STRING) {
                genvp[i] = RVAL2CSTR(RARRAY(envp)->ptr[i]);
            }
            else {
                genvp[i] = "";
            }
        }
        genvp[genc] = (gchar*)NULL;
    }

    ret = g_spawn_async_with_pipes(NIL_P(working_directory) ? NULL : RVAL2CSTR(working_directory),
                                   gargv, genvp, NUM2INT(flags),
                                   child_setup, (gpointer)func,
                                   &child_pid, 
                                   &standard_input, &standard_output,
                                   &standard_error, &err);
    
    if (! ret)
        rbglib_spawn_error(err);
    
    return rb_ary_new3(4, INT2NUM(child_pid), 
                       rb_funcall(rb_cIO, id_new, 1, INT2NUM(standard_input)),
                       rb_funcall(rb_cIO, id_new, 1, INT2NUM(standard_output)),
                       rb_funcall(rb_cIO, id_new, 1, INT2NUM(standard_error)));
}

static VALUE
rbglib_m_spawn_async(self, working_directory, argv, envp, flags)
    VALUE self, working_directory, argv, envp, flags;
{
    GError *err = NULL;
    gboolean ret;
    GPid child_pid;
    VALUE func = Qnil;
    gint gargc, genc, i;
    gchar** gargv = (gchar**)NULL;
    gchar** genvp = (gchar**)NULL;

    if (rb_block_given_p()) {
        func = G_BLOCK_PROC();
        G_RELATIVE(self, func);
    }

    if (! NIL_P(argv)){
        Check_Type(argv, T_ARRAY);
        gargc = RARRAY(argv)->len;
        gargv = ALLOCA_N(gchar*, gargc + 1);
        for (i = 0; i < gargc; i++) {
            if (TYPE(RARRAY(argv)->ptr[i]) == T_STRING) {
                gargv[i] = RVAL2CSTR(RARRAY(argv)->ptr[i]);
            }
            else {
                gargv[i] = "";
            }
        }
        gargv[gargc] = (gchar*)NULL;
    }

    if (! NIL_P(envp)){
        Check_Type(envp, T_ARRAY);
        genc = RARRAY(envp)->len;
        genvp = ALLOCA_N(gchar*, genc + 1);
        for (i = 0; i < genc; i++) {
            if (TYPE(RARRAY(envp)->ptr[i]) == T_STRING) {
                genvp[i] = RVAL2CSTR(RARRAY(envp)->ptr[i]);
            }
            else {
                genvp[i] = "";
            }
        }
        genvp[genc] = (gchar*)NULL;
    }

    ret = g_spawn_async(NIL_P(working_directory) ? NULL : RVAL2CSTR(working_directory),
                        gargv, genvp, NUM2INT(flags),
                        child_setup, (gpointer)func,
                        &child_pid, &err);

    if (! ret){
        rbglib_spawn_error(err);
    }
    
    return INT2NUM(child_pid);
}

static VALUE
rbglib_m_spawn_sync(self, working_directory, argv, envp, flags)
    VALUE self, working_directory, argv, envp, flags;
{
    GError *err = NULL;
    gboolean ret;
    VALUE func = Qnil;
    gint gargc, genc, i;
    gchar** gargv = (gchar**)NULL;
    gchar** genvp = (gchar**)NULL;
    gchar *standard_output = NULL, *standard_error = NULL;
    gint exit_status;
    VALUE std_out, std_err;

    if (rb_block_given_p()) {
        func = G_BLOCK_PROC();
        G_RELATIVE(self, func);
    }

    if (! NIL_P(argv)){
        Check_Type(argv, T_ARRAY);
        gargc = RARRAY(argv)->len;
        gargv = ALLOCA_N(gchar*, gargc + 1);
        for (i = 0; i < gargc; i++) {
            if (TYPE(RARRAY(argv)->ptr[i]) == T_STRING) {
                gargv[i] = RVAL2CSTR(RARRAY(argv)->ptr[i]);
            }
            else {
                gargv[i] = "";
            }
        }
        gargv[gargc] = (gchar*)NULL;
    }

    if (! NIL_P(envp)){
        Check_Type(envp, T_ARRAY);
        genc = RARRAY(envp)->len;
        genvp = ALLOCA_N(gchar*, genc + 1);
        for (i = 0; i < genc; i++) {
            if (TYPE(RARRAY(envp)->ptr[i]) == T_STRING) {
                genvp[i] = RVAL2CSTR(RARRAY(envp)->ptr[i]);
            }
            else {
                genvp[i] = "";
            }
        }
        genvp[genc] = (gchar*)NULL;
    }

    ret = g_spawn_sync(NIL_P(working_directory) ? NULL : RVAL2CSTR(working_directory),
                       gargv, genvp, NUM2INT(flags),
                       child_setup, (gpointer)func,
                       &standard_output, &standard_error,
                       &exit_status, &err);


    if (! ret){
        rbglib_spawn_error(err);
    }

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
        rbglib_spawn_error(err);

    return rb_ary_new3(3, std_out, std_err, INT2FIX(exit_status));

}

static VALUE
rbglib_m_spawn_command_line_sync(self, str)
    VALUE self, str;
{
    GError *err = NULL;
    const gchar *command_line;
    gchar *standard_output = NULL, *standard_error = NULL;
    gint exit_status;
    VALUE std_out, std_err;
    gboolean ret;

    StringValue(str);
    command_line = RSTRING(str)->ptr;
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
        rbglib_spawn_error(err);

    return rb_ary_new3(3, std_out, std_err, INT2FIX(exit_status));
}

static VALUE
rbglib_m_spawn_command_line_async(self, str)
    VALUE self, str;
{
    GError *err = NULL;
    const gchar *command_line;
    VALUE ret;

    StringValue(str);
    command_line = RSTRING(str)->ptr;
    ret = CBOOL2RVAL(g_spawn_command_line_async(command_line, &err));
    if (err != NULL)
        rbglib_spawn_error(err);

    return ret;
}

#ifdef HAVE_G_SPAWN_CLOSE_PID
static VALUE
rbglib_m_spawn_close_pid(self, pid)
    VALUE self, pid;
{
    g_spawn_close_pid(NUM2INT(pid));
    return Qnil;
}
#endif

void
Init_glib_spawn()
{
    VALUE mGSpawn = rb_define_module_under(mGLib, "Spawn");

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

    spawn_error[G_SPAWN_ERROR_FORK] = rb_define_class_under(mGLib, "SpawnForkError", rb_eStandardError);
    spawn_error[G_SPAWN_ERROR_READ] = rb_define_class_under(mGLib, "SpawnReadError", rb_eStandardError);
    spawn_error[G_SPAWN_ERROR_CHDIR] = rb_define_class_under(mGLib, "SpawnChDirError", rb_eStandardError);
    spawn_error[G_SPAWN_ERROR_FAILED] = rb_define_class_under(mGLib, "SpawnFailed", rb_eStandardError);

#ifdef EACCES
    spawn_error[G_SPAWN_ERROR_ACCES]  = rb_const_get(rb_mErrno, rb_intern("EACCES"));
#endif
#ifdef EPERM
    spawn_error[G_SPAWN_ERROR_PERM]   = rb_const_get(rb_mErrno, rb_intern("EPERM"));
#endif
#ifdef E2BIG
    spawn_error[G_SPAWN_ERROR_2BIG]   = rb_const_get(rb_mErrno, rb_intern("E2BIG"));
#endif
#ifdef ENOEXEC
    spawn_error[G_SPAWN_ERROR_NOEXEC] = rb_const_get(rb_mErrno, rb_intern("ENOEXEC"));
#endif
#ifdef ENAMETOOLONG
    spawn_error[G_SPAWN_ERROR_NAMETOOLONG] = rb_const_get(rb_mErrno, rb_intern("ENAMETOOLONG"));
#endif
#ifdef ENOENT
    spawn_error[G_SPAWN_ERROR_NOENT]  = rb_const_get(rb_mErrno, rb_intern("ENOENT"));
#endif
#ifdef ENOMEM
    spawn_error[G_SPAWN_ERROR_NOMEM]  = rb_const_get(rb_mErrno, rb_intern("ENOMEM"));
#endif
#ifdef ENOTDIR
    spawn_error[G_SPAWN_ERROR_NOTDIR] = rb_const_get(rb_mErrno, rb_intern("ENOTDIR"));
#endif
#ifdef ELOOP
    spawn_error[G_SPAWN_ERROR_LOOP]   = rb_const_get(rb_mErrno, rb_intern("ELOOP"));
#endif
#ifdef ETXTBSY
    spawn_error[G_SPAWN_ERROR_TXTBUSY] = rb_const_get(rb_mErrno, rb_intern("ETXTBSY"));
#endif
#ifdef EIO
    spawn_error[G_SPAWN_ERROR_IO]     = rb_const_get(rb_mErrno, rb_intern("EIO"));
#endif
#ifdef ENFILE
    spawn_error[G_SPAWN_ERROR_NFILE]  = rb_const_get(rb_mErrno, rb_intern("ENFILE"));
#endif
#ifdef EMFILE
    spawn_error[G_SPAWN_ERROR_MFILE]  = rb_const_get(rb_mErrno, rb_intern("EMFILE"));
#endif
#ifdef EINVAL
    spawn_error[G_SPAWN_ERROR_INVAL]  = rb_const_get(rb_mErrno, rb_intern("EINVAL"));
#endif
#ifdef EISDIR
    spawn_error[G_SPAWN_ERROR_ISDIR]  = rb_const_get(rb_mErrno, rb_intern("EISDIR"));
#endif
#ifdef ELIBBAD
    spawn_error[G_SPAWN_ERROR_LIBBAD] = rb_const_get(rb_mErrno, rb_intern("ELIBBAD"));
#endif
}
