/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtkspinbutton.c -

  $Author: mutoh $
  $Date: 2002/06/22 19:50:57 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

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

    if (!NIL_P(arg1)) adj = GTK_ADJUSTMENT(RVAL2GOBJ(arg1));
    climb_rate = (NIL_P(arg2))? 0.0: NUM2DBL(arg2);
    digits     = (NIL_P(arg3))?   0: NUM2INT(arg3);

    widget = gtk_spin_button_new(adj, climb_rate, digits);

    RBGTK_INITIALIZE(self, widget);
    return Qnil;
}
static VALUE
sbtn_set_adjustment(self,adj)
    VALUE self, adj;
{
    gtk_spin_button_set_adjustment(GTK_SPIN_BUTTON(RVAL2GOBJ(self))
				   ,GTK_ADJUSTMENT(RVAL2GOBJ(adj)));
    return self;
}
static VALUE
sbtn_get_adjustment(self)
    VALUE self;
{
    return GOBJ2RVAL(gtk_spin_button_get_adjustment(
                         GTK_SPIN_BUTTON(RVAL2GOBJ(self))));
}
static VALUE
sbtn_digits(self)
    VALUE self;
{
    return INT2NUM(GTK_SPIN_BUTTON(RVAL2GOBJ(self))->digits);
}
static VALUE
sbtn_set_digits(self,n)
    VALUE self, n;
{
    (void)gtk_spin_button_set_digits(GTK_SPIN_BUTTON(RVAL2GOBJ(self))
				     , NUM2INT(n));
    return self;
}
static VALUE
sbtn_get_value_as_float(self)
    VALUE self;
{
    float w;
    w = gtk_spin_button_get_value_as_float(GTK_SPIN_BUTTON(RVAL2GOBJ(self)));
    return rb_float_new(w);
}
static VALUE
sbtn_get_value_as_int(self)
    VALUE self;
{
    int w;
    w = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(RVAL2GOBJ(self)));
    return INT2NUM(w);
}
static VALUE
sbtn_set_value(self,n)
    VALUE self, n;
{
    (void)gtk_spin_button_set_value(GTK_SPIN_BUTTON(RVAL2GOBJ(self))
				    , NUM2DBL(n));
    return self;
}
static VALUE
sbtn_set_numeric(self,n)
    VALUE self, n;
{
    (void)gtk_spin_button_set_numeric(GTK_SPIN_BUTTON(RVAL2GOBJ(self))
				      , RTEST(n));
    return self;
}
static VALUE
sbtn_set_update_policy(self,w)
    VALUE self, w;
{
    (void)gtk_spin_button_set_update_policy(GTK_SPIN_BUTTON(RVAL2GOBJ(self))
					    ,NUM2INT(w) );
    return self;
}
static VALUE
sbtn_spin(self,n,m)
    VALUE self, n, m;
{
    (void)gtk_spin_button_spin(GTK_SPIN_BUTTON(RVAL2GOBJ(self))
			       ,NUM2INT(n)
			       ,NUM2DBL(m));
    return self;
}
static VALUE
sbtn_set_wrap(self,n)
    VALUE self, n;
{
    (void)gtk_spin_button_set_wrap(GTK_SPIN_BUTTON(RVAL2GOBJ(self)),
				   RTEST(n));
    return self;
}
static VALUE
sbtn_set_snap_to_ticks(self,n)
    VALUE self, n;
{
    (void)gtk_spin_button_set_snap_to_ticks(GTK_SPIN_BUTTON(RVAL2GOBJ(self)),
					    RTEST(n));
    return self;
}

void Init_gtk_spin_button()
{
    static rbgtk_class_info cinfo;

    gSButton = rb_define_class_under(mGtk, "SpinButton", gEntry);
    cinfo.klass = gSButton;
    cinfo.gtype = GTK_TYPE_SPIN_BUTTON;
    cinfo.mark = 0;
    cinfo.free = 0;
    rbgtk_register_class(&cinfo);

    /*
     * instance methods
     */
    rb_define_method(gSButton, "initialize",        sbtn_initialize, -1);
    rb_define_method(gSButton, "set_adjustment",    sbtn_set_adjustment,1);
    rb_define_method(gSButton, "get_adjustment",    sbtn_get_adjustment,0);
    rb_define_method(gSButton, "digits",            sbtn_digits, 0);
    rb_define_method(gSButton, "set_digits",        sbtn_set_digits, 1);
    rb_define_method(gSButton, "get_value_as_float",sbtn_get_value_as_float,0);
    rb_define_method(gSButton, "get_value_as_int",  sbtn_get_value_as_int, 0);
    rb_define_method(gSButton, "set_value",         sbtn_set_value, 1);
    rb_define_method(gSButton, "set_numeric",       sbtn_set_numeric, 1);
    rb_define_method(gSButton, "set_update_policy", sbtn_set_update_policy,1);
    rb_define_method(gSButton, "spin",              sbtn_spin,2);
    rb_define_method(gSButton, "set_wrap",          sbtn_set_wrap,1);
    rb_define_method(gSButton, "set_snap_to_ticks", sbtn_set_snap_to_ticks, 1);
    /*
      rb_define_method(gSButton, "get_type",          sbtn_get_type,0);
      rb_define_method(gSButton, "construct",         sbtn_construct,4);
    */

    /*
     * constants
     */
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
