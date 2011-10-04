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

/* TODO: Why are we taking a third argument?  Shouldn't it be determined by the
 * length of rbdata? */
static VALUE
imcsimple_add_table(VALUE self, VALUE rbdata, VALUE rbmax_seq_len, VALUE rbn_seqs)
{
    GtkIMContextSimple *context_simple = _SELF(self);
    gint max_seq_len = NUM2INT(rbmax_seq_len);
    gint n_seqs = NUM2INT(rbn_seqs);
    long n;
    guint16 *data;

    if (max_seq_len > GTK_MAX_COMPOSE_LEN)
        rb_raise(rb_eArgError,
                 "max_seq_len cannot be greater than GTK_MAX_COMPOSE_LEN: %d > %d",
                 max_seq_len, GTK_MAX_COMPOSE_LEN);

    data = RVAL2GUINT16S(rbdata, &n);

    gtk_im_context_simple_add_table(context_simple, data, max_seq_len, n_seqs);

    g_free(data);

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
