/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2002,2003 OGASAWARA, Takeshi
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

#define RG_TARGET_NAMESPACE cAccelGroupEntry
#define _SELF(s) ((GtkAccelGroupEntry*)RVAL2BOXED(s, GTK_TYPE_ACCEL_GROUP_ENTRY))

static GtkAccelGroupEntry *
agentry_copy(const GtkAccelGroupEntry *agentry)
{
    GtkAccelGroupEntry *new_agentry;
    g_return_val_if_fail (agentry != NULL, NULL);
    new_agentry = g_new(GtkAccelGroupEntry, 1);
    *new_agentry = *agentry;
    return new_agentry;
}

GType
gtk_accel_group_entry_get_type(void)
{
    static GType our_type = 0;
    if(our_type == 0)
        our_type = g_boxed_type_register_static("GtkAccelGroupEntry",
                                                (GBoxedCopyFunc)agentry_copy,
                                                (GBoxedFreeFunc)g_free);
    return our_type;
}

/*
struct _GtkAccelGroupEntry
{
  GtkAccelKey  key;
  GClosure    *closure;
  GQuark       accel_path_quark;
};
*/
static VALUE
agentry_initialize(VALUE self)
{
    GtkAccelGroupEntry key;
    G_INITIALIZE(self, &key);
    return Qnil;
}

static VALUE
agentry_get_accel_key(VALUE self)
{
    return BOXED2RVAL(&_SELF(self)->key, GTK_TYPE_ACCEL_KEY);
}

static VALUE
agentry_get_closure(VALUE self)
{
    return BOXED2RVAL(_SELF(self)->closure, G_TYPE_CLOSURE);
}

static VALUE
agentry_get_accel_path(VALUE self)
{
    const gchar *quark_str = g_quark_to_string(_SELF(self)->accel_path_quark);
    return quark_str ? CSTR2RVAL(quark_str) : Qnil;
}

void
Init_gtk_accel_group_entry(void)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_ACCEL_GROUP_ENTRY, "AccelGroupEntry", mGtk);

    rb_define_method(RG_TARGET_NAMESPACE, "initialize", agentry_initialize, 0);

    rb_define_method(RG_TARGET_NAMESPACE, "accel_key", agentry_get_accel_key, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "closure", agentry_get_closure, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "accel_path", agentry_get_accel_path, 0);
}
