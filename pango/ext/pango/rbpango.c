/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2002-2005 Masao Mutoh
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

#include "rbpangoprivate.h"

#define RG_TARGET_NAMESPACE mPango

VALUE RG_TARGET_NAMESPACE;

extern void Init_pango(void);

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
                                     RVAL2PANGOITEM(RARRAY_PTR(args->ary)[i]));

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
rg_s_reorder_items(G_GNUC_UNUSED VALUE self, VALUE attrs)
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
rg_s_unichar_direction(G_GNUC_UNUSED VALUE self, VALUE ch)
{
    return PANGODIRECTION2RVAL(pango_unichar_direction(NUM2UINT(ch)));
}

static VALUE
rg_s_find_base_dir(G_GNUC_UNUSED VALUE self, VALUE text)
{
    StringValue(text);
    return PANGODIRECTION2RVAL(pango_find_base_dir(RSTRING_PTR(text), RSTRING_LEN(text)));
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
        rb_ary_push(ary, PANGOLOGATTR2RVAL(&args->attrs[i]));

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
rg_s_break(G_GNUC_UNUSED VALUE self, VALUE rbtext, VALUE rbanalysis)
{
    const gchar *text = RVAL2CSTR(rbtext);
    long length = RSTRING_LEN(rbtext);
    PangoAnalysis *analysis = RVAL2PANGOANALYSIS(rbanalysis);
    long n = g_utf8_strlen(text, length) + 1;
    PangoLogAttr *attrs = g_new(PangoLogAttr, n);

    pango_break(text, length, analysis, attrs, n);

    return PANGOLOGATTRS2RVAL_FREE(attrs, n);
}

static VALUE
rg_s_get_log_attrs(G_GNUC_UNUSED VALUE self, VALUE rbtext, VALUE rblevel, VALUE rblanguage)
{
    const gchar *text = RVAL2CSTR(rbtext);
    long length = RSTRING_LEN(rbtext);
    int level = NUM2INT(rblevel);
    PangoLanguage *language = RVAL2PANGOLANGUAGE(rblanguage);
    long n = g_utf8_strlen(text, length) + 1;
    PangoLogAttr *attrs = g_new(PangoLogAttr, n);

    pango_get_log_attrs(text, length, level, language, attrs, n);

    return PANGOLOGATTRS2RVAL_FREE(attrs, n);
}

static VALUE
rg_s_find_paragraph_boundary(G_GNUC_UNUSED VALUE self, VALUE text)
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
    return PANGOGLYPHSTRING2RVAL((PangoGlyphString *)value);
}

static VALUE
rpango_shape_ensure(VALUE value)
{
    pango_glyph_string_free((PangoGlyphString *)value);

    return Qnil;
}

static VALUE
rg_s_shape(G_GNUC_UNUSED VALUE self, VALUE rbtext, VALUE rbanalysis)
{
    const gchar *text = RVAL2CSTR(rbtext);
    long length = RSTRING_LEN(rbtext);
    PangoAnalysis *analysis = RVAL2PANGOANALYSIS(rbanalysis);
    PangoGlyphString *glyphs = pango_glyph_string_new();

    pango_shape(text, length, analysis, glyphs);

    return rb_ensure(rpango_shape_result, (VALUE)glyphs,
                     rpango_shape_ensure, (VALUE)glyphs);
}

/* This method is from rbpangoattribute.c */
static VALUE
rg_s_parse_markup(int argc, VALUE *argv, G_GNUC_UNUSED VALUE self)
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
        attr_list = PANGOATTRLIST2RVAL(pattr_list); 
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
rg_s_pixels(G_GNUC_UNUSED VALUE self, VALUE pixels)
{
    return rb_float_new(PANGO_PIXELS(NUM2DBL(pixels)));
}

static VALUE
rg_s_cairo_available_p(G_GNUC_UNUSED VALUE self)
{
#ifdef CAIRO_AVAILABLE
    return Qtrue;
#else
    return Qfalse;
#endif
}

void
Init_pango(void)
{
    RG_TARGET_NAMESPACE = rb_define_module("Pango");

    rb_define_const(RG_TARGET_NAMESPACE, "BUILD_VERSION", 
                    rb_ary_new3(3, 
                                INT2FIX(PANGO_MAJOR_VERSION), 
                                INT2FIX(PANGO_MINOR_VERSION), 
                                INT2FIX(PANGO_MICRO_VERSION)));

    RG_DEF_SMETHOD(reorder_items, 1);

#if PANGO_CHECK_VERSION(1,4,0)
    RG_DEF_SMETHOD(unichar_direction, 1);
    RG_DEF_SMETHOD(find_base_dir, 1);
#endif
    RG_DEF_SMETHOD(break, 2);
    RG_DEF_SMETHOD(get_log_attrs, 3);
    RG_DEF_SMETHOD(find_paragraph_boundary, 1);
    RG_DEF_SMETHOD(shape, 2);
    RG_DEF_SMETHOD(parse_markup, -1);
    RG_DEF_SMETHOD(pixels, 1);

    rb_define_const(RG_TARGET_NAMESPACE, "SCALE", INT2FIX(PANGO_SCALE));

    RG_DEF_SMETHOD_P(cairo_available, 0);

    Init_pango_analysis(RG_TARGET_NAMESPACE);
    Init_pango_attribute(RG_TARGET_NAMESPACE);
    Init_pango_attriterator(RG_TARGET_NAMESPACE);
    Init_pango_attrlist(RG_TARGET_NAMESPACE);
    Init_pango_cairo(RG_TARGET_NAMESPACE);
    Init_pango_cairo_context(RG_TARGET_NAMESPACE);
    Init_pango_color(RG_TARGET_NAMESPACE);
    Init_pango_context(RG_TARGET_NAMESPACE);
    Init_pango_coverage(RG_TARGET_NAMESPACE);
    Init_pango_engine(RG_TARGET_NAMESPACE);
    Init_pango_font(RG_TARGET_NAMESPACE);
    Init_pango_font_description(RG_TARGET_NAMESPACE);
    Init_pango_font_face(RG_TARGET_NAMESPACE);
    Init_pango_font_family(RG_TARGET_NAMESPACE);
    Init_pango_font_map(RG_TARGET_NAMESPACE);
    Init_pango_font_metrics(RG_TARGET_NAMESPACE);
    Init_pango_fontset(RG_TARGET_NAMESPACE);
    Init_pango_fontset_simple(RG_TARGET_NAMESPACE);
    Init_pango_glyph_info(RG_TARGET_NAMESPACE);
    Init_pango_glyph_item(RG_TARGET_NAMESPACE);
    Init_pango_glyph_string(RG_TARGET_NAMESPACE);
    Init_pango_gravity(RG_TARGET_NAMESPACE);
    Init_pango_item(RG_TARGET_NAMESPACE);
    Init_pango_language(RG_TARGET_NAMESPACE);
    Init_pango_layout(RG_TARGET_NAMESPACE);
    Init_pango_layout_iter(RG_TARGET_NAMESPACE);
    Init_pango_layout_line(RG_TARGET_NAMESPACE);
    Init_pango_logattr(RG_TARGET_NAMESPACE);
    Init_pango_matrix(RG_TARGET_NAMESPACE);
    Init_pango_rectangle(RG_TARGET_NAMESPACE);
    Init_pangorenderer(RG_TARGET_NAMESPACE);
    Init_pango_script(RG_TARGET_NAMESPACE);
    Init_pango_script_iter(RG_TARGET_NAMESPACE);
    Init_pango_array(RG_TARGET_NAMESPACE);
}
