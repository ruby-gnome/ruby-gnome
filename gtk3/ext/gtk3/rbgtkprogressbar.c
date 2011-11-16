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

#define RG_TARGET_NAMESPACE cProgressBar

static VALUE
rg_initialize(VALUE self)
{
    RBGTK_INITIALIZE(self, gtk_progress_bar_new());
    return Qnil;
}

static VALUE
rg_pulse(VALUE self)
{
    gtk_progress_bar_pulse(GTK_PROGRESS_BAR(RVAL2GOBJ(self)));
    return self;
}

void 
Init_gtk_progress_bar(VALUE mGtk)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_PROGRESS_BAR, "ProgressBar", mGtk);

    /* GtkProgressBarOrientation */
    G_DEF_CLASS(GTK_TYPE_PROGRESS_BAR_ORIENTATION, "Orientation", RG_TARGET_NAMESPACE);
    G_DEF_CONSTANTS(RG_TARGET_NAMESPACE, GTK_TYPE_PROGRESS_BAR_ORIENTATION, "GTK_PROGRESS_");

    RG_DEF_METHOD(initialize, 0);
    RG_DEF_METHOD(pulse, 0);

    /* undef deprecated properties */
    rb_undef_method(RG_TARGET_NAMESPACE, "adjustment");
    rb_undef_method(RG_TARGET_NAMESPACE, "adjustment=");
    rb_undef_method(RG_TARGET_NAMESPACE, "set_adjustment");
    rb_undef_method(RG_TARGET_NAMESPACE, "bar_style");
    rb_undef_method(RG_TARGET_NAMESPACE, "bar_style=");
    rb_undef_method(RG_TARGET_NAMESPACE, "set_bar_style");
    rb_undef_method(RG_TARGET_NAMESPACE, "discrete_blocks");
    rb_undef_method(RG_TARGET_NAMESPACE, "discrete_blocks=");
    rb_undef_method(RG_TARGET_NAMESPACE, "set_discrete_blocks");
    rb_undef_method(RG_TARGET_NAMESPACE, "activity_blocks");
    rb_undef_method(RG_TARGET_NAMESPACE, "activity_blocks=");
    rb_undef_method(RG_TARGET_NAMESPACE, "set_activity_blocks");
    rb_undef_method(RG_TARGET_NAMESPACE, "activity_step");
    rb_undef_method(RG_TARGET_NAMESPACE, "activity_step=");
    rb_undef_method(RG_TARGET_NAMESPACE, "set_activity_step");
}
