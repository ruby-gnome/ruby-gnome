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
static VALUE
rbgdk_threads_init(VALUE self)
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
rbgdk_threads_enter(VALUE self)
{
    gdk_threads_enter();
    return self;
}

static VALUE
rbgdk_threads_leave(VALUE self)
{
    gdk_threads_leave();
    return self;
}

static VALUE
rbgdk_threads_synchronize(G_GNUC_UNUSED VALUE self)
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
Init_gtk_gdk_threads(void)
{
#ifdef   G_THREADS_ENABLED
    VALUE mGdkThreads = rb_define_module_under(mGdk, "Threads");

    rb_define_module_function(mGdkThreads, "init",  rbgdk_threads_init, 0);
    rb_define_module_function(mGdkThreads, "enter", rbgdk_threads_enter, 0);
    rb_define_module_function(mGdkThreads, "leave", rbgdk_threads_leave, 0);
    rb_define_module_function(mGdkThreads, "synchronize", rbgdk_threads_synchronize, 0);
#endif
}
