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

#define RG_TARGET_NAMESPACE mVte

void Init_vte3(void);

void
Init_vte3(void)
{
    VALUE RG_TARGET_NAMESPACE;

    RG_TARGET_NAMESPACE = rb_define_module("Vte");

    rb_define_const(RG_TARGET_NAMESPACE, "BUILD_VERSION",
                    rb_ary_new3(3,
                                INT2FIX(VTE_MAJOR_VERSION),
                                INT2FIX(VTE_MINOR_VERSION),
                                INT2FIX(VTE_MICRO_VERSION)));

    G_DEF_CLASS(VTE_TYPE_TERMINAL_ANTI_ALIAS, "TerminalAntiAlias", RG_TARGET_NAMESPACE);

    Init_vte_access(RG_TARGET_NAMESPACE);
    Init_vte_reaper(RG_TARGET_NAMESPACE);
    Init_vte_terminal(RG_TARGET_NAMESPACE);
    Init_vte_charattributes(RG_TARGET_NAMESPACE);
    Init_vte_pty(RG_TARGET_NAMESPACE);
}
