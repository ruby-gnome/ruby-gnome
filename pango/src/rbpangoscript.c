/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbpangoscript.c -

  $Author: ggc $
  $Date: 2006/06/22 19:52:54 $

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
    return BOXED2RVAL(lang, PANGO_TYPE_LANGUAGE);
}

#if PANGO_CHECK_VERSION(1,16,0)
static VALUE
rbpango_script_get_gravity(int argc, VALUE *argv, VALUE self)
{
    VALUE wide, gravity, gravity_hint;
    int n;
    PangoGravity g;

    n = rb_scan_args(argc, argv, "21", &gravity, &gravity_hint, &wide);

    if (n == 2) {
        g = pango_gravity_get_for_script(_SELF(self),
                                         RVAL2GENUM(gravity, PANGO_TYPE_GRAVITY),
                                         RVAL2GENUM(gravity_hint, PANGO_TYPE_GRAVITY_HINT));
    } else {
        g = pango_gravity_get_for_script_and_width(_SELF(self),
                                                   RVAL2CBOOL(wide),
                                                   RVAL2GENUM(gravity, PANGO_TYPE_GRAVITY),
                                                   RVAL2GENUM(gravity_hint, PANGO_TYPE_GRAVITY_HINT));
    }
    return GENUM2RVAL(g, PANGO_TYPE_GRAVITY);
}
#endif

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
#if PANGO_CHECK_VERSION(1,16,0)
    rb_define_method(script, "gravity", rbpango_script_get_gravity, -1);
#endif
}

