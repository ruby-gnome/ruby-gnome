/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/* $Id: rbgnome-druid-page-standard.c,v 1.3 2003/02/02 12:51:06 tkubo Exp $ */

/* Gnome::DruidPageStandard widget for Ruby/GNOME2
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

#define _SELF(self) GNOME_DRUID_PAGE_STANDARD(RVAL2GOBJ(self))
#define _PIXBUF(obj) GDK_PIXBUF(RVAL2GOBJ(obj))
#define _COLOR(obj) ((GdkColor*)RVAL2BOXED(obj, GDK_TYPE_COLOR))

static VALUE
dstandard_initialize(argc, argv, self)
    int argc;
    VALUE *argv, self;
{
    VALUE title, logo, top_watermark;
    GtkWidget *result;

    if (argc != 0 && argc != 3)
        rb_raise(rb_eArgError, "wrong # of arguments(%d for 0 or 3)", argc);
    rb_scan_args(argc, argv, "03", &title, &logo, &top_watermark);
    if (argc == 0) {
        result = gnome_druid_page_standard_new();
    } else if (argc == 3) {
        result = gnome_druid_page_standard_new_with_vals(RVAL2CSTR(title),
                                                         NIL_P(logo) ? NULL : _PIXBUF(logo),
                                                         NIL_P(top_watermark) ? NULL : _PIXBUF(top_watermark));
    } else {
        rb_bug("report to Ruby/GNOME2 maintainer (dstandard_initialize)");
    }
    RBGTK_INITIALIZE(self, result);
    return Qnil;
}

/* Why Isn't this defined as property? */
static VALUE
dstandard_set_contents_background(self, color)
    VALUE self, color;
{
    gnome_druid_page_standard_set_contents_background(_SELF(self), _COLOR(color));
    return self;
}

static VALUE
dstandard_append_item(self, question, item, additional_info)
    VALUE self, question, item, additional_info;
{
    gnome_druid_page_standard_append_item(_SELF(self),
                                          NIL_P(question) ? NULL : RVAL2CSTR(question),
                                          GTK_WIDGET(RVAL2GOBJ(item)),
                                          NIL_P(additional_info) ? NULL : RVAL2CSTR(additional_info));
    return self;
}

void
Init_gnome_druid_page_standard(mGnome)
    VALUE mGnome;
{
    VALUE gnoDruidPageStandard = G_DEF_CLASS(GNOME_TYPE_DRUID_PAGE_STANDARD, "DruidPageStandard", mGnome);

    rb_define_method(gnoDruidPageStandard, "initialize", dstandard_initialize, -1);
    rb_define_method(gnoDruidPageStandard, "set_contents_background", dstandard_set_contents_background, 1);
    rb_define_method(gnoDruidPageStandard, "append_item", dstandard_append_item, 3);

    G_DEF_SETTERS(gnoDruidPageStandard);
}
