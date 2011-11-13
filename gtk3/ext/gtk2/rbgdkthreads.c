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

#include "global.h"

#ifdef   G_THREADS_ENABLED

#define RG_TARGET_NAMESPACE mThreads

static VALUE
rg_m_init(VALUE self)
{
#ifndef GDK_WINDOWING_WIN32
    if (!g_thread_supported()){
        g_thread_init(NULL);
    }
    gdk_threads_init();
#endif
    return self;
}

static VALUE
rg_m_enter(VALUE self)
{
    gdk_threads_enter();
    return self;
}

static VALUE
rg_m_leave(VALUE self)
{
    gdk_threads_leave();
    return self;
}

static VALUE
rg_m_synchronize(G_GNUC_UNUSED VALUE self)
{
    VALUE func = rb_block_proc();
    gdk_threads_enter();
    func = rb_block_proc();
    rb_funcall(func, id_call, 0);
    gdk_threads_leave();
    return Qnil;
}
#endif

void
Init_gtk_gdk_threads(VALUE mGdk)
{
#ifdef   G_THREADS_ENABLED
    VALUE RG_TARGET_NAMESPACE = rb_define_module_under(mGdk, "Threads");

    RG_DEF_MODFUNC(init, 0);
    RG_DEF_MODFUNC(enter, 0);
    RG_DEF_MODFUNC(leave, 0);
    RG_DEF_MODFUNC(synchronize, 0);
#endif
}
