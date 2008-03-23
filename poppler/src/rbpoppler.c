/* -*- c-file-style: "ruby" -*- */
/**********************************************************************

  rbpoppler.c -

  Copyright (C) 2006-2008 Ruby-GNOME2 Project Team

**********************************************************************/

#include "rbpoppler-private.h"

static VALUE
get_backend(VALUE self)
{
    return GENUM2RVAL(poppler_get_backend(), POPPLER_TYPE_BACKEND);
}

static VALUE
get_version(VALUE self)
{
    return CSTR2RVAL(poppler_get_version());
}

static VALUE
cairo_available(VALUE self)
{
#ifdef RB_POPPLER_CAIRO_AVAILABLE
    return Qtrue;
#else
    return Qfalse;
#endif
}


void
Init_poppler(void)
{
    VALUE mPoppler;

    mPoppler = rb_define_module("Poppler");

    rb_define_const(mPoppler, "BUILD_VERSION",
                    rb_ary_new3(3,
                                INT2FIX(POPPLER_MAJOR_VERSION),
                                INT2FIX(POPPLER_MINOR_VERSION),
                                INT2FIX(POPPLER_MICRO_VERSION)));

    G_DEF_CLASS(POPPLER_TYPE_ERROR, "Error", mPoppler);
    G_DEF_CLASS(POPPLER_TYPE_ORIENTATION, "Orientation", mPoppler);

#if POPPLER_CHECK_VERSION(0, 6, 0)
    G_DEF_CLASS(POPPLER_TYPE_PAGE_TRANSITION_TYPE,
                "PageTransitionType", mPoppler);
    G_DEF_CLASS(POPPLER_TYPE_PAGE_TRANSITION_ALIGNMENT,
                "PageTransitionAlignment", mPoppler);
    G_DEF_CLASS(POPPLER_TYPE_PAGE_TRANSITION_DIRECTION,
                "PageTransitionDirection", mPoppler);
    G_DEF_CLASS(POPPLER_TYPE_SELECTION_STYLE, "SelectionStyle", mPoppler);
    G_DEF_CLASS(POPPLER_TYPE_FORM_BUTTON_TYPE, "FormButtonType", mPoppler);
    G_DEF_CLASS(POPPLER_TYPE_FORM_TEXT_TYPE, "FormTextType", mPoppler);
    G_DEF_CLASS(POPPLER_TYPE_FORM_CHOICE_TYPE, "FormChoiceType", mPoppler);
#endif

#if POPPLER_CHECK_VERSION(0, 7, 2)
    G_RENAME_NICK("3D", "TYPE_3D");
    G_DEF_CLASS(POPPLER_TYPE_ANNOT_TYPE, "AnnotationType", mPoppler);
    G_DEF_CLASS(POPPLER_TYPE_ANNOT_FLAG, "AnnotationFlag", mPoppler);
    G_DEF_CLASS(POPPLER_TYPE_ANNOT_MARKUP_REPLY_TYPE,
		"AnnotationMarkupReplyType", mPoppler);
    G_RENAME_NICK("3D", "TYPE_3D");
    G_DEF_CLASS(POPPLER_TYPE_ANNOT_EXTERNAL_DATA_TYPE,
		"AnnotationExternalDataType", mPoppler);
    G_DEF_CLASS(POPPLER_TYPE_ANNOT_TEXT_ICON, "AnnotationTextIcon", mPoppler);
    G_DEF_CLASS(POPPLER_TYPE_ANNOT_TEXT_STATE, "AnnotationTextState", mPoppler);
    G_DEF_CLASS(POPPLER_TYPE_ANNOT_FREE_TEXT_QUADDING,
		"AnnotationFreeTextQuadding", mPoppler);
#endif


    G_DEF_CLASS(POPPLER_TYPE_BACKEND, "Backend", mPoppler);

    rb_define_module_function(mPoppler, "backend", get_backend, 0);
    rb_define_module_function(mPoppler, "version", get_version, 0);
    rb_define_module_function(mPoppler, "cairo_available?", cairo_available, 0);

    Init_poppler_document(mPoppler);
    Init_poppler_page(mPoppler);
    Init_poppler_attachment(mPoppler);
    Init_poppler_action(mPoppler);
    Init_poppler_annotation(mPoppler);
    Init_poppler_form_field(mPoppler);
}
