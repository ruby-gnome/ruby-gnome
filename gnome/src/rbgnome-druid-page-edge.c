/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/* $Id: rbgnome-druid-page-edge.c,v 1.3 2003/11/08 18:49:45 mutoh Exp $ */

/* Gnome::DruidPageEdge widget for Ruby/GNOME2
 * Copyright (C) 2002-2003 Ruby-GNOME2 Project Team
 * Copyright (C) 2002      KUBO Takehiro <kubo@jiubao.org>
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

#include "rbgnome.h"

#define _SELF(self) GNOME_DRUID_PAGE_EDGE(RVAL2GOBJ(self))
#define _PIXBUF(obj) GDK_PIXBUF(RVAL2GOBJ(obj))
#define _COLOR(obj) ((GdkColor*)RVAL2BOXED(obj, GDK_TYPE_COLOR))

static VALUE
dedge_initialize(argc, argv, self)
    int argc;
    VALUE *argv, self;
{
    VALUE position, antialiased, title, text, logo, watermark, top_watermark;
    GtkWidget *result;

    rb_scan_args(argc, argv, "16", &position, &antialiased, &title, &text, &logo, &watermark, &top_watermark);
    result = gnome_druid_page_edge_new_with_vals(RVAL2GENUM(position, GNOME_TYPE_EDGE_POSITION),
                                                 RTEST(antialiased),
                                                 NIL_P(title) ? NULL : RVAL2CSTR(title),
                                                 NIL_P(text) ? NULL : RVAL2CSTR(text),
                                                 NIL_P(logo) ? NULL : _PIXBUF(logo),
                                                 NIL_P(watermark) ? NULL : _PIXBUF(watermark),
                                                 NIL_P(top_watermark) ? NULL : _PIXBUF(top_watermark));
    RBGTK_INITIALIZE(self, result);
    return Qnil;
}

static VALUE
dedge_set_bg_color(self, color)
    VALUE self, color;
{
    gnome_druid_page_edge_set_bg_color(_SELF(self), _COLOR(color));
    return self;
}

static VALUE
dedge_set_textbox_color(self, color)
    VALUE self, color;
{
    gnome_druid_page_edge_set_textbox_color(_SELF(self), _COLOR(color));
    return self;
}

static VALUE
dedge_set_logo_bg_color(self, color)
    VALUE self, color;
{
    gnome_druid_page_edge_set_logo_bg_color(_SELF(self), _COLOR(color));
    return self;
}

static VALUE
dedge_set_title_color(self, color)
    VALUE self, color;
{
    gnome_druid_page_edge_set_title_color(_SELF(self), _COLOR(color));
    return self;
}

static VALUE
dedge_set_text_color(self, color)
    VALUE self, color;
{
    gnome_druid_page_edge_set_text_color(_SELF(self), _COLOR(color));
    return self;
}

static VALUE
dedge_set_text(self, text)
    VALUE self, text;
{
    gnome_druid_page_edge_set_text(_SELF(self), RVAL2CSTR(text));
    return self;
}

static VALUE
dedge_set_title(self, title)
    VALUE self, title;
{
    gnome_druid_page_edge_set_title(_SELF(self), RVAL2CSTR(title));
    return self;
}

static VALUE
dedge_set_logo(self, logo_image)
    VALUE self, logo_image;
{
    gnome_druid_page_edge_set_logo(_SELF(self), _PIXBUF(logo_image));
    return self;
}

static VALUE
dedge_set_watermark(self, watermark)
    VALUE self, watermark;
{
    gnome_druid_page_edge_set_watermark(_SELF(self), _PIXBUF(watermark));
    return self;
}

static VALUE
dedge_set_top_watermark(self, top_watermark_image)
    VALUE self, top_watermark_image;
{
    gnome_druid_page_edge_set_top_watermark(_SELF(self), _PIXBUF(top_watermark_image));
    return self;
}

void
Init_gnome_druid_page_edge(mGnome)
    VALUE mGnome;
{
    VALUE gnoDruidPageEdge = G_DEF_CLASS(GNOME_TYPE_DRUID_PAGE_EDGE, "DruidPageEdge", mGnome);

    /* GnomeEdgePosition */
    G_DEF_CLASS(GNOME_TYPE_EDGE_POSITION, "EdgePosition", mGnome);
    G_DEF_CONSTANTS(mGnome, GNOME_TYPE_EDGE_POSITION, "GNOME_");

    rb_define_method(gnoDruidPageEdge, "initialize", dedge_initialize, -1);
    rb_define_method(gnoDruidPageEdge, "set_bg_color", dedge_set_bg_color, 1);
    rb_define_method(gnoDruidPageEdge, "set_textbox_color", dedge_set_textbox_color, 1);
    rb_define_method(gnoDruidPageEdge, "set_logo_bg_color", dedge_set_logo_bg_color, 1);
    rb_define_method(gnoDruidPageEdge, "set_title_color", dedge_set_title_color, 1);
    rb_define_method(gnoDruidPageEdge, "set_text_color", dedge_set_text_color, 1);
    rb_define_method(gnoDruidPageEdge, "set_text", dedge_set_text, 1);
    rb_define_method(gnoDruidPageEdge, "set_title", dedge_set_title, 1);
    rb_define_method(gnoDruidPageEdge, "set_logo", dedge_set_logo, 1);
    rb_define_method(gnoDruidPageEdge, "set_watermark", dedge_set_watermark, 1);
    rb_define_method(gnoDruidPageEdge, "set_top_watermark", dedge_set_top_watermark, 1);

    G_DEF_SETTERS(gnoDruidPageEdge);
}
