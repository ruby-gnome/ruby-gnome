/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/**********************************************************************

  rbpoppler.c -

  $Author: ktou $
  $Date: 2007/06/23 02:46:22 $

  Copyright (C) 2006 Ruby-GNOME2 Project Team

**********************************************************************/

#include "rbpoppler.h"

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
#if POPPLER_CHECK_VERSION(0, 5, 9)
    G_DEF_CLASS(POPPLER_TYPE_PAGE_TRANSITION_TYPE,
                "PageTransitionType", mPoppler);
    G_DEF_CLASS(POPPLER_TYPE_PAGE_TRANSITION_ALIGNMENT,
                "PageTransitionAlignment", mPoppler);
    G_DEF_CLASS(POPPLER_TYPE_PAGE_TRANSITION_DIRECTION,
                "PageTransitionDirection", mPoppler);
    G_DEF_CLASS(POPPLER_TYPE_SELECTION_STYLE, "SelectionStyle", mPoppler);
    G_DEF_CLASS(POPPLER_TYPE_FORM_FIELD_TYPE, "FormFieldType", mPoppler);
#endif
    G_DEF_CLASS(POPPLER_TYPE_BACKEND, "Backend", mPoppler);

    rb_define_module_function(mPoppler, "backend", get_backend, 0);
    rb_define_module_function(mPoppler, "version", get_version, 0);
    rb_define_module_function(mPoppler, "cairo_available?", cairo_available, 0);

    Init_poppler_document(mPoppler);
    Init_poppler_page(mPoppler);
    Init_poppler_attachment(mPoppler);
    Init_poppler_action(mPoppler);
}
