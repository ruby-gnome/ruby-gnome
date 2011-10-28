/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2003-2005 Ruby-GNOME2 Project Team
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

#define RG_TARGET_NAMESPACE cSizeGroup
#define _SELF(self) (GTK_SIZE_GROUP(RVAL2GOBJ(self)))

static VALUE
sizegrp_initialize(VALUE self, VALUE mode)
{
    G_INITIALIZE(self, gtk_size_group_new(RVAL2GENUM(mode, GTK_TYPE_SIZE_GROUP_MODE)));
    return Qnil;
}

static VALUE
sizegrp_add_widget(VALUE self, VALUE widget)
{
    gtk_size_group_add_widget(_SELF(self), GTK_WIDGET(RVAL2GOBJ(widget)));
    return self;
}

static VALUE
sizegrp_remove_widget(VALUE self, VALUE widget)
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
sizegrp_get_widgets(VALUE self)
{
    return GSLIST2ARY(gtk_size_group_get_widgets(_SELF(self)));
}
#endif

void
Init_gtk_size_group(void)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_SIZE_GROUP, "SizeGroup", mGtk);

    rb_define_method(RG_TARGET_NAMESPACE, "initialize", sizegrp_initialize, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "add_widget", sizegrp_add_widget, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "remove_widget", sizegrp_remove_widget, 1);

#if GTK_CHECK_VERSION(2,10,0)
    rb_define_method(RG_TARGET_NAMESPACE, "widgets", sizegrp_get_widgets, 0);
#endif

    /* GtkSizeGroupMode */
    G_DEF_CLASS(GTK_TYPE_SIZE_GROUP_MODE, "Mode", RG_TARGET_NAMESPACE);
    G_DEF_CONSTANTS(RG_TARGET_NAMESPACE, GTK_TYPE_SIZE_GROUP_MODE, "GTK_SIZE_GROUP_");
}

