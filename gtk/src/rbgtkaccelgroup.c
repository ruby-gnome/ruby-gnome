/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkaccelgroup.c -

  $Author: ogs $
  $Date: 2002/12/12 15:52:21 $

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

static VALUE
gaccelgrp_connect(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE key, mods, flags, func;
    GClosure *rclosure;
    
    rb_scan_args(argc, argv, "31", &key, &mods, &flags, &func);
    if(NIL_P(func))
        func = rb_f_lambda();
    G_RELATIVE(self, func);
    rclosure = g_rclosure_new(func, Qnil, NULL);

    gtk_accel_group_connect(_SELF(self), NUM2UINT(key),
                            FIX2INT(mods), FIX2INT(flags), rclosure);
    return self;
}

static VALUE
gaccelgrp_connect_by_path(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE path, func;
    GClosure *rclosure;

    rb_scan_args(argc, argv, "11", &path, &func);
    if(NIL_P(func))
        func = rb_f_lambda();
    G_RELATIVE(self, func);
    rclosure = g_rclosure_new(func, Qnil, NULL);

    gtk_accel_group_connect_by_path(_SELF(self), RVAL2CSTR(path), rclosure);
    return self;
}

static VALUE
gaccelgrp_disconnect_key(self, key, mods)
    VALUE self, key, mods;
{
    return gtk_accel_group_disconnect_key(_SELF(self), NUM2UINT(key),
                                          FIX2INT(mods)) ? Qtrue : Qfalse;
}

static VALUE
gaccelgrp_query(self, key, mods)
    VALUE self, key, mods;
{
    GtkAccelGroupEntry *entries;
    guint n_entries;
    int cnt;
    VALUE result;

    entries = gtk_accel_group_query(_SELF(self), NUM2UINT(key),
                                    FIX2INT(mods), &n_entries);
    if(n_entries == 0){
        return Qnil;
    }
    else{
        result = rb_ary_new2(n_entries);
        for(cnt=0; cnt<n_entries; cnt++, entries++)
            rb_ary_push(result, BOXED2RVAL(entries, GTK_TYPE_ACCEL_GROUP_ENTRY));
        return result;
    }
}

#if 0
gboolean    (*GtkAccelGroupActivate)        (GtkAccelGroup *accel_group,
                                             GObject *acceleratable,
                                             guint keyval,
                                             GdkModifierType modifier);
gboolean    gtk_accel_group_disconnect      (GtkAccelGroup *accel_group,
                                             GClosure *closure);
GtkAccelGroup* gtk_accel_group_from_accel_closure
                                            (GClosure *closure);
#endif

static gboolean
gaccelgrp_find_func(key, closure, func)
    GtkAccelKey *key;
    GClosure *closure;
    gpointer func;
{
    return RTEST(rb_funcall((VALUE)func, id_call, 2,
                            BOXED2RVAL(key, GTK_TYPE_ACCEL_KEY),
                            BOXED2RVAL(closure, G_TYPE_CLOSURE)));
}

static VALUE
gaccelgrp_find(self)
    VALUE self;
{
    GtkAccelKey *result;
    VALUE func = rb_f_lambda();
    G_RELATIVE(self, func);

    result = gtk_accel_group_find(_SELF(self),
                                  gaccelgrp_find_func,
                                  (gpointer)func);
    return result ? BOXED2RVAL(result, GTK_TYPE_ACCEL_KEY) : Qnil;
}

static VALUE
gaccelgrp_lock(self)
    VALUE self;
{
    gtk_accel_group_lock(_SELF(self));
    return self;
}

static VALUE
gaccelgrp_unlock(self)
    VALUE self;
{
    gtk_accel_group_unlock(_SELF(self));
    return self;
}


static VALUE
gaccelgrp_s_activate(self, obj, key, modtype)
    VALUE self, obj, key, modtype;
{
    return gtk_accel_groups_activate(G_OBJECT(RVAL2GOBJ(obj)),
                                     NUM2INT(key),
                                     NUM2INT(modtype)) ? Qtrue : Qfalse;
}

static VALUE
gaccelgrp_s_from_object(self, object)
    VALUE self, object;
{
    return GSLIST2ARY(gtk_accel_groups_from_object(RVAL2GOBJ(object)));
}

void 
Init_gtk_accel_group()
{
    VALUE gAccelGroup = G_DEF_CLASS(GTK_TYPE_ACCEL_GROUP, "AccelGroup", mGtk);

    rb_define_singleton_method(gAccelGroup, "activate", gaccelgrp_s_activate, 3);
    rb_define_singleton_method(gAccelGroup, "from_object", gaccelgrp_s_from_object, 1);
    
    rb_define_method(gAccelGroup, "initialize", gaccelgrp_initialize, 0);
    rb_define_method(gAccelGroup, "lock", gaccelgrp_lock, 0);
    rb_define_method(gAccelGroup, "unlock", gaccelgrp_unlock, 0);
    rb_define_method(gAccelGroup, "connect", gaccelgrp_connect, -1);
    rb_define_method(gAccelGroup, "connect_by_path", gaccelgrp_connect_by_path, -1);
    rb_define_method(gAccelGroup, "disconnect_key", gaccelgrp_disconnect_key, 2);
    rb_define_method(gAccelGroup, "query", gaccelgrp_query, 2);
    rb_define_method(gAccelGroup, "find", gaccelgrp_find, 0);
}
