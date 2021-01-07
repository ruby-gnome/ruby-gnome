/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011-2021  Ruby-GNOME Project Team
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

#ifdef HAVE_RUBY_THREAD_H
#  include <ruby/thread.h>
#endif

GPrivate rg_polling_key = G_PRIVATE_INIT(NULL);

/*
static ID id_poll_func;
*/
static ID id_call;

static VALUE mGLibSource;

static GThread *main_thread;

typedef struct _callback_info_t
{
    VALUE callback;
    guint id;
} callback_info_t;

/*****************************************/
static GPollFunc default_poll_func;

typedef struct _PollInfo
{
    GPollFD *ufds;
    guint nfsd;
    gint timeout;
    gint result;
} PollInfo;

static void
rg_poll_in_blocking_raw(PollInfo *info)
{
    info->result = default_poll_func(info->ufds, info->nfsd, info->timeout);
}

#ifdef HAVE_RB_THREAD_CALL_WITHOUT_GVL
static void *
rg_poll_in_blocking(void *data)
{
    PollInfo *info = data;
    rg_poll_in_blocking_raw(info);
    return NULL;
}
#else
static VALUE
rg_poll_in_blocking(void *data)
{
    PollInfo *info = data;
    rg_poll_in_blocking_raw(info);
    return Qnil;
}
#endif

static gint
rg_poll(GPollFD *ufds, guint nfsd, gint timeout)
{
    PollInfo info;

    info.ufds = ufds;
    info.nfsd = nfsd;
    info.timeout = timeout;
    info.result = 0;

    g_private_set(&rg_polling_key, GINT_TO_POINTER(TRUE));
    if (g_thread_self() == main_thread) {
#ifdef HAVE_RB_THREAD_CALL_WITHOUT_GVL
        rb_thread_call_without_gvl(rg_poll_in_blocking, &info,
                                   RUBY_UBF_IO, NULL);
#else
        rb_thread_blocking_region(rg_poll_in_blocking, &info,
                                  RUBY_UBF_IO, NULL);
#endif
    } else {
        rg_poll_in_blocking_raw(&info);
    }
    g_private_set(&rg_polling_key, GINT_TO_POINTER(FALSE));

    return info.result;
}

static VALUE
ruby_source_set_priority (G_GNUC_UNUSED VALUE self, G_GNUC_UNUSED VALUE priority)
{
    return Qnil;
}

static void
restore_poll_func(G_GNUC_UNUSED VALUE data)
{
    if (g_main_context_get_poll_func(NULL) == (GPollFunc)rg_poll) {
        g_main_context_set_poll_func(NULL, default_poll_func);
    }
}

static VALUE
source_remove(G_GNUC_UNUSED VALUE self, VALUE tag)
{
    return CBOOL2RVAL(g_source_remove(NUM2UINT(tag)));
}

static VALUE
source_current_source(G_GNUC_UNUSED VALUE self)
{
    return BOXED2RVAL(g_main_current_source, G_TYPE_SOURCE);
}

static gboolean
source_func(gpointer data)
{
    callback_info_t *info = (callback_info_t *)data;

    return RVAL2CBOOL(rb_funcall(info->callback, id_call, 0));
}

static void
source_destroy_notify(gpointer data)
{
    callback_info_t *info = data;

    rbgobj_remove_relative(mGLibSource, (ID)0, info->callback);

    xfree(info);
}

#define RG_TARGET_NAMESPACE cMainContext
#define _SELF(s) ((GMainContext*)RVAL2BOXED(s, G_TYPE_MAIN_CONTEXT))

static VALUE
rg_initialize(VALUE self)
{
    GMainContext *context;

    context = g_main_context_new();

    g_main_context_set_poll_func(context, rg_poll);

    G_INITIALIZE(self, context);
    return Qnil;
}

static VALUE
rg_s_default(G_GNUC_UNUSED VALUE self)
{
    return BOXED2RVAL(g_main_context_default(), G_TYPE_MAIN_CONTEXT);
}

