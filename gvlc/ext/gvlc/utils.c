/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2012  Ruby-GNOME2 Project Team
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

#include "rbvlcprivate.h"

char **
rval2cstrary(VALUE ary, long *len)
{
    long i;
    char **result;

    if (NIL_P(ary))
        return NULL;

    *len = RARRAY_LEN(ary);
    if (*len == 0)
        return NULL;

    result = ALLOC_N(char *, *len);
    for (i = 0; i < *len; i++)
        result[i] = g_strdup(RVAL2CSTR(RARRAY_PTR(ary)[i]));

    return result;
}

void
cstrary_free(char **ary, long len)
{
    long i;

    if (!ary)
        return;

    for (i = 0; i < len; i++)
        g_free(ary[i]);
    g_free(ary);
}

VALUE
moduledescriptionlist2rval(libvlc_module_description_t *list)
{
    libvlc_module_description_t *p;
    VALUE result;

    result = rb_ary_new();
    for (p = list; p != NULL; p = p->p_next)
        rb_ary_push(result, VLCMODULEDESCRIPTION2RVAL(p));
    libvlc_module_description_list_release(list);

    return result;
}

VALUE
trackdescriptionlist2rval(libvlc_track_description_t *list)
{
    libvlc_track_description_t *p;
    VALUE result;

    result = rb_ary_new();
    for (p = list; p != NULL; p = p->p_next)
        rb_ary_push(result, VLCTRACKDESCRIPTION2RVAL(p));
    libvlc_track_description_list_release(list);

    return result;
}

VALUE
audiooutputlist2rval(libvlc_audio_output_t *list)
{
    libvlc_audio_output_t *p;
    VALUE result;

    result = rb_ary_new();
    for (p = list; p != NULL; p = p->p_next)
        rb_ary_push(result, VLCAUDIOOUTPUT2RVAL(p));
    libvlc_audio_output_list_release(list);

    return result;
}

static VALUE
callbacks_get(VALUE self)
{
    if (!rb_ivar_defined(self, rb_intern("callbacks")))
        rb_ivar_set(self, rb_intern("callbacks"), rb_hash_new());
    return rb_ivar_get(self, rb_intern("callbacks"));
}

struct cb_event_protect_arg
{
    const struct libvlc_event_t *event;
    VALUE self;
};

static VALUE
cb_event_protect(VALUE data)
{
    struct cb_event_protect_arg *arg = (struct cb_event_protect_arg *)data;
    const struct libvlc_event_t *event = arg->event;
    VALUE self = arg->self;

    VALUE callbacks = callbacks_get(self);
    VALUE func = rb_hash_aref(callbacks, INT2NUM(event->type));
    if (NIL_P(func))
        rb_warn("event block not found: %s", libvlc_event_type_name(event->type));
    else
        rb_funcall(func, rb_intern("call"), 2, self, VLCEVENT2RVAL((struct libvlc_event_t *)event));

    return Qnil;
}

static void
cb_event(const struct libvlc_event_t *event, void *self)
{
    struct cb_event_protect_arg arg = {event, (VALUE)self};
    G_PROTECT_CALLBACK(cb_event_protect, &arg);
}

VALUE
em_attach_event(libvlc_event_manager_t *event_manager, VALUE self, VALUE event_type)
{
    int type = RVAL2VLCEVENTTYPE(event_type);
    VALUE func = rb_block_proc();
    VALUE callbacks = callbacks_get(self);

    rb_hash_aset(callbacks, INT2NUM(type), func);
    return ZEROBOOL2RVAL(libvlc_event_attach(event_manager, type, cb_event, (void *)self));
}

VALUE
em_detach_event(libvlc_event_manager_t *event_manager, VALUE self, VALUE event_type)
{
    int type = RVAL2VLCEVENTTYPE(event_type);
    VALUE callbacks = callbacks_get(self);

    rb_hash_delete(callbacks, INT2NUM(type));
    libvlc_event_detach(event_manager, type, cb_event, (void *)self);

    return self;
}
