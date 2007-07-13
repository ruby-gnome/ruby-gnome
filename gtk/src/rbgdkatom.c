/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgdkatom.c -

  $Author: ggc $
  $Date: 2007/07/13 16:07:31 $

  Copyright (C) 2002,2003 Masao Mutoh
************************************************/


#include "global.h"

#define _SELF(a) (((GdkAtomData*)RVAL2BOXED(a, GDK_TYPE_ATOM))->atom)

/*****************************************/
GdkAtomData*
gdk_atom_copy (const GdkAtom atom)
{
    GdkAtomData* data;
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


GdkAtom
get_gdkatom(atom)
    VALUE atom;
{
    if (TYPE(atom) == T_STRING)
        return gdk_atom_intern(RVAL2CSTR(atom), FALSE);
    return ((GdkAtomData*)RVAL2BOXED(atom, GDK_TYPE_ATOM))->atom;
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

    return BOXED2RVAL(gdk_atom_intern(RVAL2CSTR(name), RVAL2CBOOL(exist)), 
                      GDK_TYPE_ATOM);
}

#if GTK_CHECK_VERSION(2,10,0)
static VALUE
gdkatom_s_intern_static_string(self, name)
    VALUE self, name;
{
    return BOXED2RVAL(gdk_atom_intern_static_string(RVAL2CSTR(name)), 
                      GDK_TYPE_ATOM);
}
#endif

static VALUE
gdkatom_initialize(self, num)
    VALUE self, num;
{
    guint atom = FIX2INT(num);
    if (atom == 0){
        /* This is a trick for passing 0(NULL) */
        G_INITIALIZE(self, GUINT_TO_POINTER(1));
        _SELF(self) = GUINT_TO_POINTER(GDK_NONE);
    } else {
        G_INITIALIZE(self, GUINT_TO_POINTER(atom));
    }

    return Qnil;
}

static VALUE
gdkatom_name(self)
    VALUE self;
{
    return CSTR2RVAL2(gdk_atom_name(_SELF(self)));
}

static VALUE
gdkatom_to_i(self)
    VALUE self;
{
    return UINT2NUM(GPOINTER_TO_UINT(_SELF(self)));
}

static VALUE
gdkatom_eq(self, other)
    VALUE self, other;
{
    return CBOOL2RVAL(_SELF(self) == _SELF(other));
}

void
Init_gtk_gdk_atom()
{
    VALUE none;
    VALUE gdkAtom = G_DEF_CLASS(GDK_TYPE_ATOM, "Atom", mGdk);

    rb_define_singleton_method(gdkAtom, "intern", gdkatom_s_intern, -1);

#if GTK_CHECK_VERSION(2,10,0)
    rb_define_singleton_method(gdkAtom, "intern_static_string", gdkatom_s_intern_static_string, 1);
#endif

    rb_define_method(gdkAtom, "initialize", gdkatom_initialize, 1);
    rb_define_method(gdkAtom, "name", gdkatom_name, 0);
    rb_define_method(gdkAtom, "to_i", gdkatom_to_i, 0);
    rb_define_method(gdkAtom, "==", gdkatom_eq, 1);

    /* This is a trick to define GDK_NONE as a BOXED object */
    none = BOXED2RVAL((gpointer)1, GDK_TYPE_ATOM);
    rb_define_const(gdkAtom, "NONE", none);
    _SELF(none) = GDK_NONE;
}           
