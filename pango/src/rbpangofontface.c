/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbpangofontface.c -

  $Author: mutoh $
  $Date: 2002/12/31 07:00:57 $

  Copyright (C) 2002 Masao Mutoh <mutoh@highway.ne.jp>
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

void
Init_pango_font_face()
{
    VALUE pFace = G_DEF_CLASS(PANGO_TYPE_FONT_FACE, "FontFace", mPango);
    
    rb_define_method(pFace, "name", font_face_get_face_name, 0);
    rb_define_method(pFace, "describe", font_face_describe, 0);
}
