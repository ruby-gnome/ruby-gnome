/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkhtmlboxform.c

  $Author: mutoh $
  $Date: 2003/11/16 11:18:56 $

  Copyright (C) 2003 Masao Mutoh
************************************************/

#include <libgtkhtml/layout/html/htmlboxform.h>
#include "rbgtkhtml.h"

#define _SELF(s) (HTML_BOX_FORM(RVAL2GOBJ(s)))

static VALUE
form_initialize(self)
    VALUE self;
{
    G_INITIALIZE(self, html_box_form_new());
    return Qnil;
}

static VALUE
form_get_radio_group(self, id)
    VALUE self, id;
{
    return GSLIST2ARY(html_box_form_get_radio_group(_SELF(self), RVAL2CSTR(id)));
}

/*
void    html_box_form_set_radio_group (HtmlBoxForm *form, gchar *id, GSList *list);
*/

void 
Init_html_box_form(mGtkHtml2)
    VALUE mGtkHtml2;
{
    VALUE form = G_DEF_CLASS(HTML_TYPE_BOX_FORM, "HtmlBoxForm", mGtkHtml2);

    rb_define_method(form, "initialize", form_initialize, 0);
    rb_define_method(form, "get_radio_group", form_get_radio_group, 1);

}

