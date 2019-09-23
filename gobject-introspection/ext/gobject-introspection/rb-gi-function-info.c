/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2012-2019  Ruby-GNOME Project Team
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

#define RG_TARGET_NAMESPACE rb_cGIFunctionInfo
#define SELF(self) RVAL2GI_FUNCTION_INFO(self)

static VALUE RG_TARGET_NAMESPACE;

GType
gi_function_info_get_type(void)
{
    static GType type = 0;
    if (type == 0) {
        type = g_boxed_type_register_static("GIFunctionInfo",
                                            (GBoxedCopyFunc)g_base_info_ref,
                                            (GBoxedFreeFunc)g_base_info_unref);
    }
    return type;
}

static VALUE
rg_set_unlock_gvl(VALUE self, VALUE rb_boolean)
{
    return rb_iv_set(self, "unlock_gvl", rb_boolean);
}

static VALUE
rg_unlock_gvl_p(VALUE self)
{
    if (!RVAL2CBOOL(rb_ivar_defined(self, rb_intern("unlock_gvl")))) {
        rb_iv_set(self, "unlock_gvl", Qfalse);
    }
    return rb_iv_get(self, "unlock_gvl");
}

static VALUE
rg_symbol(VALUE self)
{
    GIFunctionInfo *info;

    info = SELF(self);
    return CSTR2RVAL(g_function_info_get_symbol(info));
}

static VALUE
rg_flags(VALUE self)
{
    GIFunctionInfo *info;

    info = SELF(self);
    return GI_FUNCTION_INFO_FLAGS2RVAL(g_function_info_get_flags(info));
}

static VALUE
rg_property(VALUE self)
{
    GIFunctionInfo *info;

    info = SELF(self);
    return GI_BASE_INFO2RVAL(g_function_info_get_property(info));
}

static VALUE
rg_vfunc(VALUE self)
{
    GIFunctionInfo *info;

    info = SELF(self);
    return GI_BASE_INFO2RVAL(g_function_info_get_vfunc(info));
}

typedef struct {
    GIFunctionInfo *info;
    GArray *in_args;
    GArray *out_args;
    GIArgument return_value;
    GError **error;
    gboolean succeeded;
} InvokeData;

static void
rb_gi_function_info_invoke_raw_call(InvokeData *data)
{
    data->succeeded =
        g_function_info_invoke(data->info,
                               (GIArgument *)((void *)(data->in_args->data)),
                               data->in_args->len,
                               (GIArgument *)((void *)(data->out_args->data)),
                               data->out_args->len,
                               &(data->return_value),
                               data->error);
}

static RB_THREAD_CALL_WITHOUT_GVL_FUNC_RETURN_TYPE
rb_gi_function_info_invoke_raw_call_without_gvl_body(void *user_data)
{
    InvokeData *data = (InvokeData *)user_data;

    rb_gi_function_info_invoke_raw_call(data);

    return RB_THREAD_CALL_WITHOUT_GVL_FUNC_RETURN_VALUE;
}

VALUE
rb_gi_function_info_invoke_raw(GIFunctionInfo *info,
                               VALUE rb_info,
                               VALUE rb_receiver,
                               VALUE rb_arguments,
                               GIArgument *return_value,
                               VALUE *rb_return_value)
{
    GICallableInfo *callable_info;
    RBGIArguments args;
    VALUE rb_out_args = Qnil;
    gboolean succeeded;
    GError *error = NULL;
    gboolean unlock_gvl = FALSE;

    unlock_gvl = RVAL2CBOOL(rb_funcall(rb_info, rb_intern("unlock_gvl?"), 0));
    rb_arguments = rbg_to_array(rb_arguments);

    callable_info = (GICallableInfo *)info;
    rb_gi_arguments_init(&args,
                         callable_info,
                         rb_receiver,
                         rb_arguments,
                         NULL);
    {
        InvokeData data;
        data.info = info;
        data.in_args = args.in_args;
        data.out_args = args.out_args;
        data.error = &error;
        if (unlock_gvl) {
            rb_thread_call_without_gvl(
                rb_gi_function_info_invoke_raw_call_without_gvl_body, &data,
                NULL, NULL);
        } else {
            rb_gi_function_info_invoke_raw_call(&data);
        }
        succeeded = data.succeeded;

        if (return_value) {
            *return_value = data.return_value;
        }
        if (rb_return_value) {
            if (succeeded) {
                *rb_return_value =
                    rb_gi_arguments_get_rb_return_value(&args,
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

static VALUE
rg_invoke(VALUE self, VALUE rb_arguments)
{
    GIFunctionInfo *info;
    VALUE rb_out_args;
    VALUE rb_return_value;

    info = SELF(self);
    /* TODO: use rb_protect() */
    rb_out_args = rb_gi_function_info_invoke_raw(info,
                                                 self,
                                                 Qnil,
                                                 rb_arguments,
                                                 NULL,
                                                 &rb_return_value);

    if (NIL_P(rb_out_args)) {
        return rb_return_value;
    } else {
        GICallableInfo *callable_info = (GICallableInfo *)info;
        GITypeInfo return_value_info;
        g_callable_info_load_return_type(callable_info, &return_value_info);
        if (g_type_info_get_tag(&return_value_info) != GI_TYPE_TAG_VOID) {
            rb_ary_unshift(rb_out_args, rb_return_value);
        }
        if (RARRAY_LEN(rb_out_args) == 1) {
            return RARRAY_PTR(rb_out_args)[0];
        } else {
            return rb_out_args;
        }
    }
}

void
rb_gi_function_info_init(VALUE rb_mGI, VALUE rb_cGICallableInfo)
{
    RG_TARGET_NAMESPACE =
	G_DEF_CLASS_WITH_PARENT(GI_TYPE_FUNCTION_INFO, "FunctionInfo", rb_mGI,
				rb_cGICallableInfo);

    RG_DEF_METHOD(set_unlock_gvl, 1);
    RG_DEF_METHOD_P(unlock_gvl, 0);

    RG_DEF_METHOD(symbol, 0);
    RG_DEF_METHOD(flags, 0);
    RG_DEF_METHOD(property, 0);
    RG_DEF_METHOD(vfunc, 0);
    RG_DEF_METHOD(invoke, 1);

    G_DEF_CLASS(G_TYPE_I_FUNCTION_INFO_FLAGS, "FunctionInfoFlags", rb_mGI);

    rb_gi_method_info_init(rb_mGI, RG_TARGET_NAMESPACE);
    rb_gi_constructor_info_init(rb_mGI, RG_TARGET_NAMESPACE);
}
