/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgdkatom.c -

  $Author: mutoh $
  $Date: 2003/01/19 14:28:24 $

  Copyright (C) 2002,2003 Masao Mutoh
************************************************/


#include "global.h"

#define _SELF(a) (((GdkAtomData*)RVAL2BOXED(self, GDK_TYPE_ATOM))->atom)

/*****************************************/
GdkAtomData*
gdk_atom_copy (const GdkAtom atom)
{
  GdkAtomData* data;
  g_return_val_if_fail (atom != NULL, NULL);
  data = g_new(GdkAtomData, 1);
  data->atom = atom;
  return data;
}

GType
gdk_atom_get_type (void)
{
  static GType our_type = 0;
  if (our_type == 0)
    our_type = g_boxed_type_register_static ("GdkAtomData",
                    (GBoxedCopyFunc)gdk_atom_copy,
                    (GBoxedFreeFunc)g_free);
  return our_type;
}

/*****************************************/

static VALUE
gdkatom_s_intern(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE name;
    VALUE exist;
  
    rb_scan_args(argc, argv, "11", &name, &exist);

    return BOXED2RVAL(gdk_atom_intern(RVAL2CSTR(name), RTEST(exist)), 
                      GDK_TYPE_ATOM);
}

static VALUE
gdkatom_name(self)
    VALUE self;
{
    gchar* name = gdk_atom_name(_SELF(self));
    return name ? rb_str_new2(name) : Qnil;
}

static VALUE
gdkatom_eq(self, other)
    VALUE self, other;
{
    return (_SELF(self) == _SELF(other)) ? Qtrue : Qfalse;
}

void
Init_gtk_gdk_atom()
{
    VALUE gdkAtom = G_DEF_CLASS(GDK_TYPE_ATOM, "Atom", mGdk);

    rb_define_singleton_method(gdkAtom, "intern", gdkatom_s_intern, -1);

    rb_define_method(gdkAtom, "name", gdkatom_name, 0);
    rb_define_method(gdkAtom, "==", gdkatom_eq, 1);
    rb_define_method(gdkAtom, "===", gdkatom_eq, 1);
    rb_define_method(gdkAtom, "eql?", gdkatom_eq, 1);

    /* FIXME. GDK_NONE should be Gdk::Atom.
    rb_define_const(gdkAtom, "NONE", BOXED2RVAL(GDK_NONE, GDK_TYPE_ATOM));
    */
}           
