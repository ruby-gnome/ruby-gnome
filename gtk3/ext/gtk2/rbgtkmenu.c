/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2002-2006 Ruby-GNOME2 Project Team
 *  Copyright (C) 1998-2000 Yukihiro Matsumoto,
 *                          Daisuke Kanda,
 *                          Hiroshi Igarashi
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 *  MA  02110-1301  USA
 */

#include "global.h"

#define RG_TARGET_NAMESPACE cMenu
#define _SELF(self) (GTK_MENU(RVAL2GOBJ(self)))
#define RVAL2WIDGET(w) (GTK_WIDGET(RVAL2GOBJ(w)))

static VALUE
rg_initialize(VALUE self)
{
    RBGTK_INITIALIZE(self, gtk_menu_new());
    return Qnil;
}

#if GTK_CHECK_VERSION(2,2,0)
static VALUE
rg_set_screen(VALUE self, VALUE screen)
{
    gtk_menu_set_screen(_SELF(self), GDK_SCREEN(RVAL2GOBJ(screen)));
    return self;
}
#endif

static VALUE
rg_reorder_child(VALUE self, VALUE child, VALUE position)
{
    gtk_menu_reorder_child(_SELF(self), GTK_WIDGET(RVAL2GOBJ(child)),
                           NUM2INT(position));
    return self;
}

#if GTK_CHECK_VERSION(2,4,0)
static VALUE
rg_attach(VALUE self, VALUE child, VALUE left_attach, VALUE right_attach, VALUE top_attach, VALUE bottom_attach)
{
    gtk_menu_attach(_SELF(self), GTK_WIDGET(RVAL2GOBJ(child)), 
                    NUM2UINT(left_attach), NUM2UINT(right_attach), 
                    NUM2UINT(top_attach), NUM2UINT(bottom_attach));
    return self;
}
#endif

static void
menu_pos_func(GtkMenu *menu, gint *px, gint *py, gboolean *push_in, gpointer data)
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
rg_popup(VALUE self, VALUE pshell, VALUE pitem, VALUE button, VALUE activate_time)
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
rg_popdown(VALUE self)
{
    gtk_menu_popdown(_SELF(self));
    return self;
}

static VALUE
rg_reposition(VALUE self)
{
    gtk_menu_reposition(_SELF(self));
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
rg_attach_to_widget(VALUE self, VALUE attach_widget)
{
    menu_detacher = rb_block_proc();
    G_RELATIVE(self, menu_detacher);
    gtk_menu_attach_to_widget(_SELF(self),
                              GTK_WIDGET(RVAL2GOBJ(attach_widget)),
                              (GtkMenuDetachFunc)detach_func);
    return self;
}

static VALUE
rg_detach(VALUE self)
{
    gtk_menu_detach(_SELF(self));
    return self;
}

#if GTK_CHECK_VERSION(2,6,0)
static VALUE
rg_s_get_for_attach_widget(G_GNUC_UNUSED VALUE self, VALUE widget)
{
    /* Owned by GTK+ */
    return GLIST2ARY(gtk_menu_get_for_attach_widget(GTK_WIDGET(RVAL2GOBJ(widget))));
}
#endif

void 
Init_gtk_menu(VALUE mGtk)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_MENU, "Menu", mGtk);

    RG_DEF_METHOD(initialize, 0);
#if GTK_CHECK_VERSION(2,2,0)
    RG_DEF_METHOD(set_screen, 1);
    G_DEF_SETTER(RG_TARGET_NAMESPACE, "screen");
#endif
    RG_DEF_METHOD(reorder_child, 2);
#if GTK_CHECK_VERSION(2,4,0)
    RG_DEF_METHOD(attach, 5);
#endif
    RG_DEF_METHOD(popup, 4);
    RG_DEF_METHOD(popdown, 0);
    RG_DEF_METHOD(reposition, 0);
    RG_DEF_METHOD(detach, 0);
#if GTK_CHECK_VERSION(2,6,0)
    RG_DEF_SMETHOD(get_for_attach_widget, 1);
#endif
    RG_DEF_METHOD(attach_to_widget, 1);
}
