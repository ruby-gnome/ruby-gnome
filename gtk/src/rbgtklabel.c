/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtklabel.c -

  $Author: mutoh $
  $Date: 2002/06/22 19:50:57 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

static VALUE
label_initialize(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE label;
    rb_scan_args(argc, argv, "01", &label);
    RBGTK_INITIALIZE(self, gtk_label_new(NIL_P(label)?NULL:STR2CSTR(label)));
    return Qnil;
}

static VALUE
label_get(self)
    VALUE self;
{
    gchar* str;
    gtk_label_get(GTK_LABEL(RVAL2GOBJ(self)), &str);
    return rb_str_new2(str);
}

static VALUE
label_set_text(self, str)
    VALUE self, str;
{
    gtk_label_set_text(GTK_LABEL(RVAL2GOBJ(self)), STR2CSTR(str));
    return Qnil;
}

static VALUE
label_get_jtype(self)
    VALUE self;
{
    return INT2FIX(GTK_LABEL(RVAL2GOBJ(self))->jtype);
}

static VALUE
label_set_jtype(self, jtype)
    VALUE self, jtype;
{
    GtkJustification j;
    j = (GtkJustification) NUM2INT(jtype);
    gtk_label_set_justify(GTK_LABEL(RVAL2GOBJ(self)), j);
    return self;
}

static VALUE
label_set_line_wrap(self, wrap)
    VALUE self, wrap;
{
    gtk_label_set_line_wrap(GTK_LABEL(RVAL2GOBJ(self)), (gboolean)RTEST(wrap));
    return self;
}

static VALUE
label_set_pattern(self, pattern)
    VALUE self, pattern;
{
    gtk_label_set_pattern(GTK_LABEL(RVAL2GOBJ(self)), STR2CSTR(pattern));
    return self;
}

static VALUE
label_parse_uline(self, string)
    VALUE self, string;
{
    guint i = gtk_label_parse_uline(GTK_LABEL(RVAL2GOBJ(self)),
				    STR2CSTR(string));
    return INT2FIX(i);
}

void Init_gtk_label()
{
    static rbgtk_class_info cinfo;

    gLabel = rb_define_class_under(mGtk, "Label", gMisc);
    cinfo.klass = gLabel;
    cinfo.gtype = GTK_TYPE_LABEL;
    cinfo.mark = 0;
    cinfo.free = 0;
    rbgtk_register_class(&cinfo);

    rb_define_method(gLabel, "initialize", label_initialize, -1);
    rb_define_method(gLabel, "get", label_get, 0);
    rb_define_alias(gLabel, "get_text", "get");
    rb_define_alias(gLabel, "text", "get");
    rb_define_method(gLabel, "set", label_set_text, 1);
    rb_define_alias(gLabel, "set_text", "set");
    rb_define_alias(gLabel, "text=", "set");
    rb_define_method(gLabel, "jtype", label_get_jtype, 0);
    rb_define_method(gLabel, "jtype=", label_set_jtype, 1);
    rb_define_method(gLabel, "set_line_wrap", label_set_line_wrap, 1);
    rb_define_method(gLabel, "set_pattern", label_set_pattern, 1);
    rb_define_method(gLabel, "parse_uline", label_parse_uline, 1);

    rb_define_alias(gLabel, "get_justify", "jtype");
    rb_define_alias(gLabel, "set_justify", "jtype=");

    /* child initialize */
    Init_gtk_tips_query();
    Init_gtk_accel_label();
}
