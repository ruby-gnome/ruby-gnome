/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtkmenubar.c -

  $Author: mutoh $
  $Date: 2002/06/22 19:50:57 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

static VALUE
mbar_initialize(self)
    VALUE self;
{
    RBGTK_INITIALIZE(self, gtk_menu_bar_new());
    return Qnil;
}

static VALUE
mbar_append(self, child)
    VALUE self, child;
{
    gtk_menu_bar_append(GTK_MENU_BAR(RVAL2GOBJ(self)), 
						GTK_WIDGET(RVAL2GOBJ(child)));
    return self;
}

static VALUE
mbar_prepend(self, child)
    VALUE self, child;
{
    gtk_menu_bar_prepend(GTK_MENU_BAR(RVAL2GOBJ(self)), 
						 GTK_WIDGET(RVAL2GOBJ(child)));
    return self;
}

static VALUE
mbar_insert(self, child, pos)
    VALUE self, child, pos;
{
    gtk_menu_bar_insert(GTK_MENU_BAR(RVAL2GOBJ(self)),
						GTK_WIDGET(RVAL2GOBJ(child)), NUM2INT(pos));
    return self;
}

void 
Init_gtk_menu_bar()
{
    static rbgtk_class_info cinfo;

    gMenuBar = rb_define_class_under(mGtk, "MenuBar", gMenuShell);
    cinfo.klass = gMenuBar;
    cinfo.gtype = GTK_TYPE_MENU_BAR;
    cinfo.mark = 0;
    cinfo.free = 0;
    rbgtk_register_class(&cinfo);

    rb_define_method(gMenuBar, "initialize", mbar_initialize, 0);
    rb_define_method(gMenuBar, "append", mbar_append, 1);
    rb_define_method(gMenuBar, "prepend", mbar_prepend, 1);
    rb_define_method(gMenuBar, "insert", mbar_insert, 2);
}
