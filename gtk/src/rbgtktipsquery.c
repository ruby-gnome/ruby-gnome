/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtktipsquery.c -

  $Author: mutoh $
  $Date: 2002/09/12 19:06:02 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#ifndef GTK_DISABLE_DEPRECATED

#include "global.h"

static VALUE
tipsquery_initialize(self)
    VALUE self;
{
    RBGTK_INITIALIZE(self, gtk_tips_query_new());
    return Qnil;
}

static VALUE
tipsquery_start(self)
    VALUE self;
{
    gtk_tips_query_start_query(GTK_TIPS_QUERY(RVAL2GOBJ(self)));
    return self;
}

static VALUE
tipsquery_stop(self)
    VALUE self;
{
    gtk_tips_query_stop_query(GTK_TIPS_QUERY(RVAL2GOBJ(self)));
    return self;
}

static VALUE
tipsquery_set_caller(self, win)
    VALUE self, win;
{
    gtk_tips_query_set_caller(GTK_TIPS_QUERY(RVAL2GOBJ(self)),
							  GTK_WIDGET(RVAL2GOBJ(win)));
    return self;
}

static VALUE
tipsquery_set_labels(self, label_inactive, label_no_tip)
    VALUE self, label_inactive, label_no_tip;
{
    gtk_tips_query_set_labels(GTK_TIPS_QUERY(RVAL2GOBJ(self)),
							  STR2CSTR(label_inactive),
							  STR2CSTR(label_no_tip));
    return self;
}

void 
Init_gtk_tips_query()  /* GTK_DISABLE_DEPRECATED */
{
    VALUE gTipsQuery = G_DEF_CLASS(GTK_TYPE_TIPS_QUERY, "TipsQuery", mGtk);

    rb_define_method(gTipsQuery, "initialize", tipsquery_initialize, 0);
    rb_define_method(gTipsQuery, "start", tipsquery_start, 0);
    rb_define_method(gTipsQuery, "stop", tipsquery_stop, 0);
    rb_define_method(gTipsQuery, "set_caller", tipsquery_set_caller, 1);
    rb_define_method(gTipsQuery, "set_labels", tipsquery_set_labels, 2);
}
#endif
