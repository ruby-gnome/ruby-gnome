/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgdkcursor.c -

  $Author: mutoh $
  $Date: 2002/06/23 16:13:32 $

  Copyright (C) 2001 MUTOH Masao
************************************************/

#include "global.h"

VALUE gdkCursor;

static VALUE
gdkcursor_s_new(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE source_or_type, mask, fg, bg, x, y;
    GdkCursor *cursor;

    rb_scan_args(argc, argv, "15", &source_or_type, 
		 &mask, &fg, &bg, &x, &y);
    if (argc == 1){
		cursor = gdk_cursor_new(NUM2INT(source_or_type));
    } else {
		cursor = gdk_cursor_new_from_pixmap(GDK_PIXMAP(RVAL2GOBJ(source_or_type)), 
											NIL_P(mask)?NULL:GDK_PIXMAP(RVAL2GOBJ(mask)), 
											NIL_P(fg)?NULL:get_gdkcolor(fg), 
											NIL_P(bg)?NULL:get_gdkcolor(bg), 
											NUM2INT(x), NUM2INT(y));
    }
    return make_gdkcursor(cursor); 
}

void
Init_gtk_gdk_cursor()
{
    gdkCursor = rb_define_class_under(mGdk, "Cursor", rb_cData);
    rb_define_singleton_method(gdkCursor, "new", gdkcursor_s_new, -1);
#include "rbgdkcursor.h"
    rb_define_const(gdkCursor, "LAST_CURSOR", INT2FIX(GDK_LAST_CURSOR));
    rb_define_const(gdkCursor, "CURSOR_IS_PIXMAP", 
		    INT2FIX(GDK_CURSOR_IS_PIXMAP));
}
