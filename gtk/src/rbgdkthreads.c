/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkthreads.c -

  $Author: mutoh $
  $Date: 2005/03/05 11:16:03 $

  Copyright (C) 2003-2005 Masao Mutoh
************************************************/

#include "global.h"

#ifdef   G_THREADS_ENABLED
static VALUE
rbgdk_threads_init(self)
    VALUE self;
{
#ifndef GDK_WINDOWING_WIN32
    if (!g_thread_supported()){
        g_thread_init(NULL);
    }
    gdk_threads_init();
#endif
    return self;
}

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
    VALUE func = G_BLOCK_PROC();
    gdk_threads_enter();
    func = G_BLOCK_PROC();
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

    rb_define_module_function(mGdkThreads, "init",  rbgdk_threads_init, 0);
    rb_define_module_function(mGdkThreads, "enter", rbgdk_threads_enter, 0);
    rb_define_module_function(mGdkThreads, "leave", rbgdk_threads_leave, 0);
    rb_define_module_function(mGdkThreads, "synchronize", rbgdk_threads_synchronize, 0);
#endif
}
