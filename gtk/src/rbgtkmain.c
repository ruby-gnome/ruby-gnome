
/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkmain.c -

  $Author: ggc $
  $Date: 2007/07/13 16:07:31 $

  Copyright (C) 2002,2003 Ruby-GNOME2 Project Team
  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/
#include "global.h"
#include <locale.h>

EXTERN VALUE rb_progname;

static VALUE rbgtk_eGtkInitError;

static ID id__quit_callbacks__, id__timeout_callbacks__;
static ID id__idle_callbacks__, id__snooper_callbacks__;


typedef struct _callback_info_t
{
    VALUE callback;
    ID key;
    guint id;
} callback_info_t;


static VALUE
gtk_m_function_body(data)
    VALUE data;
{
    callback_info_t *info = (callback_info_t *)data;
    VALUE ret = rb_funcall(info->callback, id_call, 0);

    if (info->key && !ret)
        G_REMOVE_RELATIVE(mGtk, info->key, UINT2NUM(info->id));
    return ret;
}

static gboolean
gtk_m_function(data)
    gpointer data;
{ 
    return RVAL2CBOOL(G_PROTECT_CALLBACK(gtk_m_function_body, data));
}

static VALUE
gtk_m_function2_body(gpointer proc)
{
    rb_funcall((VALUE)proc, id_call, 0);
    return Qnil;
}

static void
gtk_m_function2(gpointer proc)
{
    G_PROTECT_CALLBACK(gtk_m_function2_body, proc);
}


static VALUE
gtk_m_set_locale(self)
    VALUE self;
{
    return CSTR2RVAL(gtk_set_locale());
}

static VALUE
gtk_m_disable_setlocale(self)
    VALUE self;
{
    gtk_disable_setlocale();
    return Qnil;
}

static VALUE
gtk_m_get_default_language(self)
    VALUE self;
{
    return BOXED2RVAL(gtk_get_default_language(), PANGO_TYPE_LANGUAGE);
}

typedef void (*SignalFunc) (int);

static gboolean _initialized = FALSE;

static VALUE
gtk_m_init(int argc, VALUE *argv, VALUE self)
{
    gint i, gargc;
    VALUE argary;
    gchar** gargv;

    if (_initialized)
        return self;
    else
        _initialized = TRUE;

    rb_scan_args(argc, argv, "01", &argary);

    if (NIL_P(argary)){
        argary = rb_const_get(rb_cObject, rb_intern("ARGV"));
        gargc = RARRAY(argary)->len;
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
        if (! is_initialized) {
            const char *display_name_arg = gdk_get_display_arg_name();
            display_name_arg = display_name_arg ? display_name_arg : g_getenv("DISPLAY");
            rb_raise(rbgtk_eGtkInitError, "Cannot open display: %s",
                     display_name_arg ? display_name_arg : " ");
        }

        setlocale(LC_NUMERIC, "C");

#ifdef NT
        signal(SIGINT,  (SignalFunc)sigfunc[0]);
        signal(SIGSEGV, (SignalFunc)sigfunc[1]);
        signal(SIGTERM, (SignalFunc)sigfunc[2]);
#else
        signal(SIGHUP,  sigfunc[0]);
        signal(SIGINT,  sigfunc[1]);
        signal(SIGQUIT, sigfunc[2]);
        signal(SIGBUS,  sigfunc[3]);
        signal(SIGSEGV, sigfunc[4]);
        signal(SIGPIPE, sigfunc[5]);
        signal(SIGTERM, sigfunc[6]);
#endif
    }

    return self;
}

/* We don't need them.
gtk_init()
gtk_exit()
*/

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
    return CBOOL2RVAL(gtk_main_iteration());
}

static VALUE
gtk_m_main_iteration_do(self, blocking)
    VALUE self, blocking;
{
    return CBOOL2RVAL(gtk_main_iteration_do(RVAL2CBOOL(blocking)));
}

static VALUE
gtk_m_main_do_event(self, event)
    VALUE self, event;
{
    gtk_main_do_event(RVAL2GEV(event));
    return event;
}

/* We don't need them.
gtk_true()
gtk_false()
*/

static VALUE
gtk_m_grab_add(self, widget)
    VALUE self, widget;
{
    gtk_grab_add(GTK_WIDGET(RVAL2GOBJ(widget)));
    return Qnil;
}

static VALUE
gtk_m_get_current(self)
    VALUE self;
{
    return GOBJ2RVAL(gtk_grab_get_current());
}

static VALUE
gtk_m_grab_remove(self, widget)
    VALUE self, widget;
{
    gtk_grab_remove(GTK_WIDGET(RVAL2GOBJ(widget)));
    return Qnil;
}

static VALUE
gtk_m_init_add(self)
    VALUE self;
{
    volatile VALUE func = rb_block_proc();

    gtk_init_add((GtkFunction)gtk_m_function2, (gpointer)func);
    G_RELATIVE(self, func);
    return Qnil;
}

