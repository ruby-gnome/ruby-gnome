/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/* $Id: rbgnome-canvas-buf.c,v 1.1 2005/10/01 19:44:15 mutoh Exp $ */

/* Gnome::CanvasBuf
 *
 * Copyright (C) 2005 Ruby-GNOME2 Project Team
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
#include "rbgnomecanvas.h"
#include <libart_lgpl/art_bpath.h>
#include "rbart.h"

#define _SELF(self) ((GnomeCanvasBuf *)RVAL2BOXED(self, GNOME_TYPE_CANVAS_BUF))

/**********************************/
static GnomeCanvasBuf*
buf_copy(const GnomeCanvasBuf* val)
{
    GnomeCanvasBuf* new_val;
    g_return_val_if_fail (val != NULL, NULL);

    new_val = g_new(GnomeCanvasBuf, 1);
    *new_val = *val;
    return new_val;
}


GType
gnome_canvas_buf_get_type(void)
{
    static GType our_type = 0;

    if (our_type == 0)
        our_type = g_boxed_type_register_static ("GnomeCanvasBuf",
                    (GBoxedCopyFunc)buf_copy,
                    (GBoxedFreeFunc)g_free);
    return our_type;
}

/**********************************/
#define ATTR_BOOL(name)\
static VALUE \
buf_get_ ## name (self)\
    VALUE self;\
{\
    return CBOOL2RVAL(_SELF(self)->name);\
}\
static VALUE \
buf_set_ ## name (self, val)\
    VALUE self, val;\
{\
    _SELF(self)->name = RTEST(val);\
    return self;\
}
/**********************************/

static VALUE
buf_initialize(self)
    VALUE self;
{
    GnomeCanvasBuf buf;

    buf.buf = (guchar*)NULL;
    buf.rect.x0 = 0;
    buf.rect.x1 = 0;
    buf.rect.y0 = 0;
    buf.rect.y1 = 0;

    buf.buf_rowstride = 0;
    buf.bg_color = 0;

    G_INITIALIZE(self, &buf);
    return Qnil;
}

static VALUE
buf_get_buf(self)
    VALUE self;
{
    return CSTR2RVAL((const char*)(_SELF(self)->buf));
}
static VALUE
buf_set_buf(self, buf)
    VALUE self, buf;
{
    _SELF(self)->buf = (guchar*)RVAL2CSTR(buf);
    return self;
}

static VALUE
buf_get_rect(self)
    VALUE self;
{
    ArtIRect rect = _SELF(self)->rect;
    return make_art_irect(&rect);
}
static VALUE
buf_set_rect(self, rect)
    VALUE self, rect;
{
    ArtIRect* irect = get_art_irect(rect);
    _SELF(self)->rect = *irect;
    return self;
}

static VALUE
buf_get_buf_rowstride(self)
    VALUE self;
{
    return INT2NUM(_SELF(self)->buf_rowstride);
}
static VALUE
buf_set_buf_rowstride(self, buf_rowstride)
    VALUE self, buf_rowstride;
{
    _SELF(self)->buf_rowstride = NUM2INT(buf_rowstride);
    return self;
}

static VALUE
buf_get_bg_color(self)
    VALUE self;
{
    return UINT2NUM(_SELF(self)->bg_color);
}
static VALUE
buf_set_bg_color(self, color)
    VALUE self, color;
{
    _SELF(self)->bg_color = NUM2UINT(color);
    return self;
}

ATTR_BOOL(is_bg);
ATTR_BOOL(is_buf);

void
Init_gnome_canvas_buf(mGnome)
    VALUE mGnome;
{
    VALUE gnoCanvasBuf = G_DEF_CLASS(GNOME_TYPE_CANVAS_BUF, "CanvasBuf", mGnome);

    rb_define_method(gnoCanvasBuf, "initialize", buf_initialize, 0);

    rb_define_method(gnoCanvasBuf, "buf", buf_get_buf, 0);
    rb_define_method(gnoCanvasBuf, "set_buf", buf_set_buf, 1);

    rb_define_method(gnoCanvasBuf, "rect", buf_get_rect, 0);
    rb_define_method(gnoCanvasBuf, "set_rect", buf_set_rect, 1);

    rb_define_method(gnoCanvasBuf, "buf_rowstride", buf_get_buf_rowstride, 0);
    rb_define_method(gnoCanvasBuf, "set_buf_rowstride", buf_set_buf_rowstride, 1);

    rb_define_method(gnoCanvasBuf, "bg_color", buf_get_bg_color, 0);
    rb_define_method(gnoCanvasBuf, "set_bg_color", buf_set_bg_color, 1);

    rb_define_method(gnoCanvasBuf, "bg?", buf_get_is_bg, 0);
    rb_define_method(gnoCanvasBuf, "set_bg", buf_set_is_bg, 1);

    rb_define_method(gnoCanvasBuf, "buf?", buf_get_is_buf, 0);
    /* This is special name to avoid the collision from #set_buf */
    rb_define_method(gnoCanvasBuf, "set_is_buf", buf_set_is_buf, 1);

    G_DEF_SETTERS(gnoCanvasBuf);
}
