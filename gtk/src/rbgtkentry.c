/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtkentry.c -

  $Author: mutoh $
  $Date: 2002/07/31 17:23:54 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

static VALUE
entry_initialize(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE maxlen;
    rb_scan_args(argc, argv, "01", &maxlen);

    if (!NIL_P(maxlen))
      RBGTK_INITIALIZE(self, gtk_entry_new_with_max_length(NUM2INT(maxlen)));
    else
      RBGTK_INITIALIZE(self, gtk_entry_new());

    return Qnil;
}

static VALUE
entry_set_text(self, text)
    VALUE self, text;
{
    gtk_entry_set_text(GTK_ENTRY(RVAL2GOBJ(self)), STR2CSTR(text));

    return self;
}

static VALUE
entry_append_text(self, text)
    VALUE self, text;
{
    gtk_entry_append_text(GTK_ENTRY(RVAL2GOBJ(self)), STR2CSTR(text));
    return self;
}

static VALUE
entry_prepend_text(self, text)
    VALUE self, text;
{
    gtk_entry_prepend_text(GTK_ENTRY(RVAL2GOBJ(self)), STR2CSTR(text));
    return self;
}

static VALUE
entry_get_text(self)
    VALUE self;
{
    return rb_str_new2(gtk_entry_get_text(GTK_ENTRY(RVAL2GOBJ(self))));
}

static VALUE
entry_set_visibility(self, visibility)
    VALUE self, visibility;
{
    gtk_entry_set_visibility(GTK_ENTRY(RVAL2GOBJ(self)), RTEST(visibility));
    return self;
}

static VALUE
entry_set_max_length(self, max)
    VALUE self, max;
{
    gtk_entry_set_max_length(GTK_ENTRY(RVAL2GOBJ(self)), NUM2INT(max));
    return self;
}

void 
Init_gtk_entry()
{
    VALUE gEntry = G_DEF_CLASS(GTK_TYPE_ENTRY, "Entry", mGtk);

    rb_include_module(gEntry, mEditable);

    rb_define_method(gEntry, "initialize", entry_initialize, -1);
    rb_define_method(gEntry, "set_text", entry_set_text, 1);
    rb_define_method(gEntry, "append_text", entry_append_text, 1);
    rb_define_method(gEntry, "prepend_text", entry_prepend_text, 1);
    rb_define_method(gEntry, "get_text", entry_get_text, 0);
    rb_define_method(gEntry, "set_visibility", entry_set_visibility, 1);
    rb_define_method(gEntry, "set_max_length", entry_set_max_length, 1);

    /* child initialize */
    Init_gtk_spin_button();
}
