/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011-2013  Ruby-GNOME2 Project Team
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

#include "rbgtk3private.h"
#ifdef HAVE_SYS_TIME_H
# include <sys/time.h>
#endif
#include <locale.h>

#define RG_TARGET_NAMESPACE mGtk

static VALUE RG_TARGET_NAMESPACE;

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
rbgtk_initialize_gtkobject(VALUE obj, GObject *gtkobj)
{
    gtkobj = g_object_ref(gtkobj);
    g_object_ref_sink(gtkobj);
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
rg_s_events_pending_p(G_GNUC_UNUSED VALUE self)
{
   return CBOOL2RVAL(gtk_events_pending());
}

/* deprecated
static VALUE
rg_s_set_locale(G_GNUC_UNUSED VALUE self)
{
    return CSTR2RVAL(gtk_set_locale());
}
*/

static VALUE
rg_s_disable_setlocale(G_GNUC_UNUSED VALUE self)
{
    gtk_disable_setlocale();
    return Qnil;
}

static VALUE
rg_s_default_language(G_GNUC_UNUSED VALUE self)
{
    return PANGOLANGUAGE2RVAL(gtk_get_default_language());
}

typedef void (*SignalFunc) (int);

static gboolean _initialized = FALSE;

static VALUE
rg_s_init(int argc, VALUE *argv, VALUE self)
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

        is_initialized = gtk_init_check(&gargc, &gargv);
        if (! is_initialized) {
            const char *display_name_arg = gdk_get_display_arg_name();
            display_name_arg = display_name_arg ? display_name_arg : g_getenv("DISPLAY");
            rb_raise(rbgtk_eGtkInitError, "Cannot open display: %s",
                     display_name_arg ? display_name_arg : " ");
        }

        setlocale(LC_NUMERIC, "C");
    }

    return self;
}

/* We don't need them.
gtk_init()
*/

static VALUE
rg_s_main(G_GNUC_UNUSED VALUE self)
{
    gtk_main();
    return Qnil;
}

static VALUE
rg_s_main_level(G_GNUC_UNUSED VALUE self)
{
    return INT2FIX(gtk_main_level());
}

static VALUE
rg_s_main_quit(G_GNUC_UNUSED VALUE self)
{
    gtk_main_quit();
    return Qnil;
}

static VALUE
rg_s_main_iteration(G_GNUC_UNUSED VALUE self)
{
    return CBOOL2RVAL(gtk_main_iteration());
}

static VALUE
rg_s_main_iteration_do(G_GNUC_UNUSED VALUE self, VALUE blocking)
{
    return CBOOL2RVAL(gtk_main_iteration_do(RVAL2CBOOL(blocking)));
}

static VALUE
rg_s_main_do_event(G_GNUC_UNUSED VALUE self, VALUE event)
{
    gtk_main_do_event(RVAL2GEV(event));
    return event;
}

/* We don't need them.
gtk_true()
gtk_false()
*/

static VALUE
rg_s_grab_add(G_GNUC_UNUSED VALUE self, VALUE widget)
{
    gtk_grab_add(RVAL2GTKWIDGET(widget));
    return Qnil;
}

static VALUE
rg_s_current(G_GNUC_UNUSED VALUE self)
{
    return GOBJ2RVAL(gtk_grab_get_current());
}

static VALUE
rg_s_grab_remove(G_GNUC_UNUSED VALUE self, VALUE widget)
{
    gtk_grab_remove(RVAL2GTKWIDGET(widget));
    return Qnil;
}

static gint
gtk_m_key_snoop_func(GtkWidget *grab_widget, GdkEventKey *event, gpointer func)
{
    VALUE ret = rb_funcall((VALUE)func, id_call, 2,
                           GOBJ2RVAL(grab_widget),
                           GEV2RVAL((GdkEvent*)event));
    return RVAL2CBOOL(ret);
}

