/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkhtmlembedded.c

  $Author: mutoh $
  $Date: 2003/11/09 15:44:50 $

  Copyright (C) 2003 Masao Mutoh
************************************************/

#include <libgtkhtml/layout/html/htmlembedded.h>
#include "rbgtkhtml.h"

#define _SELF(s) (HTML_EMBEDDED(RVAL2GOBJ(s)))

static VALUE
emb_initialize(self, node, box_embedded)
    VALUE self, node, box_embedded;
{
    G_INITIALIZE(self, html_embedded_new(DOM_NODE(RVAL2GOBJ(node)), 
                                         HTML_BOX_EMBEDDED(RVAL2GOBJ(box_embedded))));
    return Qnil;
}

static VALUE
emb_get_dom_node(self)
    VALUE self;
{
    return GOBJ2RVAL(html_embedded_get_dom_node(_SELF(self)));
}

static VALUE
emb_set_descent(self, descent)
    VALUE self, descent;
{
    html_embedded_set_descent(_SELF(self), NUM2INT(descent));
    return self;
}

static VALUE
emb_get_descent(self)
    VALUE self;
{
    return INT2NUM(html_embedded_get_descent(_SELF(self)));
}

void 
Init_html_embedded(mGtkHtml2)
    VALUE mGtkHtml2;
{
    VALUE emb = G_DEF_CLASS(HTML_TYPE_EMBEDDED, "HtmlEmbedded", mGtkHtml2);

    rb_define_method(emb, "initialize", emb_initialize, 0);
    rb_define_method(emb, "dom_node", emb_get_dom_node, 0);
    rb_define_method(emb, "set_descent", emb_set_descent, 1);
    rb_define_method(emb, "descent", emb_get_descent, 0);

    G_DEF_SETTERS(emb);
}

