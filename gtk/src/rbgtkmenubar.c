/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtkmenubar.c -

  $Author: mutoh $
  $Date: 2002/08/27 02:48:41 $

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

static VALUE
mbar_set_shadow_type(self, type)
    VALUE self, type;
{
    gtk_menu_bar_set_shadow_type(GTK_MENU_BAR(RVAL2GOBJ(self)), NUM2INT(type));
    return self;
}

void 
Init_gtk_menu_bar()
{
    VALUE gMenuBar = G_DEF_CLASS(GTK_TYPE_MENU_BAR, "MenuBar", mGtk);

    rb_define_method(gMenuBar, "initialize", mbar_initialize, 0);
    rb_define_method(gMenuBar, "append", mbar_append, 1);
    rb_define_method(gMenuBar, "prepend", mbar_prepend, 1);
    rb_define_method(gMenuBar, "insert", mbar_insert, 2);
    rb_define_method(gMenuBar, "set_shadow_type", mbar_set_shadow_type, 1);
}
