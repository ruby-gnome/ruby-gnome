/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtkrc.c -

  $Author: mutoh $
  $Date: 2002/05/19 12:29:34 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

static VALUE
rc_m_reparse_all(self)
    VALUE self;
{
    return gtk_rc_reparse_all() ? Qtrue : Qfalse;
}

static VALUE
rc_m_parse(self, rc)
    VALUE self, rc;
{
    gtk_rc_parse(STR2CSTR(rc));
    return Qnil;
}

static VALUE
rc_m_parse_string(self, rc)
    VALUE self, rc;
{
    gtk_rc_parse_string(STR2CSTR(rc));
    return Qnil;
}

static VALUE
rc_m_get_style(self, w)
    VALUE self, w;
{
    GtkStyle *s = gtk_rc_get_style(get_widget(w));
    return make_gstyle(s);
}

static VALUE
rc_m_add_widget_name_style(self, style, pat)
    VALUE self, style, pat;
{
    gtk_rc_add_widget_name_style(get_grcstyle(style), STR2CSTR(pat));
    return Qnil;
}

static VALUE
rc_m_add_widget_class_style(self, style, pat)
    VALUE self, style, pat;
{
    gtk_rc_add_widget_class_style(get_grcstyle(style), STR2CSTR(pat));
    return Qnil;
}

void Init_gtk_rc()
{
    mRC = rb_define_module_under(mGtk, "RC");

    rb_define_module_function(mRC, "reparse_all", rc_m_reparse_all, 0);
    rb_define_module_function(mRC, "parse", rc_m_parse, 1);
    rb_define_module_function(mRC, "parse_string", rc_m_parse_string, 1);
    rb_define_module_function(mRC, "get_style", rc_m_get_style, 1);
    rb_define_module_function(mRC, "add_widget_name_style",
			      rc_m_add_widget_name_style, 2);
    rb_define_module_function(mRC, "add_widget_class_style",
			      rc_m_add_widget_class_style, 2);
}
