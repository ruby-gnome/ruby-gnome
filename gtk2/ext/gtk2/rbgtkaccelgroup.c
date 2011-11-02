/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2002,2003 Ruby-GNOME2 Project Team
 *  Copyright (C) 1998-2000 Yukihiro Matsumoto,
 *                          Daisuke Kanda,
 *                          Hiroshi Igarashi
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

#define RG_TARGET_NAMESPACE cAccelGroup
#define _SELF(w) GTK_ACCEL_GROUP(RVAL2GOBJ(w))
#define RVAL2MOD(mods) (NIL_P(mods) ? 0 : RVAL2GFLAGS(mods, GDK_TYPE_MODIFIER_TYPE))

static VALUE
rg_initialize(VALUE self)
{
    G_INITIALIZE(self, gtk_accel_group_new());
    return Qnil;
}

static VALUE
rg_connect(int argc, VALUE *argv, VALUE self)
{
    VALUE key, mods, flags, path, closure;
    GClosure *rclosure;

    if (argc > 2){
        rb_scan_args(argc, argv, "31", &key, &mods, &flags, &closure);
        if (NIL_P(closure)){
            rclosure = g_rclosure_new(rb_block_proc(), Qnil, NULL);
        } else {
            rclosure = (GClosure*)RVAL2BOXED(closure, G_TYPE_CLOSURE);
        }
        g_rclosure_attach(rclosure, self);
        gtk_accel_group_connect(_SELF(self), NUM2UINT(key),
                                RVAL2MOD(mods), 
                                RVAL2GFLAGS(flags, GTK_TYPE_ACCEL_FLAGS),
                                rclosure);
    } else {
        rb_scan_args(argc, argv, "11", &path, &closure);
        if (NIL_P(closure)){
            rclosure = g_rclosure_new(rb_block_proc(), Qnil, NULL);
        } else {
            rclosure = (GClosure*)RVAL2BOXED(closure, G_TYPE_CLOSURE);
        }
        g_rclosure_attach(rclosure, self);
        gtk_accel_group_connect_by_path(_SELF(self), RVAL2CSTR(path), rclosure);
    }
    return self;
}

static VALUE
rg_disconnect_key(VALUE self, VALUE key, VALUE mods)
{
    return CBOOL2RVAL(gtk_accel_group_disconnect_key(_SELF(self), NUM2UINT(key),
                                                     RVAL2MOD(mods)));
}

static VALUE
rg_query(VALUE self, VALUE key, VALUE mods)
{
    GtkAccelGroupEntry *entries;
    guint n_entries;
    guint cnt;
    VALUE result;

    entries = gtk_accel_group_query(_SELF(self), NUM2UINT(key),
                                    RVAL2MOD(mods), &n_entries);
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

static VALUE
rg_disconnect(VALUE self, VALUE closure)
{
    return CBOOL2RVAL(gtk_accel_group_disconnect(_SELF(self),
                                                 (GClosure*)RVAL2BOXED(closure, G_TYPE_CLOSURE)));
}

static VALUE
rg_s_from_accel_closure(G_GNUC_UNUSED VALUE self, VALUE closure)
{
    return GOBJ2RVAL(gtk_accel_group_from_accel_closure(
                         (GClosure*)RVAL2BOXED(closure, G_TYPE_CLOSURE)));
}

#if GTK_CHECK_VERSION(2,2,0)
static gboolean
gaccelgrp_find_func(GtkAccelKey *key, GClosure *closure, gpointer func)
{
    return RVAL2CBOOL(rb_funcall((VALUE)func, id_call, 2,
                            BOXED2RVAL(key, GTK_TYPE_ACCEL_KEY),
                            BOXED2RVAL(closure, G_TYPE_CLOSURE)));
}

static VALUE
rg_find(VALUE self)
{
    GtkAccelKey *result;
    volatile VALUE func = rb_block_proc();

    result = gtk_accel_group_find(_SELF(self),
                                  (GtkAccelGroupFindFunc)gaccelgrp_find_func,
                                  (gpointer)func);
    return BOXED2RVAL(result, GTK_TYPE_ACCEL_KEY);
}
#endif

#if GTK_CHECK_VERSION(2,4,0)
static VALUE
rg_activate(VALUE self, VALUE accel_quark, VALUE acceleratable, VALUE accel_key, VALUE accel_mods)
{
    GQuark quark = 0;
    if (TYPE(accel_quark) == T_STRING){
        quark = g_quark_from_string(RVAL2CSTR(accel_quark));
    } else {
        quark = NUM2UINT(quark);
    }
    return CBOOL2RVAL(gtk_accel_group_activate(_SELF(self), quark, RVAL2GOBJ(acceleratable),
                                               NUM2UINT(accel_key), RVAL2MOD(accel_mods)));
}
#endif

static VALUE
_gaccelgrp_lock_ensure(VALUE self)
{
    gtk_accel_group_unlock(_SELF(self));
    return Qnil;
}

static VALUE
rg_lock(VALUE self)
{
    gtk_accel_group_lock(_SELF(self));
    if (rb_block_given_p()){
        rb_ensure(rb_yield, self, _gaccelgrp_lock_ensure , self);
    }
    return self;
}

static VALUE
rg_unlock(VALUE self)
{
    gtk_accel_group_unlock(_SELF(self));
    return self;
}

static VALUE
rg_s_activate(G_GNUC_UNUSED VALUE self, VALUE obj, VALUE key, VALUE modtype)
{
    return CBOOL2RVAL(gtk_accel_groups_activate(G_OBJECT(RVAL2GOBJ(obj)),
                                                NUM2UINT(key),
                                                RVAL2MOD(modtype)));
}

static VALUE
rg_s_from_object(G_GNUC_UNUSED VALUE self, VALUE object)
{
    /* Owned by GTK+ */
    return GSLIST2ARY(gtk_accel_groups_from_object(RVAL2GOBJ(object)));
}

void 
Init_gtk_accel_group(VALUE mGtk)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_ACCEL_GROUP, "AccelGroup", mGtk);

    RG_DEF_SMETHOD(activate, 3);
    RG_DEF_SMETHOD(from_object, 1);
    RG_DEF_SMETHOD(from_accel_closure, 1);
    RG_DEF_METHOD(initialize, 0);
#if GTK_CHECK_VERSION(2,4,0)
    RG_DEF_METHOD(activate, 4);
#endif
    RG_DEF_METHOD(lock, 0);
    RG_DEF_METHOD(unlock, 0);
    RG_DEF_METHOD(connect, -1);
    RG_DEF_METHOD(disconnect, 1);
    RG_DEF_METHOD(disconnect_key, 2);
    RG_DEF_METHOD(query, 2);
#if GTK_CHECK_VERSION(2,2,0)
    RG_DEF_METHOD(find, 0);
#endif
}
