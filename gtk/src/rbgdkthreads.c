/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkthreads.c -

  $Author: mutoh $
  $Date: 2003/03/08 16:44:41 $

  Copyright (C) 2003 Masao Mutoh
************************************************/

#include "global.h"

/*
    gdk_threads_init() is called in Init_gtk2().
*/
#ifdef   G_THREADS_ENABLED
static VALUE
rbgdk_threads_enter(self)
    VALUE self;
{
    gdk_threads_enter();
    return self;
}

static VALUE
rbgdk_threads_leave(self)
    VALUE self;
{
    gdk_threads_leave();
    return self;
}

static VALUE
rbgdk_threads_synchronize(self)
    VALUE self;
{
    VALUE func = rb_f_lambda();
    gdk_threads_enter();
    func = rb_f_lambda();
    rb_funcall(func, id_call, 0);
    gdk_threads_leave();
    return Qnil;
}
#endif

void
Init_gtk_gdk_threads()
{
#ifdef   G_THREADS_ENABLED
    VALUE mGdkThreads = rb_define_module_under(mGdk, "Threads");

    rb_define_module_function(mGdkThreads, "enter", rbgdk_threads_enter, 0);
    rb_define_module_function(mGdkThreads, "leave", rbgdk_threads_leave, 0);
    rb_define_module_function(mGdkThreads, "synchronize", rbgdk_threads_synchronize, 0);
#endif
}