static VALUE
rg_s_key_snooper_install(VALUE self)
{
    VALUE func = rb_block_proc();
    VALUE id = INT2FIX(gtk_key_snooper_install(
                           (GtkKeySnoopFunc)gtk_m_key_snoop_func,
                           (gpointer)func));
    G_RELATIVE2(self, func, id__snooper_callbacks__, id);
    return id;
}

static VALUE
rg_s_key_snooper_remove(VALUE self, VALUE id)
{
    gtk_key_snooper_remove(NUM2UINT(id));
    G_REMOVE_RELATIVE(self, id__snooper_callbacks__, id);
    return Qnil;
}

static VALUE
rg_s_current_event(G_GNUC_UNUSED VALUE self)
{
    return GEV2RVAL(gtk_get_current_event());
}

static VALUE
rg_s_current_event_time(G_GNUC_UNUSED VALUE self)
{
    return INT2NUM(gtk_get_current_event_time());
}

static VALUE
rg_s_current_event_state(G_GNUC_UNUSED VALUE self)
{
    GdkModifierType state;
    gboolean ret = gtk_get_current_event_state(&state);
    return ret ? GDKMODIFIERTYPE2RVAL(state) : Qnil;
}

static VALUE
rg_s_propagate_event(G_GNUC_UNUSED VALUE self, VALUE widget, VALUE event)
{
    gtk_propagate_event(RVAL2GTKWIDGET(widget), RVAL2GEV(event));
    return Qnil;
}

static VALUE
rg_s_check_version(G_GNUC_UNUSED VALUE self, VALUE major, VALUE minor, VALUE micro)
{
    const gchar *ret;
    ret = gtk_check_version(FIX2INT(major), FIX2INT(minor), FIX2INT(micro));
    return ret ? CSTR2RVAL(ret) : Qnil;
}

static VALUE
rg_s_check_version_p(G_GNUC_UNUSED VALUE self, VALUE major, VALUE minor, VALUE micro)
{
    const gchar *ret;
    ret = gtk_check_version(FIX2INT(major), FIX2INT(minor), FIX2INT(micro));
    return CBOOL2RVAL(ret == NULL);
}

static VALUE
rg_s_show_uri(G_GNUC_UNUSED VALUE self, VALUE rb_uri_or_options)
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

void
Init_gtk(void)
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

    RG_DEF_SMETHOD_P(events_pending, 0);

