/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2002-2006 Ruby-GNOME2 Project Team
 *  Copyright (C) 1998-2001 Yukihiro Matsumoto,
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

#include "global.h"
#ifdef HAVE_SYS_TIME_H
# include <sys/time.h>
#endif
#include <locale.h>

#define RG_TARGET_NAMESPACE mGtk

VALUE RG_TARGET_NAMESPACE;

static VALUE rbgtk_eGtkInitError;

ID id_relative_callbacks;
ID id_call;
static ID id__windows__;
static ID id__quit_callbacks__, id__timeout_callbacks__;
static ID id__idle_callbacks__, id__snooper_callbacks__;

typedef struct _callback_info_t
{
    VALUE callback;
    ID key;
    guint id;
} callback_info_t;

static void
remove_from_windows(G_GNUC_UNUSED GtkWidget *window, VALUE obj)
{
    VALUE klass, windows;

    klass = rb_obj_class(obj);
    windows = rb_ivar_get(klass, id__windows__);
    if (!NIL_P(windows)) {
        rb_hash_delete(windows, obj);
    }
}

void
rbgtk_initialize_gtkobject(VALUE obj, GtkObject *gtkobj)
{
    gtkobj = g_object_ref(gtkobj);
    gtk_object_sink(gtkobj);
    G_INITIALIZE(obj, gtkobj);

    if (GTK_IS_WINDOW(gtkobj) || GTK_IS_MENU_SHELL(gtkobj)) {
        VALUE klass;
        klass = rb_obj_class(obj);
        if (rb_ivar_defined(klass, id__windows__) == Qfalse) {
            rb_ivar_set(klass, id__windows__, rb_hash_new());
        }
        rb_hash_aset(rb_ivar_get(klass, id__windows__), obj, Qnil);
        g_signal_connect_after(gtkobj, "destroy",
                               G_CALLBACK(remove_from_windows),
                               (gpointer)obj);
    }
}

void
exec_callback(GtkWidget *widget, gpointer proc)
{
    rb_funcall((VALUE)proc, id_call, 1, GOBJ2RVAL(widget));
}

/*
 * Gtk module
 */

/* We can't use rbgtk_poll() on native Win32.
   Because GPollFD doesn't have file descriptor but HANDLE. */
#define USE_POLL_FUNC

static VALUE
rg_m_events_pending_p(G_GNUC_UNUSED VALUE self)
{
   return CBOOL2RVAL(gtk_events_pending());
}

static VALUE
gtk_m_function_body(VALUE data)
{
    callback_info_t *info = (callback_info_t *)data;
    VALUE ret = rb_funcall(info->callback, id_call, 0);

    if (info->key && !ret)
        G_REMOVE_RELATIVE(RG_TARGET_NAMESPACE, info->key, UINT2NUM(info->id));
    return ret;
}

static gboolean
gtk_m_function(gpointer data)
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
rg_m_set_locale(G_GNUC_UNUSED VALUE self)
{
    return CSTR2RVAL(gtk_set_locale());
}

static VALUE
rg_m_disable_setlocale(G_GNUC_UNUSED VALUE self)
{
    gtk_disable_setlocale();
    return Qnil;
}

static VALUE
rg_m_default_language(G_GNUC_UNUSED VALUE self)
{
    return BOXED2RVAL(gtk_get_default_language(), PANGO_TYPE_LANGUAGE);
}

typedef void (*SignalFunc) (int);

static gboolean _initialized = FALSE;

