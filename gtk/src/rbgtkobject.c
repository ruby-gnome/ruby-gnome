/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkobject.c -

  $Author: sakai $
  $Date: 2003/07/20 05:05:08 $

  Copyright (C) 2002,2003 Ruby-GNOME2 Project Team
  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

/*
We shouldn't implement it.
void        gtk_object_sink                 (GtkObject *object);
#define     GTK_OBJECT_FLOATING             (obj)

gtk_object_new is deprecated. Use g_object_new instead.
GtkObject*  gtk_object_new                  (GtkType type,
                                             const gchar *first_property_name,
                                             ...);
*/

static VALUE
gobj_get_type_name(self)
    VALUE self;
{
    return CSTR2RVAL(GTK_OBJECT_TYPE_NAME(RVAL2GOBJ(self)));
}

/*
They are all Deprecated.
void        gtk_object_get                  (GtkObject *object,
                                             const gchar *first_property_name,
                                             ...);
void        gtk_object_set                  (GtkObject *object,
                                             const gchar *first_property_name,
                                             ...);
void        gtk_object_set_data             (GtkObject *object,
                                             const gchar *key,
                                             gpointer data);
void        gtk_object_set_data_full        (GtkObject *object,
                                             const gchar *key,
                                             gpointer data,
                                             GtkDestroyNotify destroy);
void        gtk_object_remove_data          (GtkObject *object,
                                             const gchar *key);
gpointer    gtk_object_get_data             (GtkObject *object,
                                             const gchar *key);
void        gtk_object_remove_no_notify     (GtkObject *object,
                                             const gchar *key);
void        gtk_object_set_user_data        (GtkObject *object,
                                             gpointer data);
gpointer    gtk_object_get_user_data        (GtkObject *object);
void        gtk_object_add_arg_type         (const gchar *arg_name,
                                             GtkType arg_type,
                                             guint arg_flags,
                                             guint arg_id);
void        gtk_object_set_data_by_id       (GtkObject *object,
                                             GQuark data_id,
                                             gpointer data);
void        gtk_object_set_data_by_id_full  (GtkObject *object,
                                             GQuark data_id,
                                             gpointer data,
                                             GtkDestroyNotify destroy);
gpointer    gtk_object_get_data_by_id       (GtkObject *object,
                                             GQuark data_id);
void        gtk_object_remove_data_by_id    (GtkObject *object,
                                             GQuark data_id);
void        gtk_object_remove_no_notify_by_id
                                            (GtkObject *object,
                                             GQuark key_id);
#define     gtk_object_data_try_key
#define     gtk_object_data_force_id
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

/* Move from Bindings */
static VALUE
gobj_bindings_activate(self, keyval, modifiers)
    VALUE self, keyval, modifiers;
{
     return gtk_bindings_activate(GTK_OBJECT(RVAL2GOBJ(self)), 
                                  NUM2UINT(keyval),
                                  NUM2UINT(modifiers)) ? Qtrue : Qfalse;
}

void 
Init_gtk_object()
{
    VALUE gObject = G_DEF_CLASS(GTK_TYPE_OBJECT, "Object", mGtk);

    rb_define_method(gObject, "type_name", gobj_get_type_name, 0);
    rb_define_method(gObject, "flags", gobj_get_flags, 0);
    rb_define_method(gObject, "set_flags", gobj_set_flags, 1);
    rb_define_method(gObject, "unset_flags", gobj_unset_flags, 1);
    rb_define_method(gObject, "destroy", gobj_destroy, 0);
    rb_define_method(gObject, "bindings_activate", gobj_bindings_activate, 2);

    /* GtkObjectFlags */
    G_DEF_CONSTANTS(gObject, GTK_TYPE_OBJECT_FLAGS, "GTK_");

    /* undef deprecated properties */
    rb_undef_method(gObject, "user_data");
    rb_undef_method(gObject, "user_data=");
    rb_undef_method(gObject, "set_user_data");

    G_DEF_SETTERS(gObject);
}
