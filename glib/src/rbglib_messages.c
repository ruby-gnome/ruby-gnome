/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbglib_messages.c -

  $Author: mutoh $
  $Date: 2005/01/09 07:20:52 $

  Copyright (C) 2002-2005 Masao Mutoh

  This file is devided from rbgtkmain.c.
  rbgtkmain.c -
  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

static VALUE rbglib_log_handler_procs;
static ID id_call;
static gboolean log_canceled;

#define _RAISE(domain, id, message) \
    rb_raise(rb_eRuntimeError, "%s-%s **:%s", domain, id, message)

static void
rbglib_log_handler(log_domain, log_level, message, user_data)
    const gchar *log_domain;
    GLogLevelFlags log_level;
    const gchar *message;
    gpointer user_data;
{
    if (! log_canceled){
        if (log_level & G_LOG_LEVEL_ERROR){
            _RAISE(log_domain, "ERROR", message);
        } else if (log_level & G_LOG_LEVEL_CRITICAL){
            _RAISE(log_domain, "CRITICAL", message);
        } else if (RTEST(ruby_debug)){
            if (log_level & G_LOG_LEVEL_WARNING){
                _RAISE(log_domain, "WARNING", message);
            } else if (log_level & G_LOG_LEVEL_MESSAGE){
                _RAISE(log_domain, "MESSAGE", message);
            } else if (log_level & G_LOG_LEVEL_INFO){
            _RAISE(log_domain, "INFO", message);
            } else if (log_level & G_LOG_LEVEL_DEBUG){
                _RAISE(log_domain, "DEBUG", message);
            }
        }
    } else if (RTEST(ruby_verbose)){
        g_log_default_handler(log_domain, log_level, message, user_data);
    } else {
        if (log_level & G_LOG_LEVEL_ERROR){
            g_log_default_handler(log_domain, log_level, message, user_data);
        } else if (log_level & G_LOG_LEVEL_CRITICAL){
            g_log_default_handler(log_domain, log_level, message, user_data);
        } else {
            /* Ignored */
        }
    }
}

/* Use Internal only */
static VALUE
rbglib_m_log_cancel_handler(self)
    VALUE self;
{
    log_canceled = TRUE;
    return Qnil;
}

static VALUE
rbglib_m_log_set_handler(self, domain, levels)
    VALUE self, domain, levels;
{
    guint handler_id = g_log_set_handler(NIL_P(domain) ? NULL : RVAL2CSTR(domain),
                                         NUM2INT(levels),
                                         rbglib_log_handler, (gpointer)self);
    return UINT2NUM(handler_id);
}

static VALUE
rbglib_m_log_remove_handler(self, domain, handler_id)
    VALUE self, domain, handler_id;
{
    g_log_remove_handler(NIL_P(domain) ? NULL : RVAL2CSTR(domain),
                         NUM2UINT(handler_id));
    G_REMOVE_RELATIVE(self, handler_id, rbglib_log_handler_procs);
    return Qnil;
}

static VALUE
rbglib_m_log_set_always_fatal(self, fatal_mask)
    VALUE self, fatal_mask;
{
    return INT2NUM(g_log_set_always_fatal(NUM2INT(fatal_mask)));
}

static VALUE
rbglib_m_log_set_fatal_mask(self, domain, fatal_mask)
    VALUE self, domain, fatal_mask;
{
    return INT2NUM(g_log_set_fatal_mask(NIL_P(domain) ? NULL : RVAL2CSTR(domain),
                                        NUM2INT(fatal_mask)));
}

static VALUE
rbglib_m_log(self, domain, level, str)
    VALUE self, domain, level, str;
{
    g_log(NIL_P(domain) ? NULL : RVAL2CSTR(domain), NUM2INT(level), RVAL2CSTR(str), NULL);
    return Qnil;
}

void
Init_glib_messages()
{
    VALUE mGLog = rb_define_module_under(mGLib, "Log");

    id_call = rb_intern("call");
    log_canceled = FALSE;

    rb_global_variable(&rbglib_log_handler_procs);
    rbglib_log_handler_procs = rb_hash_new();
    rb_define_module_function(mGLog, "set_handler", rbglib_m_log_set_handler, 2);
    rb_define_module_function(mGLog, "remove_handler", rbglib_m_log_remove_handler, 2);
    rb_define_module_function(mGLog, "cancel_handler", rbglib_m_log_cancel_handler, 0);
    rb_define_module_function(mGLog, "set_always_fatal", rbglib_m_log_set_always_fatal, 1);
    rb_define_module_function(mGLog, "set_fatal_mask", rbglib_m_log_set_fatal_mask, 2);
    rb_define_module_function(mGLog, "log", rbglib_m_log, 3);

    rb_define_const(mGLog, "FATAL_MASK", INT2NUM(G_LOG_FATAL_MASK));
    rb_define_const(mGLog, "LEVEL_USER_SHIFT", INT2NUM(G_LOG_LEVEL_USER_SHIFT));

    /* GLogLevelFlags */
    rb_define_const(mGLog, "FLAG_RECURSION", INT2NUM(G_LOG_FLAG_RECURSION));
    rb_define_const(mGLog, "FLAG_FATAL", INT2NUM(G_LOG_FLAG_FATAL));
    rb_define_const(mGLog, "LEVEL_ERROR", INT2NUM(G_LOG_LEVEL_ERROR));
    rb_define_const(mGLog, "LEVEL_CRITICAL", INT2NUM(G_LOG_LEVEL_CRITICAL));
    rb_define_const(mGLog, "LEVEL_WARNING", INT2NUM(G_LOG_LEVEL_WARNING));
    rb_define_const(mGLog, "LEVEL_MESSAGE", INT2NUM(G_LOG_LEVEL_MESSAGE));
    rb_define_const(mGLog, "LEVEL_INFO", INT2NUM(G_LOG_LEVEL_INFO));
    rb_define_const(mGLog, "LEVEL_DEBUG", INT2NUM(G_LOG_LEVEL_DEBUG));
    rb_define_const(mGLog, "LEVEL_MASK", INT2NUM(G_LOG_LEVEL_MASK));
}
