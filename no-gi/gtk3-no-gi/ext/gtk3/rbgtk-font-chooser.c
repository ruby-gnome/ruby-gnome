/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
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

#include "rbgtk3private.h"

#if GTK_CHECK_VERSION(3, 2, 0)

#define RG_TARGET_NAMESPACE mFontChooser
#define _SELF(self) (RVAL2GTKFONTCHOOSER(self))

static VALUE
rg_font_face(VALUE self)
{
    return GOBJ2RVAL(gtk_font_chooser_get_font_face(_SELF(self)));
}

static VALUE
rg_font_family(VALUE self)
{
    return GOBJ2RVAL(gtk_font_chooser_get_font_family(_SELF(self)));
}

static VALUE
rg_font_size(VALUE self)
{
    return INT2NUM(gtk_font_chooser_get_font_size(_SELF(self)));
}

static gboolean
filter_func(const PangoFontFamily *family, const PangoFontFace *face, gpointer func)
{
    return RVAL2CBOOL(rb_funcall((VALUE)func, id_call, 2,
                                 GOBJ2RVAL(family),
                                 GOBJ2RVAL(face)));
}

static VALUE
rg_set_filter_func(VALUE self)
{
    VALUE func = rb_block_proc();
    G_RELATIVE(self, func);
    gtk_font_chooser_set_filter_func(_SELF(self),
                                     (GtkFontFilterFunc)filter_func,
                                     (gpointer)func, NULL);

    return self;
}
#endif

void
Init_gtk_fontchooser(VALUE mGtk)
{
#if GTK_CHECK_VERSION(3, 2, 0)
    VALUE RG_TARGET_NAMESPACE = G_DEF_INTERFACE(GTK_TYPE_FONT_CHOOSER, "FontChooser", mGtk);

    RG_DEF_METHOD(font_face, 0);
    RG_DEF_METHOD(font_family, 0);
    RG_DEF_METHOD(font_size, 0);
    RG_DEF_METHOD(set_filter_func, 0);
#endif
}
