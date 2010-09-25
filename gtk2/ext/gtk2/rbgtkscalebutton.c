/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkscalebutton.c -

  $Author: ggc $
  $Date: 2007/07/12 14:43:11 $

  Copyright (C) 2002,2003 Ruby-GNOME2 Project Team
  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

#if GTK_CHECK_VERSION(2,12,0)

#define _SELF(s) (GTK_SCALE_BUTTON(RVAL2GOBJ(s)))

static VALUE
scalebutton_set_icons(VALUE self, VALUE icons);

static VALUE
scalebutton_initialize(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE arg1, arg2, arg3, arg4, arg5;
    GtkWidget *widget;
    gdouble min = 0;
    gdouble max = 100;
    gdouble step = 2;

    rb_scan_args(argc, argv, "14", &arg1, &arg2, &arg3, &arg4, &arg5);

    if (!NIL_P(arg2))
        min = NUM2DBL(arg2);

    if (!NIL_P(arg3))
        max = NUM2DBL(arg3);

    if (!NIL_P(arg4))
        step = NUM2DBL(arg4);

    widget = gtk_scale_button_new(RVAL2GENUM(arg1, GTK_TYPE_ICON_SIZE), min, max, step, NULL);
    RBGTK_INITIALIZE(self, widget);

    if (!NIL_P(arg5))
        scalebutton_set_icons(self, arg5);

    return Qnil;
}

static VALUE
scalebutton_set_adjustment(self, adjustment)
    VALUE self, adjustment;
{
    gtk_scale_button_set_adjustment(_SELF(self), RVAL2GOBJ(adjustment));
    return self;
}

static VALUE
scalebutton_set_icons(self, icons)
    VALUE self, icons;
{
    int i;
    gchar **icons_c = g_new0(gchar*, RARRAY_LEN(icons) + 1);
    for (i = 0; i < RARRAY_LEN(icons); i++)
        icons_c[i] = RVAL2CSTR(RARRAY_PTR(icons)[i]);
    icons_c[i] = NULL;
    gtk_scale_button_set_icons(_SELF(self), (const gchar **) icons_c);
    g_free(icons_c);
    return self;
}

static VALUE
scalebutton_set_value(self, value)
    VALUE self, value;
{
    gtk_scale_button_set_value(_SELF(self), NUM2DBL(value));
    return self;
}

static VALUE
scalebutton_get_adjustment(self)
    VALUE self;
{
    return GOBJ2RVAL(gtk_scale_button_get_adjustment(_SELF(self)));
}

static VALUE
scalebutton_get_value(self)
    VALUE self;
{
    return rb_float_new(gtk_scale_button_get_value(_SELF(self)));
}

#endif

void 
Init_gtk_scalebutton()
{
#if GTK_CHECK_VERSION(2,12,0)
    VALUE gScaleButton = G_DEF_CLASS(GTK_TYPE_SCALE_BUTTON, "ScaleButton", mGtk);
    rb_define_method(gScaleButton, "initialize", scalebutton_initialize, -1);
    rb_define_method(gScaleButton, "set_adjustment", scalebutton_set_adjustment, 1);
    rb_define_method(gScaleButton, "set_icons", scalebutton_set_icons, 1);
    rb_define_method(gScaleButton, "set_value", scalebutton_set_value, 1);
    rb_define_method(gScaleButton, "adjustment", scalebutton_get_adjustment, 0);
    rb_define_method(gScaleButton, "value", scalebutton_get_value, 0);
    G_DEF_SETTERS(gScaleButton);
#endif
}
