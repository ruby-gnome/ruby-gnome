/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2006-2013 Ruby-GNOME2 Project Team
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

extern void Init_poppler(void);

#define RG_TARGET_NAMESPACE mPoppler

static VALUE
rg_s_backend(G_GNUC_UNUSED VALUE self)
{
    return POPPLERBACKEND2RVAL(poppler_get_backend());
}

static VALUE
rg_s_version(G_GNUC_UNUSED VALUE self)
{
    return CSTR2RVAL(poppler_get_version());
}

static VALUE
rg_s_cairo_available_p(G_GNUC_UNUSED VALUE self)
{
    return Qtrue;
}

void
Init_poppler(void)
{
    VALUE RG_TARGET_NAMESPACE;

    RG_TARGET_NAMESPACE = rb_define_module("Poppler");

    rb_define_const(RG_TARGET_NAMESPACE, "BUILD_VERSION",
                    rb_ary_new3(3,
                                INT2FIX(POPPLER_MAJOR_VERSION),
                                INT2FIX(POPPLER_MINOR_VERSION),
                                INT2FIX(POPPLER_MICRO_VERSION)));

    G_DEF_CLASS(POPPLER_TYPE_ERROR, "Error", RG_TARGET_NAMESPACE);
    G_DEF_CLASS(POPPLER_TYPE_ORIENTATION, "Orientation", RG_TARGET_NAMESPACE);

    G_DEF_CLASS(POPPLER_TYPE_PAGE_TRANSITION_TYPE,
                "PageTransitionType", RG_TARGET_NAMESPACE);
    G_DEF_CLASS(POPPLER_TYPE_PAGE_TRANSITION_ALIGNMENT,
                "PageTransitionAlignment", RG_TARGET_NAMESPACE);
    G_DEF_CLASS(POPPLER_TYPE_PAGE_TRANSITION_DIRECTION,
                "PageTransitionDirection", RG_TARGET_NAMESPACE);
    G_DEF_CLASS(POPPLER_TYPE_SELECTION_STYLE, "SelectionStyle", RG_TARGET_NAMESPACE);
    G_DEF_CLASS(POPPLER_TYPE_FORM_BUTTON_TYPE, "FormButtonType", RG_TARGET_NAMESPACE);
    G_DEF_CLASS(POPPLER_TYPE_FORM_TEXT_TYPE, "FormTextType", RG_TARGET_NAMESPACE);
    G_DEF_CLASS(POPPLER_TYPE_FORM_CHOICE_TYPE, "FormChoiceType", RG_TARGET_NAMESPACE);

    G_RENAME_NICK("3D", "TYPE_3D");
    G_DEF_CLASS(POPPLER_TYPE_ANNOT_TYPE, "AnnotationType", RG_TARGET_NAMESPACE);
    G_DEF_CLASS(POPPLER_TYPE_ANNOT_FLAG, "AnnotationFlag", RG_TARGET_NAMESPACE);
    G_DEF_CLASS(POPPLER_TYPE_ANNOT_MARKUP_REPLY_TYPE,
        "AnnotationMarkupReplyType", RG_TARGET_NAMESPACE);
    G_RENAME_NICK("3D", "TYPE_3D");
    G_DEF_CLASS(POPPLER_TYPE_ANNOT_EXTERNAL_DATA_TYPE,
        "AnnotationExternalDataType", RG_TARGET_NAMESPACE);
    G_DEF_CLASS(POPPLER_TYPE_ANNOT_TEXT_STATE, "AnnotationTextState", RG_TARGET_NAMESPACE);
    G_DEF_CLASS(POPPLER_TYPE_ANNOT_FREE_TEXT_QUADDING,
        "AnnotationFreeTextQuadding", RG_TARGET_NAMESPACE);

    G_DEF_CLASS(POPPLER_TYPE_BACKEND, "Backend", RG_TARGET_NAMESPACE);

    RG_DEF_SMETHOD(backend, 0);
    RG_DEF_SMETHOD(version, 0);
    RG_DEF_SMETHOD_P(cairo_available, 0);

    Init_poppler_index_iter(RG_TARGET_NAMESPACE);
    Init_poppler_font_info(RG_TARGET_NAMESPACE);
    Init_poppler_document(RG_TARGET_NAMESPACE);
    Init_poppler_fonts_iter(RG_TARGET_NAMESPACE);
    Init_poppler_ps_file(RG_TARGET_NAMESPACE);
    Init_poppler_rectangle(RG_TARGET_NAMESPACE);
    Init_poppler_page(RG_TARGET_NAMESPACE);
    Init_poppler_color(RG_TARGET_NAMESPACE);
    Init_poppler_link_mapping(RG_TARGET_NAMESPACE);
    Init_poppler_page_transition(RG_TARGET_NAMESPACE);
    Init_poppler_image_mapping(RG_TARGET_NAMESPACE);
    Init_poppler_form_field_mapping(RG_TARGET_NAMESPACE);
    Init_poppler_annotation_mapping(RG_TARGET_NAMESPACE);
    Init_poppler_attachment(RG_TARGET_NAMESPACE);
    Init_poppler_action(RG_TARGET_NAMESPACE);
    Init_poppler_annotation(RG_TARGET_NAMESPACE);
    Init_poppler_annotation_markup(RG_TARGET_NAMESPACE);
    Init_poppler_annotation_text(RG_TARGET_NAMESPACE);
    Init_poppler_annotation_free_text(RG_TARGET_NAMESPACE);
    Init_poppler_annotation_callout_line(RG_TARGET_NAMESPACE);
    Init_poppler_form_field(RG_TARGET_NAMESPACE);
}
