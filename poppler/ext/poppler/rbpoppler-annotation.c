/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2008  Ruby-GNOME2 Project Team
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

#define RG_TARGET_NAMESPACE cAnnotation
#define SELF(self) (RVAL2POPPLERANNOT(self))

static VALUE
rg_type(VALUE self)
{
    return POPPLERANNOTTYPE2RVAL(poppler_annot_get_annot_type(SELF(self)));
}

static VALUE
rg_contents(VALUE self)
{
    return CSTR2RVAL_FREE(poppler_annot_get_contents(SELF(self)));
}

static VALUE
rg_name(VALUE self)
{
    return CSTR2RVAL_FREE(poppler_annot_get_name(SELF(self)));
}

static VALUE
rg_modified(VALUE self)
{
    return CSTR2RVAL_FREE(poppler_annot_get_modified(SELF(self)));
}

static VALUE
rg_flags(VALUE self)
{
    return POPPLERANNOTFLAG2RVAL(poppler_annot_get_flags(SELF(self)));
}

static VALUE
rg_color(VALUE self)
{
    return POPPLERCOLOR2RVAL_FREE(poppler_annot_get_color(SELF(self)));
}

void
Init_poppler_annotation(VALUE mPoppler)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(POPPLER_TYPE_ANNOT, "Annotation", mPoppler);

    RG_DEF_METHOD(type, 0);
    RG_DEF_METHOD(contents, 0);
    RG_DEF_METHOD(name, 0);
    RG_DEF_METHOD(modified, 0);
    RG_DEF_METHOD(flags, 0);
    RG_DEF_METHOD(color, 0);
}
