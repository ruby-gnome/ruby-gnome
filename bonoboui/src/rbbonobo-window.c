/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbbonobo-window.c -

  $Author: mutoh $
  $Date: 2002/12/22 14:27:43 $

  Copyright (C) 2002 Masao Mutoh <mutoh@highway.ne.jp>
************************************************/

#include "rbbonoboui.h"

#define _SELF(self) (BONOBO_WINDOW(RVAL2GOBJ(self)))

static VALUE
rbonobo_win_construct(self, ui_container, win_name, title)
    VALUE self, ui_container, win_name, title;
{
    return GOBJ2RVAL(bonobo_window_construct(_SELF(self),
                                             RVAL2GOBJ(ui_container),
                                             RVAL2CSTR(win_name),
                                             RVAL2CSTR(title)));
}

static VALUE
rbonobo_win_initialize(self, win_name, title)
    VALUE self, win_name, title;
{
    RBGTK_INITIALIZE(self, bonobo_window_new(RVAL2CSTR(win_name), RVAL2CSTR(title)));
    return Qnil;
}

static VALUE
rbonobo_win_set_contents(self, contents)
    VALUE self, contents;
{
    bonobo_window_set_contents(_SELF(self), GTK_WIDGET(RVAL2GOBJ(contents)));
    return self;
}

static VALUE
rbonobo_win_get_contents(self)
    VALUE self;
{
    return GOBJ2RVAL(bonobo_window_get_contents(_SELF(self)));
}

static VALUE
rbonobo_win_get_ui_engine(self)
    VALUE self;
{
    return GOBJ2RVAL(bonobo_window_get_ui_engine(_SELF(self)));
}

static VALUE
rbonobo_win_get_ui_container(self)
    VALUE self;
{
    return GOBJ2RVAL(bonobo_window_get_ui_container(_SELF(self)));
}

static VALUE
rbonobo_win_set_name(self, win_name)
    VALUE self, win_name;
{
    bonobo_window_set_name(_SELF(self), RVAL2CSTR(win_name));
    return self;
}

static VALUE
rbonobo_win_get_name(self)
    VALUE self;
{
    return CSTR2RVAL(bonobo_window_get_name(_SELF(self)));
}

static VALUE
rbonobo_win_get_accel_group(self)
    VALUE self;
{
    return GOBJ2RVAL(bonobo_window_get_accel_group(_SELF(self)));
}

static VALUE
rbonobo_win_add_popup(self, popup, path)
    VALUE self, popup, path;
{
    bonobo_window_add_popup(_SELF(self), GTK_MENU(RVAL2GOBJ(popup)),
                            RVAL2CSTR(path));
    return self;
}

/*
 * NB. popups are automaticaly removed on destroy, you probably don't
 * want to use this.
 */
static VALUE
rbonobo_win_remove_popup(self, path)
    VALUE self, path;
{
    bonobo_window_remove_popup(_SELF(self), RVAL2CSTR(path));
    return self;
}

void
Init_bonobo_window()
{
    VALUE bonoboWindow = G_DEF_CLASS(BONOBO_TYPE_WINDOW, "Window", mBonobo);

    rb_define_method(bonoboWindow, "construct", rbonobo_win_construct, 3);
    rb_define_method(bonoboWindow, "initialize", rbonobo_win_initialize, 2);
    rb_define_method(bonoboWindow, "set_contents", rbonobo_win_set_contents, 1);
    rb_define_method(bonoboWindow, "contents", rbonobo_win_get_contents, 0);
    rb_define_method(bonoboWindow, "ui_engine", rbonobo_win_get_ui_engine, 0);
    rb_define_method(bonoboWindow, "ui_container", rbonobo_win_get_ui_container, 0);
    rb_define_method(bonoboWindow, "set_name", rbonobo_win_set_name, 1);
    rb_define_method(bonoboWindow, "name", rbonobo_win_get_name, 0);
    rb_define_method(bonoboWindow, "accel_group", rbonobo_win_get_accel_group, 0);
    rb_define_method(bonoboWindow, "add_popup", rbonobo_win_add_popup, 2);
    rb_define_method(bonoboWindow, "remove_popup", rbonobo_win_remove_popup, 1);

    G_DEF_SETTERS(bonoboWindow);

}
