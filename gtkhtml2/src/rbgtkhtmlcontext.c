/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkhtmlcontext.c

  $Author: mutoh $
  $Date: 2003/11/09 15:44:50 $

  Copyright (C) 2003 Masao Mutoh
************************************************/

#include "rbgtkhtml.h"

static VALUE
html_s_context_get(self)
    VALUE self;
{
    return GOBJ2RVAL(gtk_html_context_get());
}

void 
Init_html_context(mGtkHtml2)
    VALUE mGtkHtml2;
{
    VALUE context = G_DEF_CLASS(GTK_HTML_CONTEXT_TYPE, "HtmlContext", mGtkHtml2);

    rb_define_singleton_method(context, "get", html_s_context_get, 0);
}

