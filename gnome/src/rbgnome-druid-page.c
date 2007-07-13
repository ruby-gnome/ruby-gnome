/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/* $Id: rbgnome-druid-page.c,v 1.3 2007/07/13 14:27:07 ggc Exp $ */

/* Gnome::DruidPage widget for Ruby/GNOME2
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

#define _SELF(self) GNOME_DRUID_PAGE(RVAL2GOBJ(self))

static VALUE
dpage_initialize(self)
    VALUE self;
{
    RBGTK_INITIALIZE(self, gnome_druid_page_new());
    return Qnil;
}

static VALUE
dpage_next(self)
    VALUE self;
{
    return CBOOL2RVAL(gnome_druid_page_next(_SELF(self)));
}

static VALUE
dpage_prepare(self)
    VALUE self;
{
    gnome_druid_page_prepare(_SELF(self));
    return self;
}

static VALUE
dpage_back(self)
    VALUE self;
{
    return CBOOL2RVAL(gnome_druid_page_back(_SELF(self)));
}

static VALUE
dpage_cancel(self)
    VALUE self;
{
    return CBOOL2RVAL(gnome_druid_page_cancel(_SELF(self)));
}

static VALUE
dpage_finish(self)
    VALUE self;
{
    gnome_druid_page_finish(_SELF(self));
    return self;
}

void
Init_gnome_druid_page(mGnome)
    VALUE mGnome;
{
    VALUE gnoDruidPage = G_DEF_CLASS(GNOME_TYPE_DRUID_PAGE, "DruidPage", mGnome);

    rb_define_method(gnoDruidPage, "initialize", dpage_initialize, 0);
    rb_define_method(gnoDruidPage, "next", dpage_next, 0);
    rb_define_method(gnoDruidPage, "prepare", dpage_prepare, 0);
    rb_define_method(gnoDruidPage, "back", dpage_back, 0);
    rb_define_method(gnoDruidPage, "cancel", dpage_cancel, 0);
    rb_define_method(gnoDruidPage, "finish", dpage_finish, 0);
}
