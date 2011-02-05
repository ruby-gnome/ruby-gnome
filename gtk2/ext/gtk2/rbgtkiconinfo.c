/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkiconinfo.c -

  $Author: ggc $
  $Date: 2007/07/13 16:07:31 $

  Copyright (C) 2004 Masao Mutoh
************************************************/

#include "global.h"

#if GTK_CHECK_VERSION(2,4,0)
#define _SELF(i) ((GtkIconInfo*)RVAL2BOXED(i, GTK_TYPE_ICON_INFO))

static VALUE
iconinfo_get_base_size(VALUE self)
{
    return INT2NUM(gtk_icon_info_get_base_size(_SELF(self)));
}

static VALUE
iconinfo_get_filename(VALUE self)
{
    return CSTR2RVAL(gtk_icon_info_get_filename(_SELF(self)));
}

static VALUE
iconinfo_get_builtin_pixbuf(VALUE self)
{
    return GOBJ2RVAL(gtk_icon_info_get_builtin_pixbuf(_SELF(self)));
}

static VALUE
iconinfo_load_icon(VALUE self)
{
    GError* error = NULL;
    GdkPixbuf* pixbuf = gtk_icon_info_load_icon(_SELF(self), &error);
    VALUE ret;

    if (error) RAISE_GERROR(error);

    ret = GOBJ2RVAL(pixbuf);
    g_object_unref(pixbuf);
    return ret;
}

static VALUE
iconinfo_set_raw_coordinates(VALUE self, VALUE raw_coordinates)
{
    gtk_icon_info_set_raw_coordinates(_SELF(self), RVAL2CBOOL(raw_coordinates));
    return self;
}

static VALUE
iconinfo_get_embedded_rect(VALUE self)
{
    GdkRectangle rectangle;
    gboolean ret = gtk_icon_info_get_embedded_rect(_SELF(self), &rectangle);

    if (ret)
        return BOXED2RVAL(&rectangle, GDK_TYPE_RECTANGLE);
    else
        return Qnil;
}

static VALUE
iconinfo_get_attach_points(VALUE self)
{
    GdkPoint* points;
    gint n_points, i;
    gboolean ret = gtk_icon_info_get_attach_points(_SELF(self), &points, &n_points);
    VALUE ary = Qnil;

    if (ret){
        ary = rb_ary_new();
        for (i = 0; i < n_points; i++){
            rb_ary_push(ary, rb_assoc_new(INT2NUM(points[i].x), INT2NUM(points[i].y)));
        }
        g_free(points);
    }
    return ary;
}

static VALUE
iconinfo_get_display_name(VALUE self)
{
    return CSTR2RVAL(gtk_icon_info_get_display_name(_SELF(self)));
}
#endif

void 
Init_gtk_iconinfo()
{
#if GTK_CHECK_VERSION(2,4,0)
    VALUE ii = G_DEF_CLASS(GTK_TYPE_ICON_INFO, "IconInfo", mGtk);

    rb_define_method(ii, "base_size", iconinfo_get_base_size, 0);
    rb_define_method(ii, "filename", iconinfo_get_filename, 0);
    rb_define_method(ii, "builtin_pixbuf", iconinfo_get_builtin_pixbuf, 0);
    rb_define_method(ii, "load_icon", iconinfo_load_icon, 0);
    rb_define_method(ii, "set_raw_coordinates", iconinfo_set_raw_coordinates, 1);
    rb_define_method(ii, "embedded_rect", iconinfo_get_embedded_rect, 0);
    rb_define_method(ii, "attach_points", iconinfo_get_attach_points, 0);
    rb_define_method(ii, "display_name", iconinfo_get_display_name, 0);

    G_DEF_SETTERS(ii);
#endif
}
