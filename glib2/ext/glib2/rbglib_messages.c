/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2002-2005 Masao Mutoh
 *  Copyright (C) 1998-2000 Yukihiro Matsumoto,
 *                          Daisuke Kanda,
 *                          Hiroshi Igarashi
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

#ifndef HAVE_RB_SOURCEFILE
#define rb_sourcefile() (ruby_sourcefile)
#endif

#ifndef HAVE_RB_SOURCELINE
#define rb_sourceline() (ruby_sourceline)
#endif

static VALUE rbglib_log_handler_procs;
static ID id_call;
static gboolean log_canceled;

static const gchar *
logmessage(GLogLevelFlags level)
{
    if (level & G_LOG_LEVEL_ERROR){
        return "ERROR";
    } else if (level & G_LOG_LEVEL_CRITICAL){
        return "CRITICAL";
    } else if (level & G_LOG_LEVEL_WARNING){
        return "WARNING";
    } else if (level & G_LOG_LEVEL_MESSAGE){
        return "MESSAGE";
    } else if (level & G_LOG_LEVEL_INFO){
        return "INFO";
    } else if (level & G_LOG_LEVEL_DEBUG){
        return "DEBUG";
    }
    return "UNKNOWN";
}

static void
rbglib_log_handler(const gchar *log_domain, GLogLevelFlags log_level, const gchar *message, gpointer user_data)
{
    if (! log_canceled){
#ifdef HAVE_RUBY_SET_CURRENT_SOURCE
        ruby_set_current_source();
#endif
        g_printerr("%s: line %d\n", rb_sourcefile(), rb_sourceline());
        g_printerr("   %s-%s **:%s\n", log_domain, logmessage(log_level), message);
    } else {
        g_log_default_handler(log_domain, log_level, message, user_data);
    }
}

/* Use Internal only */
static VALUE
rbglib_m_log_cancel_handler(G_GNUC_UNUSED VALUE self)
{
    log_canceled = TRUE;
    return Qnil;
}

static VALUE
rbglib_m_log_set_handler(VALUE self, VALUE domain, VALUE levels)
{
    guint handler_id = g_log_set_handler(NIL_P(domain) ? NULL : RVAL2CSTR(domain),
                                         NUM2INT(levels),
                                         (GLogFunc)rbglib_log_handler, (gpointer)self);
    return UINT2NUM(handler_id);
}

static VALUE
rbglib_m_log_remove_handler(VALUE self, VALUE domain, VALUE handler_id)
{
    g_log_remove_handler(NIL_P(domain) ? NULL : RVAL2CSTR(domain),
                         NUM2UINT(handler_id));
    G_REMOVE_RELATIVE(self, handler_id, rbglib_log_handler_procs);
    return Qnil;
}

static VALUE
rbglib_m_log_set_always_fatal(G_GNUC_UNUSED VALUE self, VALUE fatal_mask)
{
    return INT2NUM(g_log_set_always_fatal(NUM2INT(fatal_mask)));
}

static VALUE
rbglib_m_log_set_fatal_mask(G_GNUC_UNUSED VALUE self, VALUE domain, VALUE fatal_mask)
{
    return INT2NUM(g_log_set_fatal_mask(NIL_P(domain) ? NULL : RVAL2CSTR(domain),
                                        NUM2INT(fatal_mask)));
}

static VALUE
rbglib_m_log(G_GNUC_UNUSED VALUE self, VALUE domain, VALUE level, VALUE str)
{
    g_log(NIL_P(domain) ? NULL : RVAL2CSTR(domain), NUM2INT(level), "%s", RVAL2CSTR(str));
    return Qnil;
}

void
Init_glib_messages(void)
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