/* deprecated
    RG_DEF_SMETHOD(set_locale, 0);
*/
    RG_DEF_SMETHOD(disable_setlocale, 0);
    RG_DEF_SMETHOD(default_language, 0);
    RG_DEF_SMETHOD(init, -1);
    RG_DEF_SMETHOD(main, 0);
    RG_DEF_SMETHOD(main_level, 0);
    RG_DEF_SMETHOD(main_quit, 0);
    RG_DEF_SMETHOD(main_iteration, 0);
    RG_DEF_SMETHOD(main_iteration_do, 1);
    RG_DEF_SMETHOD(main_do_event, 1);
    RG_DEF_SMETHOD(grab_add, 1);
    RG_DEF_SMETHOD(current, 0);
    RG_DEF_SMETHOD(grab_remove, 1);
    RG_DEF_SMETHOD(key_snooper_install, 0);
    RG_DEF_SMETHOD(key_snooper_remove, 1);
    RG_DEF_SMETHOD(current_event, 0);
    RG_DEF_SMETHOD(current_event_time, 0);
    RG_DEF_SMETHOD(current_event_state, 0);
    RG_DEF_SMETHOD(propagate_event, 2);
    RG_DEF_SMETHOD(check_version, 3);
    RG_DEF_SMETHOD_P(check_version, 3);
    RG_DEF_SMETHOD(show_uri, 1);

    rb_define_const(RG_TARGET_NAMESPACE, "PRIORITY_RESIZE", INT2FIX(GTK_PRIORITY_RESIZE));

    Init_gtk_gdk_event();
    Init_gtk_drag_gdk_drag_context();
    Init_gtk_style_context_gdk_screen();
    Init_gtk_tooltip_gdk_display();

    Init_gtk_aboutdialog(RG_TARGET_NAMESPACE);
    Init_gtk_accelerator(RG_TARGET_NAMESPACE);
    Init_gtk_accel_group(RG_TARGET_NAMESPACE);
    Init_gtk_accel_group_entry(RG_TARGET_NAMESPACE);
    Init_gtk_accel_key(RG_TARGET_NAMESPACE);
    Init_gtk_accel_label(RG_TARGET_NAMESPACE);
    Init_gtk_accel_map(RG_TARGET_NAMESPACE);
    Init_gtk_accessible(RG_TARGET_NAMESPACE);
    Init_gtk_action(RG_TARGET_NAMESPACE);
    Init_gtk_actiongroup(RG_TARGET_NAMESPACE);
    Init_gtk_activatable(RG_TARGET_NAMESPACE);
    Init_gtk_adjustment(RG_TARGET_NAMESPACE);
    Init_gtk_alignment(RG_TARGET_NAMESPACE);
    Init_gtk_allocation(RG_TARGET_NAMESPACE);
    Init_gtk_appchooser(RG_TARGET_NAMESPACE);
    Init_gtk_appchooserbutton(RG_TARGET_NAMESPACE);
    Init_gtk_appchooserdialog(RG_TARGET_NAMESPACE);
    Init_gtk_appchooserwidget(RG_TARGET_NAMESPACE);
    Init_gtk_application(RG_TARGET_NAMESPACE);
    Init_gtk_arrow(RG_TARGET_NAMESPACE);
    Init_gtk_aspect_frame(RG_TARGET_NAMESPACE);
    Init_gtk_assistant(RG_TARGET_NAMESPACE);
    Init_gtk_button_box(RG_TARGET_NAMESPACE);
    Init_gtk_bin(RG_TARGET_NAMESPACE);
    Init_gtk_bindings(RG_TARGET_NAMESPACE);
    Init_gtk_border(RG_TARGET_NAMESPACE);
    Init_gtk_box(RG_TARGET_NAMESPACE);
    Init_gtk_buildable(RG_TARGET_NAMESPACE);
    Init_gtk_builder(RG_TARGET_NAMESPACE);
    Init_gtk_button(RG_TARGET_NAMESPACE);
    Init_gtk_calendar(RG_TARGET_NAMESPACE);
    Init_gtk_celleditable(RG_TARGET_NAMESPACE);
    Init_gtk_celllayout(RG_TARGET_NAMESPACE);
    Init_gtk_cellrenderer(RG_TARGET_NAMESPACE);
    Init_gtk_cellrendereraccel(RG_TARGET_NAMESPACE);
    Init_gtk_cellrenderercombo(RG_TARGET_NAMESPACE);
    Init_gtk_cellrendererpixbuf(RG_TARGET_NAMESPACE);
    Init_gtk_cellrendererprogress(RG_TARGET_NAMESPACE);
    Init_gtk_cellrendererspin(RG_TARGET_NAMESPACE);
    Init_gtk_cellrendererspinner(RG_TARGET_NAMESPACE);
    Init_gtk_cellrenderertext(RG_TARGET_NAMESPACE);
    Init_gtk_cellrenderertoggle(RG_TARGET_NAMESPACE);
    Init_gtk_cellview(RG_TARGET_NAMESPACE);
    Init_gtk_check_button(RG_TARGET_NAMESPACE);
    Init_gtk_check_menu_item(RG_TARGET_NAMESPACE);
    Init_gtk_clipboard(RG_TARGET_NAMESPACE);
    Init_gtk_colorbutton(RG_TARGET_NAMESPACE);
    Init_gtk_color_selection(RG_TARGET_NAMESPACE);
    Init_gtk_color_selection_dialog(RG_TARGET_NAMESPACE);
    Init_gtk_combobox(RG_TARGET_NAMESPACE);
    Init_gtk_comboboxtext(RG_TARGET_NAMESPACE);
    Init_gtk_const(RG_TARGET_NAMESPACE);
    Init_gtk_container(RG_TARGET_NAMESPACE);
    Init_gtk_cssprovider(RG_TARGET_NAMESPACE);
    Init_gtk_dialog(RG_TARGET_NAMESPACE);
    Init_gtk_drag(RG_TARGET_NAMESPACE);
    Init_gtk_drawing_area(RG_TARGET_NAMESPACE);
    Init_gtk_editable(RG_TARGET_NAMESPACE);
    Init_gtk_entry(RG_TARGET_NAMESPACE);
    Init_gtk_entrybuffer(RG_TARGET_NAMESPACE);
    Init_gtk_entry_completion(RG_TARGET_NAMESPACE);
    Init_gtk_eventbox(RG_TARGET_NAMESPACE);
    Init_gtk_expander(RG_TARGET_NAMESPACE);
    Init_gtk_file_chooser(RG_TARGET_NAMESPACE);
    Init_gtk_filesystemerror(RG_TARGET_NAMESPACE);
    Init_gtk_filechooserbutton(RG_TARGET_NAMESPACE);
    Init_gtk_file_chooser_dialog(RG_TARGET_NAMESPACE);
    Init_gtk_file_chooser_widget(RG_TARGET_NAMESPACE);
    Init_gtk_file_filter(RG_TARGET_NAMESPACE);
    Init_gtk_fixed(RG_TARGET_NAMESPACE);
    Init_gtk_fontbutton(RG_TARGET_NAMESPACE);
    Init_gtk_fontchooser(RG_TARGET_NAMESPACE);
    Init_gtk_fontchooserdialog(RG_TARGET_NAMESPACE);
    Init_gtk_fontchooserwidget(RG_TARGET_NAMESPACE);
    Init_gtk_frame(RG_TARGET_NAMESPACE);
    Init_gtk_grid(RG_TARGET_NAMESPACE);
    Init_gtk_handle_box(RG_TARGET_NAMESPACE);
