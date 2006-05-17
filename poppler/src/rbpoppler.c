/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/**********************************************************************

  rbpoppler.c -

  $Author: ktou $
  $Date: 2006/05/17 12:51:19 $

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
    VALUE mPoppler, cError, cOrientation, cBackend;

    mPoppler = rb_define_module("Poppler");
    cError = G_DEF_CLASS(POPPLER_TYPE_ERROR, "Error", mPoppler);
    cOrientation = G_DEF_CLASS(POPPLER_TYPE_ORIENTATION, "Orientation",
                               mPoppler);
    cBackend = G_DEF_CLASS(POPPLER_TYPE_BACKEND, "Backend", mPoppler);

    rb_define_module_function(mPoppler, "backend", get_backend, 0);
    rb_define_module_function(mPoppler, "version", get_version, 0);
    rb_define_module_function(mPoppler, "cairo_available?", cairo_available, 0);

    Init_poppler_document(mPoppler);
    Init_poppler_page(mPoppler);
    Init_poppler_attachment(mPoppler);
    Init_poppler_action(mPoppler);
}
