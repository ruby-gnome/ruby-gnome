/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/**********************************************************************

  rbart_rect.c - Art::DRect and Art::IRect classes of ruby

  $Author: ktou $
  $Date: 2005/06/10 14:34:01 $

  Copyright (C) 2005  Ruby-GNOME2 Project Team

**********************************************************************/

#include "rbart.h"

#define DRect_Ptr(obj) ((ArtDRect *)(RDATA(obj)->data))
#define DRect_Set(obj, val) memcpy(DRect_Ptr(obj), val, sizeof(ArtDrect))
#define IRect_Ptr(obj) ((ArtIRect *)(RDATA(obj)->data))
#define IRect_Set(obj, val) memcpy(IRect_Ptr(obj), val, sizeof(ArtIrect))

#define DEF_ACCESSOR(TARGET, GETTER, NAME)      \
static VALUE                                    \
TARGET ## _ ## NAME(VALUE self)                 \
{                                               \
  return rb_float_new(GETTER(self)->NAME);      \
}

#define DEF_CLASS(TARGET, TYPE, TYPE_NAME)                  \
VALUE art ## TYPE;                                          \
                                                            \
Art ## TYPE *                                               \
get_art_ ## TARGET(VALUE obj)                               \
{                                                           \
  if (!rb_obj_is_instance_of(obj, art ## TYPE)) {           \
    rb_raise(rb_eTypeError, "not an Art::" TYPE_NAME);      \
  } else {                                                  \
    return TYPE ## _Ptr(obj);                               \
  }                                                         \
}                                                           \
                                                            \
VALUE                                                       \
make_art_ ## TARGET(Art ## TYPE *rect)                      \
{                                                           \
  Art ## TYPE *ptr = xmalloc(sizeof(Art ## TYPE));          \
  memcpy(ptr, rect, sizeof(Art ## TYPE));                   \
  return Data_Wrap_Struct(art ## TYPE, 0, xfree, ptr);      \
}                                                           \
                                                            \
                                                            \
static VALUE                                                \
TARGET ## _initialize(int argc, VALUE *argv, VALUE self)    \
{                                                           \
  Art ## TYPE *rect = xmalloc(sizeof(Art ## TYPE));         \
                                                            \
  if (argc == 1) {                                          \
    Check_Type(argv[0], T_ARRAY);                           \
    if (RARRAY(argv[0])->len != 4) {                        \
      rb_raise(rb_eArgError,                                \
               "wrong size of Array (%d for 4)",            \
               RARRAY(argv[0])->len);                       \
    } else {                                                \
      argv = RARRAY(argv[0])->ptr;                          \
    }                                                       \
  } else if (argc != 4) {                                   \
    rb_raise(rb_eArgError,                                  \
             "wrong number of arguments (%d for 4)",        \
             argc);                                         \
  }                                                         \
                                                            \
  rect->x0 = NUM2DBL(argv[0]);                              \
  rect->y0 = NUM2DBL(argv[1]);                              \
  rect->x1 = NUM2DBL(argv[2]);                              \
  rect->y1 = NUM2DBL(argv[3]);                              \
                                                            \
  RDATA(self)->data = rect;                                 \
                                                            \
  return Qnil;                                              \
}                                                           \
                                                            \
DEF_ACCESSOR(TARGET, TYPE ## _Ptr, x0)                      \
DEF_ACCESSOR(TARGET, TYPE ## _Ptr, y0)                      \
DEF_ACCESSOR(TARGET, TYPE ## _Ptr, x1)                      \
DEF_ACCESSOR(TARGET, TYPE ## _Ptr, y1)                      \
                                                            \
                                                            \
static VALUE                                                \
TARGET ## _copy(VALUE self)                                 \
{                                                           \
  Art ## TYPE dest;                                         \
  art_ ## TARGET ## _copy(&dest, TYPE ## _Ptr(self));       \
  return make_art_ ## TARGET(&dest);                        \
}                                                           \
                                                            \
static VALUE                                                \
TARGET ## _union(VALUE self, VALUE other)                   \
{                                                           \
  Art ## TYPE dest;                                         \
  art_ ## TARGET ## _union(&dest,                           \
                           TYPE ## _Ptr(self),              \
                           get_art_ ## TARGET(other));      \
  return make_art_ ## TARGET(&dest);                        \
}                                                           \
                                                            \
static VALUE                                                \
TARGET ## _intersect(VALUE self, VALUE other)               \
{                                                           \
  Art ## TYPE dest;                                         \
  art_ ## TARGET ## _intersect(&dest,                       \
                               TYPE ## _Ptr(self),          \
                               get_art_ ## TARGET(other));  \
  return make_art_ ## TARGET(&dest);                        \
}                                                           \
                                                            \
static VALUE                                                \
TARGET ## _empty(VALUE self)                                \
{                                                           \
  if (art_ ## TARGET ## _empty(TYPE ## _Ptr(self))) {       \
    return Qtrue;                                           \
  } else {                                                  \
    return Qfalse;                                          \
  }                                                         \
}                                                           \
                                                            \
static VALUE                                                \
TARGET ## _to_a(VALUE self)                                 \
{                                                           \
  return rb_ary_new3(4,                                     \
                     TARGET ## _x0(self),                   \
                     TARGET ## _y0(self),                   \
                     TARGET ## _x1(self),                   \
                     TARGET ## _y1(self));                  \
}                                                           \
                                                            \
static VALUE                                                \
TARGET ## _to_s(VALUE self)                                 \
{                                                           \
  VALUE ret;                                                \
  VALUE to_s = rb_intern("to_s");                           \
                                                            \
  ret = rb_str_new2("<");                                   \
  rb_str_cat2(ret, rb_class2name(CLASS_OF(self)));          \
  rb_str_cat2(ret, ":");                                    \
  rb_str_cat2(ret, " x0:");                                 \
  rb_str_append(ret, rb_funcall(TARGET ## _x0(self),        \
                                to_s, 0, 0));               \
  rb_str_cat2(ret, " y0:");                                 \
  rb_str_append(ret, rb_funcall(TARGET ## _y0(self),        \
                                to_s, 0, 0));               \
  rb_str_cat2(ret, " x1:");                                 \
  rb_str_append(ret, rb_funcall(TARGET ## _x1(self),        \
                                to_s, 0, 0));               \
  rb_str_cat2(ret, " y1:");                                 \
  rb_str_append(ret, rb_funcall(TARGET ## _y1(self),        \
                                to_s, 0, 0));               \
  rb_str_cat2(ret, ">");                                    \
                                                            \
  return ret;                                               \
}


DEF_CLASS(irect, IRect, "IRect")
DEF_CLASS(drect, DRect, "DRect")


static VALUE
drect_affine_transform(VALUE self, VALUE matrix)
{
  ArtDRect dest;
  art_drect_affine_transform(&dest, DRect_Ptr(self), get_art_affine(matrix));
  return make_art_drect(&dest);
}

static VALUE
drect_to_irect(VALUE self)
{
  ArtIRect dest;
  art_drect_to_irect(&dest, DRect_Ptr(self));
  return make_art_irect(&dest);
}


#define REGISTER_CLASS(TARGET, TYPE, TYPE_NAME, ART_MODULE) do  \
{                                                               \
  art ## TYPE = rb_define_class_under(ART_MODULE,               \
                                      TYPE_NAME,                \
                                      rb_cData);                \
                                                                \
  RBART_INIT_FUNC(art ## TYPE);                                 \
                                                                \
  rb_define_method(art ## TYPE, "initialize",                   \
                   TARGET ## _initialize, -1);                  \
                                                                \
  rb_define_method(art ## TYPE, "x0", TARGET ## _x0, 0);        \
  rb_define_method(art ## TYPE, "y0", TARGET ## _y0, 0);        \
  rb_define_method(art ## TYPE, "x1", TARGET ## _x1, 0);        \
  rb_define_method(art ## TYPE, "y1", TARGET ## _y1, 0);        \
                                                                \
  rb_define_method(art ## TYPE, "dup", TARGET ## _copy, 0);     \
  rb_define_method(art ## TYPE, "|", TARGET ## _union, 1);      \
  rb_define_method(art ## TYPE, "&", TARGET ## _intersect, 1);  \
  rb_define_method(art ## TYPE, "empty?", TARGET ## _empty, 0); \
                                                                \
  rb_define_method(art ## TYPE, "to_a", TARGET ## _to_a, 0);    \
  rb_define_method(art ## TYPE, "to_s", TARGET ## _to_s, 0);    \
} while(0)


void
Init_art_rect(VALUE mArt)
{
  REGISTER_CLASS(irect, IRect, "IRect", mArt);
  REGISTER_CLASS(drect, DRect, "DRect", mArt);
  
  rb_define_method(artDRect, "transform", drect_affine_transform, 1);
  rb_define_method(artDRect, "to_irect", drect_to_irect, 0);
}
