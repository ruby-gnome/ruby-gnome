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

#include "rbgtksourceview3private.h"

#define RG_TARGET_NAMESPACE cGutterRenderer
#define _SELF(self) (RVAL2GTKSOURCEGUTTERRENDERER(self))

static VALUE
rg_end(VALUE self)
{
    gtk_source_gutter_renderer_end(_SELF(self));

    return self;
}

static VALUE
rg_begin(VALUE self, VALUE cr, VALUE background_area, VALUE cell_area, VALUE start, VALUE end)
{
    gtk_source_gutter_renderer_begin(_SELF(self),
                                     RVAL2CRCONTEXT(cr),
                                     RVAL2GDKRECTANGLE(background_area),
                                     RVAL2GDKRECTANGLE(cell_area),
                                     RVAL2GTKTEXTITER(start),
                                     RVAL2GTKTEXTITER(end));
    if (rb_block_given_p())
        rb_ensure(rb_yield, self, rg_end, self);

    return self;
}

static VALUE
rg_draw(VALUE self, VALUE cr, VALUE background_area, VALUE cell_area, VALUE start, VALUE end, VALUE state)
{
    gtk_source_gutter_renderer_draw(_SELF(self),
                                    RVAL2CRCONTEXT(cr),
                                    RVAL2GDKRECTANGLE(background_area),
                                    RVAL2GDKRECTANGLE(cell_area),
                                    RVAL2GTKTEXTITER(start),
                                    RVAL2GTKTEXTITER(end),
                                    RVAL2GTKSOURCEGUTTERRENDERERSTATE(state));

    return self;
}

static VALUE
rg_alignment(VALUE self)
{
    gfloat xalign, yalign;

    gtk_source_gutter_renderer_get_alignment(_SELF(self), &xalign, &yalign);

    return rb_ary_new3(2, xalign, yalign);
}

static VALUE
rg_background(VALUE self)
{
    GdkRGBA color;
    gboolean result;

    result = gtk_source_gutter_renderer_get_background(_SELF(self), &color);

    return result ? GDKRGBA2RVAL(&color) : Qnil;
}

static VALUE
rg_padding(VALUE self)
{
    gint xpad, ypad;

    gtk_source_gutter_renderer_get_padding(_SELF(self), &xpad, &ypad);

    return rb_ary_new3(2, INT2NUM(xpad), INT2NUM(ypad));
}

static VALUE
rg_queue_draw(VALUE self)
{
    gtk_source_gutter_renderer_queue_draw(_SELF(self));

    return self;
}

static VALUE
rg_set_alignment(VALUE self, VALUE xalign, VALUE yalign)
{
    gtk_source_gutter_renderer_set_alignment(_SELF(self), NUM2DBL(xalign), NUM2DBL(yalign));

    return self;
}

static VALUE
rg_set_background(VALUE self, VALUE color)
{
    gtk_source_gutter_renderer_set_background(_SELF(self), RVAL2GDKRGBA(color));

    return self;
}

static VALUE
rg_set_padding(VALUE self, VALUE xpad, VALUE ypad)
{
    gtk_source_gutter_renderer_set_padding(_SELF(self), NUM2INT(xpad), NUM2INT(ypad));

    return self;
}

void
Init_gtksource_gutterrenderer(VALUE mGtkSource)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_SOURCE_TYPE_GUTTER_RENDERER, "GutterRenderer", mGtkSource);
    G_DEF_CLASS(GTK_SOURCE_TYPE_GUTTER_RENDERER_ALIGNMENT_MODE, "AlignmentMode", RG_TARGET_NAMESPACE);
    G_DEF_CLASS(GTK_SOURCE_TYPE_GUTTER_RENDERER_STATE, "State", RG_TARGET_NAMESPACE);

    RG_DEF_METHOD(alignment, 0);
    RG_DEF_METHOD(background, 0);
    RG_DEF_METHOD(begin, 5);
    RG_DEF_METHOD(draw, 6);
    RG_DEF_METHOD(end, 0);
    RG_DEF_METHOD(padding, 0);
    RG_DEF_METHOD(queue_draw, 0);
    RG_DEF_METHOD(set_alignment, 2);
    RG_DEF_METHOD(set_background, 1);
    RG_DEF_METHOD(set_padding, 2);
}
