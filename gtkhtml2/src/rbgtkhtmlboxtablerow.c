/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkhtmlboxtablerow.c

  $Author: mutoh $
  $Date: 2003/11/09 15:44:50 $

  Copyright (C) 2003 Masao Mutoh
************************************************/

#include <libgtkhtml/layout/htmlboxtablerow.h>
#include "rbgtkhtml.h"

#define _SELF(s) (HTML_BOX_TABLE_ROW(RVAL2GOBJ(s)))

static VALUE
rb_row_initialize(self)
    VALUE self;
{
    G_INITIALIZE(self, html_box_table_row_new());
    return Qnil;
}

/*
gint html_box_table_row_get_num_cols     (HtmlBox *self, gint rownum);
gint html_box_table_row_update_spaninfo  (HtmlBoxTableRow *row, gint *spaninfo);
gint html_box_table_row_fill_cells_array (HtmlBox *self, HtmlBox **cells, gint *spaninfo);
gint html_box_table_row_fill_cells_array (HtmlBox *self, HtmlBox **cells, gint *spaninfo);
*/

void 
Init_html_box_table_row(mGtkHtml2)
    VALUE mGtkHtml2;
{
    VALUE row = G_DEF_CLASS(HTML_TYPE_BOX_TABLE_ROW, "HtmlBoxTableRow", mGtkHtml2);

    rb_define_method(row, "initialize", rb_row_initialize, 0);
}

