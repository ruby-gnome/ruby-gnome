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

#include "rbpoppler.h"

#define RG_TARGET_NAMESPACE rg_cIndexIter

#define IITER2RVAL(obj) (BOXED2RVAL(obj, POPPLER_TYPE_INDEX_ITER))
#define RVAL2IITER(obj) (RVAL2BOXED(obj, POPPLER_TYPE_INDEX_ITER))

VALUE RG_TARGET_NAMESPACE;

static ID id_valid;

/* Interface for getting the Index of a poppler_document */
#define CHECK_IITER_IS_VALID(iter) do {         \
    if (!RVAL2CBOOL(index_iter_valid_p(iter)))       \
        return Qnil;                            \
} while (0)

static VALUE
index_iter_valid_p(VALUE self)
{
    return rb_ivar_get(self, id_valid);
}

static VALUE
index_iter_initialize(VALUE self, VALUE document)
{
    PopplerIndexIter *iter;
    iter = poppler_index_iter_new(RVAL2GOBJ(document));
    G_INITIALIZE(self, iter);
    poppler_index_iter_free(iter);
    rb_ivar_set(self, id_valid, CBOOL2RVAL(iter));
    return Qnil;
}

static VALUE
index_iter_get_child(VALUE self)
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
index_iter_is_open(VALUE self)
{
    CHECK_IITER_IS_VALID(self);
    return CBOOL2RVAL(poppler_index_iter_is_open(RVAL2IITER(self)));
}

#ifndef HAVE_TYPE_POPPLERACTIONANY
static VALUE
index_iter_get_action(VALUE self)
{
    CHECK_IITER_IS_VALID(self);
    return POPPLER_ACTION2RVAL(poppler_index_iter_get_action(RVAL2IITER(self)));
}
#endif

static VALUE
index_iter_next(VALUE self)
{
    if (poppler_index_iter_next(RVAL2IITER(self))) {
        return Qtrue;
    } else {
        rb_ivar_set(self, id_valid, Qfalse);
        return Qfalse;
    }
}

static VALUE
index_iter_each(VALUE self)
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

    rb_define_method(RG_TARGET_NAMESPACE, "initialize", index_iter_initialize, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "child", index_iter_get_child, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "open?", index_iter_is_open, 0);
#ifndef HAVE_TYPE_POPPLERACTIONANY
    rb_define_method(RG_TARGET_NAMESPACE, "action", index_iter_get_action, 0);
#endif
    rb_define_method(RG_TARGET_NAMESPACE, "next", index_iter_next, 0);

    rb_define_method(RG_TARGET_NAMESPACE, "valid?", index_iter_valid_p, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "each", index_iter_each, 0);

    G_DEF_SETTERS(RG_TARGET_NAMESPACE);
}

