/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkhtmlboxembedded.c

  $Author: sakai $
  $Date: 2003/11/17 14:22:57 $

  Copyright (C) 2003 Masao Mutoh
************************************************/

#include <libgtkhtml/layout/html/htmlboxembedded.h>
#include "rbgtkhtml.h"

#define _SELF(s) (HTML_BOX_EMBEDDED(RVAL2GOBJ(s)))

#ifdef HAVE_HTML_BOX_EMBEDDED_NEW
static VALUE
emb_initialize(self)
    VALUE self;
{
    G_INITIALIZE(self, html_box_embedded_new());
    return Qnil;
}
#endif

static VALUE
emb_get_descent(self)
    VALUE self;
{
    return INT2NUM(html_box_embedded_get_descent(_SELF(self)));
}

static VALUE
emb_set_descent(self, descent)
    VALUE self, descent;
{
    html_box_embedded_set_descent(_SELF(self), NUM2INT(descent));
    return self;
}

static VALUE
emb_set_view(self, view)
    VALUE self, view;
{
    html_box_embedded_set_view(_SELF(self), HTML_VIEW(RVAL2GOBJ(view)));
    return self;
}

static VALUE
emb_set_widget(self, widget)
    VALUE self, widget;
{
    html_box_embedded_set_widget(_SELF(self), GTK_WIDGET(RVAL2GOBJ(widget)));
    return self;
}

void 
Init_html_box_embedded(mGtkHtml2)
    VALUE mGtkHtml2;
{
    VALUE emb = G_DEF_CLASS(HTML_TYPE_BOX_EMBEDDED, "HtmlBoxEmbedded", mGtkHtml2);

#ifdef HAVE_HTML_BOX_EMBEDDED_NEW
    rb_define_method(emb, "initialize", emb_initialize, 0);
#endif
    rb_define_method(emb, "descent", emb_get_descent, 0);
    rb_define_method(emb, "set_descent", emb_set_descent, 1);
    rb_define_method(emb, "set_view", emb_set_view, 1);
    rb_define_method(emb, "set_widget", emb_set_widget, 1);

    G_DEF_SETTERS(emb);
}

