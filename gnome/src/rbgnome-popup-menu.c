/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/* $Id: rbgnome-popup-menu.c,v 1.1 2002/10/30 13:36:48 tkubo Exp $ */
/* based on libgnomeui/gnome-popup-menu.h */

/* Gnome::PopupMenu widget for Ruby/GNOME2
 * Copyright (C) 2002 KUBO Takehiro <kubo@jiubao.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include "rbgnome.h"

#define _SELF(self) GTK_MENU(RVAL2GOBJ(self))

static VALUE
pmenu_initialize(argc, argv, self)
    int argc;
    VALUE *argv, self;
{
    VALUE uiinfo, accelgroup;
    GtkWidget *menu;
    VALUE obj;

    rb_scan_args(argc, argv, "11", &uiinfo, &accelgroup);
    obj = rbgno_ary_to_ui_info(uiinfo, RBUI_POPUP_MENU);
    if (NIL_P(accelgroup))
        menu = gnome_popup_menu_new(DATA_PTR(obj));
    else 
        menu = gnome_popup_menu_new_with_accelgroup(DATA_PTR(obj), GTK_ACCEL_GROUP(RVAL2GOBJ(accelgroup)));
    RBGTK_INITIALIZE(self, menu);
    G_RELATIVE(self, uiinfo);
    G_RELATIVE(self, obj);
    return self;
}

static VALUE
pmenu_get_accel_group(self)
    VALUE self;
{
    return GOBJ2RVAL(gnome_popup_menu_get_accel_group(_SELF(self)));
}

static VALUE
pmenu_attach(self, widget, user_data)
    VALUE self, widget, user_data;
{
    gnome_popup_menu_attach(GTK_WIDGET(RVAL2GOBJ(self)),
                            GTK_WIDGET(RVAL2GOBJ(widget)),
                            (gpointer)user_data); /* ??? */
    G_RELATIVE(self, user_data);
    return self;
}

static void
pmenu_pos_func(menu, px, py, data)
    GtkMenu *menu;
    gint *px, *py;
    gpointer data;
{
    VALUE arr = rb_funcall((VALUE)data, id_call, 3, GOBJ2RVAL(menu), 
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
pmenu_do_popup(self, func, event, user_data, for_widget)
    VALUE self, func, event, user_data, for_widget;
{
    GtkMenuPositionFunc pos_func;
    gpointer pos_data;
    GdkEventButton *eventbutton;

    pos_func = NIL_P(func) ? NULL : pmenu_pos_func;
    pos_data = NIL_P(func) ? NULL : (gpointer)func;
    if (!NIL_P(event)) {
        eventbutton = (GdkEventButton *)RVAL2GEV(event);
        switch (eventbutton->type) {
          case GDK_BUTTON_PRESS:
          case GDK_2BUTTON_PRESS:
          case GDK_3BUTTON_PRESS:
          case GDK_BUTTON_RELEASE:
            /* OK */
            break;
          default:
            rb_raise(rb_eArgError, "not a GtkEventButton");
        }
    } else {
        eventbutton = NULL;
    }
    gnome_popup_menu_do_popup(GTK_WIDGET(RVAL2GOBJ(self)),
                              pos_func, pos_data,
                              eventbutton,
                              (gpointer)user_data,
                              NIL_P(for_widget) ? NULL : GTK_WIDGET(RVAL2GOBJ(for_widget)));
    G_RELATIVE(self, user_data);
    return self;
}

static VALUE
pmenu_do_popup_modal(self, func, event, user_data, for_widget)
    VALUE self, func, event, user_data, for_widget;
{
    GtkMenuPositionFunc pos_func;
    gpointer pos_data;
    GdkEventButton *eventbutton;
    int result;

    pos_func = NIL_P(func) ? NULL : pmenu_pos_func;
    pos_data = NIL_P(func) ? NULL : (gpointer)func;
    if (!NIL_P(event)) {
        eventbutton = (GdkEventButton *)RVAL2GEV(event);
        switch (eventbutton->type) {
          case GDK_BUTTON_PRESS:
          case GDK_2BUTTON_PRESS:
          case GDK_3BUTTON_PRESS:
          case GDK_BUTTON_RELEASE:
            /* OK */
            break;
          default:
            rb_raise(rb_eArgError, "not a GtkEventButton");
        }
    } else {
        eventbutton = NULL;
    }
    result = gnome_popup_menu_do_popup_modal(GTK_WIDGET(RVAL2GOBJ(self)),
                                             pos_func, pos_data,
                                             eventbutton,
                                             (gpointer)user_data,
                                             NIL_P(for_widget) ? NULL : GTK_WIDGET(RVAL2GOBJ(for_widget)));
    G_RELATIVE(self, user_data);
    return INT2NUM(result);
}

static VALUE
pmenu_append(self, uiinfo)
    VALUE self, uiinfo;
{
    VALUE obj = rbgno_ary_to_ui_info(uiinfo, RBUI_POPUP_MENU);
    gnome_popup_menu_append(GTK_WIDGET(RVAL2GOBJ(self)), DATA_PTR(obj));
    return self;
}

static VALUE
pmenu_add_popup_items(self, uiinfo, user_data)
    VALUE self, uiinfo, user_data;
{
    VALUE obj = rbgno_ary_to_ui_info(uiinfo, RBUI_POPUP_MENU);
    gnome_gtk_widget_add_popup_items(GTK_WIDGET(RVAL2GOBJ(self)),
                                     DATA_PTR(obj),
                                     (gpointer)user_data);
    G_RELATIVE(self, uiinfo);
    return self;
}

void
Init_gnome_popup_menu(mGnome)
    VALUE mGnome;
{
    VALUE gWidget = GTYPE2CLASS(GTK_TYPE_WIDGET);
    VALUE gMenu = GTYPE2CLASS(GTK_TYPE_MENU);
    VALUE gnoPopupMenu = rb_define_class_under(mGnome, "PopupMenu", gMenu);

    rb_define_method(gnoPopupMenu, "initialize", pmenu_initialize, -1);
    rb_define_method(gnoPopupMenu, "accel_group", pmenu_get_accel_group, 0);
    rb_define_method(gnoPopupMenu, "attach", pmenu_attach, 2);
    rb_define_method(gnoPopupMenu, "do_popup", pmenu_do_popup, 4);
    rb_define_method(gnoPopupMenu, "do_popup_modal", pmenu_do_popup_modal, 4);
    rb_define_method(gnoPopupMenu, "append", pmenu_append, 1);
    rb_define_method(gWidget, "add_popup_items", pmenu_add_popup_items, 2);
}
