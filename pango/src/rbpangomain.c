/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbpangomain.c -

  $Author: mutoh $
  $Date: 2005/02/12 18:13:34 $

  Copyright (C) 2002-2005 Masao Mutoh
************************************************/

#include "rbpango.h"

/*
 * Rendering
 */

static VALUE
rpango_reorder_items(self, logical_items)
    VALUE self, logical_items;
{
    int i;
    GList *glist = NULL;
    GList* ret;

    Check_Type(logical_items, T_ARRAY);

    for (i = 0; i < RARRAY(logical_items)->len; i++){
        glist = g_list_append(glist, RVAL2BOXED(RARRAY(logical_items)->ptr[i], PANGO_TYPE_ITEM));
    }

    ret = pango_reorder_items(glist);
    g_list_free(glist);

    return ret ? GLIST2ARY2(ret, PANGO_TYPE_ITEM) : Qnil;
}

static VALUE
rpango_unichar_direction(self, ch)
    VALUE self, ch;
{
    return GENUM2RVAL(pango_unichar_direction(NUM2UINT(ch)), PANGO_TYPE_DIRECTION);
}

static VALUE
rpango_find_base_dir(self, text)
    VALUE self, text;
{
    StringValue(text);
    return GENUM2RVAL(pango_find_base_dir(RVAL2CSTR(text), RSTRING(text)->len), 
                      PANGO_TYPE_DIRECTION);
}


/*
static VALUE
rpango_break(self, text)
     VALUE self, text;
{
  pango_break(RVAL2CSTR(text), NUM2INT(length), 
                                             int length,
                                             PangoAnalysis *analysis,
                                             PangoLogAttr *attrs,
                                             int attrs_len);
}
void        pango_get_log_attrs             (const char *text,
                                             int length,
                                             int level,
                                             PangoLanguage *language,
                                             PangoLogAttr *log_attrs,
                                             int attrs_len);
*/

static VALUE
rpango_find_paragraph_boundary(self, text)
     VALUE self, text;
{
    gint paragraph_delimiter_index, next_paragraph_start;
    
    pango_find_paragraph_boundary(RVAL2CSTR(text), RSTRING(text)->len,
                                  &paragraph_delimiter_index,
                                  &next_paragraph_start);
    return rb_ary_new3(2, INT2NUM(paragraph_delimiter_index), 
                       INT2NUM(next_paragraph_start));
}

/*
What should I return this ?
void        pango_default_break             (const gchar *text,
                                             int length,
                                             PangoAnalysis *analysis,
                                             PangoLogAttr *attrs,
                                             int attrs_len);

static VALUE
rpango_default_break(self, text, analysis)
    VALUE self, text, anlysis;
{
    StringValue(text);
    pango_default_break(RVAL2CSTR(text), RSTRING(text)->len, 
                        NIL_P(analysis) ? NULL : RVAL2BOXED(analysis, PANGO_TYPE_ANALYSIS),
                        &attrs, attrs_len);
}
*/

static VALUE
rpango_shape(self, text, analysis)
    VALUE self, text, analysis;
{
    VALUE ret;
    PangoGlyphString* glyphs = pango_glyph_string_new();
    StringValue(text);
    pango_shape(RVAL2CSTR(text), RSTRING(text)->len, RVAL2BOXED(analysis, PANGO_TYPE_ANALYSIS), glyphs);
    ret = BOXED2RVAL(glyphs, PANGO_TYPE_GLYPH_STRING);
    pango_glyph_string_free (glyphs);
    return ret;
}

/* This method from rbpangoattribute.c */
static VALUE
rpango_parse_markup(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
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

    ret = pango_parse_markup(RVAL2CSTR(markup_text),
                             RSTRING(markup_text)->len,
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
rpango_pixels(self, pixels)
    VALUE self, pixels;
{
    return rb_float_new(PANGO_PIXELS(NUM2DBL(pixels)));
}

void
Init_pango_main()
{
    rb_define_module_function(mPango, "reorder_items", rpango_reorder_items, 1);
    rb_define_module_function(mPango, "unichar_direction", rpango_unichar_direction, 1);
    rb_define_module_function(mPango, "find_base_dir", rpango_find_base_dir, 1);
    rb_define_module_function(mPango, "find_paragraph_boundary", rpango_find_paragraph_boundary, 1);
    rb_define_module_function(mPango, "shape", rpango_shape, 2);
    rb_define_module_function(mPango, "parse_markup", rpango_parse_markup, -1);
    rb_define_module_function(mPango, "pixels", rpango_pixels, 1);
    rb_define_const(mPango, "SCALE", INT2FIX(PANGO_SCALE));
}
