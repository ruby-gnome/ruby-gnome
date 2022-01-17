/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2013-2019  Ruby-GNOME Project Team
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

#include "rbgst.h"

static void
rg_gst_value_list_r2g(VALUE from, GValue *to)
{
    int i, n;

    if (NIL_P(from)) {
        return;
    }

    from = rbg_to_array(from);
    n = RARRAY_LEN(from);
    for (i = 0; i < n; i++) {
        VALUE rb_element;
        const RGObjClassInfo *class_info;
        GValue element = G_VALUE_INIT;

        rb_element = rb_ary_entry(from, i);
        class_info = RVAL2CINFO(rb_element);
        g_value_init(&element, class_info->gtype);
        rbgobj_rvalue_to_gvalue(rb_element, &element);
        gst_value_list_append_value(to, &element);
    }
}

static VALUE
rg_gst_value_list_g2r(const GValue *from)
{
    guint i, n;
    VALUE rb_values;

    n = gst_value_list_get_size(from);
    rb_values = rb_ary_new();
    for (i = 0; i < n; i++) {
        const GValue *value;
        VALUE rb_value;

        value = gst_value_list_get_value(from, i);
        rb_value = rbgobj_gvalue_to_rvalue(value);
        rb_ary_push(rb_values, rb_value);
    }

    return rb_values;
}

void
Init_gstreamer (void)
{
    rbgobj_register_r2g_func(GST_TYPE_LIST, rg_gst_value_list_r2g);
    rbgobj_register_g2r_func(GST_TYPE_LIST, rg_gst_value_list_g2r);

    rb_gst_init_child_proxy();
}
