/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2008-2009  Ruby-GNOME2 Project Team
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

#include "rbgio2private.h"

#define RG_TARGET_NAMESPACE cEmblemedIcon
#define _SELF(value) RVAL2GEMBLEMEDICON(value)

static VALUE
rg_initialize(VALUE self, VALUE icon, VALUE emblem)
{
        G_INITIALIZE(self, g_emblemed_icon_new(RVAL2GICON(icon), RVAL2GEMBLEM(emblem)));

        return Qnil;
}

static VALUE
rg_icon(VALUE self)
{
        return GOBJ2RVAL(g_emblemed_icon_get_icon(_SELF(self)));
}

static VALUE
rg_emblems(VALUE self)
{
        return GLIST2ARY_FREE(g_emblemed_icon_get_emblems(_SELF(self)));
}

static VALUE
rg_add_emblem(VALUE self, VALUE emblem)
{
        g_emblemed_icon_add_emblem(_SELF(self), RVAL2GEMBLEM(emblem));

        return self;
}

void
Init_gemblemedicon(VALUE mGio)
{
        VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(G_TYPE_EMBLEMED_ICON, "EmblemedIcon", mGio);

        RG_DEF_METHOD(initialize, 2);
        RG_DEF_METHOD(icon, 0);
        RG_DEF_METHOD(emblems, 0);
        RG_DEF_METHOD(add_emblem, 1);
        RG_DEF_ALIAS("<<", "add_emblem");
}