static VALUE
gtk_m_quit_add(self, main_level)
    VALUE self, main_level;
{
    volatile VALUE func = rb_block_proc();
    VALUE rb_id;
    callback_info_t *info;
    guint id;

    info = ALLOC(callback_info_t);
    info->callback = func;
    info->key = id_relative_callbacks;
    id = gtk_quit_add_full(NUM2UINT(main_level), (GtkFunction)gtk_m_function,
                           NULL, (gpointer)info, g_free);
    info->id = id;
    rb_id = UINT2NUM(id);
    G_RELATIVE2(self, func, id__quit_callbacks__, rb_id);
    return rb_id;
}

static VALUE
gtk_m_quit_remove(self, quit_handler_id)
    VALUE self, quit_handler_id;
{
    gtk_quit_remove(NUM2UINT(quit_handler_id));
    G_REMOVE_RELATIVE(self, id__quit_callbacks__, quit_handler_id);
    return quit_handler_id;
}

/* We don't need this.
gtk_quit_add_full ()
gtk_quit_add_destroy()
gtk_quit_remove_by_data()
gtk_timeout_add_full()
*/

static VALUE
timeout_add(self, interval)
    VALUE self, interval;
{
    VALUE func, rb_id;
    callback_info_t *info;
    guint id;

    func = rb_block_proc();
    info = ALLOC(callback_info_t);
    info->callback = func;
    info->key = id__timeout_callbacks__;
    id = gtk_timeout_add_full(NUM2UINT(interval), (GtkFunction)gtk_m_function,
                              NULL, (gpointer)info, g_free);
    info->id = id;
    rb_id = UINT2NUM(id);
    G_RELATIVE2(self, func, id__timeout_callbacks__, rb_id);
    return rb_id;
}

static VALUE
timeout_remove(self, id)
    VALUE self, id;
{
    gtk_timeout_remove(NUM2UINT(id));
    G_REMOVE_RELATIVE(self, id__timeout_callbacks__, id);
    return Qnil;
}

static VALUE
idle_add(self)
    VALUE self;
{
    VALUE func, rb_id;
    callback_info_t *info;
    guint id;

    func = rb_block_proc();
    info = ALLOC(callback_info_t);
    info->callback = func;
    info->key = id__idle_callbacks__;
    id = gtk_idle_add_full(G_PRIORITY_DEFAULT_IDLE,
                           (GtkFunction)gtk_m_function, NULL,
                           (gpointer)info, g_free);
    info->id = id;
    rb_id = UINT2NUM(id);
    G_RELATIVE2(self, func, id__idle_callbacks__, rb_id);
    return rb_id;
}

static VALUE
idle_add_priority(self, priority)
    VALUE self;
{
    VALUE func, rb_id;
    callback_info_t *info;
    guint id;

    func = rb_block_proc();
    info = ALLOC(callback_info_t);
    info->callback = func;
    info->key = id__idle_callbacks__;
    id = gtk_idle_add_full(NUM2INT(priority), (GtkFunction)gtk_m_function,
                           NULL, (gpointer)info, g_free);
    info->id = id;
    rb_id = UINT2NUM(id);
    G_RELATIVE2(self, func, id__idle_callbacks__, rb_id);
    return rb_id;
}

static VALUE
idle_remove(self, id)
    VALUE self, id;
{
    gtk_idle_remove(NUM2UINT(id));
    G_REMOVE_RELATIVE(self, id__idle_callbacks__, id);
    return Qnil;
}

/* We don't need this.
gtk_idle_remove_by_data()
gtk_idle_add_full()

Use Gdk::Input.add, remove
gtk_input_add_full()
gtk_input_remove()
*/

static gint
gtk_m_key_snoop_func(grab_widget, event, func)
    GtkWidget* grab_widget;
    GdkEventKey* event;
    gpointer func;
{
    VALUE ret = rb_funcall((VALUE)func, id_call, 2, 
                           GOBJ2RVAL(grab_widget), 
                           GEV2RVAL((GdkEvent*)event));
    return RVAL2CBOOL(ret);
}

static VALUE
gtk_m_key_snooper_install(self)
    VALUE self;
{
    VALUE func = rb_block_proc();
    VALUE id = INT2FIX(gtk_key_snooper_install(
                           (GtkKeySnoopFunc)gtk_m_key_snoop_func, 
                           (gpointer)func));
    G_RELATIVE2(self, func, id__snooper_callbacks__, id);
    return id;
}

static VALUE
gtk_m_key_snooper_remove(self, id)
    VALUE self, id;
{
    gtk_key_snooper_remove(NUM2UINT(id));
    G_REMOVE_RELATIVE(self, id__snooper_callbacks__, id);
    return Qnil;
}

static VALUE
gtk_m_get_current_event(self)
    VALUE self;
{
    return GEV2RVAL(gtk_get_current_event());
}

static VALUE
gtk_m_get_current_event_time(self)
    VALUE self;
{
    return INT2NUM(gtk_get_current_event_time());
}

