/* $Id: rbgnome-calculator.c,v 1.2 2002/05/19 15:48:28 mutoh Exp $ */

/* Gnome::Calculator widget for Ruby/Gnome
 * Copyright (C) 2001 Neil Conway <neilconway@rogers.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include "rbgnome.h"

VALUE gnoCalculator;

static VALUE
calc_initialize(self)
    VALUE self;
{
    GtkWidget *calc;
    calc = gnome_calculator_new();
    set_widget(self, calc);
    return Qnil;
}

static VALUE
calc_clear(self, reset)
    VALUE self, reset;
{
    gnome_calculator_clear(GNOME_CALCULATOR(get_widget(self)),
			   RTEST(reset));
    return self;
}

static VALUE
calc_set(self, result)
    VALUE self, result;
{
    gnome_calculator_set(GNOME_CALCULATOR(get_widget(self)),
			 NUM2DBL(result));
    return self;
}

static VALUE
calc_get_result(self)
    VALUE self;
{
    return rb_float_new(GNOME_CALCULATOR(get_widget(self))->result);
}

void Init_gnome_calculator()
{
    gnoCalculator = rb_define_class_under(mGnome, "Calculator", gVBox);

    /* GnomeCalculatorMode */
    rb_define_const(gnoCalculator, "DEG", INT2FIX(GNOME_CALCULATOR_DEG));
    rb_define_const(gnoCalculator, "RAD", INT2FIX(GNOME_CALCULATOR_RAD));
    rb_define_const(gnoCalculator, "GRAD", INT2FIX(GNOME_CALCULATOR_GRAD));

    /* Instance methods */
    rb_define_method(gnoCalculator, "initialize", calc_initialize, 0);
    rb_define_method(gnoCalculator, "clear", calc_clear, 1);
    rb_define_method(gnoCalculator, "set", calc_set, 1);
    rb_define_method(gnoCalculator, "result", calc_get_result, 0);

    /* Signals */
    rb_define_const(gnoCalculator, "SIGNAL_RESULT_CHANGED",
		    rb_str_new2("result_changed"));
}
