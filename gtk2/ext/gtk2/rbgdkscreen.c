/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2003-2006 Ruby-GNOME2 Project Team
 *  Copyright (C) 2003  Geoff Youngs
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
#ifdef HAVE_RB_CAIRO_H
#include <rb_cairo.h>
#endif

#if GTK_CHECK_VERSION(2,2,0)

#define RG_TARGET_NAMESPACE cScreen
#define _SELF(i) GDK_SCREEN(RVAL2GOBJ(i))

static ID id_new;

static VALUE
rg_s_default(G_GNUC_UNUSED VALUE self)
{
    return GOBJ2RVAL(gdk_screen_get_default());
}

static VALUE
rg_default_colormap(VALUE self)
{
    return GOBJ2RVAL(gdk_screen_get_default_colormap(_SELF(self)));
}

static VALUE
rg_set_default_colormap(VALUE self, VALUE colormap)
{
    gdk_screen_set_default_colormap(_SELF(self),
                                    GDK_COLORMAP(RVAL2GOBJ(colormap)));
    return self;
}

static VALUE
rg_system_colormap(VALUE self)
{
    return GOBJ2RVAL(gdk_screen_get_system_colormap(_SELF(self)));
}

static VALUE
rg_system_visual(VALUE self)
{
    return GOBJ2RVAL(gdk_screen_get_system_visual(_SELF(self)));
}

static VALUE
rg_rgb_colormap(VALUE self)
{
    return GOBJ2RVAL(gdk_screen_get_rgb_colormap(_SELF(self)));
}

static VALUE
rg_rgb_visual(VALUE self)
{
    return GOBJ2RVAL(gdk_screen_get_rgb_visual(_SELF(self)));
}

#if GTK_CHECK_VERSION(2,8,0)
static VALUE
rg_rgba_colormap(VALUE self)
{
    return GOBJ2RVAL(gdk_screen_get_rgba_colormap(_SELF(self)));
}

static VALUE
rg_rgba_visual(VALUE self)
{
    return GOBJ2RVAL(gdk_screen_get_rgba_visual(_SELF(self)));
}
#endif

#if GTK_CHECK_VERSION(2,10,0)
static VALUE
rg_composited_p(VALUE self)
{
    return CBOOL2RVAL(gdk_screen_is_composited(_SELF(self)));
}
#endif

static VALUE
rg_root_window(VALUE self)
{
    return GOBJ2RVAL(gdk_screen_get_root_window(_SELF(self)));
}

static VALUE
rg_display(VALUE self)
{
    return GOBJ2RVAL(gdk_screen_get_display(_SELF(self)));
}

static VALUE
rg_number(VALUE self)
{
      return INT2NUM(gdk_screen_get_number(_SELF(self)));
}
static VALUE
rg_width(VALUE self)
{
      return INT2NUM(gdk_screen_get_width(_SELF(self)));
}
static VALUE
rg_height(VALUE self)
{
      return INT2NUM(gdk_screen_get_height(_SELF(self)));
}
static VALUE
rg_width_mm(VALUE self)
{
      return INT2NUM(gdk_screen_get_width_mm(_SELF(self)));
}
static VALUE
rg_height_mm(VALUE self)
{
      return INT2NUM(gdk_screen_get_height_mm(_SELF(self)));
}

static VALUE
rg_visuals(VALUE self)
{
    return GLIST2ARYF(gdk_screen_list_visuals(_SELF(self)));
}

static VALUE
rg_toplevel_windows(VALUE self)
{
    return GLIST2ARYF(gdk_screen_get_toplevel_windows(_SELF(self)));
}

static VALUE
rg_display_name(VALUE self)
{
    gchar* name = gdk_screen_make_display_name(_SELF(self));
    VALUE ret = CSTR2RVAL(name);
    g_free(name);
    return ret;
}

static VALUE
rg_n_monitors(VALUE self)
{
      return INT2NUM(gdk_screen_get_n_monitors(_SELF(self)));
}

static VALUE
rg_monitor_geometry(VALUE self, VALUE num)
{
      GdkRectangle rect;
      gdk_screen_get_monitor_geometry(_SELF(self), NUM2INT(num), &rect);
      return BOXED2RVAL(&rect, GDK_TYPE_RECTANGLE);
}

static VALUE
rg_get_monitor(int argc, VALUE *argv, VALUE self)
{
    VALUE arg1, arg2;
    VALUE ret;

    rb_scan_args(argc, argv, "11", &arg1, &arg2);
    if (argc == 2){
        ret = INT2NUM(gdk_screen_get_monitor_at_point(_SELF(self),
                                                      NUM2INT(arg1), NUM2INT(arg2)));
    } else if (argc == 1){
        ret = INT2NUM(gdk_screen_get_monitor_at_window(_SELF(self),
                                                       GDK_WINDOW(RVAL2GOBJ(arg1))));
    } else {
        rb_raise(rb_eArgError, "Wrong number of arguments: %d", argc);
    }
    return ret;
}

