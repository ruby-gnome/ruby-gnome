/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkimcontextsimple.c -

  $Author: mutoh $
  $Date: 2003/05/05 16:58:55 $

  Copyright (C) 2003 Masao Mutoh
************************************************/
#include "global.h"

#define _SELF(self) (GTK_IM_CONTEXT_SIMPLE(RVAL2GOBJ(self)))

static VALUE 
imcsimple_initialize(VALUE self)
{
    G_INITIALIZE(self, gtk_im_context_simple_new());
    return Qnil;
}

static VALUE
imcsimple_add_table(VALUE self, VALUE data, VALUE max_seq_len, VALUE n_seqs)
{
    int i;
    guint16* gdata = ALLOCA_N(guint16, RARRAY_LEN(data));
    
    for (i = 0; i < RARRAY_LEN(data); i++) {
        gdata[i] = NUM2INT(RARRAY_PTR(data)[i]);
    }
    
    gtk_im_context_simple_add_table(_SELF(self), gdata,
                                    NUM2INT(max_seq_len), NUM2INT(n_seqs));
    return self;
}

void 
Init_gtk_imcontext_simple()
{
    VALUE gIMContextS = G_DEF_CLASS(GTK_TYPE_IM_CONTEXT_SIMPLE, "IMContextSimple", mGtk);

    rb_define_method(gIMContextS, "initialize", imcsimple_initialize, 0);
    rb_define_method(gIMContextS, "add_table", imcsimple_add_table, 3);

    rb_define_const(gIMContextS, "MAX_COMPOSE_LEN", GTK_MAX_COMPOSE_LEN);
}
