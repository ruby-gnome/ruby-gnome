/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbpangofontfamily.c -

  $Author: mutoh $
  $Date: 2003/02/01 17:13:25 $

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

/*
void        pango_font_family_list_faces    (PangoFontFamily *family,
                                             PangoFontFace ***faces,
                                             int *n_faces);
*/

void
Init_pango_font_family()
{
    VALUE pFamily = G_DEF_CLASS(PANGO_TYPE_FONT_FAMILY, "FontFamily", mPango);
    
    rb_define_method(pFamily, "name", font_family_get_name, 0);
}
