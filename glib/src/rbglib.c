/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/**********************************************************************

  rbglib.c -

  $Author: mutoh $
  $Date: 2003/02/10 16:45:22 $

  Copyright (C) 2002,2003  Masahiro Sakai

**********************************************************************/

#include <glib.h>
#include "global.h"

VALUE mGLib;

extern void Init_utils_int64();
extern void Init_glib_convert();
extern void Init_glib_messages();
extern void Init_gobject();

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

    rb_define_const(mGLib, "VERSION",
                    rb_ary_new3(3,
                                INT2FIX(GLIB_MAJOR_VERSION),
                                INT2FIX(GLIB_MINOR_VERSION),
                                INT2FIX(GLIB_MICRO_VERSION)));
    rb_define_const(mGLib, "MAJOR_VERSION", INT2FIX(GLIB_MAJOR_VERSION));
    rb_define_const(mGLib, "MINOR_VERSION", INT2FIX(GLIB_MINOR_VERSION));
    rb_define_const(mGLib, "MICRO_VERSION", INT2FIX(GLIB_MICRO_VERSION));

    rb_eval_string(
        "module GLib\n"
        "  def __add_one_arg_setter(klass)\n"
        "    ary = klass.instance_methods\n"
        "    ary.each do |m|\n"
        "      if /^set_(.*)/ =~ m and not ary.include? \"#{$1}=\" and klass.instance_method(m).arity == 1\n"
        "        klass.module_eval(\"def #{$1}=(val); set_#{$1}(val); val; end\\n\")\n"
        "      end\n"
        "    end\n"
        "  end\n"
        "  module_function :__add_one_arg_setter\n"
        "end\n");

    Init_utils_int64();
    Init_glib_convert();
    Init_glib_messages();
    Init_gobject();
    Init_gutil();
}
