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

static VALUE gthreads;

static VALUE
gt_init(VALUE self)
{
#ifdef HAVE_G_THREAD_INIT
#ifdef G_THREADS_ENABLED
    g_thread_init(NULL);
#endif
#endif
    return self;
}

static VALUE
gt_supported(G_GNUC_UNUSED VALUE self)
{
#ifdef HAVE_G_THREAD_INIT
#ifdef G_THREADS_ENABLED
    return CBOOL2RVAL(g_thread_supported());
#else
    return Qfalse;
#endif
#else
    return Qfalse;
#endif

}

void
Init_glib_threads(void)
{
    gthreads = rb_define_class_under(mGLib, "Thread", rb_cObject);

    rb_define_singleton_method(gthreads, "init", gt_init, 0);
    rb_define_singleton_method(gthreads, "supported?", gt_supported, 0);
}
