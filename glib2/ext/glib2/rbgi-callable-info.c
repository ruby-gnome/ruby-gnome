/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2012-2026  Ruby-GNOME Project Team
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

#ifdef HAVE_GIREPOSITORY
#include "rbgi-private.h"

#define RG_TARGET_NAMESPACE rb_cGICallableInfo
#define SELF(self) RVAL2GI_CALLABLE_INFO(self)

static VALUE
rg_set_lock_gvl_default(VALUE self, VALUE rb_boolean)
{
    return rb_iv_set(self, "lock_gvl_default", rb_boolean);
}

static VALUE
rg_add_lock_gvl_predicate(VALUE self)
{
    VALUE rb_predicates;
    if (!RVAL2CBOOL(rb_ivar_defined(self, rb_intern("lock_gvl_predicates")))) {
        rb_predicates = rb_ary_new();
        rb_iv_set(self, "lock_gvl_predicates", rb_predicates);
    } else {
        rb_predicates = rb_iv_get(self, "lock_gvl_predicates");
    }
    rb_ary_push(rb_predicates, rb_block_lambda());
    return Qnil;
}

static VALUE
rg_lock_gvl_p(int argc, VALUE *argv, VALUE self)
{
    VALUE rb_receiver;
    VALUE rb_lock_gvl_default = Qtrue;
    rb_scan_args(argc, argv, "01", &rb_receiver);
    if (RVAL2CBOOL(rb_ivar_defined(self, rb_intern("lock_gvl_default")))) {
        rb_lock_gvl_default = rb_iv_get(self, "lock_gvl_default");
    }
    if (NIL_P(rb_receiver)) {
        return rb_lock_gvl_default;
    }
    if (!RVAL2CBOOL(rb_ivar_defined(self, rb_intern("lock_gvl_predicates")))) {
        return rb_lock_gvl_default;
    }
    VALUE rb_predicates = rb_iv_get(self, "lock_gvl_predicates");
    long n = RARRAY_LEN(rb_predicates);
    long i;
    VALUE rb_args = rb_ary_new_from_args(2,
                                         self,
                                         rb_receiver);
    VALUE rb_lock_gvl = rb_lock_gvl_default;
    RARRAY_PTR_USE(rb_predicates, rb_predicates_raw, {
        for (i = 0; i < n; i++) {
            VALUE rb_predicate = rb_predicates_raw[n - i - 1];
            VALUE rb_result = rb_proc_call(rb_predicate, rb_args);
            if (NIL_P(rb_result)) {
                continue;
            }
            rb_lock_gvl = rb_result;
            break;
        }
    });
    return rb_lock_gvl;
}

static VALUE
rg_can_throw_gerror_p(VALUE self)
{
    GICallableInfo *info = SELF(self);
    return CBOOL2RVAL(gi_callable_info_can_throw_gerror(info));
}

static VALUE
rg_return_type(VALUE self)
{
    GICallableInfo *info = SELF(self);
    return GOBJ2RVAL_UNREF(gi_callable_info_get_return_type(info));
}

static VALUE
rg_caller_owns(VALUE self)
{
    GICallableInfo *info = SELF(self);
    return GI_TRANSFER2RVAL(gi_callable_info_get_caller_owns(info));
}

static VALUE
rg_may_return_null_p(VALUE self)
{
    GICallableInfo *info = SELF(self);
    return CBOOL2RVAL(gi_callable_info_may_return_null(info));
}

static VALUE
rg_n_args(VALUE self)
{
    GICallableInfo *info = SELF(self);
    return INT2NUM(gi_callable_info_get_n_args(info));
}

static VALUE
rg_get_arg(VALUE self, VALUE rb_n)
{
    GICallableInfo *info = SELF(self);
    gint n = NUM2INT(rb_n);
    return GOBJ2RVAL_UNREF(gi_callable_info_get_arg(info, n));
}

void
rbgi_callable_info_init(VALUE rb_mGLib)
{
    VALUE RG_TARGET_NAMESPACE;

    RG_TARGET_NAMESPACE =
        G_DEF_CLASS(GI_TYPE_CALLABLE_INFO, "CallableInfo", rb_mGLib);

    RG_DEF_METHOD(set_lock_gvl_default, 1);
    RG_DEF_METHOD(add_lock_gvl_predicate, 0);
    RG_DEF_METHOD_P(lock_gvl, -1);

    RG_DEF_METHOD_P(can_throw_gerror, 0);
    RG_DEF_METHOD(return_type, 0);
    RG_DEF_METHOD(caller_owns, 0);
    RG_DEF_METHOD_P(may_return_null, 0);
    RG_DEF_METHOD(n_args, 0);
    RG_DEF_METHOD(get_arg, 1);
}
#endif
