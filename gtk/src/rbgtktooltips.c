/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtktooltips.c -

  $Author: mutoh $
  $Date: 2002/08/29 13:07:01 $

  Copyright (C) 2002 Masao Mutoh

  This file is derived from rbgtkdata.c.
  rbgtkdata.c -
  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

static VALUE
ttips_initialize(self)
    VALUE self;
{
    RBGTK_INITIALIZE(self, gtk_tooltips_new());
    return Qnil;
}

static VALUE
ttips_set_tip(self, win, text, priv)
    VALUE self, win, text, priv;
{
    gtk_tooltips_set_tip(GTK_TOOLTIPS(RVAL2GOBJ(self)),
			 GTK_WIDGET(RVAL2GOBJ(win)),
			 NIL_P(text)?NULL:STR2CSTR(text),
			 NIL_P(priv)?NULL:STR2CSTR(priv));

    return self;
}

static VALUE
ttips_set_delay(self, delay)
    VALUE self, delay;
{
    gtk_tooltips_set_delay(GTK_TOOLTIPS(RVAL2GOBJ(self)), NUM2INT(delay));

    return self;
}

static VALUE
ttips_enable(self)
    VALUE self;
{
    gtk_tooltips_enable(GTK_TOOLTIPS(RVAL2GOBJ(self)));
    return self;
}

static VALUE
ttips_disable(self)
    VALUE self;
{
    gtk_tooltips_disable(GTK_TOOLTIPS(RVAL2GOBJ(self)));
    return self;
}

void 
Init_gtk_tooltips()
{
    VALUE gTooltips = G_DEF_CLASS(GTK_TYPE_TOOLTIPS, "Tooltips", mGtk);

    rb_define_method(gTooltips, "initialize", ttips_initialize, 0);
    rb_define_method(gTooltips, "set_tip", ttips_set_tip, 3);
    rb_define_method(gTooltips, "set_delay", ttips_set_delay, 1);
    rb_define_method(gTooltips, "enable", ttips_enable, 0);
    rb_define_method(gTooltips, "disable", ttips_disable, 0);
}
