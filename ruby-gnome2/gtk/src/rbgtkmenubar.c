/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtkmenubar.c -

  $Author: mutoh $
  $Date: 2002/05/19 12:29:33 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

static VALUE
mbar_initialize(self)
    VALUE self;
{
    set_widget(self, gtk_menu_bar_new());
    return Qnil;
}

static VALUE
mbar_append(self, child)
    VALUE self, child;
{
    gtk_menu_bar_append(GTK_MENU_BAR(get_widget(self)), get_widget(child));
    return self;
}

static VALUE
mbar_prepend(self, child)
    VALUE self, child;
{
    gtk_menu_bar_prepend(GTK_MENU_BAR(get_widget(self)), get_widget(child));
    return self;
}

static VALUE
mbar_insert(self, child, pos)
    VALUE self, child, pos;
{
    gtk_menu_bar_insert(GTK_MENU_BAR(get_widget(self)),
			get_widget(child), NUM2INT(pos));
    return self;
}

void Init_gtk_menu_bar()
{
    gMenuBar = rb_define_class_under(mGtk, "MenuBar", gMenuShell);

    rb_define_method(gMenuBar, "initialize", mbar_initialize, 0);
    rb_define_method(gMenuBar, "append", mbar_append, 1);
    rb_define_method(gMenuBar, "prepend", mbar_prepend, 1);
    rb_define_method(gMenuBar, "insert", mbar_insert, 2);
}
