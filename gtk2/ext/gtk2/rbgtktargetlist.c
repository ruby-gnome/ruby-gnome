/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2003-2005 Masao Mutoh
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

#define RG_TARGET_NAMESPACE cTargetList
#define _SELF(r) ((GtkTargetList*)RVAL2BOXED(r, GTK_TYPE_TARGET_LIST))

/**********************************/
#if ! GTK_CHECK_VERSION(2,10,0) 
static GtkTargetList*
rbgtk_target_list_copy(const GtkTargetList* list)
{
  GtkTargetList* new_list;
  g_return_val_if_fail (list != NULL, NULL);
  new_list = g_new(GtkTargetList, 1);
  *new_list = *list;
  return new_list;
}

GType
gtk_target_list_get_type(void)
{
    static GType our_type = 0;

    if (our_type == 0)
        our_type = g_boxed_type_register_static ("GtkTargetList",
                    (GBoxedCopyFunc)rbgtk_target_list_copy,
                    (GBoxedFreeFunc)gtk_target_list_unref);
    return our_type;
}
#endif
/**********************************/

static VALUE
target_list_initialize(VALUE self, VALUE rbtargets)
{
    long n;
    GtkTargetEntry *targets = RVAL2GTKTARGETENTRIES(rbtargets, &n);
    GtkTargetList *list = gtk_target_list_new(targets, n);

    g_free(targets);

    G_INITIALIZE(self, list);

    return Qnil;
}

static VALUE
target_list_add(VALUE self, VALUE target, VALUE flags, VALUE info)
{
    gtk_target_list_add(_SELF(self), RVAL2ATOM(target),
                        FIX2UINT(flags), FIX2UINT(info));
    return self;
}

static VALUE
target_list_add_table(VALUE self, VALUE rbtargets)
{
    GtkTargetList *list = _SELF(self);
    long n;
    GtkTargetEntry *targets = RVAL2GTKTARGETENTRIES(rbtargets, &n);

    gtk_target_list_add_table(list, targets, n);

    g_free(targets);

    return self;
}

#if GTK_CHECK_VERSION(2,6,0)
static VALUE
target_list_add_text_targets(VALUE self, VALUE info)
{
    gtk_target_list_add_text_targets(_SELF(self), NUM2UINT(info));
    return self;
}

static VALUE
target_list_add_image_targets(VALUE self, VALUE info, VALUE writable)
{
    gtk_target_list_add_image_targets(_SELF(self), NUM2UINT(info), RVAL2CBOOL(writable));
    return self;
}

static VALUE
target_list_add_uri_targets(VALUE self, VALUE info)
{
    gtk_target_list_add_uri_targets(_SELF(self), NUM2UINT(info));
    return self;
}
#endif

#if GTK_CHECK_VERSION(2,10,0)
static VALUE
target_list_add_rich_text_targets(VALUE self, VALUE info, VALUE deserializable, VALUE buffer)
{
    gtk_target_list_add_rich_text_targets(_SELF(self), NUM2UINT(info),
                                          RVAL2CBOOL(deserializable),
                                          GTK_TEXT_BUFFER(RVAL2GOBJ(buffer)));
    return self;
}
#endif

static VALUE
target_list_remove(VALUE self, VALUE target)
{
    gtk_target_list_remove(_SELF(self), RVAL2ATOM(target));
    return self;
}

static VALUE
target_list_find(VALUE self, VALUE target)
{
    guint info;
    gboolean ret = gtk_target_list_find(_SELF(self), RVAL2ATOM(target),
                                        &info); 
    return ret ? INT2NUM(info) : Qnil;
} 

void
Init_gtk_target_list(void)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_TARGET_LIST, "TargetList", mGtk);

    rbgobj_boxed_not_copy_obj(GTK_TYPE_TARGET_LIST);

    rb_define_method(RG_TARGET_NAMESPACE, "initialize", target_list_initialize, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "add", target_list_add, 3);
    rb_define_method(RG_TARGET_NAMESPACE, "add_table", target_list_add_table, 1);
#if GTK_CHECK_VERSION(2,6,0)
    rb_define_method(RG_TARGET_NAMESPACE, "add_text_targets", target_list_add_text_targets, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "add_image_targets", target_list_add_image_targets, 2);
    rb_define_method(RG_TARGET_NAMESPACE, "add_uri_targets", target_list_add_uri_targets, 1);
#endif
#if GTK_CHECK_VERSION(2,10,0)
    rb_define_method(RG_TARGET_NAMESPACE, "add_rich_text_targets", target_list_add_rich_text_targets, 3);
#endif
    rb_define_method(RG_TARGET_NAMESPACE, "remove", target_list_remove, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "find", target_list_find, 1);
}
