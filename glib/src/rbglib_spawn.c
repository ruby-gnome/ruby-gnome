/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/**********************************************************************

  rbglib_spawn.c -

  $Author: mutoh $
  $Date: 2004/04/27 06:43:42 $

  Copyright (C) 2004 Masao Mutoh
  Copyright (C) 2004 Kazuhiro NISHIYAMA

**********************************************************************/
#include "global.h"
#include "rbglib.h"


static VALUE spawn_error[19];

void
rbglib_spawn_error(error)
    GError *error;
{
    VALUE exc = rb_exc_new2(spawn_error[error->code], error->message);
    g_error_free(error);
    rb_exc_raise(exc);
}

static VALUE
rbglib_m_spawn_command_line_sync(self, str)
    VALUE self, str;
{
    GError *err = NULL;
    const gchar *command_line;
    gchar *standard_output = NULL, *standard_error = NULL;
    gint exit_status;
    VALUE ret, std_out, std_err;

    StringValue(str);
    command_line = RSTRING(str)->ptr;
    ret = CBOOL2RVAL(g_spawn_command_line_sync(command_line,
                                               &standard_output,
                                               &standard_error,
                                               &exit_status,
                                               &err));
    if (standard_output) {
        std_out = rb_str_new2(standard_output);
    } else {
        std_out = Qnil;
        g_free(standard_output);
        standard_output = NULL;
    }
    if (standard_error) {
        std_err = rb_str_new2(standard_error);
        g_free(standard_error);
        standard_error = NULL;
    } else {
        std_err = Qnil;
    }

    if (err != NULL)
        rbglib_spawn_error(err);

    ret = rb_ary_new3(4, ret, std_out, std_err, INT2FIX(exit_status));

    return ret;
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

    /* glib/gspawn.h */
    rb_define_module_function(mGSpawn, "command_line_sync", rbglib_m_spawn_command_line_sync, 1);
    rb_define_module_function(mGSpawn, "command_line_async", rbglib_m_spawn_command_line_async, 1);
#ifdef HAVE_G_SPAWN_CLOSE_PID
    rb_define_module_function(mGSpawn, "close_pid", rbglib_m_spawn_close_pid, 1);
#endif

    spawn_error[G_SPAWN_ERROR_FORK] = rb_define_class_under(mGLib, "SpawnForkError", rb_eSystemCallError);
    spawn_error[G_SPAWN_ERROR_READ] = rb_define_class_under(mGLib, "SpawnReadError", rb_eSystemCallError);
    spawn_error[G_SPAWN_ERROR_CHDIR] = rb_define_class_under(mGLib, "SpawnChDirError", rb_eSystemCallError);
    spawn_error[G_SPAWN_ERROR_FAILED] = rb_define_class_under(mGLib, "SpawnFailed", rb_eSystemCallError);

    spawn_error[G_SPAWN_ERROR_ACCES]  = rb_const_get(rb_mErrno, rb_intern("EACCES"));
    spawn_error[G_SPAWN_ERROR_PERM]   = rb_const_get(rb_mErrno, rb_intern("EPERM"));
    spawn_error[G_SPAWN_ERROR_2BIG]   = rb_const_get(rb_mErrno, rb_intern("E2BIG"));
    spawn_error[G_SPAWN_ERROR_NOEXEC] = rb_const_get(rb_mErrno, rb_intern("ENOEXEC"));
    spawn_error[G_SPAWN_ERROR_NAMETOOLONG] = rb_const_get(rb_mErrno, rb_intern("ENAMETOOLONG"));
    spawn_error[G_SPAWN_ERROR_NOENT]  = rb_const_get(rb_mErrno, rb_intern("ENOENT"));
    spawn_error[G_SPAWN_ERROR_NOMEM]  = rb_const_get(rb_mErrno, rb_intern("ENOMEM"));
    spawn_error[G_SPAWN_ERROR_NOTDIR] = rb_const_get(rb_mErrno, rb_intern("ENOTDIR"));
    spawn_error[G_SPAWN_ERROR_LOOP]   = rb_const_get(rb_mErrno, rb_intern("ELOOP"));
    spawn_error[G_SPAWN_ERROR_TXTBUSY] = rb_const_get(rb_mErrno, rb_intern("ETXTBSY"));
    spawn_error[G_SPAWN_ERROR_IO]     = rb_const_get(rb_mErrno, rb_intern("EIO"));
    spawn_error[G_SPAWN_ERROR_NFILE]  = rb_const_get(rb_mErrno, rb_intern("ENFILE"));
    spawn_error[G_SPAWN_ERROR_MFILE]  = rb_const_get(rb_mErrno, rb_intern("EMFILE"));
    spawn_error[G_SPAWN_ERROR_INVAL]  = rb_const_get(rb_mErrno, rb_intern("EINVAL"));
    spawn_error[G_SPAWN_ERROR_ISDIR]  = rb_const_get(rb_mErrno, rb_intern("EISDIR"));
    spawn_error[G_SPAWN_ERROR_LIBBAD] = rb_const_get(rb_mErrno, rb_intern("ELIBBAD"));

}
