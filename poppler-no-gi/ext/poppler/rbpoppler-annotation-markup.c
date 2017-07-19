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

#define RG_TARGET_NAMESPACE cAnnotationMarkup
#define SELF(self) (RVAL2POPPLERANNOTMARKUP(self))

static ID id_new;
static VALUE rb_cDate;

static VALUE
rg_label(VALUE self)
{
    return CSTR2RVAL_FREE(poppler_annot_markup_get_label(SELF(self)));
}

static VALUE
rg_popup_is_open_p(VALUE self)
{
    return CBOOL2RVAL(poppler_annot_markup_get_popup_is_open(SELF(self)));
}

static VALUE
rg_opacity(VALUE self)
{
    return rb_float_new(poppler_annot_markup_get_opacity(SELF(self)));
}

static VALUE
rg_date(VALUE self)
{
    GDate *date;
    VALUE rb_date;

    date = poppler_annot_markup_get_date(SELF(self));
    rb_date = rb_funcall(rb_cDate, id_new, 3,
                         UINT2NUM(g_date_get_year(date)),
                         UINT2NUM(g_date_get_month(date)),
                         UINT2NUM(g_date_get_day(date)));
    g_date_free(date);
    return rb_date;
}

static VALUE
rg_subject(VALUE self)
{
    return CSTR2RVAL_FREE(poppler_annot_markup_get_subject(SELF(self)));
}

static VALUE
rg_reply_to(VALUE self)
{
    return POPPLERANNOTMARKUPREPLYTYPE2RVAL(poppler_annot_markup_get_reply_to(SELF(self)));
}

static VALUE
rg_external_data(VALUE self)
{
    return POPPLERANNOTEXTERNALDATATYPE2RVAL(poppler_annot_markup_get_external_data(SELF(self)));
}

void
Init_poppler_annotation_markup(VALUE mPoppler)
{
    VALUE RG_TARGET_NAMESPACE;

    id_new = rb_intern("new");
    rb_cDate = rb_const_get(rb_cObject, rb_intern("Date"));

    RG_TARGET_NAMESPACE= G_DEF_CLASS(POPPLER_TYPE_ANNOT_MARKUP,
                                    "AnnotationMarkup", mPoppler);

    RG_DEF_METHOD(label, 0);
    RG_DEF_METHOD_P(popup_is_open, 0);
    RG_DEF_METHOD(opacity, 0);
    RG_DEF_METHOD(date, 0);
    RG_DEF_METHOD(subject, 0);
    RG_DEF_METHOD(reply_to, 0);
    RG_DEF_METHOD(external_data, 0);
}
