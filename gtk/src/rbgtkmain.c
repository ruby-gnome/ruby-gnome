/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkmain.c -

  $Author: mutoh $
  $Date: 2002/10/08 18:53:21 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/
#include "global.h"

static VALUE
gtk_m_events_pending(self)
    VALUE self;
{
    return gtk_events_pending() ? Qtrue : Qfalse;
}

static VALUE
gtk_m_main(self)
    VALUE self;
{
    gtk_main();
    return Qnil;
}

static VALUE
gtk_m_main_level(self)
    VALUE self;
{
    return INT2FIX(gtk_main_level());
}

static VALUE
gtk_m_main_quit(self)
    VALUE self;
{
    gtk_main_quit();
    return Qnil;
}

static VALUE
gtk_m_main_iteration(self)
    VALUE self;
{
    return gtk_main_iteration() ? Qtrue : Qfalse;
}

static gboolean
exec_interval(proc)
    VALUE proc;
{
    VALUE ret = rb_funcall(proc, id_call, 0);
    return RTEST(ret);
}

static VALUE
timeout_add(self, interval)
    VALUE self, interval;
{
    VALUE id;
    VALUE func;

    func = rb_f_lambda();
    id = INT2FIX(gtk_timeout_add(NUM2INT(interval),
                                 (GtkFunction)exec_interval,
                                 (gpointer)func));
    G_RELATIVE2(self, func, id_relative_callbacks, id);
    return id;
}

static VALUE
timeout_remove(self, id)
    VALUE self, id;
{
    gtk_timeout_remove(NUM2INT(id));
    G_REMOVE_RELATIVE(self, id_relative_callbacks, id);
    return Qnil;
}

static void
exec_input(data, source, condition)
    gpointer data;
    gint source;
    GdkInputCondition condition;
{
    rb_funcall((VALUE)data, id_call, 0);
}

static VALUE
input_add(self, filedescriptor, gdk_input_condition)
    VALUE self, filedescriptor, gdk_input_condition;
{
    VALUE id;
    VALUE func;

    func = rb_f_lambda();
    id = INT2FIX(gdk_input_add(NUM2INT(rb_funcall(filedescriptor,
                                                  rb_intern("to_i"), 0)),
                               (GdkInputCondition)NUM2INT(gdk_input_condition),
                               (GdkInputFunction)exec_input,
                               (gpointer)func));
    G_RELATIVE2(self, func, id_relative_callbacks, id);
    return id;
}

static VALUE
input_remove(self, id)
    VALUE self, id;
{
    gdk_input_remove(NUM2INT(id));
    G_REMOVE_RELATIVE(self, id_relative_callbacks, id);
    return Qnil;
}

static VALUE
idle_add(self)
    VALUE self;
{
    VALUE id;
    VALUE func;

    func = rb_f_lambda();
    id = INT2FIX(gtk_idle_add((GtkFunction)exec_interval, (gpointer)func));
    G_RELATIVE2(self, func, id_relative_callbacks, id);
    return id;
}

static VALUE
idle_remove(self, id)
    VALUE self, id;
{
    gtk_idle_remove(NUM2INT(id));
    G_REMOVE_RELATIVE(self, id_relative_callbacks, id);
    return Qnil;
}

static VALUE
gtk_m_get_current_event(self)
    VALUE self;
{
    return GEV2RVAL(gtk_get_current_event());
}

static VALUE
gtk_m_signal_lookup(self, name, type)
    VALUE self, name, type;
{
    return INT2NUM(gtk_signal_lookup(RVAL2CSTR(name), NUM2INT(type)));
}

static VALUE
gtk_m_signal_name(self, signal_id)
    VALUE self, signal_id;
{
    const gchar* name = gtk_signal_name(NUM2INT(signal_id));
    return name ? CSTR2RVAL(name) : Qnil;
}

VALUE rbgtk_log_handler_procs = Qnil;

static void
rbgtk_log_handler(log_domain, log_level, message, user_data)
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
rbgtk_m_log_set_handler(self, log_domain, log_levels)
    VALUE self, log_domain, log_levels;
{
    guint handler_id;
    VALUE proc;

    proc = rb_f_lambda();
    handler_id = g_log_set_handler(RVAL2CSTR(log_domain), NUM2INT(log_levels),
				   rbgtk_log_handler, (gpointer)proc);
    rb_hash_aset(rbgtk_log_handler_procs, INT2NUM(handler_id), proc);
    return INT2NUM(handler_id);
}

static VALUE
rbgtk_m_log_remove_handler(self, log_domain, handler_id)
    VALUE self, log_domain, handler_id;
{
    g_log_remove_handler(RVAL2CSTR(log_domain), NUM2INT(handler_id));
    rb_funcall(rbgtk_log_handler_procs, rb_intern("delete"),
	       1, INT2NUM(handler_id));
    return Qnil;
}

void 
Init_gtk_main()
{
    rb_define_module_function(mGtk, "events_pending", gtk_m_events_pending, 0);
    rb_define_module_function(mGtk, "main", gtk_m_main, 0);
    rb_define_module_function(mGtk, "main_level", gtk_m_main_level, 0);
    rb_define_module_function(mGtk, "main_quit", gtk_m_main_quit, 0);
    rb_define_module_function(mGtk, "main_iteration", gtk_m_main_iteration, 0);
    rb_define_module_function(mGtk, "timeout_add", timeout_add, 1);
    rb_define_module_function(mGtk, "timeout_remove", timeout_remove, 1);
    rb_define_module_function(mGtk, "input_add", input_add, 2);
    rb_define_module_function(mGtk, "input_remove", input_remove, 1);
    rb_define_module_function(mGtk, "idle_add", idle_add, 0);
    rb_define_module_function(mGtk, "idle_remove", idle_remove, 1);
    rb_define_module_function(mGtk, "get_current_event", gtk_m_get_current_event, 0);
    rb_define_module_function(mGtk, "signal_lookup", gtk_m_signal_lookup, 2);
    rb_define_module_function(mGtk, "signal_name", gtk_m_signal_name, 1);
    rb_global_variable(&rbgtk_log_handler_procs);
    rbgtk_log_handler_procs = rb_hash_new();
    rb_define_module_function(mGtk, "log_set_handler", rbgtk_m_log_set_handler, 2);
    rb_define_module_function(mGtk, "log_remove_handler", rbgtk_m_log_remove_handler, 2);
}
