/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/* $Id: rbgnome-icon-item.c,v 1.2 2003/02/02 12:51:06 tkubo Exp $ */
/* based on libgnomeui/gnome-icon-item.h */

/* Gnome::IconTextItem widget for Ruby/GNOME2
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

#define _SELF(self) GNOME_ICON_TEXT_ITEM(RVAL2GOBJ(self))

static VALUE
iti_configure(self, x, y, width, fontname, text, is_editable)
    VALUE self, x, y, width, fontname, text, is_editable;
{
    gnome_icon_text_item_configure(_SELF(self), NUM2INT(x), NUM2INT(y),
                                   NUM2INT(width), RVAL2CSTR(fontname),
                                   RVAL2CSTR(text), RTEST(is_editable),
                                   FALSE);
    return self;
}

static VALUE
iti_setxy(self, x, y)
    VALUE self, x, y;
{
    gnome_icon_text_item_setxy(_SELF(self), NUM2INT(x), NUM2INT(y));
    return self;
}

static VALUE
iti_select(self, sel)
    VALUE self, sel;
{
    gnome_icon_text_item_select(_SELF(self), RTEST(sel));
    return self;
}

static VALUE
iti_focus(self, focused)
    VALUE self, focused;
{
    gnome_icon_text_item_focus(_SELF(self), RTEST(focused));
    return self;
}

static VALUE
iti_get_text(self)
    VALUE self;
{
    const char *result = gnome_icon_text_item_get_text(_SELF(self));
    return result ? rb_str_new2(result) : Qnil;
}

static VALUE
iti_start_editing(self)
    VALUE self;
{
    gnome_icon_text_item_start_editing(_SELF(self));
    return self;
}

static VALUE
iti_stop_editing(self, accept)
    VALUE self, accept;
{
    gnome_icon_text_item_stop_editing(_SELF(self), RTEST(accept));
    return self;
}

static VALUE
iti_get_editable(self)
    VALUE self;
{
    return GOBJ2RVAL(gnome_icon_text_item_get_editable(_SELF(self)));
}

void
Init_gnome_icon_item(mGnome)
    VALUE mGnome;
{
    VALUE gnoIconTextItem = G_DEF_CLASS(GNOME_TYPE_ICON_TEXT_ITEM, "IconTextItem", mGnome);

    /* Instance methods */
    rb_undef_method(gnoIconTextItem, "new");
    rb_define_method(gnoIconTextItem, "configure", iti_configure, 6);
    rb_define_method(gnoIconTextItem, "setxy", iti_setxy, 2);
    rb_define_method(gnoIconTextItem, "select", iti_select, 1);
    rb_define_method(gnoIconTextItem, "focus", iti_focus, 1);
    rb_define_method(gnoIconTextItem, "text", iti_get_text, 0);
    rb_define_method(gnoIconTextItem, "start_editing", iti_start_editing, 0);
    rb_define_method(gnoIconTextItem, "stop_editing", iti_stop_editing, 1);
    rb_define_method(gnoIconTextItem, "editable", iti_get_editable, 0);
}
