/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgdkdisplay.c -

  $Author: mutoh $
  $Date: 2004/08/22 13:30:40 $

  Copyright (C) 2003,2004 Ruby-GNOME2 Project Team
  Copyright (C) 2003 Geoff Youngs
************************************************/

#include "global.h"

#if GTK_CHECK_VERSION(2,2,0)
#define _SELF(i) GDK_SCREEN(RVAL2GOBJ(i))

static ID id_new;

static VALUE
gdkscreen_default(self)
    VALUE self;
{
    return GOBJ2RVAL(gdk_screen_get_default());
}

static VALUE
gdkscreen_get_default_colormap(self)
    VALUE self;
{
    return GOBJ2RVAL(gdk_screen_get_default_colormap(_SELF(self)));
}

static VALUE
gdkscreen_set_default_colormap(self, colormap)
    VALUE self, colormap;
{
    gdk_screen_set_default_colormap(_SELF(self),
                                    GDK_COLORMAP(RVAL2GOBJ(colormap)));
    return self;
}

static VALUE
gdkscreen_get_system_colormap(self)
    VALUE self;
{
    return GOBJ2RVAL(gdk_screen_get_system_colormap(_SELF(self)));
}

static VALUE
gdkscreen_get_system_visual(self)
    VALUE self;
{
    return GOBJ2RVAL(gdk_screen_get_system_visual(_SELF(self)));
}

static VALUE
gdkscreen_get_rgb_colormap(self)
    VALUE self;
{
    return GOBJ2RVAL(gdk_screen_get_rgb_colormap(_SELF(self)));
}

static VALUE
gdkscreen_get_rgb_visual(self)
    VALUE self;
{
    return GOBJ2RVAL(gdk_screen_get_rgb_visual(_SELF(self)));
}

static VALUE
gdkscreen_get_root_window(self)
    VALUE self;
{
    return GOBJ2RVAL(gdk_screen_get_root_window(_SELF(self)));
}

static VALUE
gdkscreen_get_display(self)
    VALUE self;
{
    return GOBJ2RVAL(gdk_screen_get_display(_SELF(self)));
}

static VALUE
gdkscreen_number(self)
    VALUE self;
{
      return INT2NUM(gdk_screen_get_number(_SELF(self)));
}
static VALUE
gdkscreen_width(self)
    VALUE self;
{
      return INT2NUM(gdk_screen_get_width(_SELF(self)));
}
static VALUE
gdkscreen_height(self)
    VALUE self;
{
      return INT2NUM(gdk_screen_get_height(_SELF(self)));
}
static VALUE
gdkscreen_width_mm(self)
    VALUE self;
{
      return INT2NUM(gdk_screen_get_width_mm(_SELF(self)));
}
static VALUE
gdkscreen_height_mm(self)
    VALUE self;
{
      return INT2NUM(gdk_screen_get_height_mm(_SELF(self)));
}

static VALUE
gdkscreen_list_visuals(self)
    VALUE self;
{
    GList* list = gdk_screen_list_visuals(_SELF(self));
    VALUE ret = GLIST2ARY(list);
    g_list_free(list);
    return ret;
}

static VALUE
gdkscreen_get_toplevel_windows(self)
    VALUE self;
{
    GList* list = gdk_screen_get_toplevel_windows(_SELF(self));
    VALUE ret = GLIST2ARY(list);
    g_list_free(list);
    return ret;
}

static VALUE
gdkscreen_make_display_name(self)
    VALUE self;
{
    gchar* name = gdk_screen_make_display_name(_SELF(self));
    VALUE ret = CSTR2RVAL(name);
    g_free(name);
    return ret;
}

static VALUE
gdkscreen_n_monitors(self)
    VALUE self;
{
      return INT2NUM(gdk_screen_get_n_monitors(_SELF(self)));
}

static VALUE
gdkscreen_monitor_geometry(self, num)
    VALUE self, num;
{
      GdkRectangle rect;
      gdk_screen_get_monitor_geometry(_SELF(self), NUM2INT(num), &rect);
      return BOXED2RVAL(&rect, GDK_TYPE_RECTANGLE);
}

static VALUE
gdkscreen_get_monitor(argc, argv, self)
    int argc;
    VALUE* argv;
    VALUE self;
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
gdkscreen_broadcast_client_message(self, event)
    VALUE self, event;
{
    gdk_screen_broadcast_client_message(_SELF(self), RVAL2GEV(event));
    return self;
}

/*
  type: String, Integer, Gdk::Color.
 */
