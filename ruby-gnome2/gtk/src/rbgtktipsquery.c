/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtktipsquery.c -

  $Author: mutoh $
  $Date: 2002/05/19 12:29:41 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

static VALUE
tipsquery_initialize(self)
    VALUE self;
{
    set_widget(self, gtk_tips_query_new());
    return Qnil;
}

static VALUE
tipsquery_start(self)
    VALUE self;
{
    gtk_tips_query_start_query(GTK_TIPS_QUERY(get_widget(self)));
    return self;
}

static VALUE
tipsquery_stop(self)
    VALUE self;
{
    gtk_tips_query_stop_query(GTK_TIPS_QUERY(get_widget(self)));
    return self;
}

static VALUE
tipsquery_set_caller(self, win)
    VALUE self, win;
{
    gtk_tips_query_set_caller(GTK_TIPS_QUERY(get_widget(self)),
			      get_widget(win));
    return self;
}

static VALUE
tipsquery_set_labels(self, label_inactive, label_no_tip)
    VALUE self, label_inactive, label_no_tip;
{
    gtk_tips_query_set_labels(GTK_TIPS_QUERY(get_widget(self)),
			      STR2CSTR(label_inactive),
			      STR2CSTR(label_no_tip));
    return self;
}

void Init_gtk_tips_query()
{
    gTipsQuery = rb_define_class_under(mGtk, "TipsQuery", gLabel);

    rb_define_const(gTipsQuery, "SIGNAL_START_QUERY", rb_str_new2("start_query"));
    rb_define_const(gTipsQuery, "SIGNAL_STOP_QUERY", rb_str_new2("stop_query"));
    rb_define_const(gTipsQuery, "SIGNAL_WIDGET_ENTERED", rb_str_new2("widget_entered"));
    rb_define_const(gTipsQuery, "SIGNAL_WIDGET_SELECTED", rb_str_new2("widget_selected"));

    rb_define_method(gTipsQuery, "initialize", tipsquery_initialize, 0);
    rb_define_method(gTipsQuery, "start", tipsquery_start, 0);
    rb_define_method(gTipsQuery, "stop", tipsquery_stop, 0);
    rb_define_method(gTipsQuery, "set_caller", tipsquery_set_caller, 1);
    rb_define_method(gTipsQuery, "set_labels", tipsquery_set_labels, 2);
}
