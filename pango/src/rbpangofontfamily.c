/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbpangofontfamily.c -

  $Author: sakai $
  $Date: 2003/08/16 05:30:54 $

  Copyright (C) 2002,2003 Masao Mutoh <mutoh@highway.ne.jp>
************************************************/

#include "rbpango.h"

#define _SELF(self) (PANGO_FONT_FAMILY(RVAL2GOBJ(self)))

static VALUE
font_family_get_name(self)
    VALUE self;
{
    return CSTR2RVAL(pango_font_family_get_name(_SELF(self)));
}

static VALUE
font_family_list_faces(self)
    VALUE self;
{
    int n_faces;
    PangoFontFace** faces;
    int i;
    VALUE result;

    pango_font_family_list_faces(_SELF(self),
                                 &faces,
                                 &n_faces);

    result = rb_ary_new2(n_faces);
    for (i = 0; i < n_faces; i++)
      rb_ary_store(result, i, GOBJ2RVAL(faces[i]));

    return result;
}

void
Init_pango_font_family()
{
    VALUE pFamily = G_DEF_CLASS(PANGO_TYPE_FONT_FAMILY, "FontFamily", mPango);
    
    rb_define_method(pFamily, "name", font_family_get_name, 0);
    rb_define_method(pFamily, "list_faces", font_family_list_faces, 0);
}
