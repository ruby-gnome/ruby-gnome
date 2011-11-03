/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2004  Masao Mutoh
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

#if GTK_CHECK_VERSION(2,4,0)

#define RG_TARGET_NAMESPACE cIconInfo
#define _SELF(i) ((GtkIconInfo*)RVAL2BOXED(i, GTK_TYPE_ICON_INFO))

static VALUE
rg_base_size(VALUE self)
{
    return INT2NUM(gtk_icon_info_get_base_size(_SELF(self)));
}

static VALUE
rg_filename(VALUE self)
{
    return CSTR2RVAL(gtk_icon_info_get_filename(_SELF(self)));
}

static VALUE
rg_builtin_pixbuf(VALUE self)
{
    return GOBJ2RVAL(gtk_icon_info_get_builtin_pixbuf(_SELF(self)));
}

static VALUE
rg_load_icon(VALUE self)
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
rg_set_raw_coordinates(VALUE self, VALUE raw_coordinates)
{
    gtk_icon_info_set_raw_coordinates(_SELF(self), RVAL2CBOOL(raw_coordinates));
    return self;
}

static VALUE
rg_embedded_rect(VALUE self)
{
    GdkRectangle rectangle;
    gboolean ret = gtk_icon_info_get_embedded_rect(_SELF(self), &rectangle);

    if (ret)
        return BOXED2RVAL(&rectangle, GDK_TYPE_RECTANGLE);
    else
        return Qnil;
}

static VALUE
rg_attach_points(VALUE self)
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
rg_display_name(VALUE self)
{
    return CSTR2RVAL(gtk_icon_info_get_display_name(_SELF(self)));
}
#endif

void 
Init_gtk_iconinfo(VALUE mGtk)
{
#if GTK_CHECK_VERSION(2,4,0)
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_ICON_INFO, "IconInfo", mGtk);

    RG_DEF_METHOD(base_size, 0);
    RG_DEF_METHOD(filename, 0);
    RG_DEF_METHOD(builtin_pixbuf, 0);
    RG_DEF_METHOD(load_icon, 0);
    RG_DEF_METHOD(set_raw_coordinates, 1);
    RG_DEF_METHOD(embedded_rect, 0);
    RG_DEF_METHOD(attach_points, 0);
    RG_DEF_METHOD(display_name, 0);

    G_DEF_SETTERS(RG_TARGET_NAMESPACE);
#endif
}