#if GTK_CHECK_VERSION(3, 10, 0)
    Init_gtk_header_bar(RG_TARGET_NAMESPACE);
#endif
    Init_gtk_hsv(RG_TARGET_NAMESPACE);
    Init_gtk_icon_factory(RG_TARGET_NAMESPACE);
    Init_gtk_icon_info(RG_TARGET_NAMESPACE);
    Init_gtk_icon_set(RG_TARGET_NAMESPACE);
    Init_gtk_icon_size(RG_TARGET_NAMESPACE);
    Init_gtk_icon_source(RG_TARGET_NAMESPACE);
    Init_gtk_icon_theme(RG_TARGET_NAMESPACE);
    Init_gtk_iconview(RG_TARGET_NAMESPACE);
    Init_gtk_image(RG_TARGET_NAMESPACE);
    Init_gtk_image_menu_item(RG_TARGET_NAMESPACE);
    Init_gtk_imcontext(RG_TARGET_NAMESPACE);
    Init_gtk_imcontext_simple(RG_TARGET_NAMESPACE);
    Init_gtk_im_multicontext(RG_TARGET_NAMESPACE);
    Init_gtk_infobar(RG_TARGET_NAMESPACE);
    Init_gtk_invisible(RG_TARGET_NAMESPACE);
    Init_gtk_label(RG_TARGET_NAMESPACE);
    Init_gtk_layout(RG_TARGET_NAMESPACE);
#if GTK_CHECK_VERSION(3, 6, 0)
    Init_gtk_level_bar(RG_TARGET_NAMESPACE);
#endif
    Init_gtk_link_button(RG_TARGET_NAMESPACE);
    Init_gtk_list_store(RG_TARGET_NAMESPACE);
    Init_gtk_lockbutton(RG_TARGET_NAMESPACE);
    Init_gtk_menu(RG_TARGET_NAMESPACE);
    Init_gtk_menu_bar(RG_TARGET_NAMESPACE);
#if GTK_CHECK_VERSION(3, 6, 0)
    Init_gtk_menu_button(RG_TARGET_NAMESPACE);
