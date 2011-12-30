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

#include "rbpoppler-private.h"

#define RG_TARGET_NAMESPACE cFontsIter

#define RVAL2FITER(obj) (RVAL2POPPLERFONTSITER(obj))

#define CHECK_FITER_IS_VALID(iter) do {         \
    if (!RVAL2CBOOL(rg_valid_p(iter)))       \
        return Qnil;                            \
} while (0)

static ID id_valid;

static VALUE
rg_valid_p(VALUE self)
{
    return rb_ivar_get(self, id_valid);
}

static VALUE
rg_name(VALUE self)
{
    CHECK_FITER_IS_VALID(self);
    return CSTR2RVAL(poppler_fonts_iter_get_name(RVAL2FITER(self)));
}

static VALUE
rg_full_name(VALUE self)
{
    CHECK_FITER_IS_VALID(self);
    return CSTR2RVAL(poppler_fonts_iter_get_full_name(RVAL2FITER(self)));
}

static VALUE
rg_file_name(VALUE self)
{
    CHECK_FITER_IS_VALID(self);
    return CSTR2RVAL(poppler_fonts_iter_get_file_name(RVAL2FITER(self)));
}

static VALUE
rg_font_type(VALUE self)
{
    CHECK_FITER_IS_VALID(self);
    return GENUM2RVAL(poppler_fonts_iter_get_font_type(RVAL2FITER(self)),
                      POPPLER_TYPE_FONT_TYPE);
}

static VALUE
rg_embedded_p(VALUE self)
{
    CHECK_FITER_IS_VALID(self);
    return CBOOL2RVAL(poppler_fonts_iter_is_embedded(RVAL2FITER(self)));
}

static VALUE
rg_subset_p(VALUE self)
{
    CHECK_FITER_IS_VALID(self);
    return CBOOL2RVAL(poppler_fonts_iter_is_subset(RVAL2FITER(self)));
}

static VALUE
rg_next(VALUE self)
{
    if (poppler_fonts_iter_next(RVAL2FITER(self))) {
        return Qtrue;
    } else {
        rb_ivar_set(self, id_valid, Qfalse);
        return Qfalse;
    }
}

static VALUE
rg_each(VALUE self)
{
    PopplerFontsIter *iter;

    CHECK_FITER_IS_VALID(self);
    iter = RVAL2FITER(self);
    do {
        rb_yield(self);
    } while (poppler_fonts_iter_next(iter));
    rb_ivar_set(self, id_valid, Qfalse);

    return self;
}

void
Init_poppler_fontsiter(VALUE mPoppler)
{
    id_valid = rb_intern("valid?");

    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(POPPLER_TYPE_FONTS_ITER, "FontsIter", mPoppler);

    rb_include_module(RG_TARGET_NAMESPACE, rb_mEnumerable);

    RG_DEF_METHOD(name, 0);
    RG_DEF_METHOD(full_name, 0);
#if POPPLER_CHECK_VERSION(0, 6, 0)
    RG_DEF_METHOD(file_name, 0);
#endif
    RG_DEF_METHOD(font_type, 0);
    RG_DEF_METHOD_P(embedded, 0);
    RG_DEF_METHOD_P(subset, 0);
    RG_DEF_METHOD(next, 0);

    RG_DEF_METHOD_P(valid, 0);
    RG_DEF_METHOD(each, 0);

    G_DEF_SETTERS(RG_TARGET_NAMESPACE);
}
