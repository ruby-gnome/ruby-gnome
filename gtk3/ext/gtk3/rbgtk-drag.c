/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2002-2006 Masao Mutoh
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

#include "rbgtk3private.h"

#define RG_TARGET_NAMESPACE mDrag

struct rbgtk_rval2gtktargetentries_args {
    VALUE ary;
    long n;
    GtkTargetEntry *result;
};

static VALUE
rbgtk_rval2gtktargetentries_body(VALUE value)
{
    long i;
    struct rbgtk_rval2gtktargetentries_args *args = (struct rbgtk_rval2gtktargetentries_args *)value;

    for (i = 0; i < args->n; i++) {
        VALUE entry = rb_ary_to_ary(RARRAY_PTR(args->ary)[i]);
        VALUE flags = RARRAY_PTR(entry)[1];
        VALUE info = RARRAY_PTR(entry)[2];

        args->result[i].target = (gchar *)RVAL2CSTR_ACCEPT_NIL(RARRAY_PTR(entry)[0]);
        args->result[i].flags = NIL_P(flags) ? 0 : RVAL2GTKTARGETFLAGS(flags);
        args->result[i].info = NIL_P(info) ? 0 : NUM2INT(info);
    }

    return Qnil;
}

static G_GNUC_NORETURN VALUE
rbgtk_rval2gtktargetentries_rescue(VALUE value)
{
    g_free(((struct rbgtk_rval2gtktargetentries_args *)value)->result);

    rb_exc_raise(rb_errinfo());
}

GtkTargetEntry *
rbgtk_rval2gtktargetentries(VALUE value, long *n)
{
    struct rbgtk_rval2gtktargetentries_args args;

    args.ary = rb_ary_to_ary(value);
    args.n = RARRAY_LEN(args.ary);
    args.result = g_new(GtkTargetEntry, args.n + 1);

    rb_rescue(rbgtk_rval2gtktargetentries_body, (VALUE)&args,
              rbgtk_rval2gtktargetentries_rescue, (VALUE)&args);

    *n = args.n;

    return args.result;
}

GtkTargetEntry *
rbgtk_rval2gtktargetentries_accept_nil(VALUE value, long *n)
{
    if (!NIL_P(value))
        return rbgtk_rval2gtktargetentries(value, n);

    *n = 0;

    return NULL;
}

void
Init_gtk_drag(VALUE mGtk)
{
    VALUE RG_TARGET_NAMESPACE = rb_define_module_under(mGtk, "Drag");
    G_DEF_CLASS(GTK_TYPE_DEST_DEFAULTS, "DestDefaults", RG_TARGET_NAMESPACE);
    G_DEF_CLASS(GTK_TYPE_TARGET_FLAGS, "TargetFlags", RG_TARGET_NAMESPACE);
}
