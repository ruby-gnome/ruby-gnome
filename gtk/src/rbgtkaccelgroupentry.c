/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkaccelgroupentry.c -

  $Author: ogs $
  $Date: 2002/12/09 16:33:11 $

  Copyright (C) 2002 OGASAWARA, Takeshi
************************************************/
#include "global.h"

#define _SELF(s) ((GtkAccelGroupEntry*)RVAL2BOXED(s, GTK_TYPE_ACCEL_GROUP_ENTRY))
#define GTK_TYPE_ACCEL_GROUP_ENTRY (gtk_accel_group_entry_get_type())

static GtkAccelGroupEntry*
agentry_copy(const GtkAccelGroupEntry *agentry)
{
    GtkAccelGroupEntry *new_agentry;
    g_return_val_if_fail (agentry != NULL, NULL);
    new_agentry = g_new(GtkAccelGroupEntry, 1);
    *new_agentry = *agentry;
    return new_agentry;
}

GType
gtk_accel_group_entry_get_type()
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
agentry_initialize(self)
    VALUE self;
{
    GtkAccelGroupEntry key;
    G_INITIALIZE(self, &key);
    return Qnil;
}

static VALUE
agentry_get_accel_key(self)
    VALUE self;
{
    return BOXED2RVAL(&_SELF(self)->key, GTK_TYPE_ACCEL_KEY);
}

static VALUE
agentry_get_closure(self)
    VALUE self;
{
    return BOXED2RVAL(_SELF(self)->closure, G_TYPE_CLOSURE);
}

static VALUE
agentry_get_accel_path(self)
    VALUE self;
{
    return CSTR2RVAL(g_quark_to_string(_SELF(self)->accel_path_quark));
}

void
Init_agentry()
{
    VALUE gAccelGroupEntry = G_DEF_CLASS(GTK_TYPE_ACCEL_GROUP_ENTRY, "AccelGroupEntry", mGtk);

    rb_define_method(gAccelGroupEntry, "initialize", agentry_initialize, 0);

    rb_define_method(gAccelGroupEntry, "accel_key", agentry_get_accel_key, 0);
    rb_define_method(gAccelGroupEntry, "closure", agentry_get_closure, 0);
    rb_define_method(gAccelGroupEntry, "accel_path", agentry_get_accel_path, 0);
}
