/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkhtmlparser.c

  $Author: mutoh $
  $Date: 2003/11/09 15:44:50 $

  Copyright (C) 2003 Masao Mutoh
************************************************/

#include "rbgtkhtml.h"

static VALUE
rb_html_parser_initialize(self, document, parser_type)
    VALUE self, document, parser_type;
{
    G_INITIALIZE(self, html_parser_new(HTML_DOCUMENT(RVAL2GOBJ(document)),
                                       NUM2INT(parser_type)));
    return Qnil;
}

void 
Init_html_parser(mGtkHtml2)
    VALUE mGtkHtml2;
{
    VALUE parser = G_DEF_CLASS(HTML_PARSER_TYPE, "HtmlParser", mGtkHtml2);

    rb_define_method(parser, "initialize", rb_html_parser_initialize, 2);

    rb_define_const(parser, "TYPE_HTML", INT2NUM(HTML_PARSER_TYPE_HTML));
    rb_define_const(parser, "TYPE_XML", INT2NUM(HTML_PARSER_TYPE_XML));
}

