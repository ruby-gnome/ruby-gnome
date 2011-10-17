/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbpangomain.c -

  $Author: sakai $
  $Date: 2007/07/08 02:53:10 $

  Copyright (C) 2002-2005 Masao Mutoh
************************************************/

#include "rbpangoprivate.h"

/*
 * Rendering
 */

struct rpango_reorder_items_args {
    PangoItem *item;
    VALUE ary;
    long n;
    GList *result;
};

static VALUE
rpango_reorder_items_body(VALUE value)
{
    struct rpango_reorder_items_args *args = (struct rpango_reorder_items_args *)value;
    long i;
    GList *result;

    for (i = 0; i < args->n; i++)
        args->result = g_list_append(args->result,
                                     RVAL2BOXED(RARRAY_PTR(args->ary)[i],
                                                PANGO_TYPE_ITEM));

    result = pango_reorder_items(args->result);
    g_list_free(args->result);

    return GLIST2ARY2(result, PANGO_TYPE_ITEM);
}

static VALUE
rpango_reorder_items_ensure(VALUE value)
{
    g_list_free(((struct rpango_reorder_items_args *)value)->result);

    return Qnil;
}

static VALUE
rpango_reorder_items(VALUE self, VALUE attrs)
{
    struct rpango_reorder_items_args args;
    args.ary = rb_ary_to_ary(attrs);
    args.n = RARRAY_LEN(args.ary);
    args.result = NULL;

    return rb_ensure(rpango_reorder_items_body, (VALUE)&args,
                     rpango_reorder_items_ensure, (VALUE)&args);
}

#if PANGO_CHECK_VERSION(1,4,0)
static VALUE
rpango_unichar_direction(VALUE self, VALUE ch)
{
    return GENUM2RVAL(pango_unichar_direction(NUM2UINT(ch)), PANGO_TYPE_DIRECTION);
}

static VALUE
rpango_find_base_dir(VALUE self, VALUE text)
{
    StringValue(text);
    return GENUM2RVAL(pango_find_base_dir(RSTRING_PTR(text), RSTRING_LEN(text)), 
                      PANGO_TYPE_DIRECTION);
}
#endif

struct rbg_pangologattrs2rval_free_args {
    PangoLogAttr *attrs;
    long n;
};

static VALUE
rbg_pangologattrs2rval_free_body(VALUE value)
{
    struct rbg_pangologattrs2rval_free_args *args = (struct rbg_pangologattrs2rval_free_args *)value;
    long i;
    VALUE ary = rb_ary_new();

    for (i = 0; i < args->n; i++)
        rb_ary_push(ary, BOXED2RVAL(&args->attrs[i], PANGO_TYPE_LOG_ATTR));

    return ary;
}

static VALUE
rbg_pangologattrs2rval_free_ensure(VALUE value)
{
    g_free(((struct rbg_pangologattrs2rval_free_args *)value)->attrs);

    return Qnil;
}

static VALUE
rbg_pangologattrs2rval_free(PangoLogAttr *attrs, long n)
{
    struct rbg_pangologattrs2rval_free_args args = { attrs, n };

    return rb_ensure(rbg_pangologattrs2rval_free_body, (VALUE)&args,
                     rbg_pangologattrs2rval_free_ensure, (VALUE)&args);
}

#define PANGOLOGATTRS2RVAL_FREE(attrs, n) rbg_pangologattrs2rval_free(attrs, n)

static VALUE
rpango_break(VALUE self, VALUE rbtext, VALUE rbanalysis)
{
    const gchar *text = RVAL2CSTR(rbtext);
    long length = RSTRING_LEN(rbtext);
    PangoAnalysis *analysis = RVAL2BOXED(rbanalysis, PANGO_TYPE_ANALYSIS);
    long n = g_utf8_strlen(text, length) + 1;
    PangoLogAttr *attrs = g_new(PangoLogAttr, n);

    pango_break(text, length, analysis, attrs, n);

    return PANGOLOGATTRS2RVAL_FREE(attrs, n);
}

static VALUE
rpango_get_log_attrs(VALUE self, VALUE rbtext, VALUE rblevel, VALUE rblanguage)
{
    const gchar *text = RVAL2CSTR(rbtext);
    long length = RSTRING_LEN(rbtext);
    int level = NUM2INT(rblevel);
    PangoLanguage *language = RVAL2BOXED(rblanguage, PANGO_TYPE_LANGUAGE);
    long n = g_utf8_strlen(text, length) + 1;
    PangoLogAttr *attrs = g_new(PangoLogAttr, n);

    pango_get_log_attrs(text, length, level, language, attrs, n);

    return PANGOLOGATTRS2RVAL_FREE(attrs, n);
}

