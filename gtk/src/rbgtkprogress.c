/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtkprogress.c -

  $Author: mutoh $
  $Date: 2002/07/31 17:23:54 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#ifndef GTK_DISABLE_DEPRECATED

#include "global.h"

static VALUE
progress_set_show_text(self, show_text)
     VALUE self, show_text;
{
    gtk_progress_set_show_text(GTK_PROGRESS(RVAL2GOBJ(self)),
			       RTEST(show_text));
    return self;
}

static VALUE
progress_show_text_p(self)
     VALUE self;
{
    gboolean result = GTK_PROGRESS(RVAL2GOBJ(self))->show_text;
    return result?Qtrue:Qfalse;
}

static VALUE
progress_set_text_alignment(self, x_align, y_align)
     VALUE self, x_align, y_align;
{
    gtk_progress_set_text_alignment(GTK_PROGRESS(RVAL2GOBJ(self)),
				    (gfloat)NUM2DBL(x_align),
				    (gfloat)NUM2DBL(y_align));
    return self;
}

static VALUE
progress_set_format_string(self, format)
     VALUE self, format;
{
    gtk_progress_set_format_string(GTK_PROGRESS(RVAL2GOBJ(self)),
				   STR2CSTR(format));
    return self;
}

static VALUE
progress_set_adjustment(self, adjustment)
     VALUE self, adjustment;
{
    gtk_progress_set_adjustment(GTK_PROGRESS(RVAL2GOBJ(self)),
				GTK_ADJUSTMENT(RVAL2GOBJ(adjustment)));
    return self;
}

static VALUE
progress_get_adjustment(self)
     VALUE self;
{
    GtkAdjustment* result = GTK_PROGRESS(RVAL2GOBJ(self))->adjustment;
    return result ? GOBJ2RVAL(result) : Qnil;
}

static VALUE
progress_set_percentage(self, percentage)
    VALUE self, percentage;
{
    gtk_progress_set_percentage(GTK_PROGRESS(RVAL2GOBJ(self)),
				(gfloat)NUM2DBL(percentage));
    return self;
}

static VALUE
progress_set_value(self, value)
    VALUE self, value;
{
    gtk_progress_set_value(GTK_PROGRESS(RVAL2GOBJ(self)),
			   (gfloat)NUM2DBL(value));
    return self;
}

static VALUE
progress_get_value(self)
     VALUE self;
{
    gfloat result;
    result = gtk_progress_get_value(GTK_PROGRESS(RVAL2GOBJ(self)));
    return rb_float_new((double)result);
}

static VALUE
progress_set_activity_mode(self, activity_mode)
     VALUE self, activity_mode;
{
    gtk_progress_set_activity_mode(GTK_PROGRESS(RVAL2GOBJ(self)),
				   RTEST(activity_mode));
    return self;
}

static VALUE
progress_activity_mode_p(self)
     VALUE self;
{
    gboolean result;
    result = GTK_PROGRESS(RVAL2GOBJ(self))->activity_mode;
    return result?Qtrue:Qfalse;
}

static VALUE
progress_get_current_text(self)
     VALUE self;
{
    return CSTR2OBJ(gtk_progress_get_current_text(
                GTK_PROGRESS(RVAL2GOBJ(self))));
}

static VALUE
progress_get_text_from_value(self, value)
     VALUE self, value;
{
    return CSTR2OBJ(gtk_progress_get_text_from_value(
                GTK_PROGRESS(RVAL2GOBJ(self)),
                NUM2DBL(value)));
}

static VALUE
progress_get_current_percentage(self)
     VALUE self;
{
    gfloat result;
    result = gtk_progress_get_current_percentage(
		GTK_PROGRESS(RVAL2GOBJ(self)));
    return rb_float_new((double)result);    
}

static VALUE
progress_get_percentage_from_value(self, value)
     VALUE self, value;
{
    gfloat result;
    result = gtk_progress_get_percentage_from_value(
		GTK_PROGRESS(RVAL2GOBJ(self)),
		(gfloat)NUM2DBL(value));
    return rb_float_new((double)result);
}

static VALUE
progress_configure(self, value, min, max)
     VALUE self, value, min, max;
{
    gtk_progress_configure(GTK_PROGRESS(RVAL2GOBJ(self)),
			   (gfloat)NUM2DBL(value),
			   (gfloat)NUM2DBL(min),
			   (gfloat)NUM2DBL(max));
    return self;
}

void 
Init_gtk_progress()
{
    VALUE gProgress = G_DEF_CLASS(GTK_TYPE_PROGRESS, "Progress", mGtk);

    rb_define_method(gProgress, "set_show_text", progress_set_show_text, 1);
    rb_define_method(gProgress, "show_text=", progress_set_show_text, 1);
    rb_define_method(gProgress, "show_text?", progress_show_text_p, 0);
    rb_define_method(gProgress, "set_text_alignment", progress_set_text_alignment, 2);
    rb_define_method(gProgress, "set_format_string", progress_set_format_string, 1);
    rb_define_method(gProgress, "set_adjustment", progress_set_adjustment, 1);
    rb_define_method(gProgress, "adjustment=", progress_set_adjustment, 1);
    rb_define_method(gProgress, "adjustment", progress_get_adjustment, 0);
    rb_define_method(gProgress, "set_percentage", progress_set_percentage, 1);
    rb_define_method(gProgress, "set_value", progress_set_value, 1);
    rb_define_method(gProgress, "get_value", progress_get_value, 0);
    rb_define_method(gProgress, "set_activity_mode", progress_set_activity_mode, 1);
    rb_define_method(gProgress, "activity_mode=", progress_set_activity_mode, 1);
    rb_define_method(gProgress, "activity_mode?", progress_activity_mode_p, 0);
    rb_define_method(gProgress, "get_current_text", progress_get_current_text, 0);
    rb_define_method(gProgress, "get_text_from_value", progress_get_text_from_value, 1);
    rb_define_method(gProgress, "get_current_percentage", progress_get_current_percentage, 0);
    rb_define_method(gProgress, "get_percentage_from_value", progress_get_percentage_from_value, 1);
    rb_define_method(gProgress, "configure", progress_configure, 3);

    /* child initialize */
    Init_gtk_progress_bar();
}
#endif
