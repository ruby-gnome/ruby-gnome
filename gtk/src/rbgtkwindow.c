/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtkwindow.c -

  $Author: mutoh $
  $Date: 2002/06/22 19:50:57 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

static VALUE
gwin_initialize(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE type;
    GtkWindowType tp;
    GtkWidget *widget;

    if (rb_scan_args(argc, argv, "01", &type) == 1) {
		tp = NUM2INT(type);
    } else {
		tp = GTK_WINDOW_TOPLEVEL;
    }
    widget = gtk_window_new(tp);

    RBGTK_INITIALIZE(self, widget);
    return Qnil;
}

static VALUE
gwin_set_policy(self, shrink, grow, auto_shrink)
    VALUE self, shrink, grow, auto_shrink;
{
    gtk_window_set_policy(GTK_WINDOW(RVAL2GOBJ(self)),
						  RTEST(shrink), RTEST(grow), RTEST(auto_shrink));
    return self;
}

static VALUE
gwin_set_title(self, title)
    VALUE self, title;
{
    gtk_window_set_title(GTK_WINDOW(RVAL2GOBJ(self)), STR2CSTR(title));
    return self;
}

static VALUE
gwin_set_position(self, pos)
    VALUE self, pos;
{
    gtk_window_position(GTK_WINDOW(RVAL2GOBJ(self)),
						(GtkWindowPosition)NUM2INT(pos));

    return self;
}

static VALUE
gwin_set_wmclass(self, wmclass1, wmclass2)
    VALUE self, wmclass1, wmclass2;
{
    gtk_window_set_wmclass(GTK_WINDOW(RVAL2GOBJ(self)),
						   NIL_P(wmclass1)?NULL:STR2CSTR(wmclass1),
						   NIL_P(wmclass2)?NULL:STR2CSTR(wmclass2));
    return self;
}

static VALUE
gwin_set_focus(self, win)
    VALUE self, win;
{
    gtk_window_set_focus(GTK_WINDOW(RVAL2GOBJ(self)), 
						 GTK_WIDGET(RVAL2GOBJ(win)));
    return self;
}

static VALUE
gwin_set_default_size(self, w, h)
    VALUE self, w, h;
{
    gtk_window_set_default_size(GTK_WINDOW(RVAL2GOBJ(self)),
								NUM2INT(w), NUM2INT(h));
    return self;
}

static VALUE
gwin_set_default(self, win)
    VALUE self, win;
{
    gtk_window_set_default(GTK_WINDOW(RVAL2GOBJ(self)), 
						   GTK_WIDGET(RVAL2GOBJ(win)));
    return self;
}

static VALUE
gwin_add_accel(self, accel)
    VALUE self, accel;
{
    gtk_window_add_accel_group(GTK_WINDOW(RVAL2GOBJ(self)),
							   get_gtkaccelgrp(accel));
    return self;
}

static VALUE
gwin_rm_accel(self, accel)
    VALUE self, accel;
{
    gtk_window_remove_accel_group(GTK_WINDOW(RVAL2GOBJ(self)),
								  get_gtkaccelgrp(accel));
    return self;
}

static VALUE
gwin_grab_add(self)
    VALUE self;
{
    gtk_grab_add(GTK_WIDGET(RVAL2GOBJ(self)));
    return self;
}

static VALUE
gwin_grab_remove(self)
    VALUE self;
{
    gtk_grab_remove(GTK_WIDGET(RVAL2GOBJ(self)));
    return self;
}

static VALUE
gwin_set_modal(self, modal)
    VALUE self, modal;
{
    gtk_window_set_modal(GTK_WINDOW(RVAL2GOBJ(self)), RTEST(modal));
    return self;
}

static VALUE
gwin_set_transient_for(self, parent)
    VALUE self, parent;
{
    gtk_window_set_transient_for(GTK_WINDOW(RVAL2GOBJ(self)),
								 GTK_WINDOW(RVAL2GOBJ(parent)));
    return self;
}

static VALUE
gwin_set_geometry_hints(self, geometry_widget, geometry, geom_mask)
    VALUE self, geometry_widget, geometry, geom_mask;
{
    gtk_window_set_geometry_hints(GTK_WINDOW(RVAL2GOBJ(self)),
								  GTK_WIDGET(RVAL2GOBJ(geometry_widget)),
								  rbgdk_geometry_get(geometry),
								  NUM2INT(geom_mask));

    return self;
}

void Init_gtk_window()
{
    static rbgtk_class_info cinfo;

    gWindow = rb_define_class_under(mGtk, "Window", gBin);
    cinfo.klass = gWindow;
    cinfo.gtype = GTK_TYPE_WINDOW;
    cinfo.mark = 0;
    cinfo.free = 0;
    rbgtk_register_class(&cinfo);

    rb_define_const(gWindow, "SIGNAL_MOVE_RESIZE", rb_str_new2("move_resize"));
    rb_define_const(gWindow, "SIGNAL_SET_FOCUS", rb_str_new2("set_focus"));

    rb_define_method(gWindow, "initialize", gwin_initialize, -1);
    rb_define_method(gWindow, "set_title", gwin_set_title, 1);
    rb_define_method(gWindow, "set_policy", gwin_set_policy, 3);
    rb_define_method(gWindow, "set_wmclass", gwin_set_wmclass, 2);
    rb_define_method(gWindow, "set_focus", gwin_set_focus, 1);
    rb_define_method(gWindow, "set_default_size", gwin_set_default_size, 2);
    rb_define_method(gWindow, "set_default", gwin_set_default, 1);
    rb_define_method(gWindow, "add_accel_group", gwin_add_accel, 1);
    rb_define_method(gWindow, "remove_accel_group", gwin_rm_accel, 1);
    rb_define_method(gWindow, "position", gwin_set_position, 1);
    rb_define_method(gWindow, "set_position", gwin_set_position, 1);
    rb_define_method(gWindow, "grab_add", gwin_grab_add, 0);
    rb_define_method(gWindow, "grab_remove", gwin_grab_remove, 0);
    rb_define_method(gWindow, "set_modal", gwin_set_modal, 1);
    rb_define_method(gWindow, "set_transient_for", gwin_set_transient_for, 1);
    rb_define_method(gWindow, "set_geometry_hints", gwin_set_geometry_hints, 3);

    /* child initialization */
    Init_gtk_color_selection_dialog();
    Init_gtk_dialog();
    Init_gtk_file_selection();
    Init_gtk_font_selection_dialog();
    Init_gtk_plug();
}
