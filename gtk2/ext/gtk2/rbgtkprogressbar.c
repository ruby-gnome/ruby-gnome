/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkprogressbar.c -

  $Author: mutoh $
  $Date: 2005/01/09 19:02:05 $

  Copyright (C) 2002-2005 Ruby-GNOME2 Project Team
  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

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
Init_gtk_progress_bar()
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
