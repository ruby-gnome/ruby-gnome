/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgdkcursor.c -

  $Author: mutoh $
  $Date: 2002/05/19 12:39:08 $

  Copyright (C) 2001 MUTOH Masao
************************************************/

#include "global.h"

VALUE gdkCursor;
VALUE gdkCursorConst;

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
	cursor = gdk_cursor_new_from_pixmap(get_gdkpixmap(source_or_type), 
				    NIL_P(mask)?NULL:get_gdkpixmap(mask), 
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
    gdkCursorConst = rb_define_module_under(gdkCursor, "Constants");

    rb_define_singleton_method(gdkCursor, "new", gdkcursor_s_new, -1);

#include "rbgdkcursors.h" 
    rb_define_const(gdkCursorConst, "LAST_CURSOR", INT2FIX(GDK_LAST_CURSOR));
    rb_define_const(gdkCursorConst, "CURSOR_IS_PIXMAP", 
		    INT2FIX(GDK_CURSOR_IS_PIXMAP));

    rb_include_module(gdkCursor, gdkCursorConst);
}
