/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtktext.c -

  $Author: mutoh $
  $Date: 2002/07/06 20:56:15 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#ifdef GTK_ENABLE_BROKEN

#include "global.h"

static VALUE
txt_initialize(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE arg1, arg2;
    GtkAdjustment *h_adj = NULL;
    GtkAdjustment *v_adj = NULL;

    rb_scan_args(argc, argv, "02", &arg1, &arg2);
    if (!NIL_P(arg1)) h_adj = GTK_ADJUSTMENT(RVAL2GOBJ(arg1));
    if (!NIL_P(arg2)) v_adj = GTK_ADJUSTMENT(RVAL2GOBJ(arg2));

    RBGTK_INITIALIZE(self, gtk_text_new(h_adj, v_adj));
    return Qnil;
}

static VALUE
txt_set_line_wrap(self, line_wrap)
    VALUE self, line_wrap;
{
    gtk_text_set_line_wrap(GTK_TEXT(RVAL2GOBJ(self)), RTEST(line_wrap));
    return self;
}

static VALUE
txt_set_word_wrap(self, word_wrap)
    VALUE self, word_wrap;
{
    gtk_text_set_word_wrap(GTK_TEXT(RVAL2GOBJ(self)), RTEST(word_wrap));
    return self;
}

static VALUE
txt_set_adjustments(self, h_adj, v_adj)
    VALUE self, h_adj, v_adj;
{
    gtk_text_set_adjustments(GTK_TEXT(RVAL2GOBJ(self)),
			     GTK_ADJUSTMENT(RVAL2GOBJ(h_adj)),
			     GTK_ADJUSTMENT(RVAL2GOBJ(v_adj)));

    return self;
}

static VALUE
txt_set_point(self, index)
    VALUE self, index;
{
    gtk_text_set_point(GTK_TEXT(RVAL2GOBJ(self)), NUM2INT(index));
    return self;
}

static VALUE
txt_get_point(self)
    VALUE self;
{
    int index = gtk_text_get_point(GTK_TEXT(RVAL2GOBJ(self)));
    
    return INT2FIX(index);
}

static VALUE
txt_get_length(self)
    VALUE self;
{
    int len = gtk_text_get_length(GTK_TEXT(RVAL2GOBJ(self)));
    
    return INT2FIX(len);
}

static VALUE
txt_freeze(self)
    VALUE self;
{
    gtk_text_freeze(GTK_TEXT(RVAL2GOBJ(self)));
    return self;
}

static VALUE
txt_thaw(self)
    VALUE self;
{
    gtk_text_thaw(GTK_TEXT(RVAL2GOBJ(self)));
    return self;
}

#ifndef GTK_DISABLE_DEPRECATED
static VALUE
txt_insert(self, font, fore, back, str)
    VALUE self, font, fore, back, str;
{
    Check_Type(str, T_STRING);
    gtk_text_insert(GTK_TEXT(RVAL2GOBJ(self)), 
		    get_gdkfont(font),
		    get_gdkcolor(fore),
		    get_gdkcolor(back),
		    RSTRING(str)->ptr,
		    RSTRING(str)->len);

    return self;
}

#endif

static VALUE
txt_backward_delete(self, nchars)
    VALUE self, nchars;
{
    gtk_text_backward_delete(GTK_TEXT(RVAL2GOBJ(self)), NUM2INT(nchars));
    return self;
}

static VALUE
txt_forward_delete(self, nchars)
    VALUE self, nchars;
{
    gtk_text_forward_delete(GTK_TEXT(RVAL2GOBJ(self)), NUM2INT(nchars));
    return self;
}

static VALUE
txt_has_cursor(self)
    VALUE self;
{
    rb_notimplement();
    /*  return GTK_TEXT(RVAL2GOBJ(self))->has_cursor? Qtrue: Qfalse; */
}

void Init_gtk_text()
{
    static RGObjClassInfo cinfo;

    gText = rb_define_class_under(mGtk, "Text", gEditable);
    cinfo.klass = gText;
    cinfo.gtype = GTK_TYPE_TEXT;
    cinfo.mark = 0;
    cinfo.free = 0;
    rbgtk_register_class(&cinfo);

    rb_define_method(gText, "initialize", txt_initialize, -1);
    rb_define_method(gText, "set_line_wrap", txt_set_line_wrap, 1);
    rb_define_method(gText, "set_word_wrap", txt_set_word_wrap, 1);
    rb_define_method(gText, "set_adjustments", txt_set_adjustments, 2);
    rb_define_method(gText, "set_point", txt_set_point, 1);
    rb_define_method(gText, "get_point", txt_get_point, 0);
    rb_define_method(gText, "get_length", txt_get_length, 0);
    rb_define_method(gText, "freeze", txt_freeze, 0);
    rb_define_method(gText, "thaw", txt_thaw, 0);
#ifndef GTK_DISABLE_DEPRECATED
    rb_define_method(gText, "insert", txt_insert, 4);
#endif
    rb_define_method(gText, "backward_delete", txt_backward_delete, 1);
    rb_define_method(gText, "forward_delete", txt_forward_delete, 1);
    rb_define_method(gText, "has_cursor?", txt_has_cursor, 0);
}
#endif
