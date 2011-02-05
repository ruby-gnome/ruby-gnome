/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgdkcursor.c -

  $Author: ggc $
  $Date: 2007/07/13 14:27:08 $

  Copyright (C) 2001-2004 Masao Mutoh
************************************************/

#include "global.h"

static VALUE
gdkcursor_initialize(int argc, VALUE *argv, VALUE self)
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
gdkcursor_get_display(VALUE self)
{
    return GOBJ2RVAL(gdk_cursor_get_display((GdkCursor*)RVAL2BOXED(self, GDK_TYPE_CURSOR)));
}
#endif

static VALUE
gdkcursor_is_pixmap(VALUE self)
{
    return CBOOL2RVAL(((GdkCursor*)RVAL2BOXED(self, GDK_TYPE_COLOR))->type == 
                      GDK_CURSOR_IS_PIXMAP);
}

static VALUE
gdkcursor_cursor_type(VALUE self)
{
    return GENUM2RVAL(((GdkCursor*)RVAL2BOXED(self, GDK_TYPE_CURSOR))->type, GDK_TYPE_CURSOR_TYPE);
}

#if GTK_CHECK_VERSION(2,8,0)
static VALUE
gdkcursor_get_image(VALUE self)
{
    return GOBJ2RVAL(gdk_cursor_get_image((GdkCursor*)RVAL2BOXED(self, GDK_TYPE_CURSOR)));
}
#endif

void
Init_gtk_gdk_cursor()
{
    VALUE gdkCursor = G_DEF_CLASS(GDK_TYPE_CURSOR, "Cursor", mGdk);

    rb_define_method(gdkCursor, "initialize", gdkcursor_initialize, -1);
#if GTK_CHECK_VERSION(2,2,0)
    rb_define_method(gdkCursor, "display", gdkcursor_get_display, 0);
#endif
    rb_define_method(gdkCursor, "pixmap?", gdkcursor_is_pixmap, 0);
    rb_define_method(gdkCursor, "cursor_type", gdkcursor_cursor_type, 0);
#if GTK_CHECK_VERSION(2,8,0)
    rb_define_method(gdkCursor, "image", gdkcursor_get_image, 0);
#endif

    G_DEF_CLASS(GDK_TYPE_CURSOR_TYPE, "Type", gdkCursor);
    G_DEF_CONSTANTS(gdkCursor, GDK_TYPE_CURSOR_TYPE, "GDK_");
}
