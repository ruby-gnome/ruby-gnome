/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtktargetlist.c -

  $Author: mutoh $
  $Date: 2003/05/24 11:32:12 $

  Copyright (C) 2003 Masao Mutoh
************************************************/

#include "global.h"

#define _SELF(r) ((GtkTargetList*)RVAL2BOXED(r, GTK_TYPE_TARGET_LIST))

/**********************************/
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
/**********************************/

static VALUE
target_list_initialize(self, targets)
     VALUE self, targets;
{
    Check_Type(targets, T_ARRAY);
    G_INITIALIZE(self, gtk_target_list_new(
                     rbgtk_get_target_entry(targets), 
                     RARRAY(targets)->len));
    return Qnil;
}

static VALUE
target_list_add(self, target, flags, info)
    VALUE self, target, flags, info;
{
    gtk_target_list_add(_SELF(self), RVAL2ATOM(target),
                        FIX2UINT(flags), FIX2UINT(info));
    return self;
}

static VALUE
target_list_add_table(self, targets)
    VALUE self, targets;
{
    gtk_target_list_add_table(_SELF(self),
                              rbgtk_get_target_entry(targets), 
                              RARRAY(targets)->len);
    return self;
}

static VALUE
target_list_remove(self, target)
    VALUE self, target;
{
    gtk_target_list_remove(_SELF(self), RVAL2ATOM(target));
    return self;
}

static VALUE
target_list_find(self, target)
    VALUE self, target;
{
    guint info;
    gboolean ret = gtk_target_list_find(_SELF(self), RVAL2ATOM(target),
                                        &info); 
    return ret ? INT2NUM(info) : Qnil;
} 

void
Init_gtk_target_list()
{
    VALUE gTargetList = G_DEF_CLASS(GTK_TYPE_TARGET_LIST, "TargetList", mGtk);

    rbgobj_boxed_not_copy_obj(GTK_TYPE_TARGET_LIST);

    rb_define_method(gTargetList, "initialize", target_list_initialize, 1);
    rb_define_method(gTargetList, "add", target_list_add, 3);
    rb_define_method(gTargetList, "add_table", target_list_add_table, 1);
    rb_define_method(gTargetList, "remove", target_list_remove, 1);
    rb_define_method(gTargetList, "find", target_list_find, 1);
}
