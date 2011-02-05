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
    gint argc, i;
    gchar** argv;
    GError* err = NULL;
    VALUE ary;

    gboolean ret = g_shell_parse_argv(RVAL2CSTR(command_line),
                                      &argc, &argv, &err);

    if (! ret) RAISE_GERROR(err);

    ary = rb_ary_new();
    for (i = 0; i < argc; i++) {
        rb_ary_push(ary, CSTR2RVAL(argv[i]));
    }
    g_strfreev (argv);
    return ary;
}

static VALUE
shell_quote(VALUE self, VALUE unquoted_string)
{
    return CSTR2RVAL_FREE(g_shell_quote((const gchar*)RVAL2CSTR(unquoted_string)));
}

static VALUE
shell_unquote(VALUE self, VALUE quoted_string)
{
    GError* err = NULL;
    gchar* str = g_shell_unquote((const gchar*)RVAL2CSTR(quoted_string), &err);

    if (! str) RAISE_GERROR(err);

    return CSTR2RVAL_FREE(str);
}

void
Init_glib_shell()
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
