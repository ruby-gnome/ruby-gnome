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

#include "rbgdk3private.h"

#define RG_TARGET_NAMESPACE cCursor

static VALUE
rg_initialize(int argc, VALUE *argv, VALUE self)
{
    GdkCursor* cursor = NULL;

    if (argc == 1){
        VALUE type;
        rb_scan_args(argc, argv, "10", &type);
        cursor = gdk_cursor_new(RVAL2GDKCURSORTYPE(type));
    } else if (argc == 2) {
        VALUE display, type_or_name;
        rb_scan_args(argc, argv, "20", &display, &type_or_name);
        if (TYPE(type_or_name) == T_STRING)
            cursor = gdk_cursor_new_from_name(RVAL2GDKDISPLAYOBJECT(display),
                                              RVAL2CSTR(type_or_name));
        else
            cursor = gdk_cursor_new_for_display(RVAL2GDKDISPLAYOBJECT(display), 
                                                RVAL2GDKCURSORTYPE(type_or_name));
    } else if (argc == 4) {
        VALUE display, pixbuf, x, y;
        rb_scan_args(argc, argv, "40", &display, &pixbuf, &x, &y);
        cursor = gdk_cursor_new_from_pixbuf(RVAL2GDKDISPLAYOBJECT(display), 
                                            RVAL2GDKPIXBUF(pixbuf), 
                                            NUM2INT(x), NUM2INT(y));
/* deprecated
    } else if (argc == 6) {
        VALUE pixmap, mask, fg, bg, x, y;
        rb_scan_args(argc, argv, "60", &pixmap, &mask, &fg, &bg, &x, &y);
        cursor = gdk_cursor_new_from_pixmap(RVAL2GDKPIXMAP(pixmap), 
                                            NIL_P(mask)?NULL:RVAL2GDKPIXMAP(mask), 
                                            RVAL2GDKCOLOR(fg),
                                            RVAL2GDKCOLOR(bg),
                                            NUM2INT(x), NUM2INT(y));
*/
    }
    G_INITIALIZE(self, cursor);

    return Qnil;
}

/* TODO
static VALUE
rg_cursor_type(VALUE self)
{
    return GDKCURSORTYPE2RVAL((RVAL2GDKCURSOR(self))->type);
}
*/

static VALUE
rg_image(VALUE self)
{
    return GOBJ2RVAL(gdk_cursor_get_image(RVAL2GDKCURSOR(self)));
}

void
Init_gdk_cursor(VALUE mGdk)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GDK_TYPE_CURSOR, "Cursor", mGdk);

    RG_DEF_METHOD(initialize, -1);
/* TODO
    RG_DEF_METHOD(cursor_type, 0);
*/
    RG_DEF_METHOD(image, 0);

    G_DEF_CLASS(GDK_TYPE_CURSOR_TYPE, "Type", RG_TARGET_NAMESPACE);
}
