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

#include "rb-gi-private.h"

#ifdef HAVE_RUBY_THREAD_H
#  include <ruby/thread.h>
#  define RB_THREAD_CALL_WITHOUT_GVL_FUNC_RETURN_TYPE void *
#  define RB_THREAD_CALL_WITHOUT_GVL_FUNC_RETURN_VALUE NULL
#else
#  define rb_thread_call_without_gvl(func, func_data, ubf, ubf_data) \
    rb_thread_blocking_region(func, func_data, ubf, ubf_data)
#  define RB_THREAD_CALL_WITHOUT_GVL_FUNC_RETURN_TYPE VALUE
#  define RB_THREAD_CALL_WITHOUT_GVL_FUNC_RETURN_VALUE Qnil
#endif

typedef struct {
    GICallableInfo *info;
    GType implementor_gtype;
    GArray *in_args;
    GArray *out_args;
    GIArgument return_value;
    GError **error;
    gboolean succeeded;
} InvokeData;

static void
rb_gi_callable_info_invoke_raw_call(InvokeData *data)
{
    GIInfoType type = g_base_info_get_type((GIBaseInfo *)(data->info));
    if (type == GI_INFO_TYPE_VFUNC) {
        data->succeeded =
            g_vfunc_info_invoke((GIVFuncInfo *)(data->info),
                                data->implementor_gtype,
                                (GIArgument *)((void *)(data->in_args->data)),
                                data->in_args->len,
                                (GIArgument *)((void *)(data->out_args->data)),
                                data->out_args->len,
                                &(data->return_value),
                                data->error);
    } else {
        data->succeeded =
            g_function_info_invoke((GIFunctionInfo *)(data->info),
                                   (GIArgument *)((void *)(data->in_args->data)),
                                   data->in_args->len,
                                   (GIArgument *)((void *)(data->out_args->data)),
                                   data->out_args->len,
                                   &(data->return_value),
                                   data->error);
    }
}

static RB_THREAD_CALL_WITHOUT_GVL_FUNC_RETURN_TYPE
rb_gi_callable_info_invoke_raw_call_without_gvl_body(void *user_data)
{
    InvokeData *data = (InvokeData *)user_data;

    rb_gi_callable_info_invoke_raw_call(data);

    return RB_THREAD_CALL_WITHOUT_GVL_FUNC_RETURN_VALUE;
}

VALUE
rb_gi_callable_info_invoke_raw(GICallableInfo *info,
                               VALUE rb_info,
                               GType implementor_gtype,
                               VALUE rb_receiver,
                               VALUE rb_arguments,
                               GIArgument *return_value,
                               VALUE *rb_return_value)
{
    RBGIArguments args;
    VALUE rb_out_args = Qnil;
    gboolean succeeded;
    GError *error = NULL;
    gboolean lock_gvl = FALSE;

    lock_gvl = RVAL2CBOOL(rb_funcall(rb_info,
                                     rb_intern("lock_gvl?"),
                                     1,
                                     rb_receiver));
    rb_arguments = rbg_to_array(rb_arguments);

    rb_gi_arguments_init(&args, info, rb_receiver, rb_arguments, NULL);
    {
        InvokeData data;
        data.info = info;
        data.implementor_gtype = implementor_gtype;
        data.in_args = args.in_args;
        data.out_args = args.out_args;
        data.error = &error;
        if (lock_gvl) {
            rb_gi_callable_info_invoke_raw_call(&data);
        } else {
            rb_thread_call_without_gvl(
                rb_gi_callable_info_invoke_raw_call_without_gvl_body, &data,
                NULL, NULL);
        }
        succeeded = data.succeeded;

        if (return_value) {
            *return_value = data.return_value;
        }
        if (rb_return_value) {
            if (succeeded) {
                *rb_return_value =
                    rb_gi_arguments_convert_return_value(&args,
                                                         &(data.return_value));
            } else {
                *rb_return_value = Qnil;
            }
        }
    }

    if (succeeded) {
        rb_out_args = rb_gi_arguments_get_rb_out_args(&args);
    }
    rb_gi_arguments_clear(&args);
    if (!succeeded) {
        RG_RAISE_ERROR(error);
    }

    if (!NIL_P(rb_out_args) && RARRAY_LEN(rb_out_args) == 1) {
        VALUE rb_out_arg;
        rb_out_arg = RARRAY_AREF(rb_out_args, 0);
        if (rb_obj_is_kind_of(rb_out_arg, rb_eException)) {
            rb_exc_raise(rb_out_arg);
        }
    }

    return rb_out_args;
}

