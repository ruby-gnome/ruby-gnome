/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtkmenu.c -

  $Author: mutoh $
  $Date: 2002/05/19 12:29:34 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

static VALUE
menu_initialize(self)
    VALUE self;
{
    set_widget(self, gtk_menu_new());
    return Qnil;
}

static VALUE
menu_append(self, child)
    VALUE self, child;
{
    gtk_menu_append(GTK_MENU(get_widget(self)), get_widget(child));
    return self;
}

static VALUE
menu_prepend(self, child)
    VALUE self, child;
{
    gtk_menu_prepend(GTK_MENU(get_widget(self)), get_widget(child));
    return self;
}

static VALUE
menu_insert(self, child, pos)
    VALUE self, child, pos;
{
    gtk_menu_insert(GTK_MENU(get_widget(self)),
		    get_widget(child), NUM2INT(pos));
    return self;
}

static void
menu_pos_func(menu, x, y, data)
    GtkMenu *menu;
    gint x, y;
    gpointer data;
{
    VALUE m = get_value_from_gobject(GTK_OBJECT(menu));

    rb_funcall((VALUE)data, 3, m, INT2FIX(x), INT2FIX(y));
}

static VALUE
menu_popup(self, pshell, pitem, func, button, activate_time)
    VALUE self, pshell, pitem, func, button, activate_time;
{
    GtkWidget *gpshell = NULL;
    GtkWidget *gpitem = NULL;
    GtkMenuPositionFunc pfunc = NULL;
    gpointer data = NULL;

    if (!NIL_P(func)) {
	pfunc = menu_pos_func;
	data = (gpointer)func;
	add_relative(self, func);
    }
    if (!NIL_P(pshell)){
	gpshell = get_widget(pshell);
    }
    if (!NIL_P(pitem)) {
	gpitem = get_widget(pitem);
    }

    gtk_menu_popup(GTK_MENU(get_widget(self)),
		   gpshell, gpitem,
		   pfunc,
		   data,
		   NUM2INT(button),
		   NUM2INT(activate_time));
    return self;
}

static VALUE
menu_popdown(self)
    VALUE self;
{
    gtk_menu_popdown(GTK_MENU(get_widget(self)));
    return self;
}

static VALUE
menu_get_active(self)
    VALUE self;
{
    GtkWidget *mitem = gtk_menu_get_active(GTK_MENU(get_widget(self)));

    return (mitem == NULL) ? Qnil : get_value_from_gobject(GTK_OBJECT(mitem));
}

static VALUE
menu_set_active(self, active)
    VALUE self, active;
{
    gtk_menu_set_active(GTK_MENU(get_widget(self)), NUM2INT(active));
    return self;
}

void Init_gtk_menu()
{
    gMenu = rb_define_class_under(mGtk, "Menu", gMenuShell);

    rb_define_method(gMenu, "initialize", menu_initialize, 0);
    rb_define_method(gMenu, "append", menu_append, 1);
    rb_define_method(gMenu, "prepend", menu_prepend, 1);
    rb_define_method(gMenu, "insert", menu_insert, 2);
    rb_define_method(gMenu, "popup", menu_popup, 5);
    rb_define_method(gMenu, "popdown", menu_popdown, 0);
    rb_define_method(gMenu, "get_active", menu_get_active, 0);
    rb_define_method(gMenu, "set_active", menu_set_active, 1);
}
