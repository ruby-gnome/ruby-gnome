/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkhtmlbox.c

  $Author: mutoh $
  $Date: 2003/12/23 14:25:42 $

  Copyright (C) 2003 Masao Mutoh
  Copyright (C) 2003 Martin Povolny
************************************************/

#include "rbgtkhtml.h"

#define _SELF(s) (HTML_STREAM(RVAL2GOBJ(s)))

/* Are there better way?
   typedef struct {
   HtmlStreamWriteFunc write_func;
   HtmlStreamCloseFunc close_func;
   } StreamFunc;

   static VALUE
   rb_html_stream_initialize(self, write_func, close_func)
   VALUE self, write_func, close_func;
   {
   G_RELATIVE(self, write_func);
   G_RELATIVE(self, close_func);
   HtmlStream* stream = html_stream_new(stream_write_func, stream_close_func, NULL);
*/

static VALUE
rb_html_stream_write( self, data )
    VALUE self, data;
{
    StringValue(data);
    html_stream_write( _SELF(self), RVAL2CSTR(data), RSTRING(data)->len );
    return self;
}

static VALUE
rb_html_stream_close( self ) 
    VALUE self;
{
    html_stream_close( _SELF(self) );
    return self;
}

static VALUE
rb_html_stream_get_written(self)
    VALUE self;
{
    return INT2NUM(html_stream_get_written(_SELF(self)));
}

static VALUE
rb_html_stream_cancel(self)
    VALUE self;
{
    html_stream_cancel(_SELF(self));
    return self;
}

static void
abort_func(stream, user_data, func)
    HtmlStream* stream;
    gpointer user_data;
    gpointer func;
{
    rb_funcall((VALUE)func, rb_intern("call"), 1, GOBJ2RVAL(stream));
}

static VALUE
rb_html_stream_set_cancel_func(self)
    VALUE self;
{
    VALUE func = G_BLOCK_PROC();
    G_RELATIVE(self, func);  

    html_stream_set_cancel_func(_SELF(self), abort_func, (gpointer)func);
    return self;
}

static VALUE
rb_html_stream_get_mime_type(self)
    VALUE self;
{
    return CSTR2RVAL(html_stream_get_mime_type(_SELF(self)));
}

static VALUE
rb_html_stream_set_mime_type(self, mime_type)
    VALUE self, mime_type;
{
    html_stream_set_mime_type(_SELF(self), RVAL2CSTR(mime_type));
    return self;
}

void 
Init_html_stream(mGtkHtml2)
    VALUE mGtkHtml2;
{
    VALUE gHtmlStream = G_DEF_CLASS(html_stream_get_type(), "HtmlStream", mGtkHtml2);

    rb_define_method(gHtmlStream, "write", rb_html_stream_write, 1);
    rb_define_method(gHtmlStream, "close", rb_html_stream_close, 0);
    rb_define_method(gHtmlStream, "written", rb_html_stream_get_written, 0);
    rb_define_method(gHtmlStream, "cancel", rb_html_stream_cancel, 0);
    rb_define_method(gHtmlStream, "set_cancel_func", rb_html_stream_set_cancel_func, 0);
    rb_define_method(gHtmlStream, "mime_type", rb_html_stream_get_mime_type, 0);
    rb_define_method(gHtmlStream, "set_mime_type", rb_html_stream_set_mime_type, 1);

    G_DEF_SETTERS(gHtmlStream);
}