static VALUE
rg_broadcast_client_message(VALUE self, VALUE event)
{
    gdk_screen_broadcast_client_message(_SELF(self), RVAL2GEV(event));
    return self;
}

/*
  type: String, Integer, Gdk::Color.
 */
static VALUE
rg_get_setting(int argc, VALUE *argv, VALUE self)
{
    VALUE name, type;
    GType gtype;
    GValue val = G_VALUE_INIT;
    gboolean ret;
    VALUE value;

    rb_scan_args(argc, argv, "11", &name, &type);
    if NIL_P(type) 
        gtype = G_TYPE_STRING;
    else
        gtype = CLASS2GTYPE(type);

    g_value_init(&val, gtype);
    ret = gdk_screen_get_setting(_SELF(self), RVAL2CSTR(name), &val);
    value = ret ? GVAL2RVAL(&val) : Qnil;
    g_value_unset(&val);
    return value;
}

#if GTK_CHECK_VERSION(2,10,0)
#ifdef HAVE_RB_CAIRO_H
static VALUE
gdkscreen_get_font_options(VALUE self)
{
    return CRFONTOPTIONS2RVAL((cairo_font_options_t *)gdk_screen_get_font_options(_SELF(self)));
}

static VALUE
gdkscreen_set_font_options(VALUE self, VALUE options)
{
    gdk_screen_set_font_options(_SELF(self), 
                                (const cairo_font_options_t *)RVAL2CRFONTOPTIONS(options));
    return self;
}
#endif

static VALUE
rg_active_window(VALUE self)
{
    return GOBJ2RVAL(gdk_screen_get_active_window(_SELF(self)));
}

static VALUE
rg_window_stack(VALUE self)
{
    GList* list = gdk_screen_get_window_stack(_SELF(self));
    VALUE ary = rb_ary_new();
    while (list) {
        rb_ary_push(ary, GOBJ2RVAL(list->data));
        g_object_unref(list->data);
        list = list->next;
    }
    g_list_free(list);
    return ary;
}
#endif

#if GTK_CHECK_VERSION(2,4,0)
static void
child_setup(gpointer func)
{
    if (! NIL_P(func)){
        rb_funcall((VALUE)func, id_call, 0);
    }
}

static VALUE
rg_spawn_on_screen(VALUE self, VALUE working_directory, VALUE argv, VALUE envp, VALUE flags)
{
    GError *err = NULL;
    gboolean ret;
    gint child_pid;
    VALUE func = Qnil;
    gchar **gargv;
    gchar **genvp;

    if (rb_block_given_p()) {
        func = rb_block_proc();
        G_RELATIVE(self, func);
    }

    gargv = (gchar **)RVAL2STRV(argv);
    genvp = (gchar **)RVAL2STRV_ACCEPT_NIL(envp);
    ret = gdk_spawn_on_screen(_SELF(self),
                              NIL_P(working_directory) ? NULL : RVAL2CSTR(working_directory),
                              gargv, genvp, NUM2INT(flags),
                              (GSpawnChildSetupFunc)child_setup, (gpointer)func,
                              &child_pid, &err);
    g_free(gargv);
    g_free(genvp);
    if (!ret)
        RAISE_GERROR(err);

    return INT2NUM(child_pid);
}

static VALUE
rg_spawn_on_screen_with_pipes(VALUE self, VALUE working_directory, VALUE argv, VALUE envp, VALUE flags)
{
    GError *err = NULL;
    gboolean ret;
    gint child_pid;
    VALUE func = Qnil;
    gchar **gargv;
    gchar **genvp;
    gint standard_input, standard_output, standard_error;

    if (rb_block_given_p()) {
        func = rb_block_proc();
        G_RELATIVE(self, func);
    }

    gargv = (gchar **)RVAL2STRV(argv);
    genvp = (gchar **)RVAL2STRV_ACCEPT_NIL(envp);
    ret = gdk_spawn_on_screen_with_pipes(_SELF(self),
                                         NIL_P(working_directory) ? NULL : RVAL2CSTR(working_directory),
                                         gargv, genvp, NUM2INT(flags),
                                         (GSpawnChildSetupFunc)child_setup, (gpointer)func,
                                         &child_pid, 
                                         &standard_input, &standard_output,
                                         &standard_error, &err);
    g_free(gargv);
    g_free(genvp);
    if (!ret)
        RAISE_GERROR(err);

    return rb_ary_new3(4, INT2NUM(child_pid), 
                       rb_funcall(rb_cIO, id_new, 1, INT2NUM(standard_input)),
                       rb_funcall(rb_cIO, id_new, 1, INT2NUM(standard_output)),
                       rb_funcall(rb_cIO, id_new, 1, INT2NUM(standard_error)));
}

