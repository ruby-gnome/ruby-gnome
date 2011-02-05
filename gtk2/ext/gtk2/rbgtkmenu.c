/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkmenu.c -

  $Author: ggc $
  $Date: 2007/07/13 16:07:31 $

  Copyright (C) 2002-2006 Ruby-GNOME2 Project Team
  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

#define _SELF(self) (GTK_MENU(RVAL2GOBJ(self)))
#define RVAL2WIDGET(w) (GTK_WIDGET(RVAL2GOBJ(w)))

static VALUE
menu_initialize(VALUE self)
{
    RBGTK_INITIALIZE(self, gtk_menu_new());
    return Qnil;
}

#if GTK_CHECK_VERSION(2,2,0)
static VALUE
menu_set_screen(VALUE self, VALUE screen)
{
    gtk_menu_set_screen(_SELF(self), GDK_SCREEN(RVAL2GOBJ(screen)));
    return self;
}
#endif

static VALUE
menu_reorder_child(VALUE self, VALUE child, VALUE position)
{
    gtk_menu_reorder_child(_SELF(self), GTK_WIDGET(RVAL2GOBJ(child)),
                           NUM2INT(position));
    return self;
}

#if GTK_CHECK_VERSION(2,4,0)
static VALUE
menu_attach(VALUE self, VALUE child, VALUE left_attach, VALUE right_attach, VALUE top_attach, VALUE bottom_attach)
{
    gtk_menu_attach(_SELF(self), GTK_WIDGET(RVAL2GOBJ(child)), 
                    NUM2UINT(left_attach), NUM2UINT(right_attach), 
                    NUM2UINT(top_attach), NUM2UINT(bottom_attach));
    return self;
}
#endif

static void
menu_pos_func(menu, px, py, push_in, data)
    GtkMenu *menu;
    gint *px, *py;
    gboolean *push_in;
    gpointer data;
{
    VALUE arr = rb_funcall((VALUE)data, id_call, 4, GOBJ2RVAL(menu), 
                           INT2FIX(*px), INT2FIX(*py), 
                           CBOOL2RVAL(*push_in));

    if (TYPE(arr) == T_ARRAY && (RARRAY_LEN(arr) == 2 || RARRAY_LEN(arr) == 3)){
        *px = NUM2INT(RARRAY_PTR(arr)[0]);
        *py = NUM2INT(RARRAY_PTR(arr)[1]);
        if (RARRAY_LEN(arr) == 3)
            *push_in = RVAL2CBOOL(RARRAY_PTR(arr)[2]);
    } else {
        rb_raise(rb_eArgError, "block should return [x, y, push_in]"); 
    } 
}

/* the proc should return [x, y, push_in] */
static VALUE
menu_popup(VALUE self, VALUE pshell, VALUE pitem, VALUE button, VALUE activate_time)
{
    GtkWidget *gpshell = NULL;
    GtkWidget *gpitem = NULL;
    GtkMenuPositionFunc pfunc = NULL;
    gpointer data = NULL;
    VALUE func;

    if (rb_block_given_p()) {
        func = rb_block_proc();
        pfunc = (GtkMenuPositionFunc)menu_pos_func;
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
                   pfunc, data, NUM2UINT(button),
                   NUM2UINT(activate_time));
    return self;
}

static VALUE
menu_set_accel_group(VALUE self, VALUE accel_group)
{
    gtk_menu_set_accel_group(_SELF(self), 
                             GTK_ACCEL_GROUP(RVAL2GOBJ(accel_group)));
    return self;
}

static VALUE
menu_get_accel_group(VALUE self)
{
    return GOBJ2RVAL(gtk_menu_get_accel_group(_SELF(self)));
}

static VALUE
menu_set_accel_path(VALUE self, VALUE accel_path)
{
    gtk_menu_set_accel_path(_SELF(self), RVAL2CSTR(accel_path));
    return self;
}

static VALUE
menu_get_tearoff_state(VALUE self)
{
    return CBOOL2RVAL(gtk_menu_get_tearoff_state(_SELF(self)));
}

static VALUE
menu_popdown(VALUE self)
{
    gtk_menu_popdown(_SELF(self));
    return self;
}

static VALUE
menu_reposition(VALUE self)
{
    gtk_menu_reposition(_SELF(self));
    return self;
}

static VALUE
menu_get_active(VALUE self)
{
    GtkWidget *mitem = gtk_menu_get_active(_SELF(self));

    return (mitem == NULL) ? Qnil : GOBJ2RVAL(mitem);
}

static VALUE
menu_set_active(VALUE self, VALUE active)
{
    gtk_menu_set_active(_SELF(self), NUM2INT(active));
    return self;
}

static VALUE
menu_set_tearoff_state(VALUE self, VALUE torn_off)
{
    gtk_menu_set_tearoff_state(_SELF(self), RVAL2CBOOL(torn_off));
    return self;
}

static VALUE menu_detacher;
static void
detach_func(GtkWidget *attach_widget, GtkMenu *menu)
{
    rb_funcall((VALUE)menu_detacher, id_call, 2, 
               GOBJ2RVAL(attach_widget), GOBJ2RVAL(menu));    
}

static VALUE
menu_attach_to_widget(VALUE self, VALUE attach_widget)
{
    menu_detacher = rb_block_proc();
    G_RELATIVE(self, menu_detacher);
    gtk_menu_attach_to_widget(_SELF(self),
                              GTK_WIDGET(RVAL2GOBJ(attach_widget)),
                              (GtkMenuDetachFunc)detach_func);
    return self;
}

static VALUE
menu_detach(VALUE self)
{
    gtk_menu_detach(_SELF(self));
    return self;
}

static VALUE
menu_get_attach_widget(VALUE self)
{
    return GOBJ2RVAL(gtk_menu_get_attach_widget(_SELF(self)));
}

#if GTK_CHECK_VERSION(2,6,0)
static VALUE
menu_s_get_for_attach_widget(VALUE self, VALUE widget)
{
    /* Owned by GTK+ */
    return GLIST2ARY(gtk_menu_get_for_attach_widget(GTK_WIDGET(RVAL2GOBJ(widget))));
}
#endif

#if GTK_CHECK_VERSION(2,4,0)
static VALUE
menu_set_monitor(VALUE self, VALUE monitor_num)
{
    gtk_menu_set_monitor(_SELF(self), NUM2INT(monitor_num));
    return self;
}
#endif
void 
Init_gtk_menu()
{
    VALUE gMenu = G_DEF_CLASS(GTK_TYPE_MENU, "Menu", mGtk);

    rb_define_method(gMenu, "initialize", menu_initialize, 0);
#if GTK_CHECK_VERSION(2,2,0)
    rb_define_method(gMenu, "set_screen", menu_set_screen, 1);
#endif
    rb_define_method(gMenu, "reorder_child", menu_reorder_child, 2);
#if GTK_CHECK_VERSION(2,4,0)
    rb_define_method(gMenu, "attach", menu_attach, 5);
#endif
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
#if GTK_CHECK_VERSION(2,6,0)
    rb_define_singleton_method(gMenu, "get_for_attach_widget", menu_s_get_for_attach_widget, 1);
#endif
    rb_define_method(gMenu, "attach_to_widget", menu_attach_to_widget, 1);
#if GTK_CHECK_VERSION(2,4,0)
    rb_define_method(gMenu, "set_monitor", menu_set_monitor, 1);
#endif

    G_DEF_SETTERS(gMenu);
}