#endif
    Init_gtk_menu_item(RG_TARGET_NAMESPACE);
    Init_gtk_menu_shell(RG_TARGET_NAMESPACE);
    Init_gtk_menutoolbutton(RG_TARGET_NAMESPACE);
    Init_gtk_message_dialog(RG_TARGET_NAMESPACE);
    Init_gtk_misc(RG_TARGET_NAMESPACE);
    Init_gtk_notebook(RG_TARGET_NAMESPACE);
    Init_gtk_numerableicon(RG_TARGET_NAMESPACE);
    Init_gtk_orientable(RG_TARGET_NAMESPACE);
    Init_gtk_overlay(RG_TARGET_NAMESPACE);
    Init_gtk_page_setup(RG_TARGET_NAMESPACE);
    Init_gtk_page_setup_unix_dialog(RG_TARGET_NAMESPACE);
    Init_gtk_paned(RG_TARGET_NAMESPACE);
    Init_gtk_paper_size(RG_TARGET_NAMESPACE);
    Init_gtk_plug(RG_TARGET_NAMESPACE);
    Init_gtk_print_context(RG_TARGET_NAMESPACE);
    Init_gtk_printer(RG_TARGET_NAMESPACE);
    Init_gtk_print_job(RG_TARGET_NAMESPACE);
    Init_gtk_print_operation(RG_TARGET_NAMESPACE);
    Init_gtk_print_operation_preview(RG_TARGET_NAMESPACE);
    Init_gtk_print_settings(RG_TARGET_NAMESPACE);
    Init_gtk_print_unix_dialog(RG_TARGET_NAMESPACE);
    Init_gtk_progress_bar(RG_TARGET_NAMESPACE);
    Init_gtk_radio_action(RG_TARGET_NAMESPACE);
    Init_gtk_radio_button(RG_TARGET_NAMESPACE);
    Init_gtk_radio_menu_item(RG_TARGET_NAMESPACE);
    Init_gtk_radiotoolbutton(RG_TARGET_NAMESPACE);
    Init_gtk_range(RG_TARGET_NAMESPACE);
    Init_gtk_recentaction(RG_TARGET_NAMESPACE);
    Init_gtk_recent_chooser(RG_TARGET_NAMESPACE);
    Init_gtk_recent_chooser_dialog(RG_TARGET_NAMESPACE);
    Init_gtk_recent_chooser_menu(RG_TARGET_NAMESPACE);
    Init_gtk_recent_chooser_widget(RG_TARGET_NAMESPACE);
    Init_gtk_recent_data(RG_TARGET_NAMESPACE);
    Init_gtk_recent_filter(RG_TARGET_NAMESPACE);
    Init_gtk_recent_filter_info(RG_TARGET_NAMESPACE);
    Init_gtk_recent_info(RG_TARGET_NAMESPACE);
    Init_gtk_recent_manager(RG_TARGET_NAMESPACE);
    Init_gtk_scale(RG_TARGET_NAMESPACE);
    Init_gtk_scalebutton(RG_TARGET_NAMESPACE);
    Init_gtk_scrollable(RG_TARGET_NAMESPACE);
    Init_gtk_scrollbar(RG_TARGET_NAMESPACE);
    Init_gtk_scrolled_window(RG_TARGET_NAMESPACE);
#if GTK_CHECK_VERSION(3, 6, 0)
    Init_gtk_search_entry(RG_TARGET_NAMESPACE);
