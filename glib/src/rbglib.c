/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/**********************************************************************

  rbglib.c -

  $Author: sakai $
  $Date: 2003/02/14 17:54:40 $

  Copyright (C) 2002,2003  Masahiro Sakai

**********************************************************************/

#include <glib.h>
#include <locale.h>
#include "global.h"

VALUE mGLib;

extern void Init_utils_int64();
extern void Init_glib_convert();
extern void Init_glib_messages();
extern void Init_gobject();
extern void Init_gutil();

char *
rbg_string_value_ptr(ptr)
    volatile VALUE *ptr;
{
    VALUE s = *ptr;
    if (TYPE(s) != T_STRING) {
        s = rb_str_to_str(s);
        *ptr = s;
    }
    if (!RSTRING(s)->ptr) {
        rb_str_modify(s);
    }
    return RSTRING(s)->ptr;
}

VALUE
rbg_cstr2rval(const char* str)
{
    return str ? rb_str_new2(str) : Qnil;
}

void Init_glib2()
{
    mGLib = rb_define_module("GLib");

    setlocale(LC_ALL, "");
    
    rb_define_const(mGLib, "VERSION",
                    rb_ary_new3(3,
                                INT2FIX(GLIB_MAJOR_VERSION),
                                INT2FIX(GLIB_MINOR_VERSION),
                                INT2FIX(GLIB_MICRO_VERSION)));
    rb_define_const(mGLib, "MAJOR_VERSION", INT2FIX(GLIB_MAJOR_VERSION));
    rb_define_const(mGLib, "MINOR_VERSION", INT2FIX(GLIB_MINOR_VERSION));
    rb_define_const(mGLib, "MICRO_VERSION", INT2FIX(GLIB_MICRO_VERSION));

    Init_utils_int64();
    Init_glib_convert();
    Init_glib_messages();
    Init_gobject();
    Init_gutil();
}
