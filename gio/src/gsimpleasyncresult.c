/* -*- c-file-style: "ruby" -*- */
/*
 * Ruby/GIO: a Ruby binding of gio-2.0.x.
 * Copyright (C) 2008-2009  Ruby-GNOME2 Project Team
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "gio2.h"

#define _SELF(value) G_SIMPLE_ASYNC_RESULT(RVAL2GOBJ(value))

static VALUE
simpleasyncresult_initialize(VALUE self, VALUE object)
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

/* NOTE: Can’t implement g_simple_async_result_new_error. */

/* NOTE: Can’t implement g_simple_async_result_new_from_error. */

/* NOTE: Can’t implement g_simple_async_result_set_op_res_gpointer. */

/* NOTE: Can’t implement g_simple_async_result_get_op_res_gpointer. */

static VALUE
simpleasyncresult_set_op_res_gssize(VALUE self, VALUE op_res)
{
        g_simple_async_result_set_op_res_gssize(_SELF(self),
                                                RVAL2GSSIZE(op_res));

        return self;
}

static VALUE
simpleasyncresult_get_op_res_gssize(VALUE self)
{
        return GSSIZE2RVAL(g_simple_async_result_get_op_res_gssize(_SELF(self)));
}

static VALUE
simpleasyncresult_set_op_res_gboolean(VALUE self, VALUE op_res)
{
        g_simple_async_result_set_op_res_gboolean(_SELF(self),
                                                  RVAL2CBOOL(op_res));

        return self;
}

static VALUE
simpleasyncresult_get_op_res_gboolean(VALUE self)
{
        return CBOOL2RVAL(g_simple_async_result_get_op_res_gboolean(_SELF(self)));
}

static VALUE
simpleasyncresult_get_source_tag(VALUE self)
{
        return (VALUE)g_simple_async_result_get_source_tag(_SELF(self));
}

static VALUE
simpleasyncresult_is_valid(VALUE self, VALUE source, VALUE source_tag)
{
        return CBOOL2RVAL(g_simple_async_result_is_valid(RVAL2GASYNCRESULT(self),
                                                         RVAL2GOBJ(source),
                                                         (gpointer)source_tag));
}

static VALUE
simpleasyncresult_set_handle_cancellation(VALUE self, VALUE handle_cancellation)
{
        g_simple_async_result_set_handle_cancellation(_SELF(self),
                                                      RVAL2CBOOL(handle_cancellation));

        return self;
}

static VALUE
simpleasyncresult_complete(VALUE self)
{
        g_simple_async_result_complete(_SELF(self));

        return self;
}

static VALUE
simpleasyncresult_complete_in_idle(VALUE self)
{
        g_simple_async_result_complete_in_idle(_SELF(self));

        return self;
}

/* TODO: Should we implement g_simple_async_result_run_in_thread?  I’d say no,
 * but I’m not sure. */

/* NOTE: Can’t implement g_simple_async_result_set_from_error. */

/* NOTE: Can’t implement g_simple_async_result_propagate_error. */

/* NOTE: Can’t implement g_simple_async_result_set_error. */

/* NOTE: Can’t implement g_simple_async_result_set_error_va. */

/* NOTE: Can’t implement g_simple_async_result_report_error_in_idle. */

/* NOTE: Can’t implement g_simple_async_result_report_gerror_in_idle. */

void
Init_gsimpleasyncresult(VALUE glib)
{
        VALUE simpleasyncresult = G_DEF_CLASS(G_TYPE_SIMPLE_ASYNC_RESULT, "SimpleAsyncResult", glib);

        rb_define_method(simpleasyncresult, "initialize", simpleasyncresult_initialize, 1);
        rb_define_method(simpleasyncresult, "set_op_res_gssize", simpleasyncresult_set_op_res_gssize, 1);
        G_DEF_SETTER(simpleasyncresult, "op_res_gssize");
        rb_define_method(simpleasyncresult, "op_res_gssize", simpleasyncresult_get_op_res_gssize, 0);
        rb_define_method(simpleasyncresult, "set_op_res_gboolean", simpleasyncresult_set_op_res_gboolean, 1);
        G_DEF_SETTER(simpleasyncresult, "op_res_gboolean");
        rb_define_method(simpleasyncresult, "op_res_gboolean", simpleasyncresult_get_op_res_gboolean, 0);
        rb_define_method(simpleasyncresult, "source_tag", simpleasyncresult_get_source_tag, 0);
        rb_define_method(simpleasyncresult, "valid?", simpleasyncresult_is_valid, 2);
        rb_define_method(simpleasyncresult, "set_handle_cancellation", simpleasyncresult_set_handle_cancellation, 1);
        G_DEF_SETTER(simpleasyncresult, "handle_cancellation");
        rb_define_method(simpleasyncresult, "complete", simpleasyncresult_complete, 0);
        rb_define_method(simpleasyncresult, "complete_in_idle", simpleasyncresult_complete_in_idle, 0);
}
