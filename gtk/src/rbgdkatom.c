/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgdkatom.c -

  $Author: mutoh $
  $Date: 2002/05/19 12:39:05 $

  Copyright (C) 2002 MUTOH Masao
************************************************/


#include "global.h"

VALUE gdkAtom;

static VALUE
gdkatom_s_intern(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE name;
    VALUE exist;
  
    rb_scan_args(argc, argv, "11", &name, &exist);

    return make_gdkatom(gdk_atom_intern(STR2CSTR(name), RTEST(exist)));
}

static VALUE
gdkatom_name(self)
    VALUE self;
{
    gchar* name = gdk_atom_name(get_gdkatom(self));
    
    return name ? rb_str_new2(name) : Qnil;
}

static VALUE
gdkatom_eq(self, other)
    VALUE self, other;
{
    return (get_gdkatom(self) == get_gdkatom(other)) ? Qtrue : Qfalse;
}

void
Init_gtk_gdk_atom()
{
    gdkAtom = rb_define_class_under(mGdk, "Atom", rb_cData);
    rb_define_singleton_method(gdkAtom, "intern", gdkatom_s_intern, -1);

    rb_define_method(gdkAtom, "name", gdkatom_name, 0);
    rb_define_method(gdkAtom, "==", gdkatom_eq, 1);
    rb_define_method(gdkAtom, "===", gdkatom_eq, 1);
    rb_define_method(gdkAtom, "eql?", gdkatom_eq, 1);
}           
