/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkaccelgroup.c -

  $Author: mutoh $
  $Date: 2002/10/19 13:20:41 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

#define _SELF(w) GTK_ACCEL_GROUP(RVAL2GOBJ(w))

static VALUE
gaccelgrp_initialize(self)
    VALUE self;
{
    G_INITIALIZE(self, gtk_accel_group_new());
    return Qnil;
}

/*
void        gtk_accel_group_connect         (GtkAccelGroup *accel_group,
                                             guint accel_key,
                                             GdkModifierType accel_mods,
                                             GtkAccelFlags accel_flags,
                                             GClosure *closure);
void        gtk_accel_group_connect_by_path (GtkAccelGroup *accel_group,
                                             const gchar *accel_path,
                                             GClosure *closure);
gboolean    (*GtkAccelGroupActivate)        (GtkAccelGroup *accel_group,
                                             GObject *acceleratable,
                                             guint keyval,
                                             GdkModifierType modifier);
gboolean    gtk_accel_group_disconnect      (GtkAccelGroup *accel_group,
                                             GClosure *closure);
gboolean    gtk_accel_group_disconnect_key  (GtkAccelGroup *accel_group,
                                             guint accel_key,
                                             GdkModifierType accel_mods);
GtkAccelGroupEntry* gtk_accel_group_query   (GtkAccelGroup *accel_group,
                                             guint accel_key,
                                             GdkModifierType accel_mods,
                                             guint *n_entries);
*/

static VALUE
gaccelgrp_lock(self)
    VALUE self;
{
    gtk_accel_group_lock(_SELF(self));
    return Qnil;
}

static VALUE
gaccelgrp_unlock(self)
    VALUE self;
{
    gtk_accel_group_unlock(_SELF(self));
    return Qnil;
}

/*
GtkAccelGroup* gtk_accel_group_from_accel_closure
                                            (GClosure *closure);
*/

static VALUE
gaccelgrp_s_activate(self, obj, key, modtype)
    VALUE self, obj, key, modtype;
{
    return gtk_accel_groups_activate(G_OBJECT(RVAL2GOBJ(obj)),
                                     NUM2INT(key),
                                     NUM2INT(modtype)) ? Qtrue : Qfalse;
}

/*
GSList*     gtk_accel_groups_from_object    (GObject *object);
GtkAccelKey* gtk_accel_group_find           (GtkAccelGroup *accel_group,
                                             gboolean (*find_func) (GtkAccelKey *key,GClosure    *closure,gpointer     data),
                                             gpointer data);
struct      GtkAccelKey;
gboolean    gtk_accelerator_valid           (guint keyval,
                                             GdkModifierType modifiers);
void        gtk_accelerator_parse           (const gchar *accelerator,
                                             guint *accelerator_key,
                                             GdkModifierType *accelerator_mods);
gchar*      gtk_accelerator_name            (guint accelerator_key,
                                             GdkModifierType accelerator_mods);
void        gtk_accelerator_set_default_mod_mask
                                            (GdkModifierType default_mod_mask);
guint       gtk_accelerator_get_default_mod_mask
                                            (void);
*/

void 
Init_gtk_accel_group()
{
    VALUE gAccelGroup = G_DEF_CLASS(GTK_TYPE_ACCEL_GROUP, "AccelGroup", mGtk);

    rb_define_singleton_method(gAccelGroup, "activate", gaccelgrp_s_activate, 3);
    rb_define_method(gAccelGroup, "initialize", gaccelgrp_initialize, 0);
    rb_define_method(gAccelGroup, "lock", gaccelgrp_lock, 0);
    rb_define_method(gAccelGroup, "unlock", gaccelgrp_unlock, 0);
}
