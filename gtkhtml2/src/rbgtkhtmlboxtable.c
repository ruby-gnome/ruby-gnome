/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkhtmlboxtable.c

  $Author: sakai $
  $Date: 2003/11/17 14:22:57 $

  Copyright (C) 2003 Masao Mutoh
************************************************/

#include "rbgtkhtml.h"
#include <libgtkhtml/layout/htmlboxtable.h>

#define _SELF(s) (HTML_BOX_TABLE(RVAL2GOBJ(s)))

static VALUE
rb_html_box_table_initialize(self)
    VALUE self;
{
    G_INITIALIZE(self, html_box_table_new());
    return Qnil;
}

static VALUE
table_remove_row(self, row)
    VALUE self, row;
{
    html_box_table_remove_row(_SELF(self), HTML_BOX_TABLE_ROW(RVAL2GOBJ(row)));
    return self;
}

static VALUE
table_remove_caption(self, caption)
    VALUE self, caption;
{
    html_box_table_remove_caption(_SELF(self), HTML_BOX_TABLE_CAPTION(RVAL2GOBJ(caption)));
    return self;
}

static VALUE
table_add_thead(self, row)
    VALUE self, row;
{
    html_box_table_add_thead(_SELF(self), HTML_BOX_TABLE_ROW(RVAL2GOBJ(row)));
    return self;
}

static VALUE
table_add_tbody(self, row)
    VALUE self, row;
{
    html_box_table_add_tbody(_SELF(self), HTML_BOX_TABLE_ROW(RVAL2GOBJ(row)));
    return self;
}

static VALUE
table_add_tfoot(self, row)
    VALUE self, row;
{
    html_box_table_add_tfoot(_SELF(self), HTML_BOX_TABLE_ROW(RVAL2GOBJ(row)));
    return self;
}

#ifdef HAVE_HTML_BOX_TABLE_GET_TOP_OFFSET
static VALUE
table_get_top_offset(self, boxwidth)
    VALUE self, boxwidth;
{
    return INT2NUM(html_box_table_get_top_offset(_SELF(self), NUM2INT(boxwidth)));
}
#endif

#ifdef HAVE_HTML_BOX_TABLE_GET_BOTTOM_OFFSET
static VALUE
table_get_bottom_offset(self, boxwidth)
    VALUE self, boxwidth;
{
    return INT2NUM(html_box_table_get_bottom_offset(_SELF(self), NUM2INT(boxwidth)));
}
#endif

static VALUE
table_cell_added(self)
    VALUE self;
{
    html_box_table_cell_added(_SELF(self));
    return self;
}

void 
Init_html_box_table(mGtkHtml2)
    VALUE mGtkHtml2;
{
    VALUE boxtable = G_DEF_CLASS(HTML_TYPE_BOX_TABLE, "HtmlBoxTable", mGtkHtml2);

    rb_define_method(boxtable, "initialize", rb_html_box_table_initialize, 0);
    rb_define_method(boxtable, "remove_row", table_remove_row, 1);
    rb_define_method(boxtable, "remove_caption", table_remove_caption, 1);
    rb_define_method(boxtable, "add_thead", table_add_thead, 1);
    rb_define_method(boxtable, "add_tbody", table_add_tbody, 1);
    rb_define_method(boxtable, "add_tfoot", table_add_tfoot, 1);
#ifdef HAVE_HTML_BOX_TABLE_GET_TOP_OFFSET
    rb_define_method(boxtable, "get_top_offset", table_get_top_offset, 1);
#endif
#ifdef HAVE_HTML_BOX_TABLE_GET_BOTTOM_OFFSET
    rb_define_method(boxtable, "get_bottom_offset", table_get_bottom_offset, 1);
#endif
    rb_define_method(boxtable, "cell_added", table_cell_added, 0);

}

