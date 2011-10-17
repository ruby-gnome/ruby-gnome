/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbglib_shell.c -

  $Author: mutoh $
  $Date: 2005/10/14 19:10:08 $

  Copyright (C) 2005 Masao Mutoh
************************************************/

#include "rbgprivate.h"

static VALUE
shell_parse(VALUE self, VALUE command_line)
{
    gint argc;
    gchar **argv;
    GError *error = NULL;

    if (!g_shell_parse_argv(RVAL2CSTR(command_line), &argc, &argv, &error))
        RAISE_GERROR(error);

    return STRV2RVAL_FREE(argv);
}

static VALUE
shell_quote(VALUE self, VALUE unquoted_string)
{
    return CSTR2RVAL_FREE(g_shell_quote(RVAL2CSTR(unquoted_string)));
}

static VALUE
shell_unquote(VALUE self, VALUE quoted_string)
{
    GError *error = NULL;
    gchar *str = g_shell_unquote(RVAL2CSTR(quoted_string), &error);
    if (str == NULL)
        RAISE_GERROR(error);

    return CSTR2RVAL_FREE(str);
}

void
Init_glib_shell(void)
{
    VALUE mShell = rb_define_module_under(mGLib, "Shell");
    VALUE cShellError = G_DEF_ERROR2(G_SHELL_ERROR, "ShellError", mGLib, rb_eRuntimeError);

    rb_define_module_function(mShell, "parse", shell_parse, 1);
    rb_define_module_function(mShell, "quote", shell_quote, 1);
    rb_define_module_function(mShell, "unquote", shell_unquote, 1);

    rb_define_const(cShellError, "BAD_QUOTING", INT2FIX(G_SHELL_ERROR_BAD_QUOTING));
    rb_define_const(cShellError, "EMPTY_STRING", INT2FIX(G_SHELL_ERROR_EMPTY_STRING));
    rb_define_const(cShellError, "FAILED", INT2FIX(G_SHELL_ERROR_FAILED));
}
