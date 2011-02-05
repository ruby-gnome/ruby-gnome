/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkimmulticontext.c -

  $Author: mutoh $
  $Date: 2003/05/05 16:58:55 $

  Copyright (C) 2003 Masao Mutoh
************************************************/
#include "global.h"

#define _SELF(self) (GTK_IM_MULTICONTEXT(RVAL2GOBJ(self)))

static VALUE 
immulti_initialize(VALUE self)
{
    G_INITIALIZE(self, gtk_im_multicontext_new());
    return Qnil;
}

static VALUE
immulti_append_menuitems(VALUE self, VALUE menushell)
{
    gtk_im_multicontext_append_menuitems(_SELF(self), RVAL2GOBJ(menushell));
    return self;
}

void 
Init_gtk_im_multicontext()
{
    VALUE gIMMContext = G_DEF_CLASS(GTK_TYPE_IM_MULTICONTEXT, "IMMulticontext", mGtk);

    rb_define_method(gIMMContext, "initialize", immulti_initialize, 0);
    rb_define_method(gIMMContext, "append_menuitems", immulti_append_menuitems, 1);
}
