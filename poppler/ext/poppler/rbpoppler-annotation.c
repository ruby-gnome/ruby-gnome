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
#define SELF(self) (POPPLER_ANNOT(RVAL2GOBJ(self)))

#define TYPE2RVAL(obj) (GENUM2RVAL(obj, POPPLER_TYPE_ANNOT_TYPE))
#define FLAG2RVAL(obj) (GENUM2RVAL(obj, POPPLER_TYPE_ANNOT_FLAG))

static VALUE
annot_get_annot_type(VALUE self)
{
    return TYPE2RVAL(poppler_annot_get_annot_type(SELF(self)));
}

static VALUE
annot_get_contents(VALUE self)
{
    return CSTR2RVAL_FREE(poppler_annot_get_contents(SELF(self)));
}

static VALUE
annot_get_name(VALUE self)
{
    return CSTR2RVAL_FREE(poppler_annot_get_name(SELF(self)));
}

static VALUE
annot_get_modified(VALUE self)
{
    return CSTR2RVAL_FREE(poppler_annot_get_modified(SELF(self)));
}

static VALUE
annot_get_flags(VALUE self)
{
    return FLAG2RVAL(poppler_annot_get_flags(SELF(self)));
}

static VALUE
annot_get_color(VALUE self)
{
    return POPPLER_COLOR2RVAL_FREE(poppler_annot_get_color(SELF(self)));
}

void
Init_poppler_annotation(VALUE mPoppler)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(POPPLER_TYPE_ANNOT, "Annotation", mPoppler);

    rb_define_method(RG_TARGET_NAMESPACE, "type", annot_get_annot_type, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "contents", annot_get_contents, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "name", annot_get_name, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "modified", annot_get_modified, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "flags", annot_get_flags, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "color", annot_get_color, 0);
}
