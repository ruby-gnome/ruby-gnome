/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtktooltip.c -

  $Author: ktou $
  $Date: 2007/07/10 11:48:30 $

  Copyright (C) 2002-2006 Ruby-GNOME2 Project Team
  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

#if GTK_CHECK_VERSION(2,11,0)

/* GTK_TOOLTIP type-cast is private because it should be useless */
#define _SELF(self) (RVAL2GOBJ(self))

static VALUE
tooltip_set_markup(self, markup)
    VALUE self, markup;
{
    gtk_tooltip_set_markup(_SELF(self), RVAL2CSTR(markup));
    return self;
}

static VALUE
tooltip_set_text(self, text)
    VALUE self, text;
{
    gtk_tooltip_set_text(_SELF(self), RVAL2CSTR(text));
    return self;
}

static VALUE
tooltip_set_icon(self, icon)
    VALUE self, icon;
{
    gtk_tooltip_set_icon(_SELF(self), GDK_PIXBUF(RVAL2GOBJ(icon)));
    return self;
}

static VALUE
tooltip_set_icon_from_stock(self, stock_id, size)
    VALUE self, stock_id, size;
{
    gtk_tooltip_set_icon_from_stock(_SELF(self), rb_id2name(SYM2ID(stock_id)), RVAL2GENUM(size, GTK_TYPE_ICON_SIZE));
    return self;
}

static VALUE
tooltip_set_custom(self, custom_widget)
    VALUE self, custom_widget;
{
    gtk_tooltip_set_custom(_SELF(self), GTK_WIDGET(RVAL2GOBJ(custom_widget)));
    return self;
}

#endif

void 
Init_gtk_tooltip()
{
#if GTK_CHECK_VERSION(2,11,0)
    VALUE gTooltip = G_DEF_CLASS(GTK_TYPE_TOOLTIP, "Tooltip", mGtk);
    rb_define_method(gTooltip, "set_markup", tooltip_set_markup, 1);
    rb_define_method(gTooltip, "set_text", tooltip_set_text, 1);
    rb_define_method(gTooltip, "set_icon", tooltip_set_icon, 1);
    rb_define_method(gTooltip, "set_icon_from_stock", tooltip_set_icon_from_stock, 2);
    rb_define_method(gTooltip, "set_custom", tooltip_set_custom, 1);
    G_DEF_SETTERS(gTooltip);
#endif
}
