/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbpangomain.c -

  $Author: mutoh $
  $Date: 2002/12/31 07:00:58 $

  Copyright (C) 2002 Masao Mutoh
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

void
Init_pango_main()
{
    rb_define_module_function(mPango, "find_paragraph_boundary", rpango_find_paragraph_boundary, 1);
}