#define RG_TARGET_NAMESPACE rb_cGICallableInfo
#define SELF(self) RVAL2GI_CALLABLE_INFO(self)

GType
gi_callable_info_get_type(void)
{
    static GType type = 0;
    if (type == 0) {
	type = g_boxed_type_register_static("GICallableInfo",
                                            (GBoxedCopyFunc)g_base_info_ref,
                                            (GBoxedFreeFunc)g_base_info_unref);
    }
    return type;
}

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
    for (i = 0; i < n; i++) {
        VALUE rb_predicate = RARRAY_PTR(rb_predicates)[n - i - 1];
        VALUE rb_result = rb_proc_call(rb_predicate, rb_args);
        if (NIL_P(rb_result)) {
            continue;
        }
        return rb_result;
    }
    return rb_lock_gvl_default;
}

static VALUE
rg_can_throw_gerror_p(VALUE self)
{
    GICallableInfo *info = SELF(self);
    return CBOOL2RVAL(g_callable_info_can_throw_gerror(info));
}

static VALUE
rg_return_type(VALUE self)
{
    GICallableInfo *info;

    info = SELF(self);
    return GI_BASE_INFO2RVAL_WITH_UNREF(g_callable_info_get_return_type(info));
}

static VALUE
rg_caller_owns(VALUE self)
{
    GICallableInfo *info;

    info = SELF(self);
    return GI_TRANSFER2RVAL(g_callable_info_get_caller_owns(info));
}

static VALUE
rg_may_return_null_p(VALUE self)
{
    GICallableInfo *info;

    info = SELF(self);
    return CBOOL2RVAL(g_callable_info_may_return_null(info));
}

static VALUE
rg_n_args(VALUE self)
{
    GICallableInfo *info;

    info = SELF(self);
    return INT2NUM(g_callable_info_get_n_args(info));
}

static VALUE
rg_get_arg(VALUE self, VALUE rb_n)
{
    GICallableInfo *info;
    gint n;

    info = SELF(self);
    n = NUM2INT(rb_n);
    return GI_BASE_INFO2RVAL_WITH_UNREF(g_callable_info_get_arg(info, n));
}

void
rb_gi_callable_info_init(VALUE rb_mGI, VALUE rb_cGIBaseInfo)
{
    VALUE RG_TARGET_NAMESPACE;

    RG_TARGET_NAMESPACE =
        G_DEF_CLASS_WITH_PARENT(GI_TYPE_CALLABLE_INFO, "CallableInfo", rb_mGI,
                                rb_cGIBaseInfo);

    RG_DEF_METHOD(set_lock_gvl_default, 1);
    RG_DEF_METHOD(add_lock_gvl_predicate, 0);
    RG_DEF_METHOD_P(lock_gvl, -1);

    RG_DEF_METHOD_P(can_throw_gerror, 0);
    RG_DEF_METHOD(return_type, 0);
    RG_DEF_METHOD(caller_owns, 0);
    RG_DEF_METHOD_P(may_return_null, 0);
    RG_DEF_METHOD(n_args, 0);
    RG_DEF_METHOD(get_arg, 1);

    rb_gi_function_info_init(rb_mGI, RG_TARGET_NAMESPACE);
    rb_gi_callback_info_init(rb_mGI, RG_TARGET_NAMESPACE);
    rb_gi_signal_info_init(rb_mGI, RG_TARGET_NAMESPACE);
    rb_gi_vfunc_info_init(rb_mGI, RG_TARGET_NAMESPACE);
}
