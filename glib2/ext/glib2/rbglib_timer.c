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

#ifdef G_OS_WIN32
#include <windows.h>
#endif

/*****************************************/

/* This is stolen from gtimer.c of glib-2.6.2. */
struct _GTimer
{
#ifdef G_OS_WIN32
  DWORD start;
  DWORD end;
#else /* !G_OS_WIN32 */
  struct timeval start;
  struct timeval end;
#endif /* !G_OS_WIN32 */

  guint active : 1;
};

static GTimer*
timer_copy(GTimer *timer)
{
  GTimer* new_timer;
  g_return_val_if_fail (timer != NULL, NULL);

  new_timer = g_new(struct _GTimer, 1);
  *new_timer = *timer;
  return new_timer;
}

static GType
g_timer_get_type(void)
{
    static GType our_type = 0;
    if (our_type == 0)
        our_type = g_boxed_type_register_static ("GTimer",
                                                 (GBoxedCopyFunc)timer_copy,
                                                 (GBoxedFreeFunc)g_timer_destroy);
    return our_type;
}
/*****************************************/

#define G_TYPE_TIMER (g_timer_get_type())

#define _SELF(s) ((GTimer*)RVAL2BOXED(s, G_TYPE_TIMER))

static VALUE
timer_initialize(VALUE self)
{
    G_INITIALIZE(self, g_timer_new());
    return Qnil;
}

static VALUE
timer_start(VALUE self)
{
    g_timer_start(_SELF(self));
    return self;
}

static VALUE
timer_stop(VALUE self)
{
    g_timer_stop(_SELF(self));
    return self;
}

#if GLIB_CHECK_VERSION(2,4,0)
static VALUE
timer_continue(VALUE self)
{
    g_timer_continue(_SELF(self));
    return self;
}
#endif

static VALUE
timer_elapsed(VALUE self)
{
    gulong microseconds;
    gdouble ret = g_timer_elapsed(_SELF(self), &microseconds);

    return rb_assoc_new(rb_float_new(ret), ULONG2NUM(microseconds));
}

static VALUE
timer_reset(VALUE self)
{
    g_timer_reset(_SELF(self));
    return self;
}

void
Init_glib_timer(void)
{
    VALUE timer = G_DEF_CLASS(G_TYPE_TIMER, "Timer", mGLib); 

    rb_define_method(timer, "initialize", timer_initialize, 0);
    rb_define_method(timer, "start", timer_start, 0);
    rb_define_method(timer, "stop", timer_stop, 0);
#if GLIB_CHECK_VERSION(2,4,0)
    rb_define_method(timer, "continue", timer_continue, 0);
#endif
    rb_define_method(timer, "elapsed", timer_elapsed, 0);
    rb_define_method(timer, "reset", timer_reset, 0);
}
