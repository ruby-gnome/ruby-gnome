/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkprogressbar.c -

  $Author: mutoh $
  $Date: 2003/04/30 19:44:41 $

  Copyright (C) 2002,2003 Ruby-GNOME2 Project Team
  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

static VALUE
pbar_initialize(self)
    VALUE self;
{
    RBGTK_INITIALIZE(self, gtk_progress_bar_new());
    return Qnil;
}

static VALUE
pbar_pulse(self)
    VALUE self;
{
    gtk_progress_bar_pulse(GTK_PROGRESS_BAR(RVAL2GOBJ(self)));
    return self;
}

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
    rb_define_const(gProgressBar, "LEFT_TO_RIGHT", INT2FIX(GTK_PROGRESS_LEFT_TO_RIGHT));
    rb_define_const(gProgressBar, "RIGHT_TO_LEFT", INT2FIX(GTK_PROGRESS_RIGHT_TO_LEFT));
    rb_define_const(gProgressBar, "BOTTOM_TO_TOP", INT2FIX(GTK_PROGRESS_BOTTOM_TO_TOP));
    rb_define_const(gProgressBar, "TOP_TO_BOTTOM", INT2FIX(GTK_PROGRESS_TOP_TO_BOTTOM));

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
