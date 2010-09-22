/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkmenushell.c -

  $Author: ggc $
  $Date: 2007/07/13 16:07:31 $

  Copyright (C) 2002-2004 Ruby-GNOME2 Project Team
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
    G_CHILD_ADD(self, child);
    return self;
}

static VALUE
mshell_prepend(self, child)
    VALUE self, child;
{
    gtk_menu_shell_prepend(_SELF(self), RVAL2WIDGET(child));
    G_CHILD_ADD(self, child);
    return self;
}

static VALUE
mshell_insert(self, child, pos)
    VALUE self, child, pos;
{
    gtk_menu_shell_insert(_SELF(self), RVAL2WIDGET(child),
                          NUM2INT(pos));
    G_CHILD_ADD(self, child);
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

#if GTK_CHECK_VERSION(2,2,0)
static VALUE
mshell_select_first(self, search_sensitive)
    VALUE self, search_sensitive;
{
    gtk_menu_shell_select_first(_SELF(self), RVAL2CBOOL(search_sensitive));
    return self;
}
#endif

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
                                 RVAL2CBOOL(force_deactivate));
    return self;
}

#if GTK_CHECK_VERSION(2,4,0)
static VALUE
mshell_cancel(self)
    VALUE self;
{
    gtk_menu_shell_cancel(_SELF(self));
    return self;
}
#endif

/* Defined as properties
void        gtk_menu_shell_set_take_focus   (GtkMenuShell *menu_shell,
                                             gboolean take_focus);
gboolean    gtk_menu_shell_get_take_focus   (GtkMenuShell *menu_shell);
*/

void 
Init_gtk_menu_shell()
{
    VALUE gMenuShell = G_DEF_CLASS(GTK_TYPE_MENU_SHELL, "MenuShell", mGtk);

    rb_define_method(gMenuShell, "append", mshell_append, 1);
    rb_define_method(gMenuShell, "prepend", mshell_prepend, 1);
    rb_define_method(gMenuShell, "insert", mshell_insert, 2);
    rb_define_method(gMenuShell, "deactivate", mshell_deactivate, 0);
    rb_define_method(gMenuShell, "select_item", mshell_select_item, 1);
#if GTK_CHECK_VERSION(2,2,0)
    rb_define_method(gMenuShell, "select_first", mshell_select_first, 1);
#endif
    rb_define_method(gMenuShell, "deselect", mshell_deselect, 0);
    rb_define_method(gMenuShell, "activate_item", mshell_activate_item, 2);
#if GTK_CHECK_VERSION(2,4,0)
    rb_define_method(gMenuShell, "cancel", mshell_cancel, 0);
#endif
    /* GtkMenuDirectionType */
    G_DEF_CLASS(GTK_TYPE_MENU_DIRECTION_TYPE, "DirectionType", gMenuShell);
    G_DEF_CONSTANTS(gMenuShell, GTK_TYPE_MENU_DIRECTION_TYPE, "GTK_MENU_");
}
