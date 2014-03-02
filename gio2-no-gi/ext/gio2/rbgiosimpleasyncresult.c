/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2008-2009  Ruby-GNOME2 Project Team
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

#include "rbgio2private.h"

#define RG_TARGET_NAMESPACE cSimpleAsyncResult
#define _SELF(value) RVAL2GSIMPLEASYNCRESULT(value)

static VALUE
rg_initialize(VALUE self, VALUE object)
{
        VALUE block;

        /* TODO: I have absolutely no idea if this makes sense.  Should we even
         * be providing this method? */
        block = rb_block_proc();
        SAVE_BLOCK(block);
        G_INITIALIZE(self,
                     g_simple_async_result_new(RVAL2GOBJ(object),
                                               rbgio_async_ready_callback,
                                               (gpointer)block,
                                               (gpointer)block));

        return Qnil;
}

/* NOTE: Can't implement g_simple_async_result_new_error. */

/* NOTE: Can't implement g_simple_async_result_new_from_error. */

/* NOTE: Can't implement g_simple_async_result_set_op_res_gpointer. */

/* NOTE: Can't implement g_simple_async_result_get_op_res_gpointer. */

static VALUE
rg_set_op_res_gssize(VALUE self, VALUE op_res)
{
        g_simple_async_result_set_op_res_gssize(_SELF(self),
                                                RVAL2GSSIZE(op_res));

        return self;
}

static VALUE
rg_op_res_gssize(VALUE self)
{
        return GSSIZE2RVAL(g_simple_async_result_get_op_res_gssize(_SELF(self)));
}

static VALUE
rg_set_op_res_gboolean(VALUE self, VALUE op_res)
{
        g_simple_async_result_set_op_res_gboolean(_SELF(self),
                                                  RVAL2CBOOL(op_res));

        return self;
}

static VALUE
rg_op_res_gboolean(VALUE self)
{
        return CBOOL2RVAL(g_simple_async_result_get_op_res_gboolean(_SELF(self)));
}

static VALUE
rg_source_tag(VALUE self)
{
        return (VALUE)g_simple_async_result_get_source_tag(_SELF(self));
}

static VALUE
rg_valid_p(VALUE self, VALUE source, VALUE source_tag)
{
        return CBOOL2RVAL(g_simple_async_result_is_valid(RVAL2GASYNCRESULT(self),
                                                         RVAL2GOBJ(source),
                                                         (gpointer)source_tag));
}

static VALUE
rg_set_handle_cancellation(VALUE self, VALUE handle_cancellation)
{
        g_simple_async_result_set_handle_cancellation(_SELF(self),
                                                      RVAL2CBOOL(handle_cancellation));

        return self;
}

static VALUE
rg_complete(VALUE self)
{
        g_simple_async_result_complete(_SELF(self));

        return self;
}

static VALUE
rg_complete_in_idle(VALUE self)
{
        g_simple_async_result_complete_in_idle(_SELF(self));

        return self;
}

/* TODO: Should we implement g_simple_async_result_run_in_thread?  I'd say no,
 * but I'm not sure. */

/* NOTE: Can't implement g_simple_async_result_set_from_error. */

/* NOTE: Can't implement g_simple_async_result_propagate_error. */

/* NOTE: Can't implement g_simple_async_result_set_error. */

/* NOTE: Can't implement g_simple_async_result_set_error_va. */

/* NOTE: Can't implement g_simple_async_result_report_error_in_idle. */

/* NOTE: Can't implement g_simple_async_result_report_gerror_in_idle. */

void
Init_gsimpleasyncresult(VALUE mGio)
{
        VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(G_TYPE_SIMPLE_ASYNC_RESULT, "SimpleAsyncResult", mGio);

        RG_DEF_METHOD(initialize, 1);
        RG_DEF_METHOD(set_op_res_gssize, 1);
        RG_DEF_METHOD(op_res_gssize, 0);
        RG_DEF_METHOD(set_op_res_gboolean, 1);
        RG_DEF_METHOD(op_res_gboolean, 0);
        RG_DEF_METHOD(source_tag, 0);
        RG_DEF_METHOD_P(valid, 2);
        RG_DEF_METHOD(set_handle_cancellation, 1);
        RG_DEF_METHOD(complete, 0);
        RG_DEF_METHOD(complete_in_idle, 0);
}
