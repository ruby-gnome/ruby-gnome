/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtkmenushell.c -

  $Author: sakai $
  $Date: 2002/08/02 13:44:31 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

static VALUE
mshell_append(self, child)
    VALUE self, child;
{
    gtk_menu_shell_append(GTK_MENU_SHELL(RVAL2GOBJ(self)),
						  GTK_WIDGET(RVAL2GOBJ(child)));
    return self;
}

static VALUE
mshell_prepend(self, child)
    VALUE self, child;
{
    gtk_menu_shell_prepend(GTK_MENU_SHELL(RVAL2GOBJ(self)),
						   GTK_WIDGET(RVAL2GOBJ(child)));
    return self;
}

static VALUE
mshell_insert(self, child, pos)
    VALUE self, child, pos;
{
    gtk_menu_shell_insert(GTK_MENU_SHELL(RVAL2GOBJ(self)),
						  GTK_WIDGET(RVAL2GOBJ(child)),
						  NUM2INT(pos));
    return self;
}

static VALUE
mshell_deactivate(self)
    VALUE self;
{
    gtk_menu_shell_deactivate(GTK_MENU_SHELL(RVAL2GOBJ(self)));
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
}
