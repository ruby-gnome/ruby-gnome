/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2005  Masao Mutoh
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

#include "rbgprivate.h"

#define RG_TARGET_NAMESPACE cShellError

void
Init_glib_shellerror(void)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_ERROR2(G_SHELL_ERROR, "ShellError", mGLib, rb_eRuntimeError);

    rb_define_const(RG_TARGET_NAMESPACE, "BAD_QUOTING", INT2FIX(G_SHELL_ERROR_BAD_QUOTING));
    rb_define_const(RG_TARGET_NAMESPACE, "EMPTY_STRING", INT2FIX(G_SHELL_ERROR_EMPTY_STRING));
    rb_define_const(RG_TARGET_NAMESPACE, "FAILED", INT2FIX(G_SHELL_ERROR_FAILED));
}
