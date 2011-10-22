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

#define _SELF(s) ((GtkIconSet*)RVAL2BOXED(s, GTK_TYPE_ICON_SET))

static VALUE
icon_set_initialize(int argc, VALUE *argv, VALUE self)
{
    GtkIconSet *icon_set;
    VALUE pixbuf;

    rb_scan_args(argc, argv, "01", &pixbuf);

    if(NIL_P(pixbuf))
        icon_set = gtk_icon_set_new();
    else
        icon_set = gtk_icon_set_new_from_pixbuf(RVAL2GOBJ(pixbuf));

    G_INITIALIZE(self, icon_set);
    return Qnil;
}

static VALUE
icon_set_add_source(VALUE self, VALUE source)
{
    gtk_icon_set_add_source(_SELF(self), RVAL2BOXED(source, GTK_TYPE_ICON_SOURCE));
    return self;
}

static VALUE
icon_set_get_sizes(VALUE self)
{
    GtkIconSize *sizes;
    gint n_sizes;
    VALUE result;
    int n;

    gtk_icon_set_get_sizes(_SELF(self), &sizes, &n_sizes);
    result = rb_ary_new();
    for(n=0; n<n_sizes; n++, sizes++)
        rb_ary_push(result, GENUM2RVAL(*sizes, GTK_TYPE_ICON_SIZE));
    return result;
}

static VALUE
icon_set_render_icon(int argc, VALUE *argv, VALUE self)
{
    VALUE style, direction, state, size, widget, detail;

    rb_scan_args(argc, argv, "42", &style, &direction, &state, &size, &widget, &detail);
    return GOBJ2RVAL(gtk_icon_set_render_icon(_SELF(self),
                                              RVAL2GOBJ(style),
                                              RVAL2GENUM(direction, GTK_TYPE_TEXT_DIRECTION),
                                              RVAL2GENUM(state, GTK_TYPE_STATE_TYPE),
                                              RVAL2GENUM(size, GTK_TYPE_ICON_SIZE),
                                              NIL_P(widget) ? NULL : RVAL2GOBJ(widget),
                                              NIL_P(detail) ? NULL : RVAL2CSTR(detail)));
}

void
Init_gtk_icon_set(void)
{
    VALUE gIconSet = G_DEF_CLASS(GTK_TYPE_ICON_SET, "IconSet", mGtk);

    rb_define_method(gIconSet, "initialize", icon_set_initialize, -1);
    
    rb_define_method(gIconSet, "add_source", icon_set_add_source, 1);
    rb_define_method(gIconSet, "sizes", icon_set_get_sizes, 0);
    rb_define_method(gIconSet, "render_icon", icon_set_render_icon, -1);
}
