/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2006-2011  Ruby-GNOME2 Project Team
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

#include "rbvte3private.h"

#define RG_TARGET_NAMESPACE cReaper

static VALUE
rg_s_get(G_GNUC_UNUSED VALUE self)
{
    return GOBJ2RVAL(vte_reaper_get());
}

void
Init_vte_reaper(VALUE mVte)
{
    VALUE RG_TARGET_NAMESPACE;

    RG_TARGET_NAMESPACE = G_DEF_CLASS(VTE_TYPE_REAPER, "Reaper", mVte);

    RG_DEF_SMETHOD(get, 0);
}
