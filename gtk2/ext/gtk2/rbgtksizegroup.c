/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtksizegroup.c -
 
  $Author: mutoh $
  $Date: 2006/11/12 15:28:40 $

  Copyright (C) 2003-2005 Ruby-GNOME2 Project Team
  Copyright (C) 2002,2003 OGASAWARA, Takeshi
************************************************/
#include "global.h"

#define _SELF(self) (GTK_SIZE_GROUP(RVAL2GOBJ(self)))

static VALUE
sizegrp_initialize(self, mode)
    VALUE self, mode;
{
    G_INITIALIZE(self, gtk_size_group_new(RVAL2GENUM(mode, GTK_TYPE_SIZE_GROUP_MODE)));
    return Qnil;
}

static VALUE
sizegrp_add_widget(self, widget)
    VALUE self, widget;
{
    gtk_size_group_add_widget(_SELF(self), GTK_WIDGET(RVAL2GOBJ(widget)));
    return self;
}

static VALUE
sizegrp_remove_widget(self, widget)
    VALUE self, widget;
{
    gtk_size_group_remove_widget(_SELF(self), GTK_WIDGET(RVAL2GOBJ(widget)));
    return self;
}

/* Defined as properties
void        gtk_size_group_set_ignore_hidden
                                            (GtkSizeGroup *size_group,
                                             gboolean ignore_hidden);
gboolean    gtk_size_group_get_ignore_hidden
                                            (GtkSizeGroup *size_group);
*/

#if GTK_CHECK_VERSION(2,10,0)
static VALUE
sizegrp_get_widgets(self)
    VALUE self;
{
    return GSLIST2ARY(gtk_size_group_get_widgets(_SELF(self)));
}
#endif

void
Init_sizegrp()
{
    VALUE gSizeGroup = G_DEF_CLASS(GTK_TYPE_SIZE_GROUP, "SizeGroup", mGtk);

    rb_define_method(gSizeGroup, "initialize", sizegrp_initialize, 1);
    rb_define_method(gSizeGroup, "add_widget", sizegrp_add_widget, 1);
    rb_define_method(gSizeGroup, "remove_widget", sizegrp_remove_widget, 1);

#if GTK_CHECK_VERSION(2,10,0)
    rb_define_method(gSizeGroup, "widgets", sizegrp_get_widgets, 0);
#endif

    /* GtkSizeGroupMode */
    G_DEF_CLASS(GTK_TYPE_SIZE_GROUP_MODE, "Mode", gSizeGroup);
    G_DEF_CONSTANTS(gSizeGroup, GTK_TYPE_SIZE_GROUP_MODE, "GTK_SIZE_GROUP_");
}

