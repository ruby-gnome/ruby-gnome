/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
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

#include "rbgtk3private.h"

struct add_buttons_args {
    VALUE self;
    VALUE buttons;
    RBGTKAddButtonFunc add_button;
};

static VALUE
add_buttons_body(VALUE value)
{
    struct add_buttons_args *args = (struct add_buttons_args *)value;
    VALUE button;
    long i;
    long n = RARRAY_LEN(args->buttons);

    for (i = 0; i < n; i++) {
        button = rb_ary_to_ary(RARRAY_PTR(args->buttons)[i]);

        args->add_button(args->self, RARRAY_PTR(button)[0], RARRAY_PTR(button)[1]);
    }

    return args->self;
}

static VALUE
add_buttons_ensure(VALUE value)
{
    g_object_thaw_notify(RVAL2GOBJ(((struct add_buttons_args *)value)->self));

    return Qnil;
}

VALUE
rbgtk_add_buttons(VALUE self, VALUE buttons, RBGTKAddButtonFunc add_button)
{
    struct add_buttons_args args = { self, buttons, add_button };

    if (NIL_P(RARRAY_PTR(buttons)[0]))
        return self;

    g_object_freeze_notify(RVAL2GOBJ(self));

    return rb_ensure(add_buttons_body, (VALUE)&args,
                     add_buttons_ensure, (VALUE)&args);
}

