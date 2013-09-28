/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2008-2013  Ruby-GNOME2 Project Team
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

#define RG_TARGET_NAMESPACE cAnnotationFreeText
#define SELF(self) (RVAL2POPPLERANNOTFREETEXT(self))

static VALUE
rg_quadding(VALUE self)
{
    return POPPLERANNOTFREETEXTQUADDING2RVAL(poppler_annot_free_text_get_quadding(SELF(self)));
}

static VALUE
rg_callout_line(VALUE self)
{
    return POPPLERANNOTCALLOUTLINE2RVAL(poppler_annot_free_text_get_callout_line(SELF(self)));
}

void
Init_poppler_annotation_free_text(VALUE mPoppler)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(POPPLER_TYPE_ANNOT_FREE_TEXT,
                                      "AnnotationFreeText", mPoppler);

    RG_DEF_METHOD(quadding, 0);
    RG_DEF_METHOD(callout_line, 0);
}
