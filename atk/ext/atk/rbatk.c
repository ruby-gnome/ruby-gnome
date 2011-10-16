/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2003-2005 Masao Mutoh
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

#include "rbatk.h"

VALUE mAtk;

void
Init_atk()
{
    mAtk = rb_define_module("Atk");

    rb_define_const(mAtk, "BUILD_VERSION", 
                    rb_ary_new3(3, 
                                INT2FIX(ATK_MAJOR_VERSION), 
                                INT2FIX(ATK_MINOR_VERSION), 
                                INT2FIX(ATK_MICRO_VERSION)));
    Init_atk_inits();
}
