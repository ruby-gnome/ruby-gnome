/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkhtmldocument.c

  $Author: mutoh $
  $Date: 2003/11/09 15:44:50 $

  Copyright (C) 2003 Masao Mutoh
  Copyright (C) 2003 Martin Povolny
************************************************/
#include "rbgtkhtml.h"

#define _SELF(s) (HTML_DOCUMENT(RVAL2GOBJ(s)))

static VALUE
rb_html_document_initialize(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    HtmlDocument *document = NULL;

    document = html_document_new ();

    G_INITIALIZE(self, document); 
    return Qnil;
}

static VALUE
rb_html_document_open_stream (self, mime_type )
    VALUE self;
    VALUE mime_type;
{
    return CBOOL2RVAL(html_document_open_stream( _SELF(self), RVAL2CSTR(mime_type) ));
}

static VALUE
rb_html_document_write_stream (self, data )
    VALUE self;
    VALUE data;
{
    html_document_write_stream( _SELF(self), RVAL2CSTR(data), -1 );
    
    return self;
}

static VALUE
rb_html_document_close_stream(self)
    VALUE self;
{
    html_document_close_stream( _SELF(self) );

    return self;
}

static VALUE
rb_html_document_clear(self)
    VALUE self;
{
    html_document_clear( _SELF(self) );

    return self;
}

static VALUE
rb_html_document_update_hover_node(self, node)
     VALUE self, node;
{
    html_document_update_hover_node(_SELF(self), DOM_NODE(RVAL2GOBJ(node)));
    return self;
}

static VALUE
rb_html_document_update_active_node(self, node)
     VALUE self, node;
{
    html_document_update_active_node(_SELF(self), DOM_NODE(RVAL2GOBJ(node)));
    return self;
}

static VALUE
rb_html_document_update_focus_element(self, element)
     VALUE self, element;
{
    html_document_update_focus_element(_SELF(self), DOM_ELEMENT(RVAL2GOBJ(element)));
    return self;
}

static VALUE
rb_html_document_find_anchor(self, anchor)
     VALUE self, anchor;
{
    html_document_find_anchor(_SELF(self), RVAL2CSTR(anchor));
    return self;
}

//// void (*link_clicked) (HtmlDocument *document, const gchar *url);
//static VALUE
//rb_html_document_link_clicked_sigf(num, values)
//    guint num;
//    const GValue* values;
//{
//    //HtmlDocument* document  = HTML_DOCUMENT( g_value_get_object(&values[0]) );
//    //const gchar* path             = g_value_get_string(&values[1]);
//    //                                                                                       
//    //return rb_ary_new3(2, GOBJ2RVAL(document), CSTR2RVAL(path));
//
//    return rb_ary_new3(2, GVAL2RVAL(&values[0]), GVAL2RVAL(&values[1]));
//}

void 
Init_html_document(mGtkHtml2)
    VALUE mGtkHtml2;
{
    VALUE gHtmlDoc = G_DEF_CLASS(html_document_get_type(), "HtmlDocument", mGtkHtml2);

    rb_define_method(gHtmlDoc, "initialize",   rb_html_document_initialize,  -1);
    rb_define_method(gHtmlDoc, "open_stream",  rb_html_document_open_stream,  1);
    rb_define_method(gHtmlDoc, "write_stream", rb_html_document_write_stream, 1);
    rb_define_method(gHtmlDoc, "close_stream", rb_html_document_close_stream, 0);
    rb_define_method(gHtmlDoc, "clear",        rb_html_document_clear,        0);
    rb_define_method(gHtmlDoc, "update_hover_node", rb_html_document_update_hover_node, 1);
    rb_define_method(gHtmlDoc, "update_active_node", rb_html_document_update_active_node, 1);
    rb_define_method(gHtmlDoc, "update_focus_element", rb_html_document_update_focus_element, 1);
    rb_define_method(gHtmlDoc, "find_anchor", rb_html_document_find_anchor, 1);

    rb_define_const(gHtmlDoc, "STATE_DONE", INT2NUM(HTML_DOCUMENT_STATE_DONE));
    rb_define_const(gHtmlDoc, "STATE_PARSING", INT2NUM(HTML_DOCUMENT_STATE_PARSING));

    //G_DEF_SIGNAL_FUNC(gHtmlDoc, "link_clicked", rb_html_document_link_clicked_sigf);
}

