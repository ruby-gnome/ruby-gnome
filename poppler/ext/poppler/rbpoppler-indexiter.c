/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2006  Ruby-GNOME2 Project Team
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

#include "rbpoppler-private.h"

#define RG_TARGET_NAMESPACE cIndexIter

#define IITER2RVAL(obj) (POPPLERINDEXITER2RVAL(obj))
#define RVAL2IITER(obj) (RVAL2POPPLERINDEXITER(obj))

static VALUE RG_TARGET_NAMESPACE;

static ID id_valid;

/* Interface for getting the Index of a poppler_document */
#define CHECK_IITER_IS_VALID(iter) do {         \
    if (!RVAL2CBOOL(rg_valid_p(iter)))       \
        return Qnil;                            \
} while (0)

static VALUE
rg_valid_p(VALUE self)
{
    return rb_ivar_get(self, id_valid);
}

static VALUE
rg_initialize(VALUE self, VALUE document)
{
    PopplerIndexIter *iter;
    iter = poppler_index_iter_new(RVAL2GOBJ(document));
    G_INITIALIZE(self, iter);
    poppler_index_iter_free(iter);
    rb_ivar_set(self, id_valid, CBOOL2RVAL(iter));
    return Qnil;
}

static VALUE
rg_child(VALUE self)
{
    PopplerIndexIter *child;
    VALUE rb_child;

    CHECK_IITER_IS_VALID(self);
    child = poppler_index_iter_get_child(RVAL2IITER(self));
    rb_child = IITER2RVAL(child);
    poppler_index_iter_free(child);
    return rb_child;
}

static VALUE
rg_open_p(VALUE self)
{
    CHECK_IITER_IS_VALID(self);
    return CBOOL2RVAL(poppler_index_iter_is_open(RVAL2IITER(self)));
}

#ifndef HAVE_TYPE_POPPLERACTIONANY
static VALUE
rg_action(VALUE self)
{
    CHECK_IITER_IS_VALID(self);
    return POPPLERACTION2RVAL(poppler_index_iter_get_action(RVAL2IITER(self)));
}
#endif

static VALUE
rg_next(VALUE self)
{
    if (poppler_index_iter_next(RVAL2IITER(self))) {
        return Qtrue;
    } else {
        rb_ivar_set(self, id_valid, Qfalse);
        return Qfalse;
    }
}

static VALUE
rg_each(VALUE self)
{
    PopplerIndexIter *iter;

    CHECK_IITER_IS_VALID(self);
    iter = RVAL2IITER(self);
    do {
        rb_yield(self);
    } while (poppler_index_iter_next(iter));
    rb_ivar_set(self, id_valid, Qfalse);

    return self;
}

void
Init_poppler_indexiter(VALUE mPoppler)
{
    id_valid = rb_intern("valid?");

    RG_TARGET_NAMESPACE = G_DEF_CLASS(POPPLER_TYPE_INDEX_ITER, "IndexIter", mPoppler);

/* Interface for getting the Index of a poppler_document */
    rb_include_module(RG_TARGET_NAMESPACE, rb_mEnumerable);

    RG_DEF_METHOD(initialize, 1);
    RG_DEF_METHOD(child, 0);
    RG_DEF_METHOD_P(open, 0);
#ifndef HAVE_TYPE_POPPLERACTIONANY
    RG_DEF_METHOD(action, 0);
#endif
    RG_DEF_METHOD(next, 0);

    RG_DEF_METHOD_P(valid, 0);
    RG_DEF_METHOD(each, 0);

    G_DEF_SETTERS(RG_TARGET_NAMESPACE);
}
