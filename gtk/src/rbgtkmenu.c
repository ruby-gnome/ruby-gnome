/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkmenu.c -

  $Author: mutoh $
  $Date: 2003/10/04 15:25:57 $

  Copyright (C) 2002,2003 Ruby-GNOME2 Project Team
  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

#define _SELF(self) (GTK_MENU(RVAL2GOBJ(self)))
#define RVAL2WIDGET(w) (GTK_WIDGET(RVAL2GOBJ(w)))

static VALUE
menu_initialize(self)
    VALUE self;
{
    RBGTK_INITIALIZE(self, gtk_menu_new());
    return Qnil;
}

static VALUE
menu_reorder_child(self, child, position)
    VALUE self, child, position;
{
    gtk_menu_reorder_child(_SELF(self), GTK_WIDGET(RVAL2GOBJ(child)),
                           NUM2INT(position));
    return self;
}

static void
menu_pos_func(menu, px, py, push_in, data)
    GtkMenu *menu;
    gint *px, *py;
    gboolean *push_in;
    gpointer data;
{
    VALUE arr = rb_funcall((VALUE)data, id_call, 4, GOBJ2RVAL(menu), 
                           INT2FIX(*px), INT2FIX(*py), 
                           *push_in ? Qtrue : Qfalse);

    if (TYPE(arr) == T_ARRAY && (RARRAY(arr)->len == 2 || RARRAY(arr)->len == 3)){
        *px = NUM2INT(RARRAY(arr)->ptr[0]);
        *py = NUM2INT(RARRAY(arr)->ptr[1]);
        if (RARRAY(arr)->len == 3)
            *push_in = RVAL2CBOOL(RARRAY(arr)->ptr[2]);
    } else {
        rb_raise(rb_eArgError, "block should return [x, y, push_in]"); 
    } 
}

/* the proc should return [x, y, push_in] */
static VALUE
menu_popup(self, pshell, pitem, button, activate_time)
    VALUE self, pshell, pitem, button, activate_time;
{
    GtkWidget *gpshell = NULL;
    GtkWidget *gpitem = NULL;
    GtkMenuPositionFunc pfunc = NULL;
    gpointer data = NULL;
    VALUE func;

    if (rb_block_given_p()) {
        func = G_BLOCK_PROC();
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

static VALUE
menu_set_accel_group(self, accel_group)
    VALUE self, accel_group;
{
    gtk_menu_set_accel_group(_SELF(self), 
                             GTK_ACCEL_GROUP(RVAL2GOBJ(accel_group)));
    return self;
}

static VALUE
menu_get_accel_group(self)
    VALUE self;
{
    return GOBJ2RVAL(gtk_menu_get_accel_group(_SELF(self)));
}

static VALUE
menu_set_accel_path(self, accel_path)
    VALUE self, accel_path;
{
    gtk_menu_set_accel_path(_SELF(self), RVAL2CSTR(accel_path));
    return self;
}

static VALUE
menu_get_tearoff_state(self)
    VALUE self;
{
    return gtk_menu_get_tearoff_state(_SELF(self)) ? Qtrue : Qfalse;
}

static VALUE
menu_popdown(self)
    VALUE self;
{
    gtk_menu_popdown(_SELF(self));
    return self;
}

static VALUE
menu_reposition(self)
    VALUE self;
{
    gtk_menu_reposition(_SELF(self));
    return self;
}

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

static VALUE menu_detacher;
static void
detach_func(attach_widget, menu)
    GtkWidget* attach_widget;
    GtkMenu* menu;
{
    rb_funcall((VALUE)menu_detacher, id_call, 2, 
               GOBJ2RVAL(attach_widget), GOBJ2RVAL(menu));    
}

static VALUE
menu_attach_to_widget(self, attach_widget)
    VALUE self, attach_widget;
{
    menu_detacher = G_BLOCK_PROC();
    G_RELATIVE(self, menu_detacher);
    gtk_menu_attach_to_widget(_SELF(self),
                              GTK_WIDGET(RVAL2GOBJ(attach_widget)),
                              (GtkMenuDetachFunc)detach_func);
    return self;
}

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

void 
Init_gtk_menu()
{
    VALUE gMenu = G_DEF_CLASS(GTK_TYPE_MENU, "Menu", mGtk);

    rb_define_method(gMenu, "initialize", menu_initialize, 0);
    rb_define_method(gMenu, "reorder_child", menu_reorder_child, 2);
    rb_define_method(gMenu, "popup", menu_popup, 4);
    rb_define_method(gMenu, "popdown", menu_popdown, 0);
    rb_define_method(gMenu, "reposition", menu_reposition, 0);
    rb_define_method(gMenu, "set_accel_group", menu_set_accel_group, 1);
    rb_define_method(gMenu, "accel_group", menu_get_accel_group, 0);
    rb_define_method(gMenu, "set_accel_path", menu_set_accel_path, 1);
    rb_define_method(gMenu, "tearoff_state?", menu_get_tearoff_state, 0);
    rb_define_method(gMenu, "active", menu_get_active, 0);
    rb_define_method(gMenu, "set_active", menu_set_active, 1);
    rb_define_method(gMenu, "set_tearoff_state", menu_set_tearoff_state, 1);
    rb_define_method(gMenu, "detach", menu_detach, 0);
    rb_define_method(gMenu, "attach_widget", menu_get_attach_widget, 0);
    rb_define_method(gMenu, "attach_to_widget", menu_attach_to_widget, 1);
 
    G_DEF_SETTERS(gMenu);
}
