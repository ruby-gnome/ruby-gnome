/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbglib_messages.c -

  $Author: mutoh $
  $Date: 2002/11/09 13:25:31 $

  Copyright (C) 2002 Masao Mutoh

  This file is devided from rbgtkmain.c.
  rbgtkmain.c -
  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

VALUE rbglib_log_handler_procs = Qnil;
static ID id_call;

static void
rbglib_log_handler(log_domain, log_level, message, user_data)
    const gchar *log_domain;
    GLogLevelFlags log_level;
    const gchar *message;
    gpointer user_data;
{
    rb_funcall((VALUE)user_data, id_call, 3,
	       rb_str_new2(log_domain), INT2NUM(log_level),
	       rb_str_new2(message));
}

static VALUE
rbglib_m_log_set_handler(self, log_domain, log_levels)
    VALUE self, log_domain, log_levels;
{
    guint handler_id;
    VALUE proc;

    proc = rb_f_lambda();
    handler_id = g_log_set_handler(RVAL2CSTR(log_domain), NUM2INT(log_levels),
				   rbglib_log_handler, (gpointer)proc);
    rb_hash_aset(rbglib_log_handler_procs, INT2NUM(handler_id), proc);
    return INT2NUM(handler_id);
}

static VALUE
rbglib_m_log_remove_handler(self, log_domain, handler_id)
    VALUE self, log_domain, handler_id;
{
    g_log_remove_handler(RVAL2CSTR(log_domain), NUM2INT(handler_id));
    rb_funcall(rbglib_log_handler_procs, rb_intern("delete"),
	       1, INT2NUM(handler_id));
    return Qnil;
}

void
Init_glib_messages()
{
    VALUE mGLog = rb_define_module_under(mGLib, "Log");

    id_call = rb_intern("call");

    rb_global_variable(&rbglib_log_handler_procs);
    rbglib_log_handler_procs = rb_hash_new();
    rb_define_module_function(mGLog, "set_handler", rbglib_m_log_set_handler, 2);
    rb_define_module_function(mGLog, "remove_handler", rbglib_m_log_remove_handler, 2);
}
