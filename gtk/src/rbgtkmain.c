
/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkmain.c -

  $Author: mutoh $
  $Date: 2003/05/24 11:32:12 $

  Copyright (C) 2002,2003 Ruby-GNOME2 Project Team
  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/
#include "global.h"

EXTERN VALUE rb_progname, rb_argv;

static VALUE rbgtk_main_threads = Qnil;

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

    {
        gboolean is_initialized;

        /* Gdk modifies sighandlers, sigh */
#ifdef NT
        RETSIGTYPE (*sigfunc[3])();
#else
        RETSIGTYPE (*sigfunc[7])();
#endif

#ifdef NT
        sigfunc[0] = signal(SIGINT, SIG_IGN);
        sigfunc[1] = signal(SIGSEGV, SIG_IGN);
        sigfunc[2] = signal(SIGTERM, SIG_IGN);
#else
        sigfunc[0] = signal(SIGHUP, SIG_IGN);
        sigfunc[1] = signal(SIGINT, SIG_IGN);
        sigfunc[2] = signal(SIGQUIT, SIG_IGN);
        sigfunc[3] = signal(SIGBUS, SIG_IGN);
        sigfunc[4] = signal(SIGSEGV, SIG_IGN);
        sigfunc[5] = signal(SIGPIPE, SIG_IGN);
        sigfunc[6] = signal(SIGTERM, SIG_IGN);
#endif

        is_initialized = gtk_init_check(&gargc, &gargv);

#ifdef NT
        signal(SIGINT,  sigfunc[0]);
        signal(SIGSEGV, sigfunc[1]);
        signal(SIGTERM, sigfunc[2]);
#else
        signal(SIGHUP,  sigfunc[0]);
        signal(SIGINT,  sigfunc[1]);
        signal(SIGQUIT, sigfunc[2]);
        signal(SIGBUS,  sigfunc[3]);
        signal(SIGSEGV, sigfunc[4]);
        signal(SIGPIPE, sigfunc[5]);
        signal(SIGTERM, sigfunc[6]);
#endif
        
        if (!is_initialized)
            rb_raise(rb_eRuntimeError, "failed to initialize gtk+");
    }

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
    VALUE thread = rb_ary_pop(rbgtk_main_threads);
    gtk_main_quit();
    if (NIL_P(thread)){
        rb_warning("Gtk.main_quit was called incorrectly.");
    } else {
        rb_thread_wakeup(thread);
    }
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
    return Qnil;
}

static VALUE
rbgtk_m_grab_remove(self, widget)
    VALUE self, widget;
{
    gtk_grab_remove(GTK_WIDGET(RVAL2GOBJ(widget)));
    return Qnil;
}

/* From Version Information */
static VALUE
rbgtk_m_check_version(self, major, minor, micro)
    VALUE self, major, minor, micro;
{
    gchar * ret = NULL;
    ret = gtk_check_version(FIX2INT(major),
                            FIX2INT(minor), FIX2INT(micro));
    return ret ? CSTR2RVAL(ret) : Qnil;
}

void 
Init_gtk_main()
{
    rb_define_module_function(mGtk, "events_pending?", gtk_m_events_pending, 0);
    rb_define_module_function(mGtk, "init", gtk_m_init, -1);
    rb_global_variable(&rbgtk_main_threads);
    rbgtk_main_threads = rb_ary_new();
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
    rb_define_module_function(mGtk, "check_version", rbgtk_m_check_version, 3);
 
}
