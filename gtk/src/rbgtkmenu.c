/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtkmenu.c -

  $Author: igapy $
  $Date: 2002/05/30 00:46:41 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

static ID call;

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
menu_pos_func(menu, px, py, data)
    GtkMenu *menu;
    gint *px, *py;
    gpointer data;
{
    VALUE arr;
    VALUE m = get_value_from_gobject(GTK_OBJECT(menu));

    arr = rb_funcall((VALUE)data, call, 3, m, INT2FIX(*px), INT2FIX(*py));
    Check_Type(arr, T_ARRAY);
    if (RARRAY(arr)->len != 2) {
	rb_raise(rb_eTypeError, "wrong number of result (%d for 2)", RARRAY(arr)->len);
    }
    *px = NUM2INT(RARRAY(arr)->ptr[0]);
    *py = NUM2INT(RARRAY(arr)->ptr[1]);
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
    static rbgtk_class_info cinfo;

    gMenu = rb_define_class_under(mGtk, "Menu", gMenuShell);
    cinfo.klass = gMenu;
    cinfo.gtype = GTK_TYPE_MENU;
    cinfo.mark = 0;
    cinfo.free = 0;
    rbgtk_register_class(&cinfo);

    rb_define_method(gMenu, "initialize", menu_initialize, 0);
    rb_define_method(gMenu, "append", menu_append, 1);
    rb_define_method(gMenu, "prepend", menu_prepend, 1);
    rb_define_method(gMenu, "insert", menu_insert, 2);
    rb_define_method(gMenu, "popup", menu_popup, 5);
    rb_define_method(gMenu, "popdown", menu_popdown, 0);
    rb_define_method(gMenu, "get_active", menu_get_active, 0);
    rb_define_method(gMenu, "set_active", menu_set_active, 1);

    call = rb_intern("call");
}
