
/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkmain.c -

  $Author: igapy $
  $Date: 2002/12/07 14:11:52 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/
#include "global.h"

EXTERN VALUE rb_progname, rb_argv;

static VALUE rbgtk_main_threads = Qnil;
static VALUE rbgtk_quit_threads = Qnil;

static VALUE
gtk_m_init(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    gint i, gargc;
    VALUE argary;
    gchar** gargv;

    rb_scan_args(argc, argv, "01", &argary);

    if (NIL_P(argary)){
        gargc = RARRAY(rb_argv)->len;
        argary = rb_argv;
    } else {
        Check_Type(argary, T_ARRAY);
        gargc = RARRAY(argary)->len;
    }

    gargv = ALLOCA_N(char*,gargc + 1);
    gargv[0] = RVAL2CSTR(rb_progname);

    for (i = 0; i < gargc; i++) {
        if (TYPE(RARRAY(argary)->ptr[i]) == T_STRING) {
            gargv[i+1] = RVAL2CSTR(RARRAY(argary)->ptr[i]);
        }
        else {
            gargv[i+1] = "";
        }
    }
    gargc++;
    gtk_init(&gargc, &gargv);
    return self;
}

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
    rb_ary_push(rbgtk_main_threads, rb_thread_current());
    gtk_main();
    if (!NIL_P(rbgtk_quit_threads) &&
	rbgtk_quit_threads != rb_thread_current()) {
	rb_funcall(rbgtk_quit_threads, rb_intern("join"), 0);
	rbgtk_quit_threads = Qnil;
    }
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
    if (!NIL_P(rbgtk_quit_threads)) return Qnil;
    gtk_main_quit();
    rbgtk_quit_threads = rb_thread_current();
    rb_thread_wakeup(rb_ary_pop(rbgtk_main_threads));
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
rbgtk_m_grab_add(self, widget)
    VALUE self, widget;
{
    gtk_grab_add(GTK_WIDGET(RVAL2GOBJ(widget)));
    return self;
}

static VALUE
rbgtk_m_grab_remove(self, widget)
    VALUE self, widget;
{
    gtk_grab_remove(GTK_WIDGET(RVAL2GOBJ(widget)));
    return self;
}

void 
Init_gtk_main()
{
    rb_define_module_function(mGtk, "events_pending?", gtk_m_events_pending, 0);
    rb_define_module_function(mGtk, "init", gtk_m_init, -1);
    rb_global_variable(&rbgtk_main_threads);
    rbgtk_main_threads = rb_ary_new();
    rb_global_variable(&rbgtk_quit_threads);
    rb_define_module_function(mGtk, "main", gtk_m_main, 0);
    rb_define_module_function(mGtk, "main_level", gtk_m_main_level, 0);
    rb_define_module_function(mGtk, "main_quit", gtk_m_main_quit, 0);
    rb_define_module_function(mGtk, "main_iteration", gtk_m_main_iteration, 0);
    rb_define_module_function(mGtk, "timeout_add", timeout_add, 1);
    rb_define_module_function(mGtk, "timeout_remove", timeout_remove, 1);
    rb_define_module_function(mGtk, "idle_add", idle_add, 0);
    rb_define_module_function(mGtk, "idle_remove", idle_remove, 1);
    rb_define_module_function(mGtk, "current_event", gtk_m_get_current_event, 0);
    rb_define_module_function(mGtk, "grab_add", rbgtk_m_grab_add, 1);
    rb_define_module_function(mGtk, "grab_remove", rbgtk_m_grab_remove, 1);
}
