#include "rbgtkhtml.h"

#define _SELF(s) (HTML_STREAM(RVAL2GOBJ(s)))

static VALUE
rb_html_stream_write( self, data, length )
    VALUE self, data;
{
    html_stream_write( _SELF(self), RVAL2CSTR(data), NUM2INT(length) );
    return self;
}

static VALUE
rb_html_stream_close( self ) 
    VALUE self;
{
    html_stream_close( _SELF(self) );
    return self;
}

void 
Init_html_stream(mGtkHtml2)
    VALUE mGtkHtml2;
{
    VALUE gHtmlStream = G_DEF_CLASS(html_stream_get_type(), "HtmlStream", mGtkHtml2);

    rb_define_method(gHtmlStream, "write", rb_html_stream_write, 2);
    rb_define_method(gHtmlStream, "close", rb_html_stream_close, 0);
}
