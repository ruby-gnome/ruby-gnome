/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkhtmlboxtablecaption.c

  $Author: mutoh $
  $Date: 2003/11/09 15:44:50 $

  Copyright (C) 2003 Masao Mutoh
************************************************/

#include "rbgtkhtml.h"
#include <libgtkhtml/layout/htmlboxtablecaption.h>

#define _SELF(s) (HTML_BOX_TABLE_CAPTION(RVAL2GOBJ(s)))

static VALUE
rb_html_box_table_caption_initialize(self)
    VALUE self;
{
    G_INITIALIZE(self, html_box_table_caption_new());
    return Qnil;
}

/*
static VALUE
rb_html_box_table_caption_relayout_width(self)
    VALUE self;
{
    html_box_table_caption_relayout_width(_SELF(self), HtmlRelayout* relayout, int width);
    return self;
}
*/

void 
Init_html_box_table_caption(mGtkHtml2)
    VALUE mGtkHtml2;
{
    VALUE boxtable_caption = G_DEF_CLASS(HTML_TYPE_BOX_TABLE_CAPTION, "HtmlBoxTableCaption", mGtkHtml2);

    rb_define_method(boxtable_caption, "initialize", rb_html_box_table_caption_initialize, 0);
}

