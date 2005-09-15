/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbatkstreamablecontent.c -

  $Author: mutoh $
  $Date: 2005/09/15 17:30:46 $

  Copyright (C) 2003 Masao Mutoh
************************************************/

#include "rbatk.h"

#define _SELF(s) (ATK_STREAMABLE_CONTENT(RVAL2GOBJ(s)))

static VALUE
rbatkst_get_n_mime_types(self)
     VALUE self;
{
    return INT2NUM(atk_streamable_content_get_n_mime_types(_SELF(self)));
}

static VALUE
rbatkst_get_mime_type(self, i)
     VALUE self, i;
{
    return CSTR2RVAL(atk_streamable_content_get_mime_type(_SELF(self), NUM2INT(i)));
}

static VALUE
rbatkst_get_stream(self, mime_type)
     VALUE self, mime_type;
{
    GIOChannel* io = atk_streamable_content_get_stream(_SELF(self), RVAL2CSTR(mime_type));
    if (!io)
        rb_raise(rb_eRuntimeError, "Couldn't get the stream.");

    return BOXED2RVAL(io, G_TYPE_IO_CHANNEL);
}


void
Init_atk_streamable_content()
{
    VALUE mContent = G_DEF_INTERFACE(ATK_TYPE_STREAMABLE_CONTENT, "StreamableContent", mAtk);

    rb_define_method(mContent, "n_mime_types", rbatkst_get_n_mime_types, 0);
    rb_define_method(mContent, "mime_type", rbatkst_get_mime_type, 1);
    rb_define_method(mContent, "get_stream", rbatkst_get_stream, 1);
}
