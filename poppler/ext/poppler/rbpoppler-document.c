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

#include "rbpoppler.h"

#define RG_TARGET_NAMESPACE cDocument

#define RVAL2DOC(obj) (POPPLER_DOCUMENT(RVAL2GOBJ(obj)))

#define IITER2RVAL(obj) (BOXED2RVAL(obj, POPPLER_TYPE_INDEX_ITER))
#define RVAL2IITER(obj) (RVAL2BOXED(obj, POPPLER_TYPE_INDEX_ITER))
#define FITER2RVAL(obj) (BOXED2RVAL(obj, POPPLER_TYPE_FONTS_ITER))
#define RVAL2FITER(obj) (RVAL2BOXED(obj, POPPLER_TYPE_FONTS_ITER))

static ID id_new, id_valid, id_pdf_data_p, id_ensure_uri;
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

    password = NIL_P(rb_password) ? NULL : RVAL2CSTR(rb_password);

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
    result = poppler_document_save(RVAL2DOC(self), RVAL2CSTR(uri), &error);

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
    result = poppler_document_save_a_copy(RVAL2DOC(self), RVAL2CSTR(uri),
                      &error);

    if (error)
        RAISE_GERROR(error);

    return CBOOL2RVAL(result);
}

static VALUE
rg_n_pages(VALUE self)
{
    return INT2NUM(poppler_document_get_n_pages(RVAL2DOC(self)));
}

