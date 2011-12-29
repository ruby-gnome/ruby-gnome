/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2005  Masao Mutoh
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 *  MA  02110-1301  USA
 */

#include "rbpangoprivate.h"

#if PANGO_CHECK_VERSION(1,4,0)
#define RG_TARGET_NAMESPACE cScript
#define _SELF(r) (RVAL2PANGOSCRIPT(r))

static VALUE
rg_s_for_unichar(G_GNUC_UNUSED VALUE self, VALUE ch)
{
    return PANGOSCRIPT2RVAL(pango_script_for_unichar(NUM2UINT(ch)));
}

static VALUE
rg_sample_language(VALUE self)
{
    PangoLanguage* lang = pango_script_get_sample_language(_SELF(self));
    return PANGOLANGUAGE2RVAL(lang);
}

#if PANGO_CHECK_VERSION(1,16,0)
static VALUE
rg_get_gravity(int argc, VALUE *argv, VALUE self)
{
    VALUE wide, gravity, gravity_hint;
    int n;
    PangoGravity g;

    n = rb_scan_args(argc, argv, "21", &gravity, &gravity_hint, &wide);

    if (n == 2) {
        g = pango_gravity_get_for_script(_SELF(self),
                                         RVAL2PANGOGRAVITY(gravity),
                                         RVAL2PANGOGRAVITYHINT(gravity_hint));
    } else {
#  if PANGO_CHECK_VERSION(1,26,0)
        g = pango_gravity_get_for_script_and_width(_SELF(self),
                                                   RVAL2CBOOL(wide),
                                                   RVAL2PANGOGRAVITY(gravity),
                                                   RVAL2PANGOGRAVITYHINT(gravity_hint));
#  else
        rb_raise(rb_eArgError,
                 "the 3rd 'wide' argument requires Pango >= 1.26");
#  endif
    }
    return PANGOGRAVITY2RVAL(g);
}
#endif

/* Move to Pango::Language
gboolean    pango_language_includes_script  (PangoLanguage *language,
                                             PangoScript script);
*/
#endif

void
Init_pango_script(VALUE mPango)
{
#if PANGO_CHECK_VERSION(1,4,0)
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(PANGO_TYPE_SCRIPT, "Script", mPango);

    RG_DEF_SMETHOD(for_unichar, 1);
    RG_DEF_METHOD(sample_language, 0);
#endif
#if PANGO_CHECK_VERSION(1,16,0)
    RG_DEF_METHOD(get_gravity, -1);
#endif
}
