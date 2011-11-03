/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2002,2003 Ruby-GNOME2 Project Team
 *  Copyright (C) 1998-2000 Yukihiro Matsumoto,
 *                          Daisuke Kanda,
 *                          Hiroshi Igarashi
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

#include "global.h"

#if GTK_CHECK_VERSION(2,12,0)

#define RG_TARGET_NAMESPACE cScaleButton
#define _SELF(s) (GTK_SCALE_BUTTON(RVAL2GOBJ(s)))

static VALUE
rg_initialize(int argc, VALUE *argv, VALUE self)
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

    if (!NIL_P(arg5)) {
        const gchar **icons = RVAL2STRV(arg5);

        gtk_scale_button_set_icons(_SELF(self), icons);

        g_free(icons);
    }

    return Qnil;
}

#endif

void 
Init_gtk_scalebutton(VALUE mGtk)
{
#if GTK_CHECK_VERSION(2,12,0)
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_SCALE_BUTTON, "ScaleButton", mGtk);
    RG_DEF_METHOD(initialize, -1);
#endif
}
