/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkspinbutton.c -

  $Author: mutoh $
  $Date: 2003/02/01 16:46:24 $

  Copyright (C) 2002,2003 Ruby-GNOME2 Project Team
  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

#define _SELF(self) (GTK_SPIN_BUTTON(RVAL2GOBJ(self)))

static VALUE
sbtn_initialize(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE arg1, arg2, arg3;
    GtkAdjustment *adj = NULL;
    gfloat climb_rate;
    gint digits;
    GtkWidget *widget;

    rb_scan_args(argc, argv, "03", &arg1, &arg2, &arg3);

    if (NIL_P(arg1) || RVAL2GTYPE(arg1) == GTK_TYPE_ADJUSTMENT){
        if (!NIL_P(arg1)) adj = GTK_ADJUSTMENT(RVAL2GOBJ(arg1));
        climb_rate = (NIL_P(arg2))? 0.0: NUM2DBL(arg2);
        digits     = (NIL_P(arg3))?   0: NUM2UINT(arg3);
        widget = gtk_spin_button_new(adj, climb_rate, digits);
    } else {
        widget = gtk_spin_button_new_with_range(NUM2DBL(arg1), 
                                                NUM2DBL(arg2), NUM2DBL(arg3));
    }
    RBGTK_INITIALIZE(self, widget);
    return Qnil;
}

static VALUE
sbtn_configure(self, adj, climb_rate, digits)
    VALUE self, adj, climb_rate, digits;
{
    gtk_spin_button_configure(_SELF(self), GTK_ADJUSTMENT(RVAL2GOBJ(adj)),
                              NUM2DBL(climb_rate), NUM2UINT(digits));
    return self;
}

static VALUE
sbtn_set_increments(self, step, page)
    VALUE self, step, page;
{
    gtk_spin_button_set_increments(_SELF(self), NUM2DBL(step), NUM2DBL(page));
    return self;
}

static VALUE
sbtn_set_range(self, min, max)
    VALUE self, min, max;
{
    gtk_spin_button_set_range(_SELF(self), NUM2DBL(min), NUM2DBL(max));
    return self;
}


static VALUE
sbtn_get_value_as_int(self)
    VALUE self;
{
    return INT2NUM(gtk_spin_button_get_value_as_int(_SELF(self)));
}

static VALUE
sbtn_spin(self, direction, increment)
    VALUE self, direction, increment;
{
    gtk_spin_button_spin(_SELF(self), NUM2INT(direction), NUM2DBL(increment));
    return self;
}

static VALUE
sbtn_update(self)
    VALUE self;
{
    gtk_spin_button_update(_SELF(self));
    return self;
}

static VALUE
sbtn_get_increments(self)
    VALUE self;
{
    gdouble step, page;
    gtk_spin_button_get_increments(_SELF(self), &step, &page);

    return rb_ary_new3(2, rb_float_new(step), rb_float_new(page));
}

static VALUE
sbtn_get_range(self)
    VALUE self;
{
    gdouble min, max;
    gtk_spin_button_get_increments(_SELF(self), &min, &max);

    return rb_ary_new3(2, rb_float_new(min), rb_float_new(max));
}

void 
Init_gtk_spin_button()
{
    VALUE gSButton = G_DEF_CLASS(GTK_TYPE_SPIN_BUTTON, "SpinButton", mGtk);

    rb_define_method(gSButton, "initialize", sbtn_initialize, -1);
    rb_define_method(gSButton, "configure", sbtn_configure, 3);
    rb_define_method(gSButton, "set_increments", sbtn_set_increments, 2);
    rb_define_method(gSButton, "set_range", sbtn_set_range, 2);
    rb_define_method(gSButton, "value_as_int", sbtn_get_value_as_int, 0);
    rb_define_method(gSButton, "spin", sbtn_spin, 2);
    rb_define_method(gSButton, "update", sbtn_update, 0);
    rb_define_method(gSButton, "increments", sbtn_get_increments, 0);
    rb_define_method(gSButton, "range", sbtn_get_range, 0);

    /* GtkSpinType */
    rb_define_const(gSButton, "STEP_FORWARD", INT2FIX(GTK_SPIN_STEP_FORWARD));
    rb_define_const(gSButton, "STEP_BACKWARD", INT2FIX(GTK_SPIN_STEP_BACKWARD));
    rb_define_const(gSButton, "PAGE_FORWARD", INT2FIX(GTK_SPIN_PAGE_FORWARD));
    rb_define_const(gSButton, "PAGE_BACKWARD", INT2FIX(GTK_SPIN_PAGE_BACKWARD));
    rb_define_const(gSButton, "HOME", INT2FIX(GTK_SPIN_HOME));
    rb_define_const(gSButton, "END", INT2FIX(GTK_SPIN_END));
    rb_define_const(gSButton, "USER_DEFINED", INT2FIX(GTK_SPIN_USER_DEFINED));

    /* GtkSpinButtonUpdatePolicy */
    rb_define_const(gSButton, "UPDATE_ALWAYS", INT2FIX(GTK_UPDATE_ALWAYS));
    rb_define_const(gSButton, "UPDATE_IF_VALID", INT2FIX(GTK_UPDATE_IF_VALID));
}