static VALUE
gdkscreen_get_setting(argc, argv, self)
    int argc;
    VALUE* argv;
    VALUE self;
{
    VALUE name, type;
    GType gtype;
    GValue val = {0,};
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

static void
child_setup(func)
    gpointer func;
{
    if (! NIL_P(func)){
        rb_funcall((VALUE)func, id_call, 0);
    }
}

#if GTK_CHECK_VERSION(2,4,0)
static VALUE
gdkscreen_spawn_on_screen(self, working_directory, argv, envp, flags)
    VALUE self, working_directory, argv, envp, flags;
{
    GError *err = NULL;
    gboolean ret;
    gint child_pid;
    VALUE func = Qnil;
    gint gargc, genc, i;
    gchar** gargv = (gchar**)NULL;
    gchar** genvp = (gchar**)NULL;

    if (rb_block_given_p()) {
        func = G_BLOCK_PROC();
        G_RELATIVE(self, func);
    }

    if (! NIL_P(argv)){
        Check_Type(argv, T_ARRAY);
        gargc = RARRAY(argv)->len;
        gargv = ALLOCA_N(gchar*, gargc + 1);
        for (i = 0; i < gargc; i++) {
            if (TYPE(RARRAY(argv)->ptr[i]) == T_STRING) {
                gargv[i] = RVAL2CSTR(RARRAY(argv)->ptr[i]);
            }
            else {
                gargv[i] = "";
            }
        }
        gargv[gargc] = (gchar*)NULL;
    }

    if (! NIL_P(envp)){
        Check_Type(envp, T_ARRAY);
        genc = RARRAY(envp)->len;
        genvp = ALLOCA_N(gchar*, genc + 1);
        for (i = 0; i < genc; i++) {
            if (TYPE(RARRAY(envp)->ptr[i]) == T_STRING) {
                genvp[i] = RVAL2CSTR(RARRAY(envp)->ptr[i]);
            }
            else {
                genvp[i] = "";
            }
        }
        genvp[genc] = (gchar*)NULL;
    }

    ret = gdk_spawn_on_screen(_SELF(self),
                              NIL_P(working_directory) ? NULL : RVAL2CSTR(working_directory),
                              gargv, genvp, NUM2INT(flags),
                              child_setup, (gpointer)func,
                              &child_pid, &err);

    if (! ret){
        RAISE_GERROR(err);
    }
    
    return INT2NUM(child_pid);
}

static VALUE
gdkscreen_spawn_on_screen_with_pipes(self, working_directory, argv, envp, flags)
    VALUE self, working_directory, argv, envp, flags;
{
    GError *err = NULL;
    gboolean ret;
    gint child_pid;
    VALUE func = Qnil;
    gint gargc, genc, i;
    gchar** gargv = (gchar**)NULL;
    gchar** genvp = (gchar**)NULL;
    gint standard_input, standard_output, standard_error;

    if (rb_block_given_p()) {
        func = G_BLOCK_PROC();
        G_RELATIVE(self, func);
    }

    if (! NIL_P(argv)){
        Check_Type(argv, T_ARRAY);
        gargc = RARRAY(argv)->len;
        gargv = ALLOCA_N(gchar*, gargc + 1);
        for (i = 0; i < gargc; i++) {
            if (TYPE(RARRAY(argv)->ptr[i]) == T_STRING) {
                gargv[i] = RVAL2CSTR(RARRAY(argv)->ptr[i]);
            }
            else {
                gargv[i] = "";
            }
        }
        gargv[gargc] = (gchar*)NULL;
    }

    if (! NIL_P(envp)){
        Check_Type(envp, T_ARRAY);
        genc = RARRAY(envp)->len;
        genvp = ALLOCA_N(gchar*, genc + 1);
        for (i = 0; i < genc; i++) {
            if (TYPE(RARRAY(envp)->ptr[i]) == T_STRING) {
                genvp[i] = RVAL2CSTR(RARRAY(envp)->ptr[i]);
            }
            else {
                genvp[i] = "";
            }
        }
        genvp[genc] = (gchar*)NULL;
    }

    ret = gdk_spawn_on_screen_with_pipes(_SELF(self),
                                         NIL_P(working_directory) ? NULL : RVAL2CSTR(working_directory),
                                         gargv, genvp, NUM2INT(flags),
                                         child_setup, (gpointer)func,
                                         &child_pid, 
                                         &standard_input, &standard_output,
                                         &standard_error, &err);

    if (! ret) RAISE_GERROR(err);
    
    return rb_ary_new3(4, INT2NUM(child_pid), 
                       rb_funcall(rb_cIO, id_new, 1, INT2NUM(standard_input)),
                       rb_funcall(rb_cIO, id_new, 1, INT2NUM(standard_output)),
                       rb_funcall(rb_cIO, id_new, 1, INT2NUM(standard_error)));
}

static VALUE
gdkscreen_spawn_command_line_on_screen(self, command_line)
    VALUE self, command_line;
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
gdkscreen_xnumber(self)
    VALUE self;
{
    return INT2NUM(GDK_SCREEN_XNUMBER(_SELF(self)));
}
static VALUE
gdkscreen_supports_net_wm_hint(self, property)
    VALUE self, property;
{
    return CBOOL2RVAL(gdk_x11_screen_supports_net_wm_hint(_SELF(self),
                                                          RVAL2ATOM(property)));
}

static VALUE
gdkscreen_get_window_manager_name(self)
    VALUE self;
{
    return CSTR2RVAL(gdk_x11_screen_get_window_manager_name(_SELF(self)));
}
static VALUE
gdkscreen_get_screen_number(self)
    VALUE self;
{
    return INT2NUM(gdk_x11_screen_get_screen_number(_SELF(self)));
}
#endif

#endif

void 
Init_gtk_gdk_screen()
{
#if GTK_CHECK_VERSION(2,2,0)
    VALUE gdkScreen = G_DEF_CLASS(GDK_TYPE_SCREEN, "Screen", mGdk);

    id_new = rb_intern("new");

    rb_define_singleton_method(gdkScreen, "default", gdkscreen_default, 0);
    rb_define_method(gdkScreen, "default_colormap", gdkscreen_get_default_colormap, 0);
    rb_define_method(gdkScreen, "set_default_colormap", gdkscreen_set_default_colormap, 1);
    rb_define_method(gdkScreen, "system_colormap", gdkscreen_get_system_colormap, 0);
    rb_define_method(gdkScreen, "system_visual", gdkscreen_get_system_visual, 0);
    rb_define_method(gdkScreen, "rgb_colormap", gdkscreen_get_rgb_colormap, 0);
    rb_define_method(gdkScreen, "rgb_visual", gdkscreen_get_rgb_visual, 0);
    rb_define_method(gdkScreen, "root_window", gdkscreen_get_root_window, 0);
    rb_define_method(gdkScreen, "display", gdkscreen_get_display, 0);
    rb_define_method(gdkScreen, "number", gdkscreen_number, 0);
    rb_define_method(gdkScreen, "width", gdkscreen_width, 0);
    rb_define_method(gdkScreen, "height", gdkscreen_height, 0);
    rb_define_method(gdkScreen, "width_mm", gdkscreen_width_mm, 0);
    rb_define_method(gdkScreen, "height_mm", gdkscreen_height_mm, 0);
    rb_define_method(gdkScreen, "visuals", gdkscreen_list_visuals, 0);
    rb_define_method(gdkScreen, "toplevel_windows", gdkscreen_get_toplevel_windows, 0);
    rb_define_method(gdkScreen, "display_name", gdkscreen_make_display_name, 0);
    rb_define_method(gdkScreen, "n_monitors", gdkscreen_n_monitors, 0);
    rb_define_method(gdkScreen, "monitor_geometry", gdkscreen_monitor_geometry, 1);
    rb_define_method(gdkScreen, "get_monitor", gdkscreen_get_monitor, -1);
    rb_define_method(gdkScreen, "broadcast_client_message", gdkscreen_broadcast_client_message, 1);
    rb_define_method(gdkScreen, "get_setting", gdkscreen_get_setting, -1);

#if GTK_CHECK_VERSION(2,4,0)
    rb_define_method(gdkScreen, "spawn_on_screen", gdkscreen_spawn_on_screen, 4);
    rb_define_method(gdkScreen, "spawn_on_screen_with_pipes", gdkscreen_spawn_on_screen_with_pipes, 4);
    rb_define_method(gdkScreen, "spawn_command_line_on_screen", gdkscreen_spawn_command_line_on_screen, 1);
#endif

#ifdef GDK_WINDOWING_X11
    rb_define_method(gdkScreen, "xnumber", gdkscreen_xnumber, 0);
    rb_define_method(gdkScreen, "supports_net_wm_hint?", gdkscreen_supports_net_wm_hint, 0);
    rb_define_method(gdkScreen, "window_manager_name", gdkscreen_get_window_manager_name, 0);
    rb_define_method(gdkScreen, "screen_number", gdkscreen_get_screen_number, 0);
#endif
    G_DEF_SETTERS(gdkScreen);

  #ifdef GDK_WINDOWING_X11
    G_DEF_CLASS3("GdkScreenX11", "ScreenX11", mGdk);
  #endif
#endif
}
