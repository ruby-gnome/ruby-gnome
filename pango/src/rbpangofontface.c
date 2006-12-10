/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbpangofontface.c -

  $Author: mutoh $
  $Date: 2006/12/10 15:13:10 $

  Copyright (C) 2002,2003 Masao Mutoh <mutoh@highway.ne.jp>
************************************************/

#include "rbpango.h"

#define _SELF(self) (PANGO_FONT_FACE(RVAL2GOBJ(self)))

static VALUE
font_face_get_face_name(self)
    VALUE self;
{
    return CSTR2RVAL(pango_font_face_get_face_name(_SELF(self)));
}

static VALUE
font_face_describe(self)
    VALUE self;
{
    return BOXED2RVAL(pango_font_face_describe(_SELF(self)), PANGO_TYPE_FONT_DESCRIPTION);
}

#if PANGO_CHECK_VERSION(1,4,0)
static VALUE
font_face_list_sizes(self)
    VALUE self;
{
    int n_sizes;
    int* sizes;
    int i;
    VALUE result;

    pango_font_face_list_sizes(_SELF(self),
                               &sizes,
                               &n_sizes);

    result = rb_ary_new();
    for (i = 0; i < n_sizes; i++)
      rb_ary_push(result, GOBJ2RVAL(&sizes[i]));

    g_free(sizes);
    return result;
}
#endif

void
Init_pango_font_face()
{
    VALUE pFace = G_DEF_CLASS(PANGO_TYPE_FONT_FACE, "FontFace", mPango);
    
    rb_define_method(pFace, "name", font_face_get_face_name, 0);
    rb_define_method(pFace, "describe", font_face_describe, 0);
#if PANGO_CHECK_VERSION(1,4,0)
    rb_define_method(pFace, "sizes", font_face_list_sizes, 0);
#endif
    G_DEF_CLASS3("PangoFcFace", "FcFace", mPango);
    G_DEF_CLASS3("PangoFT2Face", "FT2Face", mPango);
    G_DEF_CLASS3("PangoXftFace", "XftFace", mPango);
    G_DEF_CLASS3("PangoXFace", "XFace", mPango);
    G_DEF_CLASS3("PangoWin32Face", "Win32Face", mPango);
#if PANGO_CHECK_VERSION(1,12,0)
    G_DEF_CLASS3("PangoATSUIFace", "ATSUIFace", mPango);
#endif
}
