/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/**********************************************************************

  rbgp-gpa-utils.c -

  $Author: ktou $
  $Date: 2005/10/10 02:07:41 $

  Copyright (C) 2005 Ruby-GNOME2 Project Team
  Copyright (C) 2004 Kouhei Sutou <kou@cozmixng.org>

**********************************************************************/

#include "rbgp.h"
#include <libgnomeprint/gnome-print.h>

static VALUE cGPError, cGPReturnCode;
static ID s_new, s_to_s, s_code;

VALUE
rbgp_check_return_code(gint code) 
{
    if (code != GNOME_PRINT_OK) {
        VALUE rb_code = rb_funcall(cGPReturnCode, s_new, 1, INT2NUM(code));
        rb_exc_raise(rb_funcall(cGPError, s_new, 1, rb_code));
    }
    return Qnil;
}

static VALUE
gp_error_initialize(VALUE self, VALUE code)
{
    VALUE *message = ALLOCA_N(VALUE, 1);
    rb_ivar_set(self, s_code, code);
    message[0] = rb_funcall(code, s_to_s, 0);
    return rb_call_super(1, message);
}

void
Init_rbgp_utils(VALUE mGnome)
{
    cGPError = rb_define_class_under(mGnome, "PrintError", rb_eStandardError);
    cGPReturnCode = rb_const_get(mGnome, rb_intern("PrintReturnCode"));
  
    s_new = rb_intern("new");
    s_to_s = rb_intern("to_s");
    s_code = rb_intern("@code");
  
    rb_define_method(cGPError, "initialize", gp_error_initialize, 1);
    rb_define_attr(cGPError, "code", 1, 0);
}
