/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbpangofontfamily.c -

  $Author: mutoh $
  $Date: 2005/03/05 18:46:23 $

  Copyright (C) 2002-2005 Masao Mutoh <mutoh@highway.ne.jp>
************************************************/

#include "rbpango.h"

#define _SELF(self) (PANGO_FONT_FAMILY(RVAL2GOBJ(self)))

static VALUE
font_family_get_name(self)
    VALUE self;
{
    return CSTR2RVAL(pango_font_family_get_name(_SELF(self)));
}

#if PANGO_CHECK_VERSION(1,2,0)
static VALUE
font_family_is_monospace(self)
    VALUE self;
{
    return CBOOL2RVAL(pango_font_family_is_monospace(_SELF(self)));
}
#endif

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

    g_free(faces);

    return result;
}

void
Init_pango_font_family()
{
    VALUE pFamily = G_DEF_CLASS(PANGO_TYPE_FONT_FAMILY, "FontFamily", mPango);
    
    rb_define_method(pFamily, "name", font_family_get_name, 0);
#if PANGO_CHECK_VERSION(1,2,0)
    rb_define_method(pFamily, "monospace?", font_family_is_monospace, 0);
#endif
    rb_define_method(pFamily, "faces", font_family_list_faces, 0);

    G_DEF_CLASS3("PangoFcFamily", "FcFamily", mPango);
    G_DEF_CLASS3("PangoXftFamily", "XftFamily", mPango);
}
