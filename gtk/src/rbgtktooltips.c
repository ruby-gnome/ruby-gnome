/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtktooltips.c -

  $Author: mutoh $
  $Date: 2004/07/31 05:44:45 $

  Copyright (C) 2002-2004 Masao Mutoh

  This file is derived from rbgtkdata.c.
  rbgtkdata.c -
  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

#define _SELF(self) (GTK_TOOLTIPS(RVAL2GOBJ(self)))

static VALUE
ttips_initialize(self)
    VALUE self;
{
    RBGTK_INITIALIZE(self, gtk_tooltips_new());
    return Qnil;
}

static VALUE
ttips_enable(self)
    VALUE self;
{
    gtk_tooltips_enable(_SELF(self));
    return self;
}

static VALUE
ttips_disable(self)
    VALUE self;
{
    gtk_tooltips_disable(_SELF(self));
    return self;
}

static VALUE
ttips_set_tip(self, win, text, priv)
    VALUE self, win, text, priv;
{
    gtk_tooltips_set_tip(_SELF(self),
			 GTK_WIDGET(RVAL2GOBJ(win)),
			 NIL_P(text)?NULL:RVAL2CSTR(text),
			 NIL_P(priv)?NULL:RVAL2CSTR(priv));

    return self;
}

static VALUE
ttips_s_data_get(self, widget)
    VALUE self, widget;
{
    GtkTooltipsData* data = gtk_tooltips_data_get(GTK_WIDGET(RVAL2GOBJ(widget)));

    return rb_ary_new3(3, GOBJ2RVAL(data->widget), CSTR2RVAL(data->tip_text),
                       CSTR2RVAL(data->tip_private));
}

static VALUE
ttips_force_window(self)
    VALUE self;
{
    gtk_tooltips_force_window(_SELF(self));
    return self;
}

#if GTK_CHECK_VERSION(2,4,0)
static VALUE
ttips_s_get_info_from_tip_window(self, window)
    VALUE self, window;
{
    GtkTooltips* tooltips;
    GtkWidget* current_widget;
    gboolean ret = gtk_tooltips_get_info_from_tip_window(GTK_WINDOW(RVAL2GOBJ(window)),
                                                         &tooltips, 
                                                         &current_widget);
    if (ret)
        return rb_assoc_new(GOBJ2RVAL(tooltips), GOBJ2RVAL(current_widget));
    else
        return Qnil;
}
#endif

void 
Init_gtk_tooltips()
{
    VALUE gTooltips = G_DEF_CLASS(GTK_TYPE_TOOLTIPS, "Tooltips", mGtk);

    rb_define_method(gTooltips, "initialize", ttips_initialize, 0);
    rb_define_method(gTooltips, "set_tip", ttips_set_tip, 3);
    rb_define_method(gTooltips, "enable", ttips_enable, 0);
    rb_define_method(gTooltips, "disable", ttips_disable, 0);
    rb_define_method(gTooltips, "force_window", ttips_force_window, 0);
#if GTK_CHECK_VERSION(2,4,0)
    rb_define_singleton_method(gTooltips, "get_info", ttips_s_get_info_from_tip_window, 1);
#endif
    rb_define_singleton_method(gTooltips, "get_data", ttips_s_data_get, 1);
}
