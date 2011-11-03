/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2002,2003 Masao Mutoh
 *  Copyright (C) 1998-2000 Yukihiro Matsumoto,
 *                          Daisuke Kanda,
 *                          Hiroshi Igarashi
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

#include "global.h"

#define RG_TARGET_NAMESPACE mInput

static void
exec_input(gpointer data, G_GNUC_UNUSED gint source, GdkInputCondition condition)
{
    rb_funcall((VALUE)data, id_call, 1, 
               GFLAGS2RVAL(condition, GDK_TYPE_INPUT_CONDITION));
}

static VALUE
rg_m_add(VALUE self, VALUE filedescriptor, VALUE gdk_input_condition)
{
    VALUE id;
    VALUE func;

    func = rb_block_proc();
    id = INT2FIX(gdk_input_add(NUM2INT(rb_funcall(filedescriptor,
                                                  rb_intern("to_i"), 0)),
                               RVAL2GFLAGS(gdk_input_condition, GDK_TYPE_INPUT_CONDITION),
                               (GdkInputFunction)exec_input,
                               (gpointer)func));
    G_RELATIVE2(self, func, id_relative_callbacks, id);
    return id;
}

static VALUE
rg_m_remove(VALUE self, VALUE id)
{
    gdk_input_remove(NUM2INT(id));
    G_REMOVE_RELATIVE(self, id_relative_callbacks, id);
    return id;
}

void
Init_gtk_gdk_input(VALUE mGdk)
{
    VALUE RG_TARGET_NAMESPACE = rb_define_module_under(mGdk, "Input");

    RG_DEF_MODFUNC(add, 2);
    RG_DEF_MODFUNC(remove, 1);

    /* GdkInputCondition */
    G_DEF_CLASS(GDK_TYPE_INPUT_CONDITION, "Condition", RG_TARGET_NAMESPACE);
    G_DEF_CONSTANTS(RG_TARGET_NAMESPACE, GDK_TYPE_INPUT_CONDITION, "GDK_INPUT_");

}
