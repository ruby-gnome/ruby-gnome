/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkmenu.c -

  $Author: mutoh $
  $Date: 2003/01/19 14:28:25 $

  Copyright (C) 2002,2003 The Ruby-GNOME2 Project
  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

#define _SELF(self) (GTK_MENU(RVAL2GOBJ(self)))
#define RVAL2WIDGET(w) (GTK_WIDGET(RVAL2GOBJ(w)))

static ID call;

static VALUE
menu_initialize(self)
    VALUE self;
{
    RBGTK_INITIALIZE(self, gtk_menu_new());
    return Qnil;
}

/*
void        gtk_menu_set_screen             (GtkMenu *menu,
                                             GdkScreen *screen);
void        gtk_menu_reorder_child          (GtkMenu *menu,
                                             GtkWidget *child,
                                             gint position);
*/
static void
menu_pos_func(menu, px, py, data)
    GtkMenu *menu;
    gint *px, *py;
    gpointer data;
{
    VALUE arr = rb_funcall((VALUE)data, call, 3, GOBJ2RVAL(menu), 
                           INT2FIX(*px), INT2FIX(*py));
    Check_Type(arr, T_ARRAY);
    if (RARRAY(arr)->len != 2) {
        rb_raise(rb_eTypeError, "wrong number of result (%d for 2)", 
                 RARRAY(arr)->len);
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
        G_RELATIVE(self, func);
    }
    if (!NIL_P(pshell)){
        gpshell = RVAL2WIDGET(pshell);
    }
    if (!NIL_P(pitem)) {
        gpitem = RVAL2WIDGET(pitem);
    }
    
    gtk_menu_popup(_SELF(self), gpshell, gpitem,
                   pfunc, data, NUM2INT(button),
                   NUM2INT(activate_time));
    return self;
}

/*
void        gtk_menu_set_accel_group        (GtkMenu *menu,
                                             GtkAccelGroup *accel_group);
GtkAccelGroup* gtk_menu_get_accel_group     (GtkMenu *menu);
void        gtk_menu_set_accel_path         (GtkMenu *menu,
                                             const gchar *accel_path);
gboolean    gtk_menu_get_tearoff_state      (GtkMenu *menu);
G_CONST_RETURN gchar* gtk_menu_get_title    (GtkMenu *menu);
*/

static VALUE
menu_popdown(self)
    VALUE self;
{
    gtk_menu_popdown(_SELF(self));
    return self;
}

/*
void        gtk_menu_reposition             (GtkMenu *menu);
*/

static VALUE
menu_get_active(self)
    VALUE self;
{
    GtkWidget *mitem = gtk_menu_get_active(_SELF(self));

    return (mitem == NULL) ? Qnil : GOBJ2RVAL(mitem);
}

static VALUE
menu_set_active(self, active)
    VALUE self, active;
{
    gtk_menu_set_active(_SELF(self), NUM2INT(active));
    return self;
}

static VALUE
menu_set_tearoff_state(self, torn_off)
    VALUE self, torn_off;
{
    gtk_menu_set_tearoff_state(_SELF(self), RTEST(torn_off));
    return self;
}

/*
void        gtk_menu_attach_to_widget       (GtkMenu *menu,
                                             GtkWidget *attach_widget,
                                             GtkMenuDetachFunc detacher);
*/

static VALUE
menu_detach(self)
    VALUE self;
{
    gtk_menu_detach(_SELF(self));
    return self;
}

static VALUE
menu_get_attach_widget(self)
    VALUE self;
{
    return GOBJ2RVAL(gtk_menu_get_attach_widget(_SELF(self)));
}

/*
void        (*GtkMenuDetachFunc)            (GtkWidget *attach_widget,
                                            GtkMenu *menu);
*/

void 
Init_gtk_menu()
{
    VALUE gMenu = G_DEF_CLASS(GTK_TYPE_MENU, "Menu", mGtk);

    rb_define_method(gMenu, "initialize", menu_initialize, 0);
    rb_define_method(gMenu, "popup", menu_popup, 5);
    rb_define_method(gMenu, "popdown", menu_popdown, 0);
    rb_define_method(gMenu, "active", menu_get_active, 0);
    rb_define_method(gMenu, "set_active", menu_set_active, 1);
    rb_define_method(gMenu, "set_tearoff_state", menu_set_tearoff_state, 1);
    rb_define_method(gMenu, "detach", menu_detach, 0);
    rb_define_method(gMenu, "attach_widget", menu_get_attach_widget, 0);
 
    call = rb_intern("call");

    G_DEF_SETTERS(gMenu);
}
