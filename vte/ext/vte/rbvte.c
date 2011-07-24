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

#include "rbvte.h"

void
Init_vte(void)
{
    VALUE mVte;

    mVte = rb_define_module("Vte");

    rb_define_const(mVte, "BUILD_VERSION",
                    rb_ary_new3(3,
                                INT2FIX(VTE_MAJOR_VERSION),
                                INT2FIX(VTE_MINOR_VERSION),
                                INT2FIX(VTE_MICRO_VERSION)));

    G_DEF_CLASS(VTE_TYPE_TERMINAL_ERASE_BINDING, "TerminalEraseBinding", mVte);
#if VTE_CHECK_VERSION(0, 18, 0)
    G_DEF_CLASS(VTE_TYPE_TERMINAL_CURSOR_BLINK_MODE, "TerminalCursorBlinkMode", mVte);
#endif
#if VTE_CHECK_VERSION(0, 19, 1)
    G_DEF_CLASS(VTE_TYPE_TERMINAL_CURSOR_SHAPE, "TerminalCursorShape", mVte);
#endif
    G_DEF_CLASS(VTE_TYPE_TERMINAL_ANTI_ALIAS, "TerminalAntiAlias", mVte);

    Init_vte_access(mVte);
    Init_vte_reaper(mVte);
    Init_vte_terminal(mVte);
    Init_vte_charattributes(mVte);
}
