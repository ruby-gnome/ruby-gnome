/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbatkdocument.c -

  $Author: mutoh $
  $Date: 2003/12/04 18:06:17 $

  Copyright (C) 2003 Masao Mutoh
************************************************/

#include "rbatk.h"

#define _SELF(s) (ATK_DOCUMENT(RVAL2GOBJ(s)))

static VALUE
rbatk_document_get_document_type(self)
    VALUE self;
{
    return CSTR2RVAL(atk_document_get_document_type(_SELF(self)));
}

/*
How can I implement this?
static VALUE
rbatk_document_get_document(self)
    VALUE self;
{
    return GOBJ2RVAL(atk_document_get_document(_SELF(self)));
}
*/

void
Init_atk_document()
{
    VALUE mDoc = G_DEF_INTERFACE(ATK_TYPE_DOCUMENT, "Document", mAtk);

    rb_define_method(mDoc, "document_type", rbatk_document_get_document_type, 0);
/*
    rb_define_method(mDoc, "document", rbatk_document_get_document, 0);
*/
}
