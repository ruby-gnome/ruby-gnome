/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbpangoscript.c -

  $Author: mutoh $
  $Date: 2005/09/17 17:09:13 $

  Copyright (C) 2005 Masao Mutoh
************************************************/

#include "rbpango.h"

#if PANGO_CHECK_VERSION(1,4,0)
#define _SELF(r) (RVAL2GENUM(r, PANGO_TYPE_SCRIPT))

static VALUE
rbpango_s_script_for_unichar(self, ch)
    VALUE self, ch;
{
    return GENUM2RVAL(pango_script_for_unichar(NUM2UINT(ch)), PANGO_TYPE_SCRIPT);
}

static VALUE
rbpango_script_get_sample_language(self)
    VALUE self;
{
    PangoLanguage* lang = pango_script_get_sample_language(_SELF(self));
    return lang ? BOXED2RVAL(lang, PANGO_TYPE_LANGUAGE) : Qnil;
}

/* Move to Pango::Language
gboolean    pango_language_includes_script  (PangoLanguage *language,
                                             PangoScript script);
*/
#endif

void
Init_pango_script()
{
#if PANGO_CHECK_VERSION(1,4,0)
    VALUE script = G_DEF_CLASS(PANGO_TYPE_SCRIPT, "Script", mPango);

    rb_define_singleton_method(script, "for_unichar", rbpango_s_script_for_unichar, 1);
    rb_define_method(script, "sample_language", rbpango_script_get_sample_language, 0);
#endif
}

