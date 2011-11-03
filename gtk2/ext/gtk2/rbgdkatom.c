/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2002,2003 Masao Mutoh
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 *  MA  02110-1301  USA
 */

#include "global.h"
#include "rbgdk.h"

#define RG_TARGET_NAMESPACE cAtom
#define _SELF(a) (((GdkAtomData*)RVAL2BOXED(a, GDK_TYPE_ATOM))->atom)

/*****************************************/
static GdkAtomData *
gdk_atom_copy(const GdkAtom atom)
{
    GdkAtomData* data;
    data = g_new(GdkAtomData, 1);
    data->atom = atom;
    return data;
}

GType
gdk_atom_get_type(void)
{
    static GType our_type = 0;
    if (our_type == 0)
        our_type = g_boxed_type_register_static ("GdkAtomData",
                                                 (GBoxedCopyFunc)gdk_atom_copy,
                                                 (GBoxedFreeFunc)g_free);
    return our_type;
}

GdkAtom
get_gdkatom(VALUE atom)
{
    if (TYPE(atom) == T_STRING)
        return gdk_atom_intern(RVAL2CSTR(atom), FALSE);
    return ((GdkAtomData*)RVAL2BOXED(atom, GDK_TYPE_ATOM))->atom;
}
/*****************************************/

static VALUE
rg_s_intern(int argc, VALUE *argv, G_GNUC_UNUSED VALUE self)
{
    VALUE name;
    VALUE exist;

    rb_scan_args(argc, argv, "11", &name, &exist);

    return BOXED2RVAL(gdk_atom_intern(RVAL2CSTR(name), RVAL2CBOOL(exist)), 
                      GDK_TYPE_ATOM);
}

/* We don't need them.
GdkAtom gdk_atom_intern_static_string(const gchar *atom_name);
 */

static VALUE
rg_initialize(VALUE self, VALUE num)
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
rg_name(VALUE self)
{
    return CSTR2RVAL_FREE(gdk_atom_name(_SELF(self)));
}

static VALUE
rg_to_i(VALUE self)
{
    return UINT2NUM(GPOINTER_TO_UINT(_SELF(self)));
}

static VALUE
rg_operator_equal(VALUE self, VALUE other)
{
    return CBOOL2RVAL(_SELF(self) == _SELF(other));
}

void
Init_gtk_gdk_atom(VALUE mGdk)
{
    VALUE none;
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GDK_TYPE_ATOM, "Atom", mGdk);

    RG_DEF_SMETHOD(intern, -1);

    RG_DEF_METHOD(initialize, 1);
    RG_DEF_METHOD(name, 0);
    RG_DEF_METHOD(to_i, 0);
    RG_DEF_METHOD_OPERATOR("==", equal, 1);

    /* This is a trick to define GDK_NONE as a BOXED object */
    none = BOXED2RVAL((gpointer)1, GDK_TYPE_ATOM);
    rb_define_const(RG_TARGET_NAMESPACE, "NONE", none);
    _SELF(none) = GDK_NONE;
}           
