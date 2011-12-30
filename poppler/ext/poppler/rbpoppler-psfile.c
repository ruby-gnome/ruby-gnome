/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2006  Ruby-GNOME2 Project Team
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

#define RG_TARGET_NAMESPACE cPSFile

/* Export to ps */
static VALUE
rg_initialize(VALUE self, VALUE document, VALUE filename,
                   VALUE first_page, VALUE n_pages)
{
    PopplerPSFile *ps_file;

    ps_file = poppler_ps_file_new(RVAL2POPPLERDOCUMENT(document), RVAL2CSTR(filename),
                                  NUM2INT(first_page), NUM2INT(n_pages));

    if (!ps_file)
        rb_raise(rb_eRuntimeError, "can't create Poppler::PSFile");

    G_INITIALIZE(self, ps_file);
    return Qnil;
}

static VALUE
rg_set_paper_size(VALUE self, VALUE width, VALUE height)
{
    poppler_ps_file_set_paper_size(RVAL2POPPLERPSFILE(self),
                                   NUM2DBL(width), NUM2DBL(height));
    return Qnil;
}

static VALUE
rg_set_duplex(VALUE self, VALUE duplex)
{
    poppler_ps_file_set_duplex(RVAL2POPPLERPSFILE(self), RVAL2CBOOL(duplex));
    return Qnil;
}

void
Init_poppler_psfile(VALUE mPoppler)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(POPPLER_TYPE_PS_FILE, "PSFile", mPoppler);

/* Export to ps */
    RG_DEF_METHOD(initialize, 4);
    RG_DEF_METHOD(set_paper_size, 2);
    RG_DEF_METHOD(set_duplex, 1);

    G_DEF_SETTERS(RG_TARGET_NAMESPACE);
}
