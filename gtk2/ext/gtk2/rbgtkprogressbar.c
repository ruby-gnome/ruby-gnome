/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2002-2005 Ruby-GNOME2 Project Team
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

static VALUE
pbar_initialize(VALUE self)
{
    RBGTK_INITIALIZE(self, gtk_progress_bar_new());
    return Qnil;
}

static VALUE
pbar_pulse(VALUE self)
{
    gtk_progress_bar_pulse(GTK_PROGRESS_BAR(RVAL2GOBJ(self)));
    return self;
}

/* Defined as Property
void        gtk_progress_bar_set_ellipsize  (GtkProgressBar *pbar,
                                             PangoEllipsizeMode mode);
*/

void 
Init_gtk_progress_bar(void)
{
    VALUE gProgressBar = G_DEF_CLASS(GTK_TYPE_PROGRESS_BAR, "ProgressBar", mGtk);

    /* GtkProgressBarStyle */
/* Deprecated.
    rb_define_const(gProgressBar, "CONTINUOUS", INT2FIX(GTK_PROGRESS_CONTINUOUS));
    rb_define_const(gProgressBar, "DISCRETE", INT2FIX(GTK_PROGRESS_DISCRETE));
*/

    /* GtkProgressBarOrientation */
    G_DEF_CLASS(GTK_TYPE_PROGRESS_BAR_ORIENTATION, "Orientation", gProgressBar);
    G_DEF_CONSTANTS(gProgressBar, GTK_TYPE_PROGRESS_BAR_ORIENTATION, "GTK_PROGRESS_");

    rb_define_method(gProgressBar, "initialize", pbar_initialize, 0);
    rb_define_method(gProgressBar, "pulse", pbar_pulse, 0);

    /* undef deprecated properties */
    rb_undef_method(gProgressBar, "adjustment");
    rb_undef_method(gProgressBar, "adjustment=");
    rb_undef_method(gProgressBar, "set_adjustment");
    rb_undef_method(gProgressBar, "bar_style");
    rb_undef_method(gProgressBar, "bar_style=");
    rb_undef_method(gProgressBar, "set_bar_style");
    rb_undef_method(gProgressBar, "discrete_blocks");
    rb_undef_method(gProgressBar, "discrete_blocks=");
    rb_undef_method(gProgressBar, "set_discrete_blocks");
    rb_undef_method(gProgressBar, "activity_blocks");
    rb_undef_method(gProgressBar, "activity_blocks=");
    rb_undef_method(gProgressBar, "set_activity_blocks");
    rb_undef_method(gProgressBar, "activity_step");
    rb_undef_method(gProgressBar, "activity_step=");
    rb_undef_method(gProgressBar, "set_activity_step");
}