#endif
    Init_gtk_selection(RG_TARGET_NAMESPACE);
    Init_gtk_selectiondata(RG_TARGET_NAMESPACE);
    Init_gtk_separator(RG_TARGET_NAMESPACE);
    Init_gtk_separator_menu_item(RG_TARGET_NAMESPACE);
    Init_gtk_separatortoolitem(RG_TARGET_NAMESPACE);
    Init_gtk_settings(RG_TARGET_NAMESPACE);
    Init_gtk_size_group(RG_TARGET_NAMESPACE);
    Init_gtk_socket(RG_TARGET_NAMESPACE);
    Init_gtk_spin_button(RG_TARGET_NAMESPACE);
    Init_gtk_spinner(RG_TARGET_NAMESPACE);
    Init_gtk_statusbar(RG_TARGET_NAMESPACE);
    Init_gtk_status_icon(RG_TARGET_NAMESPACE);
    Init_gtk_stock(RG_TARGET_NAMESPACE);
    Init_gtk_stylecontext(RG_TARGET_NAMESPACE);
    Init_gtk_styleproperties(RG_TARGET_NAMESPACE);
    Init_gtk_styleprovider(RG_TARGET_NAMESPACE);
    Init_gtk_switch(RG_TARGET_NAMESPACE);
    Init_gtk_table(RG_TARGET_NAMESPACE);
    Init_gtk_target_list(RG_TARGET_NAMESPACE);
    Init_gtk_tearoff_menu_item(RG_TARGET_NAMESPACE);
    Init_gtk_text_appearance(RG_TARGET_NAMESPACE);
    Init_gtk_text_attributes(RG_TARGET_NAMESPACE);
    Init_gtk_textbuffer(RG_TARGET_NAMESPACE);
    Init_gtk_textchild(RG_TARGET_NAMESPACE);
    Init_gtk_textiter(RG_TARGET_NAMESPACE);
    Init_gtk_textmark(RG_TARGET_NAMESPACE);
    Init_gtk_texttag(RG_TARGET_NAMESPACE);
    Init_gtk_text_tag_table(RG_TARGET_NAMESPACE);
    Init_gtk_textview(RG_TARGET_NAMESPACE);
    Init_gtk_themingengine(RG_TARGET_NAMESPACE);
    Init_gtk_toggle_action(RG_TARGET_NAMESPACE);
    Init_gtk_toggle_button(RG_TARGET_NAMESPACE);
    Init_gtk_toggletoolbutton(RG_TARGET_NAMESPACE);
    Init_gtk_toolbar(RG_TARGET_NAMESPACE);
    Init_gtk_toolbutton(RG_TARGET_NAMESPACE);
    Init_gtk_toolitem(RG_TARGET_NAMESPACE);
    Init_gtk_toolitemgroup(RG_TARGET_NAMESPACE);
    Init_gtk_toolshell(RG_TARGET_NAMESPACE);
    Init_gtk_tooltip(RG_TARGET_NAMESPACE);
    Init_gtk_treedragdest(RG_TARGET_NAMESPACE);
    Init_gtk_treedragsource(RG_TARGET_NAMESPACE);
    Init_gtk_treeiter(RG_TARGET_NAMESPACE);
    Init_gtk_treemodel(RG_TARGET_NAMESPACE);
    Init_gtk_treemodelfilter(RG_TARGET_NAMESPACE);
    Init_gtk_tmodelsort(RG_TARGET_NAMESPACE);
    Init_gtk_treepath(RG_TARGET_NAMESPACE);
    Init_gtk_treerowreference(RG_TARGET_NAMESPACE);
    Init_gtk_treeselection(RG_TARGET_NAMESPACE);
    Init_gtk_treesortable(RG_TARGET_NAMESPACE);
    Init_gtk_tree_store(RG_TARGET_NAMESPACE);
    Init_gtk_treeview(RG_TARGET_NAMESPACE);
    Init_gtk_treeviewcolumn(RG_TARGET_NAMESPACE);
    Init_gtk_uimanager(RG_TARGET_NAMESPACE);
    Init_gtk_viewport(RG_TARGET_NAMESPACE);
    Init_gtk_volumebutton(RG_TARGET_NAMESPACE);
    Init_gtk_widget(RG_TARGET_NAMESPACE);
    Init_gtk_widgetpath(RG_TARGET_NAMESPACE);
    Init_gtk_window(RG_TARGET_NAMESPACE);
    Init_gtk_windowgroup(RG_TARGET_NAMESPACE);
}
