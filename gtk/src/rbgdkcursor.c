/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgdkcursor.c -

  $Author: mutoh $
  $Date: 2002/08/18 06:28:32 $

  Copyright (C) 2001 MUTOH Masao
************************************************/

#include "global.h"

static VALUE
gdkcursor_s_allocate(self)
	VALUE self;
{
	GdkCursor c;
	return COBJ2RVAL("Gdk::Cursor", &c);
}

#ifdef HAVE_OBJECT_ALLOCATE
#define gdkcursor_s_new rb_class_new_instance
#else
static VALUE
gdkcursor_s_new(argc, argv, klass)
    int argc;
    VALUE* argv;
    VALUE klass;
{
	VALUE obj = gdkcursor_s_allocate(klass);
	rb_obj_call_init(obj, argc ,argv);
    return obj;
}
#endif

static VALUE
gdkcursor_initialize(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE source_or_type, mask, fg, bg, x, y;
    GdkCursor *cursor = DATA_PTR(self);

    rb_scan_args(argc, argv, "15", &source_or_type, &mask, &fg, &bg, &x, &y);
    if (argc == 1){
		cursor = gdk_cursor_new(NUM2INT(source_or_type));
    } else {
		cursor = gdk_cursor_new_from_pixmap(GDK_PIXMAP(RVAL2GOBJ(source_or_type)), 
		  		   		NIL_P(mask)?NULL:GDK_PIXMAP(RVAL2GOBJ(mask)), 
		   		 		NIL_P(fg)?NULL:(GdkColor*)RVAL2COBJ("Gdk::Color",fg), 
		  	   			NIL_P(bg)?NULL:(GdkColor*)RVAL2COBJ("Gdk::Color",bg), 
		   				NUM2INT(x), NUM2INT(y));
    }
	DATA_PTR(self) = cursor;
	return self;
}

static void*
gdkcursor_alloc(cobj)
	void* cobj;
{
	GdkCursor* c = xmalloc(sizeof(GdkCursor));
	gdk_cursor_ref((GdkCursor*)cobj);
	memcpy(c, cobj, sizeof(GdkCursor));
	return c;
}

void
Init_gtk_gdk_cursor()
{
    VALUE gdkCursor = RB_DEF_CLASS2("Cursor", mGdk, "Data", GdkCursor, 0, 
									gdkcursor_alloc, gdk_cursor_unref);

	rb_define_singleton_method(gdkCursor, "allocate", gdkcursor_s_allocate, 0);
#ifndef HAVE_OBJECT_ALLOCATE
    rb_define_singleton_method(gdkCursor, "new", gdkcursor_s_new, -1);
#endif
    rb_define_method(gdkCursor, "initialize", gdkcursor_initialize, -1);
#include "rbgdkcursor.h"
    rb_define_const(gdkCursor, "LAST_CURSOR", INT2FIX(GDK_LAST_CURSOR));
    rb_define_const(gdkCursor, "CURSOR_IS_PIXMAP", 
		    INT2FIX(GDK_CURSOR_IS_PIXMAP));
}
