/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2003  Masao Mutoh
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 *  MA  02110-1301  USA
 */

#include "global.h"

#define RG_TARGET_NAMESPACE cIMContextSimple
#define _SELF(self) (GTK_IM_CONTEXT_SIMPLE(RVAL2GOBJ(self)))

static VALUE 
rg_initialize(VALUE self)
{
    G_INITIALIZE(self, gtk_im_context_simple_new());

    return Qnil;
}

/* TODO: Why are we taking a third argument?  Shouldn't it be determined by the
 * length of rbdata? */
static VALUE
rg_add_table(VALUE self, VALUE rbdata, VALUE rbmax_seq_len, VALUE rbn_seqs)
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

    data = RVAL2GUINT16S(rbdata, n);

    gtk_im_context_simple_add_table(context_simple, data, max_seq_len, n_seqs);

    g_free(data);

    return self;
}

void 
Init_gtk_imcontext_simple(VALUE mGtk)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_IM_CONTEXT_SIMPLE, "IMContextSimple", mGtk);

    RG_DEF_METHOD(initialize, 0);
    RG_DEF_METHOD(add_table, 3);

    rb_define_const(RG_TARGET_NAMESPACE, "MAX_COMPOSE_LEN", GTK_MAX_COMPOSE_LEN);
}
