/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtkmenushell.c -

  $Author: mutoh $
  $Date: 2002/06/22 19:50:57 $

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

void Init_gtk_menu_shell()
{
    static rbgtk_class_info cinfo;

    gMenuShell = rb_define_class_under(mGtk, "MenuShell", gContainer);
    cinfo.klass = gMenuShell;
    cinfo.gtype = GTK_TYPE_MENU_SHELL;
    cinfo.mark = 0;
    cinfo.free = 0;
    rbgtk_register_class(&cinfo);

    rb_define_const(gMenuShell, "SIGNAL_DEACTIVATE", rb_str_new2("deactivate"));

    rb_define_method(gMenuShell, "append", mshell_append, 1);
    rb_define_method(gMenuShell, "prepend", mshell_prepend, 1);
    rb_define_method(gMenuShell, "insert", mshell_insert, 2);
    rb_define_method(gMenuShell, "deactivate", mshell_deactivate, 0);

    /* child initialization */

    Init_gtk_menu();
    Init_gtk_menu_bar();
}
