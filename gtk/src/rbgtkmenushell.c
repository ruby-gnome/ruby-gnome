/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkmenushell.c -

  $Author: sakai $
  $Date: 2003/08/20 17:07:03 $

  Copyright (C) 2002,2003 Ruby-GNOME2 Project Team
  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

#define _SELF(self) (GTK_MENU_SHELL(RVAL2GOBJ(self)))
#define RVAL2WIDGET(w) (GTK_WIDGET(RVAL2GOBJ(w)))

static VALUE
mshell_append(self, child)
    VALUE self, child;
{
    gtk_menu_shell_append(_SELF(self),RVAL2WIDGET(child));
    return self;
}

static VALUE
mshell_prepend(self, child)
    VALUE self, child;
{
    gtk_menu_shell_prepend(_SELF(self), RVAL2WIDGET(child));
    return self;
}

static VALUE
mshell_insert(self, child, pos)
    VALUE self, child, pos;
{
    gtk_menu_shell_insert(_SELF(self), RVAL2WIDGET(child),
                          NUM2INT(pos));
    return self;
}

static VALUE
mshell_deactivate(self)
    VALUE self;
{
    gtk_menu_shell_deactivate(_SELF(self));
    return self;
}

static VALUE
mshell_select_item(self, menu_item)
    VALUE self, menu_item;
{
    gtk_menu_shell_select_item(_SELF(self), RVAL2WIDGET(menu_item));
    return self;
}

static VALUE
mshell_deselect(self)
    VALUE self;
{
    gtk_menu_shell_deselect(_SELF(self));
    return self;
}

static VALUE
mshell_activate_item(self, menu_item, force_deactivate)
    VALUE self, menu_item, force_deactivate;
{
    gtk_menu_shell_activate_item(_SELF(self), RVAL2WIDGET(menu_item), 
                                 RTEST(force_deactivate));
    return self;
}

void 
Init_gtk_menu_shell()
{
    VALUE gMenuShell = G_DEF_CLASS(GTK_TYPE_MENU_SHELL, "MenuShell", mGtk);

    rb_define_method(gMenuShell, "append", mshell_append, 1);
    rb_define_method(gMenuShell, "prepend", mshell_prepend, 1);
    rb_define_method(gMenuShell, "insert", mshell_insert, 2);
    rb_define_method(gMenuShell, "deactivate", mshell_deactivate, 0);
    rb_define_method(gMenuShell, "select_item", mshell_select_item, 1);
    rb_define_method(gMenuShell, "deselect", mshell_deselect, 0);
    rb_define_method(gMenuShell, "activate_item", mshell_activate_item, 2);

    /* GtkMenuDirectionType */
    G_DEF_CLASS(GTK_TYPE_MENU_DIRECTION_TYPE, "DirectionType", gMenuShell);
    G_DEF_CONSTANTS(gMenuShell, GTK_TYPE_MENU_DIRECTION_TYPE, "GTK_MENU_");
}
