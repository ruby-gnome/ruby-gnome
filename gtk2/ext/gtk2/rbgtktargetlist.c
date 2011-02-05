/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtktargetlist.c -

  $Author: ggc $
  $Date: 2007/07/13 16:07:32 $

  Copyright (C) 2003-2005 Masao Mutoh
************************************************/

#include "global.h"

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
target_list_initialize(VALUE self, VALUE targets)
{
    Check_Type(targets, T_ARRAY);
    G_INITIALIZE(self, gtk_target_list_new(
                     rbgtk_get_target_entry(targets), 
                     RARRAY_LEN(targets)));
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
target_list_add_table(VALUE self, VALUE targets)
{
    gtk_target_list_add_table(_SELF(self),
                              rbgtk_get_target_entry(targets), 
                              RARRAY_LEN(targets));
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
Init_gtk_target_list()
{
    VALUE gTargetList = G_DEF_CLASS(GTK_TYPE_TARGET_LIST, "TargetList", mGtk);

    rbgobj_boxed_not_copy_obj(GTK_TYPE_TARGET_LIST);

    rb_define_method(gTargetList, "initialize", target_list_initialize, 1);
    rb_define_method(gTargetList, "add", target_list_add, 3);
    rb_define_method(gTargetList, "add_table", target_list_add_table, 1);
#if GTK_CHECK_VERSION(2,6,0)
    rb_define_method(gTargetList, "add_text_targets", target_list_add_text_targets, 1);
    rb_define_method(gTargetList, "add_image_targets", target_list_add_image_targets, 2);
    rb_define_method(gTargetList, "add_uri_targets", target_list_add_uri_targets, 1);
#endif
#if GTK_CHECK_VERSION(2,10,0)
    rb_define_method(gTargetList, "add_rich_text_targets", target_list_add_rich_text_targets, 3);
#endif
    rb_define_method(gTargetList, "remove", target_list_remove, 1);
    rb_define_method(gTargetList, "find", target_list_find, 1);
}
