/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbglib_maincontext.c -

  $Author: mutoh $
  $Date: 2005/03/12 18:03:56 $

  Copyright (C) 2005 Masao Mutoh
************************************************/

#include "global.h"

/*
static ID id_poll_func;
*/

/*****************************************/
GType
g_main_context_get_type(void)
{
  static GType our_type = 0;
  if (our_type == 0)
    our_type = g_boxed_type_register_static ("GMainContext",
                    (GBoxedCopyFunc)g_main_context_ref,
                    (GBoxedFreeFunc)g_main_context_unref);
  return our_type;
}
/*****************************************/

#define _SELF(s) ((GMainContext*)RVAL2BOXED(s, G_TYPE_MAIN_CONTEXT))

static VALUE
mc_initialize(self)
    VALUE self;
{
    G_INITIALIZE(self, g_main_context_new());
    return Qnil;
}

static VALUE
mc_s_default(self)
    VALUE self;
{
    return BOXED2RVAL(g_main_context_default(), G_TYPE_MAIN_CONTEXT);
}

static VALUE
mc_iteration(self, may_block)
    VALUE self, may_block;
{
    return CBOOL2RVAL(g_main_context_iteration(_SELF(self), RTEST(may_block)));
}

static VALUE
mc_pending(self)
    VALUE self;
{
    return CBOOL2RVAL(g_main_context_pending(_SELF(self)));
}

static VALUE
mc_find_source(self, source_id)
    VALUE self, source_id;
{
    GSource* src = g_main_context_find_source_by_id(_SELF(self), NUM2UINT(source_id));
    return src ? BOXED2RVAL(src, G_TYPE_SOURCE) : Qnil;
}

/*
GSource*    g_main_context_find_source_by_user_data
                                            (GMainContext *context,
                                             gpointer user_data);
GSource*    g_main_context_find_source_by_funcs_user_data
                                            (GMainContext *context,
                                             GSourceFuncs *funcs,
                                             gpointer user_data);
*/

static VALUE
mc_wakeup(self)
    VALUE self;
{
    g_main_context_wakeup(_SELF(self));
    return self;
}

static VALUE
mc_acquire(self)
    VALUE self;
{
    return CBOOL2RVAL(g_main_context_acquire(_SELF(self)));
}

static VALUE
mc_release(self)
    VALUE self;
{
    g_main_context_release(_SELF(self));
    return self;
}

/*
gboolean    g_main_context_wait             (GMainContext *context,
                                             GCond *cond,
                                             GMutex *mutex);
*/

static VALUE
mc_prepare(self)
    VALUE self;
{
    gint priority;
    gboolean ret = g_main_context_prepare(_SELF(self), &priority);

    return rb_assoc_new(CBOOL2RVAL(ret), INT2NUM(priority));
}

/*
gint        g_main_context_query            (GMainContext *context,
                                             gint max_priority,
                                             gint *timeout_,
                                             GPollFD *fds,
                                             gint n_fds);
gint        g_main_context_check            (GMainContext *context,
                                             gint max_priority,
                                             GPollFD *fds,
                                             gint n_fds);
*/

static VALUE
mc_dispatch(self)
    VALUE self;
{
    g_main_context_dispatch(_SELF(self));
    return self;
}

/* How can I get "self" or something like it as key .... 
static gint
poll_func(ufds, nfsd, timeout_)
    GPollFD* ufds;
    guint nfsd;
    gint timeout_;
{
    VALUE func = rb_ivar_get(self, id_poll_func);
    if NIL_P(func) return -1;

    return INT2NUM(rb_funcall(func, 3, BOXED2RVAL(ufds, G_TYPE_POLL_FD),
                              UINT2NUM(nfsd), INT2NUM(timeout_)));
}

static VALUE
mc_set_poll_func(self)
    VALUE self;
{
    rb_ivar_set(self, id_poll_func, G_BLOCK_PROC());
    g_main_context_set_poll_func(_SELF(self), (GPollFunc)poll_func);

    return self;
}
*/

/*
GPollFunc   g_main_context_get_poll_func    (GMainContext *context);
*/

static VALUE
mc_add_poll(self, fd, priority)
    VALUE self, fd, priority;
{
    g_main_context_add_poll(_SELF(self), RVAL2BOXED(fd, G_TYPE_POLL_FD),
                            NUM2INT(priority));
    return self;
}

static VALUE
mc_remove_poll(self, fd)
    VALUE self, fd;
{
    g_main_context_remove_poll(_SELF(self), RVAL2BOXED(fd, G_TYPE_POLL_FD));
    return self;
}

static VALUE
mc_s_depth(self)
    VALUE self;
{
    return INT2NUM(g_main_depth());
}

void
Init_glib_main_context()
{
    VALUE mc = G_DEF_CLASS(G_TYPE_MAIN_CONTEXT, "MainContext", mGLib); 
/*
    id_poll_func = rb_intern("__poll_func__");
*/
    rb_define_method(mc, "initialize", mc_initialize, 0);
    rb_define_singleton_method(mc, "default", mc_s_default, 0);
    rb_define_method(mc, "iteration", mc_iteration, 1);
    rb_define_method(mc, "pending?", mc_pending, 0);
    rb_define_method(mc, "find_source", mc_find_source, 1);
    rb_define_method(mc, "wakeup", mc_wakeup, 0);
    rb_define_method(mc, "acquire", mc_acquire, 0);
    rb_define_method(mc, "release", mc_release, 0);
    rb_define_method(mc, "prepare", mc_prepare, 0);
    rb_define_method(mc, "dispatch", mc_dispatch, 0);
/*
    rb_define_method(mc, "set_poll_func", mc_set_poll_func, 0);
*/
    rb_define_method(mc, "add_poll", mc_add_poll, 2);
    rb_define_method(mc, "remove_poll", mc_remove_poll, 1);
    rb_define_singleton_method(mc, "depth", mc_s_depth, 0);
}
