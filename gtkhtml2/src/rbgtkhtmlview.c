#include "rbgtkhtml.h"

#define _SELF(s) (HTML_VIEW(RVAL2GOBJ(s)))

static VALUE
rb_html_view_initialize(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE rdoc;
    GtkWidget *widget = NULL;
    HtmlDocument *document = NULL;

    widget = html_view_new ();

	/* check if we were passed a HtmlDocument to use */
	if (rb_scan_args(argc, argv, "01", &rdoc) == 1) {
        html_view_set_document (HTML_VIEW (widget), HTML_DOCUMENT(RVAL2GOBJ(rdoc)));
    } else {
        document = html_document_new ();
        html_view_set_document (HTML_VIEW (widget), document);
    }

    RBGTK_INITIALIZE(self, widget); 
    return Qnil;
}

static VALUE
rb_html_view_set_html (self, htmlv)
    VALUE self;
    VALUE htmlv;
{
    HtmlDocument *document;
    char *html;
    
    document = _SELF(self)->document;
    html_document_clear (document);
    
    html = RVAL2CSTR(htmlv);
    
    if (html_document_open_stream (document, "text/html")) {
        html_document_write_stream (document, html, strlen (html));
        html_document_close_stream (document);
    }
    
    return self;
}

static VALUE
rb_html_view_set_document (self, document )
    VALUE self;
    VALUE document;
{
    html_view_set_document (_SELF(self), HTML_DOCUMENT(RVAL2GOBJ(document)));
    return self;
}

static VALUE
rb_html_view_get_document (self)
    VALUE self;
{
    return GOBJ2RVAL(_SELF(self)->document);
}

void 
Init_html_view(mGtkHtml2)
    VALUE mGtkHtml2;
{
    VALUE gHtmlView = G_DEF_CLASS(html_view_get_type(), "HtmlView", mGtkHtml2);

    rb_define_method(gHtmlView, "initialize",   rb_html_view_initialize,  -1);
    rb_define_method(gHtmlView, "set_html",     rb_html_view_set_html,     1);
    rb_define_method(gHtmlView, "set_document", rb_html_view_set_document, 1);
    rb_define_method(gHtmlView, "document",     rb_html_view_get_document, 0);

    G_DEF_SETTERS(gHtmlView);
}

