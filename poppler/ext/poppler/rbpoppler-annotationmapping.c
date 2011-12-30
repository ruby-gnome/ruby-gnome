/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2006-2008 Ruby-GNOME2 Project Team
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

#define RG_TARGET_NAMESPACE cAnnotationMapping

#define RVAL2AM(obj) (RVAL2POPPLERANNOTMAPPING(obj))

static VALUE
rg_initialize(int argc, VALUE *argv, VALUE self)
{
    VALUE area, annotation;
    PopplerAnnotMapping *mapping;

    rb_scan_args(argc, argv, "02", &area, &annotation);

    mapping = poppler_annot_mapping_new();
    mapping->area = *RVAL2POPPLER_RECT(area);
    mapping->annot = RVAL2POPPLERANNOT(annotation);
    G_INITIALIZE(self, mapping);

    return Qnil;
}

DEF_ACCESSOR_WITH_SETTER(annot_mapping, area,
                         RVAL2AM, RECT_ENTITY2RVAL, RECT_ENTITY_SET)
DEF_READER(annot_mapping, annotation, annot, RVAL2AM, POPPLERANNOT2RVAL)

static VALUE
rg_set_annotation(VALUE self, VALUE annotation)
{
    PopplerAnnotMapping *mapping;

    mapping = RVAL2AM(self);
    if (mapping->annot)
        g_object_unref(mapping->annot);

    mapping->annot = RVAL2POPPLERANNOT(annotation);
    return Qnil;
}

void
Init_poppler_annotationmapping(VALUE mPoppler)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(POPPLER_TYPE_ANNOT_MAPPING,
                                             "AnnotationMapping", mPoppler);

    RG_DEF_METHOD(initialize, -1);

    rb_define_method(RG_TARGET_NAMESPACE, "area", annot_mapping_get_area, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "annotation",
                     annot_mapping_get_annotation, 0);

    rb_define_method(RG_TARGET_NAMESPACE, "set_area", annot_mapping_set_area, 1);
    RG_DEF_METHOD(set_annotation, 1);

    G_DEF_SETTERS(RG_TARGET_NAMESPACE);
}
