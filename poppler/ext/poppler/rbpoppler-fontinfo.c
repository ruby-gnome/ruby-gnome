/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2006  Ruby-GNOME2 Project Team
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

#include "rbpoppler.h"

#define RG_TARGET_NAMESPACE cFontInfo

static VALUE RG_TARGET_NAMESPACE;

#if POPPLER_CHECK_VERSION(0, 6, 0)

static ID id_valid;

static VALUE
rg_initialize(VALUE self, VALUE document)
{
    G_INITIALIZE(self, poppler_font_info_new(RVAL2POPPLERDOCUMENT(document)));
    return Qnil;
}

static VALUE
rg_scan(VALUE self, VALUE n_pages)
{
    VALUE rb_iter = Qnil;
    PopplerFontsIter *iter;

    if (poppler_font_info_scan(RVAL2FONTINFO(self), NUM2INT(n_pages), &iter)) {
        rb_iter = POPPLERFONTSITER2RVAL(iter);
        rb_ivar_set(rb_iter, id_valid, Qtrue);
        poppler_fonts_iter_free(iter);
    }
    return rb_iter;
}
#endif

void
Init_poppler_fontinfo(VALUE mPoppler)
{
    id_valid = rb_intern("valid?");

    RG_TARGET_NAMESPACE = G_DEF_CLASS(POPPLER_TYPE_FONT_INFO, "FontInfo", mPoppler);

#if POPPLER_CHECK_VERSION(0, 6, 0)
    RG_DEF_METHOD(initialize, 1);
    RG_DEF_METHOD(scan, 1);
    G_DEF_SETTERS(RG_TARGET_NAMESPACE);
#endif
}