static VALUE
gtk_m_get_current_event_state(self)
    VALUE self;
{
    GdkModifierType state;
    gboolean ret = gtk_get_current_event_state(&state);
    return ret ? GFLAGS2RVAL(state, GDK_TYPE_MODIFIER_TYPE) : Qnil;
}

static VALUE
gtk_m_get_event_widget(argc, argv, self)
    int argc;
    VALUE* argv;
    VALUE self;
{
    VALUE event;
    rb_scan_args(argc, argv, "01", &event);

    return GOBJ2RVAL(gtk_get_event_widget(NIL_P(event) ? NULL :RVAL2GEV(event)));
}

static VALUE
gtk_m_propagate_event(self, widget, event)
    VALUE self, widget, event;
{
    gtk_propagate_event(GTK_WIDGET(RVAL2GOBJ(widget)), RVAL2GEV(event));
    return Qnil;
}

/* From Version Information */
static VALUE
gtk_m_check_version(self, major, minor, micro)
    VALUE self, major, minor, micro;
{
    const gchar *ret;
    ret = gtk_check_version(FIX2INT(major), FIX2INT(minor), FIX2INT(micro));
    return ret ? CSTR2RVAL(ret) : Qnil;
}

static VALUE
gtk_m_check_version_q(self, major, minor, micro)
    VALUE self, major, minor, micro;
{
    const gchar *ret;
    ret = gtk_check_version(FIX2INT(major), FIX2INT(minor), FIX2INT(micro));
    return CBOOL2RVAL(ret == NULL);
}


void 
Init_gtk_main()
{
    id__quit_callbacks__ = rb_intern("__quit_callbacks__");
    id__timeout_callbacks__ = rb_intern("__timeout_callbacks__");
    id__idle_callbacks__ = rb_intern("__idle_callbacks__");
    id__snooper_callbacks__ = rb_intern("__snooper_callbacks__");

    rbgtk_eGtkInitError = rb_define_class_under(mGtk, "InitError",
                                                rb_eRuntimeError);

    rb_define_module_function(mGtk, "set_locale", gtk_m_set_locale, 0);
    rb_define_module_function(mGtk, "disable_setlocale", gtk_m_disable_setlocale, 0);
    rb_define_module_function(mGtk, "default_language", gtk_m_get_default_language, 0);
    rb_define_module_function(mGtk, "init", gtk_m_init, -1);
    rb_define_module_function(mGtk, "main", gtk_m_main, 0);
    rb_define_module_function(mGtk, "main_level", gtk_m_main_level, 0);
    rb_define_module_function(mGtk, "main_quit", gtk_m_main_quit, 0);
    rb_define_module_function(mGtk, "main_iteration", gtk_m_main_iteration, 0);
    rb_define_module_function(mGtk, "main_iteration_do", gtk_m_main_iteration_do, 1);
    rb_define_module_function(mGtk, "main_do_event", gtk_m_main_do_event, 1);
    rb_define_module_function(mGtk, "grab_add", gtk_m_grab_add, 1);
    rb_define_module_function(mGtk, "current", gtk_m_get_current, 0);
    rb_define_module_function(mGtk, "grab_remove", gtk_m_grab_remove, 1);
    rb_define_module_function(mGtk, "init_add", gtk_m_init_add, 0);
    rb_define_module_function(mGtk, "quit_add", gtk_m_quit_add, 1);
    rb_define_module_function(mGtk, "quit_remove", gtk_m_quit_remove, 1);

    rb_define_module_function(mGtk, "timeout_add", timeout_add, 1);
    rb_define_module_function(mGtk, "timeout_remove", timeout_remove, 1);
    rb_define_module_function(mGtk, "idle_add", idle_add, 0);
    rb_define_module_function(mGtk, "idle_add_priority", idle_add_priority, 1);
    rb_define_module_function(mGtk, "idle_remove", idle_remove, 1);
    rb_define_module_function(mGtk, "key_snooper_install", gtk_m_key_snooper_install, 0);
    rb_define_module_function(mGtk, "key_snooper_remove", gtk_m_key_snooper_remove, 1);
    rb_define_module_function(mGtk, "current_event", gtk_m_get_current_event, 0);
    rb_define_module_function(mGtk, "current_event_time", gtk_m_get_current_event_time, 0);
    rb_define_module_function(mGtk, "current_event_state", gtk_m_get_current_event_state, 0);
    rb_define_module_function(mGtk, "get_event_widget", gtk_m_get_event_widget, -1);
    rb_define_module_function(mGtk, "propagate_event", gtk_m_propagate_event, 2);
    rb_define_module_function(mGtk, "check_version", gtk_m_check_version, 3);
    rb_define_module_function(mGtk, "check_version?", gtk_m_check_version_q, 3);

    rb_define_const(mGtk, "PRIORITY_RESIZE", INT2FIX(GTK_PRIORITY_RESIZE));

}
