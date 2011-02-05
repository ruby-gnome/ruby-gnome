/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgdkinput.c -

  $Author: sakai $
  $Date: 2007/07/08 03:00:50 $

  Copyright (C) 2002,2003 Masao Mutoh

  This file is devided from rbgtkmain.c.
  rbgtkmain.c -
  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

static void
exec_input(gpointer data, gint source, GdkInputCondition condition)
{
    rb_funcall((VALUE)data, id_call, 1, 
               GFLAGS2RVAL(condition, GDK_TYPE_INPUT_CONDITION));
}

static VALUE
input_add(VALUE self, VALUE filedescriptor, VALUE gdk_input_condition)
{
    VALUE id;
    VALUE func;

    func = rb_block_proc();
    id = INT2FIX(gdk_input_add(NUM2INT(rb_funcall(filedescriptor,
                                                  rb_intern("to_i"), 0)),
                               RVAL2GFLAGS(gdk_input_condition, GDK_TYPE_INPUT_CONDITION),
                               (GdkInputFunction)exec_input,
                               (gpointer)func));
    G_RELATIVE2(self, func, id_relative_callbacks, id);
    return id;
}

static VALUE
input_remove(VALUE self, VALUE id)
{
    gdk_input_remove(NUM2INT(id));
    G_REMOVE_RELATIVE(self, id_relative_callbacks, id);
    return id;
}


void
Init_gtk_gdk_input()
{
    VALUE mGdkInput = rb_define_module_under(mGdk, "Input");

    rb_define_module_function(mGdkInput, "add", input_add, 2);
    rb_define_module_function(mGdkInput, "remove", input_remove, 1);

    /* GdkInputCondition */
    G_DEF_CLASS(GDK_TYPE_INPUT_CONDITION, "Condition", mGdkInput);
    G_DEF_CONSTANTS(mGdkInput, GDK_TYPE_INPUT_CONDITION, "GDK_INPUT_");

}

