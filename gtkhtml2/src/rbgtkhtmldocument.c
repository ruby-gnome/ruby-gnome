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
    VALUE gHtmlView = G_DEF_CLASS(html_document_get_type(), "HtmlDocument", mGtkHtml2);

    rb_define_method(gHtmlView, "initialize",   rb_html_document_initialize,  -1);
    rb_define_method(gHtmlView, "open_stream",  rb_html_document_open_stream,  1);
    rb_define_method(gHtmlView, "write_stream", rb_html_document_write_stream, 1);
    rb_define_method(gHtmlView, "close_stream", rb_html_document_close_stream, 0);
    rb_define_method(gHtmlView, "clear",        rb_html_document_clear,        0);

    //G_DEF_SIGNAL_FUNC(gHtmlView, "link_clicked", rb_html_document_link_clicked_sigf);
}

