/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkoptionmenu.c -

  $Author: mutoh $
  $Date: 2004/05/30 16:41:13 $

  Copyright (C) 2002,2003 Ruby-GNOME2 Project Team
  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

#ifndef GTK_DISABLE_DEPRECATED  /* Since 2.4 */

static VALUE
omenu_initialize(self)
    VALUE self;
{
    RBGTK_INITIALIZE(self, gtk_option_menu_new());
    return Qnil;
}

static VALUE
omenu_remove_menu(self)
    VALUE self;
{
    gtk_option_menu_remove_menu(GTK_OPTION_MENU(RVAL2GOBJ(self)));
    return self;
}

static VALUE
omenu_set_history(self, index)
    VALUE self, index;
{
    gtk_option_menu_set_history(GTK_OPTION_MENU(RVAL2GOBJ(self)), NUM2INT(index));
    return self;
}

static VALUE
omenu_get_history(self)
    VALUE self;
{
    return INT2NUM(gtk_option_menu_get_history(GTK_OPTION_MENU(RVAL2GOBJ(self))));
}

#endif

void 
Init_gtk_option_menu()
{
#ifndef GTK_DISABLE_DEPRECATED  /* Since 2.4 */
    VALUE gOptionMenu = G_DEF_CLASS(GTK_TYPE_OPTION_MENU, "OptionMenu", mGtk);

    rb_define_method(gOptionMenu, "initialize", omenu_initialize, 0);
    rb_define_method(gOptionMenu, "remove_menu", omenu_remove_menu, 0);
    rb_define_method(gOptionMenu, "set_history", omenu_set_history, 1);
    rb_define_method(gOptionMenu, "history", omenu_get_history, 0);

    G_DEF_SETTERS(gOptionMenu);
#endif
}
