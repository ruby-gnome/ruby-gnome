/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011-2021  Ruby-GNOME Project Team
 *  Copyright (C) 2005,2006  Masao Mutoh
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

#define RG_TARGET_NAMESPACE cMainLoop
#define _SELF(s) ((GMainLoop*)RVAL2BOXED(s, G_TYPE_MAIN_LOOP))

/*****************************************/

static VALUE
rg_initialize(int argc, VALUE *argv, VALUE self)
{
    VALUE context, is_running;
    GMainLoop *loop;
    GMainContext *main_context = NULL;

    rb_scan_args(argc, argv, "02", &context, &is_running);

    if (!NIL_P(context))
        main_context = RVAL2BOXED(context, G_TYPE_MAIN_CONTEXT);
    loop = g_main_loop_new(main_context, RVAL2CBOOL(is_running));
    G_INITIALIZE(self, loop);
    return Qnil;
}

typedef struct {
    GMainLoop *loop;
    int state;
} CheckInterruptData;

static VALUE
check_interrupt_raw(G_GNUC_UNUSED VALUE user_data)
{
    rb_thread_check_ints();
    return Qnil;
}

static gboolean
check_interrupt(gpointer user_data)
{
    CheckInterruptData *data = user_data;

    rb_protect(check_interrupt_raw, Qnil, &(data->state));
    if (data->state == 0) {
        return G_SOURCE_CONTINUE;
    } else {
        g_main_loop_quit(data->loop);
        return G_SOURCE_REMOVE;
    }
}

static VALUE
rg_run(VALUE self)
{
    CheckInterruptData data;
    GSource *interrupt_source;

    data.loop = _SELF(self);
    data.state = 0;

    interrupt_source = rbg_interrupt_source_new();
    g_source_set_callback(interrupt_source,
                          check_interrupt,
                          &data,
                          NULL);
    g_source_attach(interrupt_source,
                    g_main_loop_get_context(data.loop));
    g_main_loop_run(data.loop);
    g_source_destroy(interrupt_source);
    g_source_unref(interrupt_source);

    if (data.state == 0) {
        rb_thread_check_ints();
    } else {
        rb_jump_tag(data.state);
    }

    return self;
}

static VALUE
rg_quit(VALUE self)
{
    g_main_loop_quit(_SELF(self));
    return Qnil;
}

static VALUE
rg_running_p(VALUE self)
{
    return CBOOL2RVAL(g_main_loop_is_running(_SELF(self)));
}

static VALUE
rg_context(VALUE self)
{
    return BOXED2RVAL(g_main_loop_get_context(_SELF(self)), G_TYPE_MAIN_CONTEXT);
}

void
Init_glib_main_loop(void)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(G_TYPE_MAIN_LOOP, "MainLoop", mGLib);

    RG_DEF_METHOD(initialize, -1);
    RG_DEF_METHOD(run, 0);
    RG_DEF_METHOD(quit, 0);
    RG_DEF_METHOD_P(running, 0);
    RG_DEF_METHOD(context, 0);
}
