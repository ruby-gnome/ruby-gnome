/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2002,2003 Masahiro Sakai
 *  Copyright (C) 2002-2006 Ruby-GNOME2 Project Team
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

#define RG_TARGET_NAMESPACE cTextChildAnchor
#define _SELF(self) GTK_TEXT_CHILD_ANCHOR(RVAL2GOBJ(self))

static VALUE
textchild_initialize(VALUE self)
{
    RBGTK_INITIALIZE(self, gtk_text_child_anchor_new());
    return Qnil;
}

static VALUE
textchild_get_widgets(VALUE self)
{
    return GLIST2ARYF(gtk_text_child_anchor_get_widgets(_SELF(self)));
}

static VALUE
textchild_get_deleted(VALUE self)
{
    return CBOOL2RVAL(gtk_text_child_anchor_get_deleted(_SELF(self)));
}

void
Init_gtk_textchild(void)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_TEXT_CHILD_ANCHOR, "TextChildAnchor", mGtk);

    rb_define_method(RG_TARGET_NAMESPACE, "initialize", textchild_initialize, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "widgets", textchild_get_widgets, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "deleted?", textchild_get_deleted, 0);
}
