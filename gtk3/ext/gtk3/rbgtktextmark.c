/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2002,2003 Masahiro Sakai
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

#define RG_TARGET_NAMESPACE cTextMark
#define _SELF(s) (GTK_TEXT_MARK(RVAL2GOBJ(s)))

#if GTK_CHECK_VERSION(2,12,0)
static VALUE
rg_initialize(VALUE self, VALUE name, VALUE left_gravity)
{
    if (NIL_P(name))
        G_INITIALIZE(self, gtk_text_mark_new(NULL, RVAL2CBOOL(left_gravity)));
    else
        G_INITIALIZE(self, gtk_text_mark_new(RVAL2CSTR(name), RVAL2CBOOL(left_gravity)));
    return Qnil;
}
#endif

static VALUE
rg_set_visible(VALUE self, VALUE setting)
{
    gtk_text_mark_set_visible(_SELF(self), RVAL2CBOOL(setting));
    return setting;
}

static VALUE
rg_visible_p(VALUE self)
{
    return CBOOL2RVAL(gtk_text_mark_get_visible(_SELF(self)));
}

static VALUE
rg_deleted_p(VALUE self)
{
    return CBOOL2RVAL(gtk_text_mark_get_deleted(_SELF(self)));
}

static VALUE
rg_buffer(VALUE self)
{
    return GOBJ2RVAL(gtk_text_mark_get_buffer(_SELF(self)));
}

void 
Init_gtk_textmark(VALUE mGtk)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_TEXT_MARK, "TextMark", mGtk);
#if GTK_CHECK_VERSION(2,12,0)
    RG_DEF_METHOD(initialize, 2);
#endif
    RG_DEF_METHOD(set_visible, 1);
    G_DEF_SETTER(RG_TARGET_NAMESPACE, "visible");
    RG_DEF_METHOD_P(visible, 0);
    RG_DEF_METHOD_P(deleted, 0);
    RG_DEF_METHOD(buffer, 0);
}
