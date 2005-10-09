/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkhtmlcontext.c

  $Author: mutoh $
  $Date: 2005/10/09 17:43:57 $

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

    rb_define_const(context, "BUILD_VERSION",
                    rb_ary_new3(3,
                                INT2FIX(GTKHTML_MAJOR_VERSION),
                                INT2FIX(GTKHTML_MINOR_VERSION),
                                INT2FIX(GTKHTML_MICRO_VERSION)));

    rb_define_singleton_method(context, "get", html_s_context_get, 0);
}

