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

#define RG_TARGET_NAMESPACE cAnnotationFreeText
#define SELF(self) (POPPLER_ANNOT_FREE_TEXT(RVAL2GOBJ(self)))

#define LINE2RVAL(obj) (BOXED2RVAL(obj, POPPLER_TYPE_ANNOT_CALLOUT_LINE))
#define QUADDING2RVAL(obj) (GENUM2RVAL(obj, POPPLER_TYPE_ANNOT_FREE_TEXT_QUADDING))

static VALUE
annot_free_text_get_quadding(VALUE self)
{
    return QUADDING2RVAL(poppler_annot_free_text_get_quadding(SELF(self)));
}

static VALUE
annot_free_text_get_callout_line(VALUE self)
{
    return LINE2RVAL(poppler_annot_free_text_get_callout_line(SELF(self)));
}

void
Init_poppler_annotationfreetext(VALUE mPoppler)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(POPPLER_TYPE_ANNOT_FREE_TEXT,
                                      "AnnotationFreeText", mPoppler);

    rb_define_method(RG_TARGET_NAMESPACE, "quadding",
                     annot_free_text_get_quadding, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "callout_line",
                     annot_free_text_get_callout_line, 0);
}

