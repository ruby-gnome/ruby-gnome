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

#define RG_TARGET_NAMESPACE cCoverage
#define _SELF(self) (RVAL2PANGOCOVERAGE(self))

/**********************************/
GType
pango_coverage_get_type(void)
{
    static GType our_type = 0;

    if (our_type == 0)
        our_type = g_boxed_type_register_static ("PangoCoverage",
                    (GBoxedCopyFunc)pango_coverage_copy,
                    (GBoxedFreeFunc)pango_coverage_unref);
    return our_type;
}
/**********************************/

static VALUE
rg_initialize(VALUE self)
{
    G_INITIALIZE(self, pango_coverage_new());
    return Qnil;
}

static VALUE
rg_get_level(VALUE self, VALUE index_)
{
    return PANGOCOVERAGELEVEL2RVAL(pango_coverage_get(_SELF(self), NUM2INT(index_)));
}

static VALUE
rg_max_bang(VALUE self, VALUE other)
{
    pango_coverage_max(_SELF(self), _SELF(other));
    return self;
}

static VALUE
rg_set(VALUE self, VALUE index_, VALUE level)
{
    pango_coverage_set(_SELF(self), NUM2INT(index_), 
                       RVAL2PANGOCOVERAGELEVEL(level));
    return self;
}

static VALUE
rg_to_bytes(VALUE self)
{
    guchar* bytes;
    int n_bytes;
    VALUE ret;

    pango_coverage_to_bytes(_SELF(self), &bytes, &n_bytes);
    ret = rb_str_new((const char*)bytes, n_bytes);
    g_free(bytes);

    return ret;
}

static VALUE
rg_s_from_bytes(G_GNUC_UNUSED VALUE self, VALUE bytes)
{
    StringValue(bytes);
    return PANGOCOVERAGE2RVAL(pango_coverage_from_bytes((guchar *)RSTRING_PTR(bytes), 
                                                        RSTRING_LEN(bytes)));
}

void
Init_pango_coverage(VALUE mPango)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(PANGO_TYPE_COVERAGE, "Coverage", mPango);

    RG_DEF_METHOD(initialize, 0);
    RG_DEF_METHOD(get_level, 0);
    RG_DEF_METHOD_BANG(max, 1);
    RG_DEF_METHOD(set, 2);
    RG_DEF_METHOD(to_bytes, 0);
    RG_DEF_SMETHOD(from_bytes, 1);

    /* PangoCoverageLevel */
    G_DEF_CLASS(PANGO_TYPE_COVERAGE_LEVEL, "Level", RG_TARGET_NAMESPACE);
    G_DEF_CONSTANTS(RG_TARGET_NAMESPACE, PANGO_TYPE_COVERAGE_LEVEL, "PANGO_COVERAGE_");
}