static VALUE
rg_get_page(VALUE self, VALUE index_or_label)
{
    VALUE rb_page;
    PopplerPage *page;

    if (RVAL2CBOOL(rb_obj_is_kind_of(index_or_label, rb_cInteger))) {
        page = poppler_document_get_page(RVAL2DOC(self),
                                         NUM2INT(index_or_label));
    } else if (RVAL2CBOOL(rb_obj_is_kind_of(index_or_label, rb_cString))) {
        page = poppler_document_get_page_by_label(RVAL2DOC(self),
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
    return CBOOL2RVAL(poppler_document_has_attachments(RVAL2DOC(self)));
}

static VALUE
rg_attachments(VALUE self)
{
    return GLIST2ARYF(poppler_document_get_attachments(RVAL2DOC(self)));
}

static VALUE
rg_find_dest(VALUE self, VALUE link_name)
{
    return GOBJ2RVAL(poppler_document_find_dest(RVAL2DOC(self),
                                                RVAL2CSTR(link_name)));
}

static VALUE
rg_get_form_field(VALUE self, VALUE id)
{
    return GOBJ2RVAL(poppler_document_get_form_field(RVAL2DOC(self),
                                                     NUM2INT(id)));
}

static VALUE
rg_each(VALUE self)
{
    PopplerDocument *document;
    int i, n_pages;

    document = RVAL2DOC(self);
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

/* Interface for getting the Index of a poppler_document */
#define CHECK_IITER_IS_VALID(iter) do {         \
    if (!RVAL2CBOOL(index_iter_valid_p(iter)))       \
        return Qnil;                            \
} while (0)

static VALUE
index_iter_valid_p(VALUE self)
{
    return rb_ivar_get(self, id_valid);
}

static VALUE
index_iter_initialize(VALUE self, VALUE document)
{
    PopplerIndexIter *iter;
    iter = poppler_index_iter_new(RVAL2GOBJ(document));
    G_INITIALIZE(self, iter);
    poppler_index_iter_free(iter);
    rb_ivar_set(self, id_valid, CBOOL2RVAL(iter));
    return Qnil;
}

static VALUE
index_iter_get_child(VALUE self)
{
    PopplerIndexIter *child;
    VALUE rb_child;

    CHECK_IITER_IS_VALID(self);
    child = poppler_index_iter_get_child(RVAL2IITER(self));
    rb_child = IITER2RVAL(child);
    poppler_index_iter_free(child);
    return rb_child;
}

static VALUE
index_iter_is_open(VALUE self)
{
    CHECK_IITER_IS_VALID(self);
    return CBOOL2RVAL(poppler_index_iter_is_open(RVAL2IITER(self)));
}

#ifndef HAVE_TYPE_POPPLERACTIONANY
static VALUE
index_iter_get_action(VALUE self)
{
    CHECK_IITER_IS_VALID(self);
    return POPPLER_ACTION2RVAL(poppler_index_iter_get_action(RVAL2IITER(self)));
}
#endif

static VALUE
index_iter_next(VALUE self)
{
    if (poppler_index_iter_next(RVAL2IITER(self))) {
        return Qtrue;
    } else {
        rb_ivar_set(self, id_valid, Qfalse);
        return Qfalse;
    }
}

static VALUE
index_iter_each(VALUE self)
{
    PopplerIndexIter *iter;

    CHECK_IITER_IS_VALID(self);
    iter = RVAL2IITER(self);
    do {
        rb_yield(self);
    } while (poppler_index_iter_next(iter));
    rb_ivar_set(self, id_valid, Qfalse);

    return self;
}

#if POPPLER_CHECK_VERSION(0, 6, 0)

static VALUE
font_info_initialize(VALUE self, VALUE document)
{
    G_INITIALIZE(self, poppler_font_info_new(RVAL2GOBJ(document)));
    return Qnil;
}

static VALUE
font_info_scan(VALUE self, VALUE n_pages)
{
    VALUE rb_iter = Qnil;
    PopplerFontsIter *iter;

    if (poppler_font_info_scan(RVAL2GOBJ(self), NUM2INT(n_pages), &iter)) {
        rb_iter = FITER2RVAL(iter);
        rb_ivar_set(rb_iter, id_valid, Qtrue);
        poppler_fonts_iter_free(iter);
    }
    return rb_iter;
}
#endif

#define CHECK_FITER_IS_VALID(iter) do {         \
    if (!RVAL2CBOOL(fonts_iter_valid_p(iter)))       \
        return Qnil;                            \
} while (0)

static VALUE
fonts_iter_valid_p(VALUE self)
{
    return rb_ivar_get(self, id_valid);
}

static VALUE
fonts_iter_get_name(VALUE self)
{
    CHECK_FITER_IS_VALID(self);
    return CSTR2RVAL(poppler_fonts_iter_get_name(RVAL2FITER(self)));
}

static VALUE
fonts_iter_get_full_name(VALUE self)
{
    CHECK_FITER_IS_VALID(self);
    return CSTR2RVAL(poppler_fonts_iter_get_full_name(RVAL2FITER(self)));
}

static VALUE
fonts_iter_get_file_name(VALUE self)
{
    CHECK_FITER_IS_VALID(self);
    return CSTR2RVAL(poppler_fonts_iter_get_file_name(RVAL2FITER(self)));
}

static VALUE
fonts_iter_get_font_type(VALUE self)
{
    CHECK_FITER_IS_VALID(self);
    return GENUM2RVAL(poppler_fonts_iter_get_font_type(RVAL2FITER(self)),
                      POPPLER_TYPE_FONT_TYPE);
}

static VALUE
fonts_iter_is_embedded(VALUE self)
{
    CHECK_FITER_IS_VALID(self);
    return CBOOL2RVAL(poppler_fonts_iter_is_embedded(RVAL2FITER(self)));
}

static VALUE
fonts_iter_is_subset(VALUE self)
{
    CHECK_FITER_IS_VALID(self);
    return CBOOL2RVAL(poppler_fonts_iter_is_subset(RVAL2FITER(self)));
}

static VALUE
fonts_iter_next(VALUE self)
{
    if (poppler_fonts_iter_next(RVAL2FITER(self))) {
        return Qtrue;
    } else {
        rb_ivar_set(self, id_valid, Qfalse);
        return Qfalse;
    }
}

static VALUE
fonts_iter_each(VALUE self)
{
    PopplerFontsIter *iter;

    CHECK_FITER_IS_VALID(self);
    iter = RVAL2FITER(self);
    do {
        rb_yield(self);
    } while (poppler_fonts_iter_next(iter));
    rb_ivar_set(self, id_valid, Qfalse);

    return self;
}

/* Export to ps */
static VALUE
ps_file_initialize(VALUE self, VALUE document, VALUE filename,
                   VALUE first_page, VALUE n_pages)
{
    PopplerPSFile *ps_file;

    ps_file = poppler_ps_file_new(RVAL2GOBJ(document), RVAL2CSTR(filename),
                                  NUM2INT(first_page), NUM2INT(n_pages));

    if (!ps_file)
        rb_raise(rb_eRuntimeError, "can't create Poppler::PSFile");

    G_INITIALIZE(self, ps_file);
    return Qnil;
}

static VALUE
ps_file_set_paper_size(VALUE self, VALUE width, VALUE height)
{
    poppler_ps_file_set_paper_size(RVAL2GOBJ(self),
                                   NUM2DBL(width), NUM2DBL(height));
    return Qnil;
}

static VALUE
ps_file_set_duplex(VALUE self, VALUE duplex)
{
    poppler_ps_file_set_duplex(RVAL2GOBJ(self), RVAL2CBOOL(duplex));
    return Qnil;
}

void
Init_poppler_document(VALUE mPoppler)
{
    VALUE RG_TARGET_NAMESPACE, cFontsIter, cPSFile;

    id_new = rb_intern("new");
    id_valid = rb_intern("valid?");
    id_pdf_data_p = rb_intern("pdf_data?");
    id_ensure_uri = rb_intern("ensure_uri");

    RG_TARGET_NAMESPACE = G_DEF_CLASS(POPPLER_TYPE_DOCUMENT, "Document", mPoppler);
    cIndexIter = G_DEF_CLASS(POPPLER_TYPE_INDEX_ITER, "IndexIter", mPoppler);
    cFontInfo = G_DEF_CLASS(POPPLER_TYPE_FONT_INFO, "FontInfo", mPoppler);
    cFontsIter = G_DEF_CLASS(POPPLER_TYPE_FONTS_ITER, "FontsIter", mPoppler);
    cPSFile = G_DEF_CLASS(POPPLER_TYPE_PS_FILE, "PSFile", mPoppler);

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

/* Interface for getting the Index of a poppler_document */
    rb_include_module(cIndexIter, rb_mEnumerable);

    rb_define_method(cIndexIter, "initialize", index_iter_initialize, 1);
    rb_define_method(cIndexIter, "child", index_iter_get_child, 0);
    rb_define_method(cIndexIter, "open?", index_iter_is_open, 0);
#ifndef HAVE_TYPE_POPPLERACTIONANY
    rb_define_method(cIndexIter, "action", index_iter_get_action, 0);
#endif
    rb_define_method(cIndexIter, "next", index_iter_next, 0);

    rb_define_method(cIndexIter, "valid?", index_iter_valid_p, 0);
    rb_define_method(cIndexIter, "each", index_iter_each, 0);

    G_DEF_SETTERS(cIndexIter);

#if POPPLER_CHECK_VERSION(0, 6, 0)
    rb_define_method(cFontInfo, "initialize", font_info_initialize, 1);
    rb_define_method(cFontInfo, "scan", font_info_scan, 1);
    G_DEF_SETTERS(cFontInfo);
#endif

    rb_include_module(cFontsIter, rb_mEnumerable);

    rb_define_method(cFontsIter, "name", fonts_iter_get_name, 0);
    rb_define_method(cFontsIter, "full_name", fonts_iter_get_full_name, 0);
#if POPPLER_CHECK_VERSION(0, 6, 0)
    rb_define_method(cFontsIter, "file_name", fonts_iter_get_file_name, 0);
#endif
    rb_define_method(cFontsIter, "font_type", fonts_iter_get_font_type, 0);
    rb_define_method(cFontsIter, "embedded?", fonts_iter_is_embedded, 0);
    rb_define_method(cFontsIter, "subset?", fonts_iter_is_subset, 0);
    rb_define_method(cFontsIter, "next", fonts_iter_next, 0);

    rb_define_method(cFontsIter, "valid?", fonts_iter_valid_p, 0);
    rb_define_method(cFontsIter, "each", fonts_iter_each, 0);

    G_DEF_SETTERS(cFontsIter);

/* Export to ps */
    rb_define_method(cPSFile, "initialize", ps_file_initialize, 4);
    rb_define_method(cPSFile, "set_paper_size", ps_file_set_paper_size, 2);
    rb_define_method(cPSFile, "set_duplex", ps_file_set_duplex, 1);

    G_DEF_SETTERS(cPSFile);
}