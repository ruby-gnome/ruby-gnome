/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkstatusicon.c -

  $Author: mutoh $
  $Date: 2006/11/17 18:12:41 $

  Copyright (C) 2006 Masao Mutoh
************************************************/

#include "global.h"

#if GTK_CHECK_VERSION(2,10,0)

#define _SELF(w) (GTK_STATUS_ICON(RVAL2GOBJ(w)))

static VALUE
si_initialize(self)
    VALUE self;
{
    G_INITIALIZE(self, gtk_status_icon_new());
    return Qnil;
}

/* Don't implement this. Use Gtk::StatusIcon.new.set_foo(v) instead.
GtkStatusIcon* gtk_status_icon_new_from_pixbuf
                                            (GdkPixbuf *pixbuf);
GtkStatusIcon* gtk_status_icon_new_from_file
                                            (const gchar *filename);
GtkStatusIcon* gtk_status_icon_new_from_stock
                                            (const gchar *stock_id);
GtkStatusIcon* gtk_status_icon_new_from_icon_name
                                            (const gchar *icon_name);
*/
/* Defined as properties
void        gtk_status_icon_set_from_pixbuf (GtkStatusIcon *status_icon,
                                             GdkPixbuf *pixbuf);
void        gtk_status_icon_set_from_file   (GtkStatusIcon *status_icon,
                                             const gchar *filename);
void        gtk_status_icon_set_from_stock  (GtkStatusIcon *status_icon,
                                             const gchar *stock_id);
void        gtk_status_icon_set_from_icon_name
                                            (GtkStatusIcon *status_icon,
                                             const gchar *icon_name);
GtkImageType gtk_status_icon_get_storage_type
                                            (GtkStatusIcon *status_icon);
GdkPixbuf*  gtk_status_icon_get_pixbuf      (GtkStatusIcon *status_icon);
const gchar* gtk_status_icon_get_stock      (GtkStatusIcon *status_icon);
const gchar* gtk_status_icon_get_icon_name  (GtkStatusIcon *status_icon);
gint        gtk_status_icon_get_size        (GtkStatusIcon *status_icon);
void        gtk_status_icon_set_visible     (GtkStatusIcon *status_icon,
                                             gboolean visible);
gboolean    gtk_status_icon_get_visible     (GtkStatusIcon *status_icon);
void        gtk_status_icon_set_blinking    (GtkStatusIcon *status_icon,
                                             gboolean blinking);
gboolean    gtk_status_icon_get_blinking    (GtkStatusIcon *status_icon);
*/

static VALUE
si_set_tooltip(self, tooltip_text)
    VALUE self, tooltip_text;
{
    gtk_status_icon_set_tooltip(_SELF(self), RVAL2CSTR(tooltip_text));
    return self;
}

static VALUE
si_is_embedded(self)
    VALUE self;
{
    return CBOOL2RVAL(gtk_status_icon_is_embedded(_SELF(self)));
}

static VALUE
si_position_menu(self, menu)
    VALUE self, menu;
{
    gint x, y;
    gboolean push_in;

    gtk_status_icon_position_menu(GTK_MENU(RVAL2GOBJ(menu)), &x, &y, &push_in,
                                  (gpointer)(_SELF(self)));
    return rb_ary_new3(3, INT2NUM(x), INT2NUM(y), CBOOL2RVAL(push_in));
}

static VALUE
si_get_geometry(self)
    VALUE self;
{
    GdkScreen* screen;
    GdkRectangle area;
    GtkOrientation orientation;
    gboolean ret = gtk_status_icon_get_geometry(_SELF(self), &screen, 
                                                &area, &orientation);
    if (ret) {
        return rb_ary_new3(3, GOBJ2RVAL(screen), 
                           BOXED2RVAL(&area, GDK_TYPE_RECTANGLE),
                           GENUM2RVAL(orientation, GTK_TYPE_ORIENTATION));
    } else {
        return rb_ary_new3(3, Qnil, Qnil, Qnil);
    }
}
#endif

void 
Init_gtk_status_icon()
{
#if GTK_CHECK_VERSION(2,10,0)
    VALUE si = G_DEF_CLASS(GTK_TYPE_STATUS_ICON, "StatusIcon", mGtk);

    rb_define_method(si, "initialize", si_initialize, 0);
    rb_define_method(si, "set_tooltip", si_set_tooltip, 1);
    rb_define_method(si, "embedded?", si_is_embedded, 0);
    rb_define_method(si, "position_menu", si_position_menu, 1);
    rb_define_method(si, "geometry", si_get_geometry, 0);

    G_DEF_SETTERS(si);   
#endif
}
