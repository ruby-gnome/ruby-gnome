/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkprogressbar.c -

  $Author: mutoh $
  $Date: 2002/11/11 15:32:34 $

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
    rb_define_const(gProgressBar, "CONTINUOUS", INT2FIX(GTK_PROGRESS_CONTINUOUS));
    rb_define_const(gProgressBar, "DISCRETE", INT2FIX(GTK_PROGRESS_DISCRETE));

    /* GtkProgressBarOrientation */
    rb_define_const(gProgressBar, "LEFT_TO_RIGHT", INT2FIX(GTK_PROGRESS_LEFT_TO_RIGHT));
    rb_define_const(gProgressBar, "RIGHT_TO_LEFT", INT2FIX(GTK_PROGRESS_RIGHT_TO_LEFT));
    rb_define_const(gProgressBar, "BOTTOM_TO_TOP", INT2FIX(GTK_PROGRESS_BOTTOM_TO_TOP));
    rb_define_const(gProgressBar, "TOP_TO_BOTTOM", INT2FIX(GTK_PROGRESS_TOP_TO_BOTTOM));

    rb_define_method(gProgressBar, "initialize", pbar_initialize, 0);
    rb_define_method(gProgressBar, "pulse", pbar_pulse, 0);
}
