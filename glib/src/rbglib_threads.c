/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbglib_threads.c -

  $Author: mutoh $
  $Date: 2005/03/05 07:04:19 $

  Copyright (C) 2005 Masao Mutoh
************************************************/

#include "global.h"

static VALUE gthreads;

static VALUE
gt_init(self)
    VALUE self;
{
#ifdef G_THREADS_ENABLED
    g_thread_init(NULL);
#endif
    return self;
}

static VALUE
gt_supported(self)
    VALUE self;
{
#ifdef G_THREADS_ENABLED
    return CBOOL2RVAL(g_thread_supported());
#else
    return Qfalse;
#endif
}

void
Init_glib_threads()
{
    gthreads = rb_define_class_under(mGLib, "Thread", rb_cObject);

    rb_define_singleton_method(gthreads, "init", gt_init, 0);
    rb_define_singleton_method(gthreads, "supported?", gt_supported, 0);
}
