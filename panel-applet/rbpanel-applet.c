/************************************************

  rbapplet.c: Wrapper for AppletWidget in libpanel

  $Author: mutoh $
  $Date: 2002/05/19 12:39:31 $

  Copyright (C) 2001 Hiroshi Igarashi
  Copyright (C) 2002 Neil Conway

************************************************/

#include <rbgnome.h>
#include <applet-widget.h>

static VALUE cAppletWidget;
static VALUE mGnomePanel;

static VALUE
rb_s_applet_widget_gtk_main(self)
    VALUE self;
{
    applet_widget_gtk_main();
    return Qnil;
}

static VALUE
rb_s_applet_widget_gtk_main_quit(self)
    VALUE self;
{
    applet_widget_gtk_main_quit();
    return Qnil;
}

static VALUE
rb_s_applet_widget_get_applet_count(self)
    VALUE self;
{
    return NUM2INT(applet_widget_get_applet_count());
}

static VALUE
rb_s_applet_widget_panel_quit(self)
    VALUE self;
{
    applet_widget_panel_quit();
    return Qnil;
}

static VALUE
rb_applet_widget_initialize(self, goad_id)
    VALUE self, goad_id;
{
    GtkWidget *applet = applet_widget_new(STR2CSTR(goad_id));
    set_widget(self, applet);

    return Qnil;
}

static VALUE
rb_applet_widget_add(self, widget)
    VALUE self, widget;
{
    applet_widget_add(APPLET_WIDGET(get_widget(self)), get_widget(widget));
    return self;
}

static VALUE
rb_applet_widget_panel_orient(self)
    VALUE self;
{
    return INT2NUM(
            applet_widget_get_panel_orient(APPLET_WIDGET(get_widget(self))));
}

static VALUE
rb_applet_widget_panel_pixel_size(self)
    VALUE self;
{
    return INT2NUM(
            applet_widget_get_panel_pixel_size(APPLET_WIDGET(get_widget(self))));
}

static VALUE
rb_applet_widget_get_free_space(self)
    VALUE self;
{
    return INT2NUM(applet_widget_get_free_space(APPLET_WIDGET(get_widget(self))));
}

static VALUE
rb_applet_widget_send_position(self, enable)
    VALUE self, enable;
{
    applet_widget_send_position(APPLET_WIDGET(get_widget(self)),
                                RTEST(enable));
    return Qnil;
}

static VALUE
rb_applet_widget_send_draw(self, enable)
    VALUE self, enable;
{
    applet_widget_send_draw(APPLET_WIDGET(get_widget(self)),
                            RTEST(enable));
    return Qnil;
}

static VALUE
rb_applet_widget_sync_config(self)
    VALUE self;
{
    applet_widget_sync_config(APPLET_WIDGET(get_widget(self)));
    return Qnil;
}

static VALUE
rb_applet_widget_queue_resize(self)
    VALUE self;
{
    applet_widget_queue_resize(APPLET_WIDGET(get_widget(self)));
    return Qnil;
}

void
Init_panel_applet()
{
    int argc, i;
    char **argv;

    /* setup args */
    argc = RARRAY(rb_argv)->len;
    argv = ALLOCA_N(char*,argc+1);
    argv[0] = STR2CSTR(rb_argv0);
    for (i = 0; i < argc; ++i) {
        if (TYPE(RARRAY(rb_argv)->ptr[i]) == T_STRING) {
            argv[i+1] = RSTRING(RARRAY(rb_argv)->ptr[i])->ptr;
        } else {
            argv[i+1] = "";
        }
    }
    ++argc;

    applet_widget_init(argv[0], "0.0.0", argc, argv, NULL, 0, NULL);

    /* sync args */
    for (i = 1; i < argc; ++i) {
        RARRAY(rb_argv)->ptr[i-1] = rb_obj_taint(rb_str_new2(argv[i]));
    }
    RARRAY(rb_argv)->len = argc-1;

    /* define classes and methods */
    cAppletWidget = rb_define_class_under(mGnome, "AppletWidget", gPlug);
    rb_define_singleton_method(cAppletWidget, "main", rb_s_applet_widget_gtk_main, 0);
    rb_define_singleton_method(cAppletWidget, "main_quit", rb_s_applet_widget_gtk_main_quit, 0);
    rb_define_singleton_method(cAppletWidget, "applet_count", rb_s_applet_widget_get_applet_count, 0);
    rb_define_singleton_method(cAppletWidget, "panel_quit", rb_s_applet_widget_panel_quit, 0);
    rb_define_method(cAppletWidget, "initialize", rb_applet_widget_initialize, 1);
    rb_define_method(cAppletWidget, "add", rb_applet_widget_add, 1);
    rb_define_method(cAppletWidget, "panel_orient", rb_applet_widget_panel_orient, 0);
    rb_define_method(cAppletWidget, "panel_pixel_size", rb_applet_widget_panel_pixel_size, 0);
    rb_define_method(cAppletWidget, "free_space", rb_applet_widget_get_free_space, 0);
    rb_define_method(cAppletWidget, "send_position", rb_applet_widget_send_position, 1);
    rb_define_method(cAppletWidget, "send_draw", rb_applet_widget_send_draw, 1);
    rb_define_method(cAppletWidget, "sync_config", rb_applet_widget_sync_config, 0);
    rb_define_method(cAppletWidget, "queue_resize", rb_applet_widget_queue_resize, 0);

    /* signals */
    rb_define_const(cAppletWidget, "SIGNAL_CHANGE_ORIENT", rb_str_new2("change_orient"));
    rb_define_const(cAppletWidget, "SIGNAL_CHANGE_PIXEL_SIZE", rb_str_new2("change_pixel_size"));
    rb_define_const(cAppletWidget, "SIGNAL_SAVE_SESSION", rb_str_new2("save_session"));
    rb_define_const(cAppletWidget, "SIGNAL_BACK_CHANGE", rb_str_new2("back_change"));
    rb_define_const(cAppletWidget, "SIGNAL_DO_DRAW", rb_str_new2("do_draw"));
    rb_define_const(cAppletWidget, "SIGNAL_TOOLTIP_STATE", rb_str_new2("tooltip_state"));
    rb_define_const(cAppletWidget, "SIGNAL_CHANGE_POSITION", rb_str_new2("change_position"));

    /* constants */
    mGnomePanel = rb_define_module_under(mGnome, "Panel");
    rb_define_const(mGnomePanel, "ORIENT_UP", INT2NUM(GNOME_Panel_ORIENT_UP));
    rb_define_const(mGnomePanel, "ORIENT_DOWN", INT2NUM(GNOME_Panel_ORIENT_DOWN));
    rb_define_const(mGnomePanel, "ORIENT_LEFT", INT2NUM(GNOME_Panel_ORIENT_LEFT));
    rb_define_const(mGnomePanel, "ORIENT_RIGHT", INT2NUM(GNOME_Panel_ORIENT_RIGHT));
}
