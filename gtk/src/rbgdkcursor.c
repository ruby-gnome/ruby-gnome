/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgdkcursor.c -

  $Author: mutoh $
  $Date: 2002/08/29 04:44:30 $

  Copyright (C) 2001 MUTOH Masao
************************************************/

#include "global.h"

static VALUE
gdkcursor_initialize(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
	GdkCursor* cursor;
    VALUE source_or_type, mask, fg, bg, x, y;

    rb_scan_args(argc, argv, "15", &source_or_type, &mask, &fg, &bg, &x, &y);
    if (argc == 1){
		cursor = gdk_cursor_new(NUM2INT(source_or_type));
    } else {
		cursor = gdk_cursor_new_from_pixmap(GDK_PIXMAP(RVAL2GOBJ(source_or_type)), 
		  		   		NIL_P(mask)?NULL:GDK_PIXMAP(RVAL2GOBJ(mask)), 
		   		 		NIL_P(fg)?NULL:(GdkColor*)RVAL2BOXED(fg), 
		  	   			NIL_P(bg)?NULL:(GdkColor*)RVAL2BOXED(bg), 
		   				NUM2INT(x), NUM2INT(y));
    }

	RBGOBJ_INITIALIZE(self, cursor);

	return Qnil;
}

void
Init_gtk_gdk_cursor()
{
	VALUE gdkCursor = G_DEF_CLASS(GDK_TYPE_CURSOR, "Cursor", mGdk);

    rb_define_method(gdkCursor, "initialize", gdkcursor_initialize, -1);
#include "rbgdkcursor.h"
    rb_define_const(gdkCursor, "LAST_CURSOR", INT2FIX(GDK_LAST_CURSOR));
    rb_define_const(gdkCursor, "CURSOR_IS_PIXMAP", 
		    INT2FIX(GDK_CURSOR_IS_PIXMAP));
}
