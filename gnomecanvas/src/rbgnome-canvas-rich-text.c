/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/* $Id: rbgnome-canvas-rich-text.c,v 1.2 2002/10/03 13:09:59 tkubo Exp $ */

/* Gnome::CanvasRichText widget for Ruby/Gnome
 * Copyright (C) 2002 KUBO Takehiro <kubo@jiubao.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */
#include "rbgnomecanvas.h"

#define _SELF(self) GNOME_CANVAS_RICH_TEXT(RVAL2GOBJ(self))

static VALUE
crtext_cut_clipboard(self)
    VALUE self;
{
    gnome_canvas_rich_text_cut_clipboard(_SELF(self));
    return self;
}

static VALUE
crtext_copy_clipboard(self)
    VALUE self;
{
    gnome_canvas_rich_text_copy_clipboard(_SELF(self));
    return self;
}

static VALUE
crtext_paste_clipboard(self)
    VALUE self;
{
    gnome_canvas_rich_text_paste_clipboard(_SELF(self));
    return self;
}

static VALUE
crtext_set_buffer(self, buffer)
    VALUE self, buffer;
{
    gnome_canvas_rich_text_set_buffer(_SELF(self), GTK_TEXT_BUFFER(RVAL2GOBJ(buffer)));
    return self;
}

static VALUE
crtext_get_buffer(self)
    VALUE self;
{
    return GOBJ2RVAL(gnome_canvas_rich_text_get_buffer(_SELF(self)));
}

static VALUE
crtext_get_iter_location(self, iter)
    VALUE self, iter;
{
    GdkRectangle location;
    gnome_canvas_rich_text_get_iter_location(_SELF(self),
                                             (GtkTextIter *)RVAL2BOXED(iter, GTK_TYPE_TEXT_ITER),
                                             &location);
    return BOXED2RVAL(&location, GDK_TYPE_RECTANGLE);
}

static VALUE
crtext_get_iter_at_location(self, x, y)
    VALUE self, x, y;
{
    GtkTextIter iter;
    gnome_canvas_rich_text_get_iter_at_location(_SELF(self),
                                                &iter,
                                                NUM2INT(x),
                                                NUM2INT(y));
    return BOXED2RVAL(&iter, GTK_TYPE_TEXT_ITER);
}

void
Init_gnome_canvas_rich_text(mGnome)
    VALUE mGnome;
{
    VALUE gnoCanvasRichText = G_DEF_CLASS(GNOME_TYPE_CANVAS_RICH_TEXT, "CanvasRichText", mGnome);

    rb_define_method(gnoCanvasRichText, "cut_clipboard", crtext_cut_clipboard, 0);
    rb_define_method(gnoCanvasRichText, "copy_clipboard", crtext_copy_clipboard, 0);
    rb_define_method(gnoCanvasRichText, "paste_clipboard", crtext_paste_clipboard, 0);
    rb_define_method(gnoCanvasRichText, "set_buffer", crtext_set_buffer, 1);
    rb_define_method(gnoCanvasRichText, "get_buffer", crtext_get_buffer, 0);
    rb_define_method(gnoCanvasRichText, "get_iter_location", crtext_get_iter_location, 1);
    rb_define_method(gnoCanvasRichText, "get_iter_at_location", crtext_get_iter_at_location, 2);
}
