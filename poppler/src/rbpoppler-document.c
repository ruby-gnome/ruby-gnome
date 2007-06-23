/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/**********************************************************************

  rbpoppler-document.c -

  $Author: ktou $
  $Date: 2007/06/23 02:43:53 $

  Copyright (C) 2006 Ruby-GNOME2 Project Team

**********************************************************************/

#include "rbpoppler.h"

#define IITER2RVAL(obj) (BOXED2RVAL(obj, POPPLER_TYPE_INDEX_ITER))
#define RVAL2IITER(obj) (RVAL2BOXED(obj, POPPLER_TYPE_INDEX_ITER))
#define FITER2RVAL(obj) (BOXED2RVAL(obj, POPPLER_TYPE_FONTS_ITER))
#define RVAL2FITER(obj) (RVAL2BOXED(obj, POPPLER_TYPE_FONTS_ITER))

#ifdef POPPLER_TYPE_FONT_INFO
#  define HAVE_POPPLER_FONT_INFO 1
#endif

static ID id_new, id_valid, id_ensure_uri;
static VALUE cIndexIter;
#ifdef HAVE_POPPLER_FONT_INFO
static VALUE cFontInfo;
#endif


static VALUE
doc_initialize(int argc, VALUE *argv, VALUE self)
{
    PopplerDocument *document;
    GError *error = NULL;
    VALUE uri, rb_password;
    const char *password;

    rb_scan_args(argc, argv, "11", &uri, &rb_password);

    password = NIL_P(rb_password) ? NULL : RVAL2CSTR(rb_password);
    uri = rb_funcall(self, id_ensure_uri, 1, uri);
    document = poppler_document_new_from_file(RVAL2CSTR(uri), password, &error);

    if (error)
        RAISE_GERROR(error);

    G_INITIALIZE(self, document);
    return Qnil;
}

static VALUE
doc_save(VALUE self, VALUE uri)
{
    gboolean result;
    GError *error = NULL;

    result = poppler_document_save(RVAL2GOBJ(self), RVAL2CSTR(uri), &error);

    if (error)
        RAISE_GERROR(error);

    return CBOOL2RVAL(result);
}

static VALUE
doc_get_n_pages(VALUE self)
{
    return INT2NUM(poppler_document_get_n_pages(RVAL2GOBJ(self)));
}

static VALUE
doc_get_page(VALUE self, VALUE index_or_label)
{
    VALUE rb_page;
    PopplerPage *page;

    if (RTEST(rb_obj_is_kind_of(index_or_label, rb_cInteger))) {
        page = poppler_document_get_page(RVAL2GOBJ(self),
                                         NUM2INT(index_or_label));
    } else if (RTEST(rb_obj_is_kind_of(index_or_label, rb_cString))) {
        page = poppler_document_get_page_by_label(RVAL2GOBJ(self),
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
doc_has_attachments(VALUE self)
{
    return CBOOL2RVAL(poppler_document_has_attachments(RVAL2GOBJ(self)));
}

static VALUE
doc_get_attachments(VALUE self)
{
    return GLIST2ARYF(poppler_document_get_attachments(RVAL2GOBJ(self)));
}

static VALUE
doc_find_dest(VALUE self, VALUE link_name)
{
    return GOBJ2RVAL(poppler_document_find_dest(RVAL2GOBJ(self),
                                                RVAL2CSTR(link_name)));
}

static VALUE
doc_each(VALUE self)
{
    PopplerDocument *document;
    int i, n_pages;

    document = RVAL2GOBJ(self);
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
doc_get_index_iter(VALUE self)
{
    return rb_funcall(cIndexIter, id_new, 1, self);
}

#ifdef HAVE_POPPLER_FONT_INFO
static VALUE
doc_get_font_info(VALUE self)
{
    return rb_funcall(cFontInfo, id_new, 1, self);
}
#endif



/* Interface for getting the Index of a poppler_document */
#define CHECK_IITER_IS_VALID(iter) do {         \
    if (!RTEST(index_iter_valid_p(iter)))       \
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
    rb_ivar_set(self, id_valid, iter ? Qtrue : Qfalse);
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
    return ACTION2RVAL(poppler_index_iter_get_action(RVAL2IITER(self)));
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


#ifdef HAVE_POPPLER_FONT_INFO

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
    if (!RTEST(fonts_iter_valid_p(iter)))       \
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
    VALUE cDocument, cFontsIter, cPSFile;

    id_new = rb_intern("new");
    id_valid = rb_intern("valid?");
    id_ensure_uri = rb_intern("ensure_uri");

    cDocument = G_DEF_CLASS(POPPLER_TYPE_DOCUMENT, "Document", mPoppler);
    cIndexIter = G_DEF_CLASS(POPPLER_TYPE_INDEX_ITER, "IndexIter", mPoppler);
#ifdef HAVE_POPPLER_FONT_INFO
    cFontInfo = G_DEF_CLASS(POPPLER_TYPE_FONT_INFO, "FontInfo", mPoppler);
#endif
    cFontsIter = G_DEF_CLASS(POPPLER_TYPE_FONTS_ITER, "FontsIter", mPoppler);
    cPSFile = G_DEF_CLASS(POPPLER_TYPE_PS_FILE, "PSFile", mPoppler);

    G_DEF_CLASS(POPPLER_TYPE_PAGE_LAYOUT, "PageLayout", mPoppler);
    G_DEF_CLASS(POPPLER_TYPE_PAGE_MODE, "PageMode", mPoppler);
    G_DEF_CLASS(POPPLER_TYPE_FONT_TYPE, "FontType", mPoppler);
    G_DEF_CLASS(POPPLER_TYPE_VIEWER_PREFERENCES, "ViewerPreferences", mPoppler);
    G_DEF_CLASS(POPPLER_TYPE_PERMISSIONS, "Permissions", mPoppler);

    rb_include_module(cDocument, rb_mEnumerable);

    rb_define_method(cDocument, "initialize", doc_initialize, -1);
    rb_define_method(cDocument, "save", doc_save, 1);
    rb_define_method(cDocument, "n_pages", doc_get_n_pages, 0);
    rb_define_alias(cDocument, "size", "n_pages");
    rb_define_method(cDocument, "get_page", doc_get_page, 1);
    rb_define_alias(cDocument, "[]", "get_page");
    rb_define_method(cDocument, "has_attachments?", doc_has_attachments, 0);
    rb_define_alias(cDocument, "have_attachments?", "has_attachments?");
    rb_define_method(cDocument, "attachments", doc_get_attachments, 0);
    rb_define_method(cDocument, "find_dest", doc_find_dest, 1);

    rb_define_method(cDocument, "each", doc_each, 0);
    rb_define_alias(cDocument, "pages", "to_a");

    rb_define_method(cDocument, "index_iter", doc_get_index_iter, 0);
#ifdef HAVE_POPPLER_FONT_INFO
    rb_define_method(cDocument, "font_info", doc_get_font_info, 0);
#endif

    G_DEF_SETTERS(cDocument);


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


#ifdef HAVE_POPPLER_FONT_INFO
    rb_define_method(cFontInfo, "initialize", font_info_initialize, 1);
    rb_define_method(cFontInfo, "scan", font_info_scan, 1);
    G_DEF_SETTERS(cFontInfo);
#endif

    rb_include_module(cFontsIter, rb_mEnumerable);

    rb_define_method(cFontsIter, "name", fonts_iter_get_name, 0);
    rb_define_method(cFontsIter, "full_name", fonts_iter_get_full_name, 0);
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
