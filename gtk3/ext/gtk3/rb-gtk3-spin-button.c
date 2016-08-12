/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2016  Ruby-GNOME2 Project Team
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

#include "rb-gtk3-private.h"

typedef struct {
    RGClosureCallData *data;
    VALUE args;
    double *new_value;
} InputData;

static VALUE
rb_gtk3_spin_button_input_body(VALUE user_data)
{
    InputData *input_data = (InputData *)user_data;
    VALUE new_value;

    new_value = rb_apply(input_data->data->callback,
                         rb_intern("call"),
                         input_data->args);
    if (NIL_P(new_value)) {
        g_value_set_int(input_data->data->return_value, FALSE);
    } else {
        g_value_set_int(input_data->data->return_value, TRUE);
        *(input_data->new_value) = NUM2DBL(new_value);
    }

    return Qnil;
}

static VALUE
rb_gtk3_spin_button_input_rescue(VALUE user_data)
{
    InputData *input_data = (InputData *)user_data;
    g_value_set_int(input_data->data->return_value, GTK_INPUT_ERROR);
    return Qnil;
}

static void
rb_gtk3_spin_button_input(RGClosureCallData *data)
{
    InputData input_data;
    VALUE args;

    args = rb_ary_new_from_args(1, GVAL2RVAL(&(data->param_values[0])));
    if (!NIL_P(data->extra_args)) {
        rb_ary_concat(args, data->extra_args);
    }
    input_data.data = data;
    input_data.args = args;
    input_data.new_value = g_value_get_pointer(&(data->param_values[1]));
    rb_rescue(rb_gtk3_spin_button_input_body, (VALUE)&input_data,
              rb_gtk3_spin_button_input_rescue, (VALUE)&input_data);
}

void
rbgtk3_spin_button_init(void)
{
    VALUE mGtk;
    VALUE RG_TARGET_NAMESPACE;

    mGtk = rb_const_get(rb_cObject, rb_intern("Gtk"));
    RG_TARGET_NAMESPACE = rb_const_get(mGtk, rb_intern("SpinButton"));

    rbgobj_set_signal_call_func(RG_TARGET_NAMESPACE,
                                "input",
                                rb_gtk3_spin_button_input);
}
