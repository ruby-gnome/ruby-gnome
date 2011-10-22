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

#define _SELF(s) (GTK_TEXT_MARK(RVAL2GOBJ(s)))

#if GTK_CHECK_VERSION(2,12,0)
static VALUE
initialize(VALUE self, VALUE name, VALUE left_gravity)
{
    if (NIL_P(name))
        G_INITIALIZE(self, gtk_text_mark_new(NULL, RVAL2CBOOL(left_gravity)));
    else
        G_INITIALIZE(self, gtk_text_mark_new(RVAL2CSTR(name), RVAL2CBOOL(left_gravity)));
    return Qnil;
}
#endif

static VALUE
set_visible(VALUE self, VALUE setting)
{
    gtk_text_mark_set_visible(_SELF(self), RVAL2CBOOL(setting));
    return setting;
}

static VALUE
get_visible(VALUE self)
{
    return CBOOL2RVAL(gtk_text_mark_get_visible(_SELF(self)));
}

/* Defined as Properties:
const gchar *       gtk_text_mark_get_name              (GtkTextMark *mark);
*/

static VALUE
get_deleted(VALUE self)
{
    return CBOOL2RVAL(gtk_text_mark_get_deleted(_SELF(self)));
}

static VALUE
get_buffer(VALUE self)
{
    return GOBJ2RVAL(gtk_text_mark_get_buffer(_SELF(self)));
}

/* Defined as Properties:
gboolean            gtk_text_mark_get_left_gravity      (GtkTextMark *mark);
*/

void 
Init_gtk_textmark(void)
{
    VALUE gTextMark = G_DEF_CLASS(GTK_TYPE_TEXT_MARK, "TextMark", mGtk);
#if GTK_CHECK_VERSION(2,12,0)
    rb_define_method(gTextMark, "initialize", initialize, 2);
#endif
    rb_define_method(gTextMark, "set_visible", set_visible, 1);
    G_DEF_SETTER(gTextMark, "visible");
    rb_define_method(gTextMark, "visible?", get_visible, 0);
    rb_define_method(gTextMark, "deleted?", get_deleted, 0);
    rb_define_method(gTextMark, "buffer", get_buffer, 0);
}
