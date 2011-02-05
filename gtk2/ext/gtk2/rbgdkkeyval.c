/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgdkkeyval.c -

  $Author: ggc $
  $Date: 2007/07/13 14:27:08 $

  Copyright (C) 2002,2003 Ruby-GNOME2 Project Team
  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"
#include <gdk/gdkkeysyms.h>

static VALUE
keyval_to_name(VALUE self, VALUE keyval)
{
    gchar* name = gdk_keyval_name(NUM2UINT(keyval));
    return name ? CSTR2RVAL(name) : Qnil;
}

static VALUE
keyval_from_name(VALUE self, VALUE keyval_name)
{
    return UINT2NUM(gdk_keyval_from_name(RVAL2CSTR(keyval_name)));
}

static VALUE
keyval_is_upper(VALUE self, VALUE keyval)
{
    return CBOOL2RVAL(gdk_keyval_is_upper(NUM2UINT(keyval)));
}

static VALUE
keyval_is_lower(VALUE self, VALUE keyval)
{
    return CBOOL2RVAL(gdk_keyval_is_lower(NUM2UINT(keyval)));
}

static VALUE
keyval_convert_case(VALUE self, VALUE symbol)
{
    guint upper, lower;
    gdk_keyval_convert_case(NUM2UINT(symbol), &lower, &upper);
    return rb_ary_new3(2, UINT2NUM(lower), UINT2NUM(upper));
}

static VALUE
keyval_to_upper(VALUE self, VALUE keyval)
{
    return INT2NUM(gdk_keyval_to_upper(NUM2UINT(keyval)));
}

static VALUE
keyval_to_lower(VALUE self, VALUE keyval)
{
    return INT2NUM(gdk_keyval_to_lower(NUM2UINT(keyval)));
}

static VALUE
keyval_to_unicode(VALUE self, VALUE keyval)
{
    return UINT2NUM(gdk_keyval_to_unicode(NUM2UINT(keyval)));
}

static VALUE
unicode_to_keyval(VALUE self, VALUE wc)
{
    VALUE unicode;
    if (TYPE(wc) == T_STRING) {
        unicode = NUM2UINT(rb_funcall(wc, rb_intern("[]"), 1, INT2FIX(0)));
    } else {
        unicode = NUM2UINT(wc);
    }
    return UINT2NUM(gdk_unicode_to_keyval(unicode));
}

void
Init_gtk_gdk_keyval()
{
    VALUE mGdkKeyval = rb_define_module_under(mGdk, "Keyval");
    rb_define_module_function(mGdkKeyval, "to_name", keyval_to_name, 1);
    rb_define_module_function(mGdkKeyval, "from_name", keyval_from_name, 1);
    rb_define_module_function(mGdkKeyval, "upper?", keyval_is_upper, 1);
    rb_define_module_function(mGdkKeyval, "lower?", keyval_is_lower, 1);
    rb_define_module_function(mGdkKeyval, "convert_case", keyval_convert_case, 1);
    rb_define_module_function(mGdkKeyval, "to_upper", keyval_to_upper, 1);
    rb_define_module_function(mGdkKeyval, "to_lower", keyval_to_lower, 1);
    rb_define_module_function(mGdkKeyval, "to_unicode", keyval_to_unicode, 1);
    rb_define_module_function(mGdkKeyval, "from_unicode", unicode_to_keyval, 1);

#include "rbgdkkeysyms.h"
}