static VALUE
rg_iteration(VALUE self, VALUE may_block)
{
    return CBOOL2RVAL(g_main_context_iteration(_SELF(self), RVAL2CBOOL(may_block)));
}

static VALUE
rg_pending_p(VALUE self)
{
    return CBOOL2RVAL(g_main_context_pending(_SELF(self)));
}

static VALUE
rg_find_source(VALUE self, VALUE source_id)
{
    GSource* src = g_main_context_find_source_by_id(_SELF(self), NUM2UINT(source_id));
    return BOXED2RVAL(src, G_TYPE_SOURCE);
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
rg_wakeup(VALUE self)
{
    g_main_context_wakeup(_SELF(self));
    return self;
}

static VALUE
rg_acquire(VALUE self)
{
    return CBOOL2RVAL(g_main_context_acquire(_SELF(self)));
}

static VALUE
rg_release(VALUE self)
{
    g_main_context_release(_SELF(self));
    return self;
}

static VALUE
rg_owner_p(VALUE self)
{
    return CBOOL2RVAL(g_main_context_is_owner(_SELF(self)));
}

/*
gboolean    g_main_context_wait             (GMainContext *context,
                                             GCond *cond,
                                             GMutex *mutex);
*/

static VALUE
rg_prepare(VALUE self)
{
    gint priority;
    gboolean ret = g_main_context_prepare(_SELF(self), &priority);

    return rb_assoc_new(CBOOL2RVAL(ret), INT2NUM(priority));
}

struct mc_query_body_args {
    gint timeout_;
    GPollFD *fds;
    gint n_fds;
};

static VALUE
mc_query_body(VALUE value)
{
    struct mc_query_body_args *args = (struct mc_query_body_args *)value;
    gint i;
    VALUE ary = rb_ary_new();

    for (i = 0; i < args->n_fds; i++)
        rb_ary_push(ary, GPOLLFD2RVAL(&args->fds[i]));

    return rb_assoc_new(INT2NUM(args->timeout_), ary);
}

static VALUE
mc_query_ensure(VALUE value)
{
    g_free((GPollFD *)value);

    return Qnil;
}

#define QUERY_DEFAULT_FDS 100

static VALUE
rg_query(VALUE self, VALUE rbmax_priority)
{
    GMainContext *context = _SELF(self);
    gint max_priority = NUM2INT(rbmax_priority);
    gint timeout_;
    GPollFD *fds;
    gint n_fds;
    struct mc_query_body_args args;

    fds = g_new(GPollFD, QUERY_DEFAULT_FDS);
    n_fds = g_main_context_query(context, max_priority, &timeout_, fds, QUERY_DEFAULT_FDS);
    if (n_fds > QUERY_DEFAULT_FDS) {
        g_free(fds);
        fds = g_new(GPollFD, n_fds);
        g_main_context_query(context, max_priority, &timeout_, fds, n_fds);
    }

    args.timeout_ = timeout_;
    args.fds = fds;
    args.n_fds = n_fds;
    return rb_ensure(mc_query_body, (VALUE)&args,
                     mc_query_ensure, (VALUE)fds);
}

/* How can I implement this?
static VALUE
rg_check(VALUE self, VALUE max_priority)
{
    gint i, timeout_;
    VALUE ary;

    GPollFD* fds;
    gint ret, n_fds;

    fds = g_new (GPollFD, 10);    
    n_fds = g_main_context_query(_SELF(self), NUM2INT(max_priority), 
                                      &timeout_, fds, 10);
    printf("n_fds = %d\n", n_fds);

    g_free(fds);
    fds = g_new (GPollFD, n_fds);    

    ret = g_main_context_check(_SELF(self), NUM2INT(max_priority),
                               fds, n_fds);
    printf("ret = %d\n", ret);
    ary = rb_ary_new();
    for (i = 0; i < ret; i++)
        rb_ary_push(ary, GPOLLFD2RVAL(&fds[i]));

    g_free(fds);    
    return ary;
}
*/

static VALUE
rg_dispatch(VALUE self)
{
    g_main_context_dispatch(_SELF(self));
    return self;
}

/* How can I get "self" or something like it as key .... 
static gint
poll_func(GPollFD *ufds, guint nfsd, gint timeout_)
{
    VALUE func = rb_ivar_get(self, id_poll_func);
    if (NIL_P(func)) return -1;

    return INT2NUM(rb_funcall(func, 3, GPOLLFD2RVAL(ufds),
                              UINT2NUM(nfsd), INT2NUM(timeout_)));
}

static VALUE
rg_set_poll_func(VALUE self)
{
    rb_ivar_set(self, id_poll_func, rb_block_proc());
    g_main_context_set_poll_func(_SELF(self), (GPollFunc)poll_func);

    return self;
}
*/

/*
GPollFunc   g_main_context_get_poll_func    (GMainContext *context);
*/

static VALUE
rg_add_poll(VALUE self, VALUE fd, VALUE priority)
{
    g_main_context_add_poll(_SELF(self), RVAL2GPOLLFD(fd),
                            NUM2INT(priority));
    return self;
}

static VALUE
rg_remove_poll(VALUE self, VALUE fd)
{
    g_main_context_remove_poll(_SELF(self), RVAL2GPOLLFD(fd));
    return self;
}

static VALUE
rg_s_depth(G_GNUC_UNUSED VALUE self)
{
    return INT2NUM(g_main_depth());
}

static VALUE
timeout_source_new(G_GNUC_UNUSED VALUE self, VALUE interval)
{
    return BOXED2RVAL(g_timeout_source_new(NUM2UINT(interval)), G_TYPE_SOURCE);
}

static VALUE
timeout_source_new_seconds(G_GNUC_UNUSED VALUE self, VALUE interval)
{
    return BOXED2RVAL(g_timeout_source_new_seconds(NUM2UINT(interval)), G_TYPE_SOURCE);
}

static VALUE
timeout_add(int argc, VALUE *argv, G_GNUC_UNUSED VALUE self)
{
    VALUE interval, rb_priority, func, rb_id;
    gint priority;
    callback_info_t *info;
    guint id;

    rb_scan_args(argc, argv, "11&", &interval, &rb_priority, &func);

    priority = NIL_P(rb_priority) ? G_PRIORITY_DEFAULT : NUM2INT(rb_priority);
    info = ALLOC(callback_info_t);
    info->callback = func;
    id = g_timeout_add_full(priority,
                            NUM2UINT(interval),
                            source_func,
                            info,
                            source_destroy_notify);
    info->id = id;
    rb_id = UINT2NUM(id);
    rbgobj_add_relative(mGLibSource, func);
    return rb_id;
}

static VALUE
timeout_add_seconds(int argc, VALUE *argv, G_GNUC_UNUSED VALUE self)
{
    VALUE interval, rb_priority, func, rb_id;
    gint priority;
    callback_info_t *info;
    guint id;

    rb_scan_args(argc, argv, "11&", &interval, &rb_priority, &func);

    priority = NIL_P(rb_priority) ? G_PRIORITY_DEFAULT : NUM2INT(rb_priority);
    info = ALLOC(callback_info_t);
    info->callback = func;
    id = g_timeout_add_seconds_full(priority,
                                    NUM2UINT(interval),
                                    source_func,
                                    info,
                                    source_destroy_notify);
    info->id = id;
    rb_id = UINT2NUM(id);
    rbgobj_add_relative(mGLibSource, func);
    return rb_id;
}

static VALUE
idle_source_new(G_GNUC_UNUSED VALUE self)
{
    return BOXED2RVAL(g_idle_source_new(), G_TYPE_SOURCE);
}

static VALUE
idle_add(gint argc, VALUE *argv, G_GNUC_UNUSED VALUE self)
{
    VALUE arg1, arg2, func, rb_id;
    callback_info_t *info;
    guint id;
    gint priority = G_PRIORITY_DEFAULT_IDLE;

    rb_scan_args(argc, argv, "02", &arg1, &arg2);

    if (RVAL2CBOOL(rb_obj_is_kind_of(arg1, rb_cProc))) {
        func = arg1;
    } else if (RVAL2CBOOL(rb_obj_is_kind_of(arg1, rb_cInteger))) {
        priority = NUM2INT(arg1);
        func = rb_block_proc();
    } else {
        func = rb_block_proc();
    }

    info = ALLOC(callback_info_t);
    info->callback = func;
    id = g_idle_add_full(priority,
                         source_func,
                         info,
                         source_destroy_notify);
    info->id = id;
    rb_id = UINT2NUM(id);
    rbgobj_add_relative(mGLibSource, func);
    return rb_id;
}

static VALUE
child_watch_source_new(G_GNUC_UNUSED VALUE self, VALUE pid)
{
    return BOXED2RVAL(g_child_watch_source_new((GPid)NUM2INT(pid)), G_TYPE_SOURCE);
}

static void
child_watch_func(GPid pid, gint status, gpointer func)
{
    rb_funcall((VALUE)func, id_call, 2, INT2NUM((long)pid), INT2NUM(status));
}

static VALUE
child_watch_add(VALUE self, VALUE pid)
{
    VALUE func = rb_block_proc();
    G_RELATIVE(self, func);
    return UINT2NUM(g_child_watch_add((GPid)NUM2INT(pid), 
                                      (GChildWatchFunc)child_watch_func, (gpointer)func));
}

void
Init_glib_main_context(void)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(G_TYPE_MAIN_CONTEXT, "MainContext", mGLib); 

    VALUE timeout = rb_define_module_under(mGLib, "Timeout");
    VALUE idle = rb_define_module_under(mGLib, "Idle");
    VALUE child_watch = rb_define_module_under(mGLib, "ChildWatch");

    id_call = rb_intern("call");

    g_private_set(&rg_polling_key, GINT_TO_POINTER(FALSE));

    main_thread = g_thread_self();

    rbg_define_singleton_method(mGLib, "set_ruby_thread_priority",
                               ruby_source_set_priority, 1);

    mGLibSource = rb_const_get(mGLib, rb_intern("Source"));
    rbg_define_singleton_method(mGLibSource, "remove", source_remove, 1);
    rbg_define_singleton_method(mGLibSource, "current", source_current_source, 0);
/*
    id_poll_func = rb_intern("__poll_func__");
*/
    RG_DEF_METHOD(initialize, 0);
    RG_DEF_SMETHOD(default, 0);
    RG_DEF_METHOD(iteration, 1);
    RG_DEF_METHOD_P(pending, 0);
    RG_DEF_METHOD(find_source, 1);
    RG_DEF_METHOD(wakeup, 0);
    RG_DEF_METHOD(acquire, 0);
    RG_DEF_METHOD(release, 0);
    RG_DEF_METHOD_P(owner, 0);
    RG_DEF_METHOD(prepare, 0);
    RG_DEF_METHOD(query, 1);
/*
    RG_DEF_METHOD(check, 1);
*/
    RG_DEF_METHOD(dispatch, 0);
/*
    RG_DEF_METHOD(set_poll_func, 0);
*/
    RG_DEF_METHOD(add_poll, 2);
    RG_DEF_METHOD(remove_poll, 1);
    RG_DEF_SMETHOD(depth, 0);
    rbg_define_singleton_method(timeout, "source_new", timeout_source_new, 1);
    rbg_define_singleton_method(timeout, "source_new_seconds", timeout_source_new_seconds, 1);
    rbg_define_singleton_method(timeout, "add", timeout_add, -1);
    rbg_define_singleton_method(timeout, "add_seconds", timeout_add_seconds, -1);
    rbg_define_singleton_method(idle, "source_new", idle_source_new, 0);
    rbg_define_singleton_method(idle, "add", idle_add, -1);

    rbg_define_singleton_method(child_watch, "source_new", child_watch_source_new, 1);
    rbg_define_singleton_method(child_watch, "add", child_watch_add, 1);

    default_poll_func = g_main_context_get_poll_func(NULL);
    g_main_context_set_poll_func(NULL, rg_poll);
    rb_set_end_proc(restore_poll_func, Qnil);
}
