/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/**********************************************************************

  rbart.c -

  $Author: mutoh $
  $Date: 2004/11/13 11:19:13 $

  Copyright (C) 2004  Ruby-GNOME2 Project Team
  Copyright (C) 2002,2003  KUBO Takehiro <kubo@jiubao.org>

**********************************************************************/

#include "rbart.h"

#ifndef HAVE_OBJECT_ALLOCATE
VALUE
rbart_s_new(int argc, VALUE* argv, VALUE self)
{
    VALUE obj = rb_funcall(self, id_allocate, 0);
    rb_obj_call_init(obj, argc, argv);
    return obj;
}
#endif /* HAVE_OBJECT_ALLOCATE */

#ifdef HAVE_OBJECT_ALLOCATE
static VALUE
rbart_s_allocate(klass)
    VALUE klass;
{
    return Data_Wrap_Struct(klass, 0, xfree, 0);
}
#endif

void
rbart_init_func(klass, func)
    VALUE klass;
    void* func;
{
    if (func == NULL)
        func = rbart_s_allocate;
#ifndef HAVE_RB_DEFINE_ALLOC_FUNC
    rb_define_singleton_method(klasss, "allocate", func, 0);
#else
    rb_define_alloc_func(klass, func);
#endif
#ifndef HAVE_OBJECT_ALLOCATE
    rb_define_singleton_method(klass, "new", rbart_s_new, -1);
#endif
}

void
Init_libart2()
{
    VALUE mArt = rb_define_module("Art");

    Init_art_affine(mArt);
    Init_art_bpath(mArt);
    Init_art_canvas(mArt);
    Init_art_svp(mArt);
    Init_art_vpath(mArt);
    Init_art_vpathdash(mArt);
}
