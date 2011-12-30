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

#define RG_TARGET_NAMESPACE cDocument

static ID id_new, id_pdf_data_p, id_ensure_uri;
static VALUE cIndexIter;
static VALUE cFontInfo;

static VALUE
rg_initialize(int argc, VALUE *argv, VALUE self)
{
    PopplerDocument *document = NULL;
    GError *error = NULL;
    VALUE uri_or_data, rb_password;
    const char *password;

    rb_scan_args(argc, argv, "11", &uri_or_data, &rb_password);

    password = RVAL2CSTR_ACCEPT_NIL(rb_password);

    if (RVAL2CBOOL(rb_funcall(self, id_pdf_data_p, 1, uri_or_data))) {
        document = poppler_document_new_from_data(RSTRING_PTR(uri_or_data),
                                                  RSTRING_LEN(uri_or_data),
                                                  password, &error);
    }

    if (!document && !error) {
        uri_or_data = rb_funcall(self, id_ensure_uri, 1, uri_or_data);
        document = poppler_document_new_from_file(RVAL2CSTR(uri_or_data),
                                                  password, &error);
    }

    if (error)
        RAISE_GERROR(error);

    G_INITIALIZE(self, document);
    return Qnil;
}

static VALUE
rg_save(VALUE self, VALUE uri)
{
    gboolean result;
    GError *error = NULL;

    uri = rb_funcall(self, id_ensure_uri, 1, uri);
    result = poppler_document_save(RVAL2POPPLERDOCUMENT(self), RVAL2CSTR(uri), &error);

    if (error)
        RAISE_GERROR(error);

    return CBOOL2RVAL(result);
}

static VALUE
rg_save_a_copy(VALUE self, VALUE uri)
{
    gboolean result;
    GError *error = NULL;

    uri = rb_funcall(self, id_ensure_uri, 1, uri);
    result = poppler_document_save_a_copy(RVAL2POPPLERDOCUMENT(self), RVAL2CSTR(uri),
                      &error);

    if (error)
        RAISE_GERROR(error);

    return CBOOL2RVAL(result);
}

static VALUE
rg_n_pages(VALUE self)
{
    return INT2NUM(poppler_document_get_n_pages(RVAL2POPPLERDOCUMENT(self)));
}

static VALUE
rg_get_page(VALUE self, VALUE index_or_label)
{
    VALUE rb_page;
    PopplerPage *page;

    if (RVAL2CBOOL(rb_obj_is_kind_of(index_or_label, rb_cInteger))) {
        page = poppler_document_get_page(RVAL2POPPLERDOCUMENT(self),
                                         NUM2INT(index_or_label));
    } else if (RVAL2CBOOL(rb_obj_is_kind_of(index_or_label, rb_cString))) {
        page = poppler_document_get_page_by_label(RVAL2POPPLERDOCUMENT(self),
                                                  RVAL2CSTR(index_or_label));
    } else {
        VALUE inspect;
        inspect = rb_inspect(index_or_label);
        rb_raise(rb_eArgError, "%s must be Integer or String",
                 RVAL2CSTR(inspect));
    }

    rb_page = GOBJ2RVAL(page);
    if (page)
        g_object_unref(page);
    return rb_page;
}

static VALUE
rg_has_attachments_p(VALUE self)
{
    return CBOOL2RVAL(poppler_document_has_attachments(RVAL2POPPLERDOCUMENT(self)));
}

static VALUE
rg_attachments(VALUE self)
{
    return GLIST2ARYF(poppler_document_get_attachments(RVAL2POPPLERDOCUMENT(self)));
}

static VALUE
rg_find_dest(VALUE self, VALUE link_name)
{
    PopplerDest *dest;
    dest = poppler_document_find_dest(RVAL2POPPLERDOCUMENT(self), RVAL2CSTR(link_name));
    return POPPLERDEST2RVAL(dest);
}

static VALUE
rg_get_form_field(VALUE self, VALUE id)
{
    return GOBJ2RVAL(poppler_document_get_form_field(RVAL2POPPLERDOCUMENT(self),
                                                     NUM2INT(id)));
}

static VALUE
rg_each(VALUE self)
{
    PopplerDocument *document;
    int i, n_pages;

    document = RVAL2POPPLERDOCUMENT(self);
    n_pages = poppler_document_get_n_pages(document);
    for (i = 0; i < n_pages; i++) {
        PopplerPage *page;
        VALUE rb_page;

        page = poppler_document_get_page(document, i);
        rb_page = GOBJ2RVAL(page);
        if (page)
            g_object_unref(page);
        rb_yield(rb_page);
    }
    return self;
}

static VALUE
rg_index_iter(VALUE self)
{
    return rb_funcall(cIndexIter, id_new, 1, self);
}

static VALUE
rg_font_info(VALUE self)
{
    return rb_funcall(cFontInfo, id_new, 1, self);
}

void
Init_poppler_document(VALUE mPoppler)
{
    VALUE RG_TARGET_NAMESPACE;
    cIndexIter = rb_const_get(mPoppler, rb_intern("IndexIter"));
    cFontInfo = rb_const_get(mPoppler, rb_intern("FontInfo"));

    id_new = rb_intern("new");
    id_pdf_data_p = rb_intern("pdf_data?");
    id_ensure_uri = rb_intern("ensure_uri");

    RG_TARGET_NAMESPACE = G_DEF_CLASS(POPPLER_TYPE_DOCUMENT, "Document", mPoppler);

    G_DEF_CLASS(POPPLER_TYPE_PAGE_LAYOUT, "PageLayout", mPoppler);
    G_DEF_CLASS(POPPLER_TYPE_PAGE_MODE, "PageMode", mPoppler);
    G_DEF_CLASS(POPPLER_TYPE_FONT_TYPE, "FontType", mPoppler);
    G_DEF_CLASS(POPPLER_TYPE_VIEWER_PREFERENCES, "ViewerPreferences", mPoppler);
    G_DEF_CLASS(POPPLER_TYPE_PERMISSIONS, "Permissions", mPoppler);

    rb_include_module(RG_TARGET_NAMESPACE, rb_mEnumerable);

    RG_DEF_METHOD(initialize, -1);
    RG_DEF_METHOD(save, 1);
    RG_DEF_METHOD(save_a_copy, 1);
    RG_DEF_METHOD(n_pages, 0);
    RG_DEF_ALIAS("size", "n_pages");
    RG_DEF_METHOD(get_page, 1);
    RG_DEF_ALIAS("[]", "get_page");
    RG_DEF_METHOD_P(has_attachments, 0);
    RG_DEF_ALIAS("have_attachments?", "has_attachments?");
    RG_DEF_METHOD(attachments, 0);
    RG_DEF_METHOD(find_dest, 1);
    RG_DEF_ALIAS("get_destination", "find_dest");

    RG_DEF_METHOD(get_form_field, 1);

    RG_DEF_METHOD(each, 0);
    RG_DEF_ALIAS("pages", "to_a");

    RG_DEF_METHOD(index_iter, 0);
    RG_DEF_METHOD(font_info, 0);

    G_DEF_SETTERS(RG_TARGET_NAMESPACE);
}
