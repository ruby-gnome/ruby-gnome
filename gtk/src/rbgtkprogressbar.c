/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtkprogressbar.c -

  $Author: igapy $
  $Date: 2002/05/30 00:46:41 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

static VALUE
pbar_initialize(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE arg;
    GtkAdjustment *adj;
    GtkWidget* widget;

    if (rb_scan_args(argc, argv, "01", &arg) == 1) {
	adj = NIL_P(arg)?0:GTK_ADJUSTMENT(get_gobject(arg));
	widget = gtk_progress_bar_new_with_adjustment(adj);
    } else {
	widget = gtk_progress_bar_new();
    }

    set_widget(self, widget);
    return Qnil;
}

static VALUE
pbar_set_bar_style(self, style)
     VALUE self, style;
{
    gtk_progress_bar_set_bar_style(GTK_PROGRESS_BAR(get_widget(self)),
				   NUM2INT(style));
    return self;
}

static VALUE
pbar_set_discrete_blocks(self, blocks)
     VALUE self, blocks;
{
    gtk_progress_bar_set_discrete_blocks(GTK_PROGRESS_BAR(get_widget(self)),
					 NUM2INT(blocks));
    return self;
}

static VALUE
pbar_set_activity_step(self, step)
     VALUE self, step;
{
    gtk_progress_bar_set_activity_step(GTK_PROGRESS_BAR(get_widget(self)),
				       NUM2INT(step));
    return self;
}

static VALUE
pbar_set_activity_blocks(self, blocks)
     VALUE self, blocks;
{
    gtk_progress_bar_set_activity_blocks(GTK_PROGRESS_BAR(get_widget(self)),
					 NUM2INT(blocks));
    return self;
}

static VALUE
pbar_set_orientation(self, orientation)
     VALUE self, orientation;
{
    gtk_progress_bar_set_orientation(GTK_PROGRESS_BAR(get_widget(self)),
				     NUM2INT(orientation));
    return self;
}

static VALUE
pbar_update(self, percentage)
    VALUE self, percentage;
{
    gtk_progress_bar_update(GTK_PROGRESS_BAR(get_widget(self)),
			    NUM2DBL(percentage));
    return self;
}    

void Init_gtk_progress_bar()
{
    static rbgtk_class_info cinfo;

    gProgressBar = rb_define_class_under(mGtk, "ProgressBar", gProgress);
    cinfo.klass = gProgressBar;
    cinfo.gtype = GTK_TYPE_PROGRESS_BAR;
    cinfo.mark = 0;
    cinfo.free = 0;
    rbgtk_register_class(&cinfo);

    /* GtkProgressBarStyle */
    rb_define_const(gProgressBar, "CONTINUOUS", INT2FIX(GTK_PROGRESS_CONTINUOUS));
    rb_define_const(gProgressBar, "DISCRETE", INT2FIX(GTK_PROGRESS_DISCRETE));

    /* GtkProgressBarOrientation */
    rb_define_const(gProgressBar, "LEFT_TO_RIGHT", INT2FIX(GTK_PROGRESS_LEFT_TO_RIGHT));
    rb_define_const(gProgressBar, "RIGHT_TO_LEFT", INT2FIX(GTK_PROGRESS_RIGHT_TO_LEFT));
    rb_define_const(gProgressBar, "BOTTOM_TO_TOP", INT2FIX(GTK_PROGRESS_BOTTOM_TO_TOP));
    rb_define_const(gProgressBar, "TOP_TO_BOTTOM", INT2FIX(GTK_PROGRESS_TOP_TO_BOTTOM));

    rb_define_method(gProgressBar, "initialize", pbar_initialize, -1);
    rb_define_method(gProgressBar, "set_bar_style", pbar_set_bar_style, 1);
    rb_define_method(gProgressBar, "set_discrete_blocks", pbar_set_discrete_blocks, 1);
    rb_define_method(gProgressBar, "set_activity_step", pbar_set_activity_step, 1);
    rb_define_method(gProgressBar, "set_activity_blocks", pbar_set_activity_blocks, 1);
    rb_define_method(gProgressBar, "set_orientation", pbar_set_orientation, 1);
    rb_define_method(gProgressBar, "update", pbar_update, 1);
}
