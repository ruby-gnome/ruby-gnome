/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtkobject.c -

  $Author: mutoh $
  $Date: 2002/09/11 16:35:59 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

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

static VALUE
gobj_get_gtk_type(self)
    VALUE self;
{
    return INT2NUM(GTK_OBJECT_TYPE(RVAL2GOBJ(self)));
}

void 
Init_gtk_object()
{
    VALUE gObject = G_DEF_CLASS(GTK_TYPE_OBJECT, "Object", mGtk);

    /* GtkArgFlags */
    rb_define_const(gObject, "ARG_READABLE", INT2NUM(GTK_ARG_READABLE));
    rb_define_const(gObject, "ARG_WRITABLE", INT2NUM(GTK_ARG_WRITABLE));
    rb_define_const(gObject, "ARG_CONSTRUCT", INT2NUM(GTK_ARG_CONSTRUCT));
    rb_define_const(gObject, "ARG_CONSTRUCT_ONLY", INT2NUM(GTK_ARG_CONSTRUCT_ONLY));
    rb_define_const(gObject, "ARG_CHILD_ARG", INT2NUM(GTK_ARG_CHILD_ARG));
    rb_define_const(gObject, "ARG_READWRITE", INT2NUM(GTK_ARG_READWRITE));
    /* GtkSignalRunType */
    rb_define_const(gObject, "RUN_FIRST", INT2FIX(GTK_RUN_FIRST));
    rb_define_const(gObject, "RUN_LAST", INT2FIX(GTK_RUN_LAST));
    rb_define_const(gObject, "RUN_BOTH", INT2FIX(GTK_RUN_BOTH));
    rb_define_const(gObject, "RUN_NO_RECURSE", INT2FIX(GTK_RUN_NO_RECURSE));
    rb_define_const(gObject, "RUN_ACTION", INT2FIX(GTK_RUN_ACTION));
    rb_define_const(gObject, "RUN_NO_HOOKS", INT2FIX(GTK_RUN_NO_HOOKS));

    /*
     * instance methods
     */

    rb_define_method(gObject, "flags", gobj_get_flags, 0);
    rb_define_method(gObject, "flags=", gobj_set_flags, 1);
    rb_define_method(gObject, "unset_flags", gobj_unset_flags, 1);
    rb_define_alias(gObject, "get_flags", "flags");
    rb_define_alias(gObject, "set_flags", "flags=");
    rb_define_method(gObject, "gtk_type", gobj_get_gtk_type, 0);

    rb_define_method(gObject, "destroy", gobj_destroy, 0);

}
