/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbpangomain.c -

  $Author: mutoh $
  $Date: 2003/02/02 09:10:53 $

  Copyright (C) 2002,2003 Masao Mutoh
************************************************/

#include "rbpango.h"

/*
 * Rendering
 */

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
void        pango_default_break             (const gchar *text,
                                             int length,
                                             PangoAnalysis *analysis,
                                             PangoLogAttr *attrs,
                                             int attrs_len);

void        pango_shape                     (const gchar *text,
                                             gint length,
                                             PangoAnalysis *analysis,
                                             PangoGlyphString *glyphs);
*/

/* This method from rbpangoattribute.c */
static VALUE
rpango_parse_markup(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE markup_text, accel_marker;
    PangoAttrList *attr_list;
    gchar* text;
    gunichar accel_char;
    GError *error = NULL;
    gboolean ret;
    char c;

    rb_scan_args(argc, argv, "11", &markup_text, &accel_marker);

    ret = pango_parse_markup(RVAL2CSTR(markup_text),
                             RSTRING(markup_text)->len,
                             NIL_P(accel_marker) ? 0 : NUM2CHR(accel_marker),
                             &attr_list, &text, &accel_char, &error);

    if (!ret) RAISE_GERROR(error);

    c = (char)accel_char;
    return rb_ary_new3(3, 
                       attr_list ? BOXED2RVAL(attr_list, PANGO_TYPE_ATTR_LIST) : Qnil,
                       text ? CSTR2RVAL(text) : Qnil,
                       accel_char ? rb_str_new(&c, 1) : Qnil);
}

void
Init_pango_main()
{
    rb_define_module_function(mPango, "find_paragraph_boundary", rpango_find_paragraph_boundary, 1);
    rb_define_module_function(mPango, "parse_markup", rpango_parse_markup, -1);
    rb_define_const(mPango, "SCALE", INT2FIX(PANGO_SCALE));
}