static VALUE
rpango_find_paragraph_boundary(VALUE self, VALUE text)
{
    gint paragraph_delimiter_index, next_paragraph_start;
    
    StringValue(text);
    pango_find_paragraph_boundary(RSTRING_PTR(text), RSTRING_LEN(text),
                                  &paragraph_delimiter_index,
                                  &next_paragraph_start);
    return rb_ary_new3(2, INT2NUM(paragraph_delimiter_index), 
                       INT2NUM(next_paragraph_start));
}

/*
Don't need to implement this. Use pango_break instead.
void        pango_default_break             (const gchar *text,
                                             int length,
                                             PangoAnalysis *analysis,
                                             PangoLogAttr *attrs,
                                             int attrs_len);
*/

static VALUE
rpango_shape_result(VALUE value)
{
    return BOXED2RVAL((PangoGlyphString *)value, PANGO_TYPE_GLYPH_STRING);
}

static VALUE
rpango_shape_ensure(VALUE value)
{
    pango_glyph_string_free((PangoGlyphString *)value);

    return Qnil;
}

static VALUE
rpango_shape(VALUE self, VALUE rbtext, VALUE rbanalysis)
{
    const gchar *text = RVAL2CSTR(rbtext);
    long length = RSTRING_LEN(rbtext);
    PangoAnalysis *analysis = RVAL2BOXED(rbanalysis, PANGO_TYPE_ANALYSIS);
    PangoGlyphString *glyphs = pango_glyph_string_new();

    pango_shape(text, length, analysis, glyphs);

    return rb_ensure(rpango_shape_result, (VALUE)glyphs,
                     rpango_shape_ensure, (VALUE)glyphs);
}

/* This method is from rbpangoattribute.c */
static VALUE
rpango_parse_markup(int argc, VALUE *argv, VALUE self)
{
    VALUE markup_text, accel_marker;
    PangoAttrList *pattr_list;
    gchar* gtext;
    gunichar accel_char;
    GError *error = NULL;
    gboolean ret;
    char c;
    VALUE text = Qnil;
    VALUE attr_list = Qnil;

    rb_scan_args(argc, argv, "11", &markup_text, &accel_marker);

    if (NIL_P(markup_text)) rb_raise(rb_eRuntimeError, "1st argument can't accept nil");

    StringValue(markup_text);
    ret = pango_parse_markup(RSTRING_PTR(markup_text),
                             RSTRING_LEN(markup_text),
                             NIL_P(accel_marker) ? 0 : NUM2CHR(accel_marker),
                             &pattr_list, &gtext, &accel_char, &error);

    if (!ret) RAISE_GERROR(error);

    if (pattr_list){
        attr_list = BOXED2RVAL(pattr_list, PANGO_TYPE_ATTR_LIST); 
        pango_attr_list_unref(pattr_list);
    }

    c = (char)accel_char;
    if (text){
        text = CSTR2RVAL(gtext);
        g_free(gtext);
    }
    return rb_ary_new3(3, attr_list, text,
                       accel_char ? rb_str_new(&c, 1) : Qnil);
}

static VALUE
rpango_pixels(VALUE self, VALUE pixels)
{
    return rb_float_new(PANGO_PIXELS(NUM2DBL(pixels)));
}

void
Init_pango_main(void)
{
    rb_define_module_function(mPango, "reorder_items", rpango_reorder_items, 1);

#if PANGO_CHECK_VERSION(1,4,0)
    rb_define_module_function(mPango, "unichar_direction", rpango_unichar_direction, 1);
    rb_define_module_function(mPango, "find_base_dir", rpango_find_base_dir, 1);
#endif
    rb_define_module_function(mPango, "break", rpango_break, 2);
    rb_define_module_function(mPango, "get_log_attrs", rpango_get_log_attrs, 3);
    rb_define_module_function(mPango, "find_paragraph_boundary", rpango_find_paragraph_boundary, 1);
    rb_define_module_function(mPango, "shape", rpango_shape, 2);
    rb_define_module_function(mPango, "parse_markup", rpango_parse_markup, -1);
    rb_define_module_function(mPango, "pixels", rpango_pixels, 1);

    rb_define_const(mPango, "SCALE", INT2FIX(PANGO_SCALE));
}