static VALUE
rg_m_init(int argc, VALUE *argv, VALUE self)
{
    gint i, gargc;
    VALUE argary;
    char** gargv;
    VALUE progname;

    if (_initialized)
        return self;
    else
        _initialized = TRUE;

    rb_scan_args(argc, argv, "01", &argary);

    if (NIL_P(argary)){
        argary = rb_const_get(rb_cObject, rb_intern("ARGV"));
        gargc = RARRAY_LEN(argary);
    } else {
        Check_Type(argary, T_ARRAY);
        gargc = RARRAY_LEN(argary);
    }

    gargv = ALLOCA_N(char *, gargc + 1);
    progname = rb_gv_get("$0");
    gargv[0] = (char *)RVAL2CSTR(progname);

    for (i = 0; i < gargc; i++)
        if (TYPE(RARRAY_PTR(argary)[i]) == T_STRING)
            gargv[i+1] = (char *)RVAL2CSTR(RARRAY_PTR(argary)[i]);
        else
            gargv[i+1] = (char *)"";
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
rg_m_main(G_GNUC_UNUSED VALUE self)
{
    gtk_main();
    return Qnil;
}

static VALUE
rg_m_main_level(G_GNUC_UNUSED VALUE self)
{
    return INT2FIX(gtk_main_level());
}

static VALUE
rg_m_main_quit(G_GNUC_UNUSED VALUE self)
{
    gtk_main_quit();
    return Qnil;
}

static VALUE
rg_m_main_iteration(G_GNUC_UNUSED VALUE self)
{
    return CBOOL2RVAL(gtk_main_iteration());
}

static VALUE
rg_m_main_iteration_do(G_GNUC_UNUSED VALUE self, VALUE blocking)
{
    return CBOOL2RVAL(gtk_main_iteration_do(RVAL2CBOOL(blocking)));
}

static VALUE
rg_m_main_do_event(G_GNUC_UNUSED VALUE self, VALUE event)
{
    gtk_main_do_event(RVAL2GEV(event));
    return event;
}

/* We don't need them.
gtk_true()
gtk_false()
*/

static VALUE
rg_m_grab_add(G_GNUC_UNUSED VALUE self, VALUE widget)
{
    gtk_grab_add(GTK_WIDGET(RVAL2GOBJ(widget)));
    return Qnil;
}

static VALUE
rg_m_current(G_GNUC_UNUSED VALUE self)
{
    return GOBJ2RVAL(gtk_grab_get_current());
}

static VALUE
rg_m_grab_remove(G_GNUC_UNUSED VALUE self, VALUE widget)
{
    gtk_grab_remove(GTK_WIDGET(RVAL2GOBJ(widget)));
    return Qnil;
}

static VALUE
rg_m_init_add(VALUE self)
{
    volatile VALUE func = rb_block_proc();

    gtk_init_add((GtkFunction)gtk_m_function2, (gpointer)func);
    G_RELATIVE(self, func);
    return Qnil;
}

static VALUE
rg_m_quit_add(VALUE self, VALUE main_level)
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
rg_m_quit_remove(VALUE self, VALUE quit_handler_id)
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
rg_m_timeout_add(VALUE self, VALUE interval)
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
rg_m_timeout_remove(VALUE self, VALUE id)
{
    gtk_timeout_remove(NUM2UINT(id));
    G_REMOVE_RELATIVE(self, id__timeout_callbacks__, id);
    return Qnil;
}

static VALUE
rg_m_idle_add(VALUE self)
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
rg_m_idle_add_priority(VALUE self, VALUE priority)
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
rg_m_idle_remove(VALUE self, VALUE id)
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
gtk_m_key_snoop_func(GtkWidget *grab_widget, GdkEventKey *event, gpointer func)
{
    VALUE ret = rb_funcall((VALUE)func, id_call, 2, 
                           GOBJ2RVAL(grab_widget), 
                           GEV2RVAL((GdkEvent*)event));
    return RVAL2CBOOL(ret);
}

static VALUE
rg_m_key_snooper_install(VALUE self)
{
    VALUE func = rb_block_proc();
    VALUE id = INT2FIX(gtk_key_snooper_install(
                           (GtkKeySnoopFunc)gtk_m_key_snoop_func, 
                           (gpointer)func));
    G_RELATIVE2(self, func, id__snooper_callbacks__, id);
    return id;
}

static VALUE
rg_m_key_snooper_remove(VALUE self, VALUE id)
{
    gtk_key_snooper_remove(NUM2UINT(id));
    G_REMOVE_RELATIVE(self, id__snooper_callbacks__, id);
    return Qnil;
}

static VALUE
rg_m_current_event(G_GNUC_UNUSED VALUE self)
{
    return GEV2RVAL(gtk_get_current_event());
}

static VALUE
rg_m_current_event_time(G_GNUC_UNUSED VALUE self)
{
    return INT2NUM(gtk_get_current_event_time());
}

static VALUE
rg_m_current_event_state(G_GNUC_UNUSED VALUE self)
{
    GdkModifierType state;
    gboolean ret = gtk_get_current_event_state(&state);
    return ret ? GFLAGS2RVAL(state, GDK_TYPE_MODIFIER_TYPE) : Qnil;
}

static VALUE
rg_m_get_event_widget(int argc, VALUE *argv, G_GNUC_UNUSED VALUE self)
{
    VALUE event;
    rb_scan_args(argc, argv, "01", &event);

    return GOBJ2RVAL(gtk_get_event_widget(NIL_P(event) ? NULL :RVAL2GEV(event)));
}

static VALUE
rg_m_propagate_event(G_GNUC_UNUSED VALUE self, VALUE widget, VALUE event)
{
    gtk_propagate_event(GTK_WIDGET(RVAL2GOBJ(widget)), RVAL2GEV(event));
    return Qnil;
}

/* From Version Information */
static VALUE
rg_m_check_version(G_GNUC_UNUSED VALUE self, VALUE major, VALUE minor, VALUE micro)
{
    const gchar *ret;
    ret = gtk_check_version(FIX2INT(major), FIX2INT(minor), FIX2INT(micro));
    return ret ? CSTR2RVAL(ret) : Qnil;
}

static VALUE
rg_m_check_version_p(G_GNUC_UNUSED VALUE self, VALUE major, VALUE minor, VALUE micro)
{
    const gchar *ret;
    ret = gtk_check_version(FIX2INT(major), FIX2INT(minor), FIX2INT(micro));
    return CBOOL2RVAL(ret == NULL);
}

#if GTK_CHECK_VERSION(2,14,0)
static VALUE
rg_m_show_uri(G_GNUC_UNUSED VALUE self, VALUE rb_uri_or_options)
{
    VALUE rb_screen = Qnil;
    VALUE rb_uri = Qnil;
    VALUE rb_timestamp = Qnil;
    GdkScreen *screen = NULL;
    const gchar *uri = NULL;
    guint32 timestamp = GDK_CURRENT_TIME;
    GError *error = NULL;

    if (TYPE(rb_uri_or_options) == T_HASH) {
        rbg_scan_options(rb_uri_or_options,
                         "screen", &rb_screen,
                         "uri", &rb_uri,
                         "timestamp", &rb_timestamp,
                         NULL);
    } else {
        rb_uri = rb_uri_or_options;
    }

    screen = RVAL2GOBJ(rb_screen);
    uri = StringValueCStr(rb_uri);
    if (!NIL_P(rb_timestamp)) {
        timestamp = NUM2UINT(rb_timestamp);
    }

    if (!gtk_show_uri(screen, uri, timestamp, &error)) {
        RAISE_GERROR(error);
    }

    return self;
}
#endif

/*
 * Init
 */
void 
Init_gtk_gtk(void)
{
    id_call = rb_intern("call");
    id__windows__ = rb_intern("__windows__");
    id__quit_callbacks__ = rb_intern("__quit_callbacks__");
    id__timeout_callbacks__ = rb_intern("__timeout_callbacks__");
    id__idle_callbacks__ = rb_intern("__idle_callbacks__");
    id__snooper_callbacks__ = rb_intern("__snooper_callbacks__");

    RG_TARGET_NAMESPACE = rb_define_module("Gtk");
    rb_ivar_set(RG_TARGET_NAMESPACE, id_relative_callbacks, Qnil);

    rbgtk_eGtkInitError = rb_define_class_under(RG_TARGET_NAMESPACE, "InitError",
                                                rb_eRuntimeError);

    RG_DEF_MODFUNC_P(events_pending, 0);

    RG_DEF_MODFUNC(set_locale, 0);
    RG_DEF_MODFUNC(disable_setlocale, 0);
    RG_DEF_MODFUNC(default_language, 0);
    RG_DEF_MODFUNC(init, -1);
    RG_DEF_MODFUNC(main, 0);
    RG_DEF_MODFUNC(main_level, 0);
    RG_DEF_MODFUNC(main_quit, 0);
    RG_DEF_MODFUNC(main_iteration, 0);
    RG_DEF_MODFUNC(main_iteration_do, 1);
    RG_DEF_MODFUNC(main_do_event, 1);
    RG_DEF_MODFUNC(grab_add, 1);
    RG_DEF_MODFUNC(current, 0);
    RG_DEF_MODFUNC(grab_remove, 1);
    RG_DEF_MODFUNC(init_add, 0);
    RG_DEF_MODFUNC(quit_add, 1);
    RG_DEF_MODFUNC(quit_remove, 1);

    RG_DEF_MODFUNC(timeout_add, 1);
    RG_DEF_MODFUNC(timeout_remove, 1);
    RG_DEF_MODFUNC(idle_add, 0);
    RG_DEF_MODFUNC(idle_add_priority, 1);
    RG_DEF_MODFUNC(idle_remove, 1);
    RG_DEF_MODFUNC(key_snooper_install, 0);
    RG_DEF_MODFUNC(key_snooper_remove, 1);
    RG_DEF_MODFUNC(current_event, 0);
    RG_DEF_MODFUNC(current_event_time, 0);
    RG_DEF_MODFUNC(current_event_state, 0);
    RG_DEF_MODFUNC(get_event_widget, -1);
    RG_DEF_MODFUNC(propagate_event, 2);
    RG_DEF_MODFUNC(check_version, 3);
    RG_DEF_MODFUNC_P(check_version, 3);

#if GTK_CHECK_VERSION(2,14,0)
    RG_DEF_MODFUNC(show_uri, 1);
#endif

    rb_define_const(RG_TARGET_NAMESPACE, "PRIORITY_RESIZE", INT2FIX(GTK_PRIORITY_RESIZE));
}