static VALUE
rg_spawn_command_line_on_screen(G_GNUC_UNUSED VALUE self, VALUE command_line)
{
    GError *err = NULL;
    VALUE ret;

    ret = CBOOL2RVAL(g_spawn_command_line_async(RVAL2CSTR(command_line), &err));
    if (!ret) RAISE_GERROR(err);

    return ret;   
}
#endif

/* From X Window System Interaction */
#ifdef GDK_WINDOWING_X11
#include <gdk/gdkx.h>
static VALUE
rg_xnumber(VALUE self)
{
    return INT2NUM(GDK_SCREEN_XNUMBER(_SELF(self)));
}
static VALUE
rg_supports_net_wm_hint_p(VALUE self, VALUE property)
{
    return CBOOL2RVAL(gdk_x11_screen_supports_net_wm_hint(_SELF(self),
                                                          RVAL2ATOM(property)));
}

static VALUE
rg_window_manager_name(VALUE self)
{
    return CSTR2RVAL(gdk_x11_screen_get_window_manager_name(_SELF(self)));
}
static VALUE
rg_screen_number(VALUE self)
{
    return INT2NUM(gdk_x11_screen_get_screen_number(_SELF(self)));
}
#endif

#endif

void 
Init_gtk_gdk_screen(VALUE mGdk)
{
#if GTK_CHECK_VERSION(2,2,0)
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GDK_TYPE_SCREEN, "Screen", mGdk);

    id_new = rb_intern("new");

    RG_DEF_SMETHOD(default, 0);
    RG_DEF_METHOD(default_colormap, 0);
    RG_DEF_METHOD(set_default_colormap, 1);
    G_DEF_SETTER(RG_TARGET_NAMESPACE, "default_colormap");
    RG_DEF_METHOD(system_colormap, 0);
    RG_DEF_METHOD(system_visual, 0);
    RG_DEF_METHOD(rgb_colormap, 0);
    RG_DEF_METHOD(rgb_visual, 0);
#if GTK_CHECK_VERSION(2,8,0)
    RG_DEF_METHOD(rgba_colormap, 0);
    RG_DEF_METHOD(rgba_visual, 0);
#endif
#if GTK_CHECK_VERSION(2,10,0)
    RG_DEF_METHOD_P(composited, 0);
#endif
    RG_DEF_METHOD(root_window, 0);
    RG_DEF_METHOD(display, 0);
    RG_DEF_METHOD(number, 0);
    RG_DEF_METHOD(width, 0);
    RG_DEF_METHOD(height, 0);
    RG_DEF_METHOD(width_mm, 0);
    RG_DEF_METHOD(height_mm, 0);
    RG_DEF_METHOD(visuals, 0);
    RG_DEF_METHOD(toplevel_windows, 0);
    RG_DEF_METHOD(display_name, 0);
    RG_DEF_METHOD(n_monitors, 0);
    RG_DEF_METHOD(monitor_geometry, 1);
    RG_DEF_METHOD(get_monitor, -1);
    RG_DEF_METHOD(broadcast_client_message, 1);
    RG_DEF_METHOD(get_setting, -1);
#if GTK_CHECK_VERSION(2,10,0)
#ifdef HAVE_RB_CAIRO_H
    G_REPLACE_GET_PROPERTY(RG_TARGET_NAMESPACE, "font_options", gdkscreen_get_font_options, 0);
    G_REPLACE_SET_PROPERTY(RG_TARGET_NAMESPACE, "font_options", gdkscreen_set_font_options, 1);
#endif
    RG_DEF_METHOD(active_window, 0);
    RG_DEF_METHOD(window_stack, 0);
#endif

#if GTK_CHECK_VERSION(2,4,0)
    RG_DEF_METHOD(spawn_on_screen, 4);
    RG_DEF_METHOD(spawn_on_screen_with_pipes, 4);
    RG_DEF_METHOD(spawn_command_line_on_screen, 1);
#endif

#ifdef GDK_WINDOWING_X11
    RG_DEF_METHOD(xnumber, 0);
    RG_DEF_METHOD_P(supports_net_wm_hint, 0);
    RG_DEF_METHOD(window_manager_name, 0);
    RG_DEF_METHOD(screen_number, 0);
#endif

#ifdef GDK_WINDOWING_X11
    G_DEF_CLASS3("GdkScreenX11", "ScreenX11", mGdk);
#endif
#endif
}
