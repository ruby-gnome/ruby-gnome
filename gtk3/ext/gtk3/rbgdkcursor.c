/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2001-2004 Masao Mutoh
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

#define RG_TARGET_NAMESPACE cCursor

static VALUE
rg_initialize(int argc, VALUE *argv, VALUE self)
{
    GdkCursor* cursor = NULL;

    if (argc == 1){
        VALUE type;
        rb_scan_args(argc, argv, "10", &type);
        cursor = gdk_cursor_new(RVAL2GENUM(type, GDK_TYPE_CURSOR_TYPE));
    } else if (argc == 2) {
#if GTK_CHECK_VERSION(2,2,0)
        VALUE display, type_or_name;
        rb_scan_args(argc, argv, "20", &display, &type_or_name);
#if GTK_CHECK_VERSION(2,8,0)
        if (TYPE(type_or_name) == T_STRING)
            cursor = gdk_cursor_new_from_name(GDK_DISPLAY_OBJECT(RVAL2GOBJ(display)),
                                              RVAL2CSTR(type_or_name));
        else
#endif
            cursor = gdk_cursor_new_for_display(GDK_DISPLAY_OBJECT(RVAL2GOBJ(display)), 
                                                RVAL2GENUM(type_or_name, GDK_TYPE_CURSOR_TYPE));
#else
        rb_raise(rb_eRuntimeError, "Gdk::Cursor.new(display, cursor_type) has been supported since GTK+-2.2.");
#endif
    } else if (argc == 4) {
#if GTK_CHECK_VERSION(2,4,0)
        VALUE display, pixbuf, x, y;
        rb_scan_args(argc, argv, "40", &display, &pixbuf, &x, &y);
        cursor = gdk_cursor_new_from_pixbuf(GDK_DISPLAY_OBJECT(RVAL2GOBJ(display)), 
                                            GDK_PIXBUF(RVAL2GOBJ(pixbuf)), 
                                            NUM2INT(x), NUM2INT(y));
#else
        rb_raise(rb_eRuntimeError, "Gdk::Cursor.new(pixbuf) has been supported since GTK+-2.4.");
#endif
    } else if (argc == 6) {
        VALUE pixmap, mask, fg, bg, x, y;
        rb_scan_args(argc, argv, "60", &pixmap, &mask, &fg, &bg, &x, &y);
        cursor = gdk_cursor_new_from_pixmap(GDK_PIXMAP(RVAL2GOBJ(pixmap)), 
                                            NIL_P(mask)?NULL:GDK_PIXMAP(RVAL2GOBJ(mask)), 
                                            RVAL2GDKCOLOR(fg),
                                            RVAL2GDKCOLOR(bg),
                                            NUM2INT(x), NUM2INT(y));
    }
    G_INITIALIZE(self, cursor);

    return Qnil;
}

#if GTK_CHECK_VERSION(2,2,0)
static VALUE
rg_display(VALUE self)
{
    return GOBJ2RVAL(gdk_cursor_get_display((GdkCursor*)RVAL2BOXED(self, GDK_TYPE_CURSOR)));
}
#endif

static VALUE
rg_pixmap_p(VALUE self)
{
    return CBOOL2RVAL(((GdkCursor*)RVAL2BOXED(self, GDK_TYPE_COLOR))->type == 
                      GDK_CURSOR_IS_PIXMAP);
}

static VALUE
rg_cursor_type(VALUE self)
{
    return GENUM2RVAL(((GdkCursor*)RVAL2BOXED(self, GDK_TYPE_CURSOR))->type, GDK_TYPE_CURSOR_TYPE);
}

#if GTK_CHECK_VERSION(2,8,0)
static VALUE
rg_image(VALUE self)
{
    return GOBJ2RVAL(gdk_cursor_get_image((GdkCursor*)RVAL2BOXED(self, GDK_TYPE_CURSOR)));
}
#endif

void
Init_gtk_gdk_cursor(VALUE mGdk)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GDK_TYPE_CURSOR, "Cursor", mGdk);

    RG_DEF_METHOD(initialize, -1);
#if GTK_CHECK_VERSION(2,2,0)
    RG_DEF_METHOD(display, 0);
#endif
    RG_DEF_METHOD_P(pixmap, 0);
    RG_DEF_METHOD(cursor_type, 0);
#if GTK_CHECK_VERSION(2,8,0)
    RG_DEF_METHOD(image, 0);
#endif

    G_DEF_CLASS(GDK_TYPE_CURSOR_TYPE, "Type", RG_TARGET_NAMESPACE);
    G_DEF_CONSTANTS(RG_TARGET_NAMESPACE, GDK_TYPE_CURSOR_TYPE, "GDK_");
}
