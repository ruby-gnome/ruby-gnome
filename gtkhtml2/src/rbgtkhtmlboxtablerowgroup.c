/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkhtmlboxtablerowgroup.c

  $Author: mutoh $
  $Date: 2003/11/16 11:18:56 $

  Copyright (C) 2003 Masao Mutoh
************************************************/

#include <libgtkhtml/layout/htmlboxtablerowgroup.h>
#include "rbgtkhtml.h"

#define _SELF(s) (HTML_BOX_TABLE_ROW_GROUP(RVAL2GOBJ(s)))

static VALUE
rb_row_group_initialize(self, type)
    VALUE self, type;
{
    G_INITIALIZE(self, html_box_table_row_group_new(NUM2INT(type)));
    return Qnil;
}

void 
Init_html_box_table_row_group(mGtkHtml2)
    VALUE mGtkHtml2;
{
    VALUE row_group = G_DEF_CLASS(HTML_TYPE_BOX_TABLE_ROW_GROUP, "HtmlBoxTableRowGroup", mGtkHtml2);

    rb_define_method(row_group, "initialize", rb_row_group_initialize, 1);
}

