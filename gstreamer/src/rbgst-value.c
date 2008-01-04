/*
 * Copyright (C) 2008 Ruby-GNOME2 Project Team
 *
 * This file is part of Ruby/GStreamer.
 *
 * Ruby/GStreamer is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * Ruby/GStreamer is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with Ruby/GStreamer; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307  USA
 */

#include "rbgst.h"

static RGConvertTable value_list_table = {0};

static VALUE
value_list_get_superclass(void)
{
    return rb_cObject;
}

static void
value_list_rvalue2gvalue(VALUE value, GValue *result)
{
    guint i, len;

    len = RARRAY_LEN(value);
    for (i = 0; i < len; i++) {
        GValue val = {0};
        rbgobj_initialize_gvalue(&val, RARRAY_PTR(value)[i]);
        gst_value_list_append_value(result, &val);
        g_value_unset(&val);
    }
}

static VALUE
value_list_gvalue2rvalue(const GValue *value)
{
    guint i, len;
    VALUE result;

    len = gst_value_list_get_size(value);
    result = rb_ary_new2(len);
    for (i = 0; i < len; i++) {
        rb_ary_push(result, GVAL2RVAL(gst_value_list_get_value(value, i)));
    }

    return result;
}

void
Init_gst_value(void)
{
    value_list_table.type = GST_TYPE_LIST;
    value_list_table.get_superclass = value_list_get_superclass;
    value_list_table.type_init_hook = NULL;
    value_list_table.rvalue2gvalue = value_list_rvalue2gvalue;
    value_list_table.gvalue2rvalue = value_list_gvalue2rvalue;
    value_list_table.initialize = NULL;
    value_list_table.robj2instance = NULL;
    value_list_table.instance2robj = NULL;
    value_list_table.unref = NULL;

    RG_DEF_CONVERSION(&value_list_table);
}
