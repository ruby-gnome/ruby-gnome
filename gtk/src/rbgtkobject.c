/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkobject.c -

  $Author: mutoh $
  $Date: 2003/02/01 16:46:23 $

  Copyright (C) 2002,2003 Ruby-GNOME2 Project Team
  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

/*
GtkObject*  gtk_object_new                  (GtkType type,
                                             const gchar *first_property_name,
                                             ...);
void        gtk_object_sink                 (GtkObject *object);
*/

static VALUE
gobj_destroy(self)
    VALUE self;
{
    gtk_object_destroy(GTK_OBJECT(RVAL2GOBJ(self)));
    return Qnil;
}

static VALUE
gobj_get_flags(self)
    VALUE self;
{
    /* _GtkObject.flags is int32 */
    return(INT2FIX(GTK_OBJECT_FLAGS(RVAL2GOBJ(self))));
}

static VALUE
gobj_set_flags(self, flags)
    VALUE self, flags;
{
    GTK_OBJECT_SET_FLAGS(RVAL2GOBJ(self), NUM2INT(flags));
    return self;
}

static VALUE
gobj_unset_flags(self, flags)
    VALUE self, flags;
{
    GTK_OBJECT_UNSET_FLAGS(RVAL2GOBJ(self), NUM2INT(flags));
    return self;
}

void 
Init_gtk_object()
{
    VALUE gObject = G_DEF_CLASS(GTK_TYPE_OBJECT, "Object", mGtk);

    rb_define_method(gObject, "flags", gobj_get_flags, 0);
    rb_define_method(gObject, "set_flags", gobj_set_flags, 1);
    rb_define_method(gObject, "unset_flags", gobj_unset_flags, 1);
    rb_define_method(gObject, "destroy", gobj_destroy, 0);

    /* GtkObjectFlags */
    rb_define_const(gObject, "IN_DESTRUCTION", INT2FIX(GTK_IN_DESTRUCTION));
    rb_define_const(gObject, "FLOATING", INT2FIX(GTK_FLOATING));
    rb_define_const(gObject, "RESERVED_1", INT2FIX(GTK_RESERVED_1));
    rb_define_const(gObject, "RESERVED_2", INT2FIX(GTK_RESERVED_2));

    G_DEF_SETTERS(gObject);
}
