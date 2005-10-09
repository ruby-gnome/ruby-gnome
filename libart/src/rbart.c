/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/**********************************************************************

  rbart.c -

  $Author: mutoh $
  $Date: 2005/10/09 18:49:17 $

  Copyright (C) 2004,2005  Ruby-GNOME2 Project Team
  Copyright (C) 2002,2003  KUBO Takehiro <kubo@jiubao.org>

**********************************************************************/

#include "rbart.h"

#ifndef HAVE_OBJECT_ALLOCATE
static ID id_allocate;

VALUE
rbart_s_new(int argc, VALUE* argv, VALUE self)
{
    VALUE obj = rb_funcall(self, id_allocate, 0);
    rb_obj_call_init(obj, argc, argv);
    return obj;
}
#endif /* HAVE_OBJECT_ALLOCATE */

static VALUE
rbart_s_allocate(klass)
    VALUE klass;
{
    return Data_Wrap_Struct(klass, 0, xfree, 0);
}

void
rbart_init_func(klass, func)
    VALUE klass;
    void* func;
{
  if (func == NULL)
    func = rbart_s_allocate;
#ifndef HAVE_RB_DEFINE_ALLOC_FUNC
  rb_define_singleton_method(klass, "allocate", func, 0);
#else
  rb_define_alloc_func(klass, func);
#endif
#ifndef HAVE_OBJECT_ALLOCATE
  id_allocate = rb_intern("allocate");
  rb_define_singleton_method(klass, "new", rbart_s_new, -1);
#endif
}

void
Init_libart2()
{
    VALUE mArt = rb_define_module("Art");

    rb_define_const(mArt, "BUILD_VERSION",
                    rb_ary_new3(3,
                                INT2FIX(LIBART_MAJOR_VERSION),
                                INT2FIX(LIBART_MINOR_VERSION),
                                INT2FIX(LIBART_MICRO_VERSION)));

    rb_define_const(mArt, "VERSION",
                    rb_ary_new3(3, INT2FIX(libart_major_version),
                                INT2FIX(libart_minor_version),
                                INT2FIX(libart_micro_version)));

    rb_define_const(mArt, "MAJOR_VERSION", INT2FIX(libart_major_version));
    rb_define_const(mArt, "MINOR_VERSION", INT2FIX(libart_minor_version));
    rb_define_const(mArt, "MICRO_VERSION", INT2FIX(libart_micro_version));
    
    Init_art_affine(mArt);
    Init_art_bpath(mArt);
    Init_art_canvas(mArt);
    Init_art_svp(mArt);
    Init_art_vpath(mArt);
    Init_art_vpathdash(mArt);
    Init_art_rect(mArt);
    Init_art_uta(mArt);
}
