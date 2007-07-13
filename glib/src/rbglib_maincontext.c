/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbglib_maincontext.c -

  $Author: ggc $
  $Date: 2007/07/13 16:07:28 $

  Copyright (C) 2005 Masao Mutoh
************************************************/

#include "global.h"

/*
static ID id_poll_func;
*/
static ID id_call;


static VALUE mGLibSource;
static ID id__callbacks__;
static GHashTable *callbacks_table;

typedef struct _callback_info_t
{
    VALUE callback;
    guint id;
} callback_info_t;

static VALUE
source_remove(self, tag)
    VALUE self, tag;
{
    VALUE callback;
    callback = G_GET_RELATIVE(mGLibSource, id__callbacks__, tag);
    G_REMOVE_RELATIVE(mGLibSource, id__callbacks__, tag);
    g_hash_table_remove(callbacks_table, (gpointer)callback);
    return CBOOL2RVAL(g_source_remove(NUM2UINT(tag)));
}

#if GLIB_CHECK_VERSION(2,12,0)
static VALUE
source_current_source(self)
    VALUE self;
{
    return BOXED2RVAL(g_main_current_source, G_TYPE_SOURCE);
}
#endif

static gboolean
invoke_source_func(data)
    gpointer data;
{
    callback_info_t *info = (callback_info_t *)data;
    gboolean ret;

    ret = RVAL2CBOOL(rb_funcall(info->callback, id_call, 0));
    if (!ret)
        G_REMOVE_RELATIVE(mGLibSource, id__callbacks__, UINT2NUM(info->id));
    return ret;
}


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
    return CBOOL2RVAL(g_main_context_iteration(_SELF(self), RVAL2CBOOL(may_block)));
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

#if GLIB_CHECK_VERSION(2,10,0)
static VALUE
mc_is_owner(self)
    VALUE self;
{
    return CBOOL2RVAL(g_main_context_is_owner(_SELF(self)));
}
#endif

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

static VALUE
mc_query(self, max_priority)
    VALUE self, max_priority;
{
    gint i, timeout_;
    VALUE ary;
   
    GPollFD* fds = g_new (GPollFD, 100);
    gint ret = g_main_context_query(_SELF(self), NUM2INT(max_priority), 
                               &timeout_, fds, 100);
    if (ret > 100) {
        g_free(fds);
        fds = g_new(GPollFD, ret);
        g_main_context_query(_SELF(self), NUM2INT(max_priority),
                             &timeout_, fds, ret);
    }

    ary = rb_ary_new();
    for (i = 0; i < ret; i++)
        rb_ary_push(ary, BOXED2RVAL(&fds[i], G_TYPE_POLL_FD));
    
    g_free(fds);
    return rb_assoc_new(INT2NUM(timeout_), ary);
}

/* How can I implement this?
static VALUE
mc_check(self, max_priority)
    VALUE self, max_priority;
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
        rb_ary_push(ary, BOXED2RVAL(&fds[i], G_TYPE_POLL_FD));

    g_free(fds);    
    return ary;
}
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
    rb_ivar_set(self, id_poll_func, rb_block_proc());
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

#ifdef HAVE_G_MAIN_DEPTH
static VALUE
mc_s_depth(self)
    VALUE self;
{
    return INT2NUM(g_main_depth());
}
#endif


static VALUE
timeout_source_new(self, interval)
    VALUE self, interval;
{
    return BOXED2RVAL(g_timeout_source_new(NUM2UINT(interval)), G_TYPE_SOURCE);
}
#if GLIB_CHECK_VERSION(2,14,0)
static VALUE
timeout_source_new_seconds(self, interval)
    VALUE self, interval;
{
    return BOXED2RVAL(g_timeout_source_new_seconds(NUM2UINT(interval)), G_TYPE_SOURCE);
}
#endif

static VALUE
timeout_add(int argc, VALUE *argv, VALUE self)
{
    VALUE interval, rb_priority, func, rb_id;
    gint priority;
    callback_info_t *info;
    guint id;

    rb_scan_args(argc, argv, "11&", &interval, &rb_priority, &func);

    priority = NIL_P(rb_priority) ? G_PRIORITY_DEFAULT : INT2NUM(rb_priority);
    info = ALLOC(callback_info_t);
    info->callback = func;
    id = g_timeout_add_full(priority, NUM2UINT(interval),
                            (GSourceFunc)invoke_source_func,
                            (gpointer)info, g_free);
    info->id = id;
    rb_id = UINT2NUM(id);
    G_RELATIVE2(mGLibSource, func, id__callbacks__, rb_id);
    g_hash_table_insert(callbacks_table, (gpointer)func, info);
    return rb_id;
}

#if GLIB_CHECK_VERSION(2,14,0)
static VALUE
timeout_add_seconds(VALUE self, VALUE interval)
{
    VALUE func, rb_id;
    callback_info_t *info;
    guint id;

    func = rb_block_proc();

    info = ALLOC(callback_info_t);
    info->callback = func;
    id = g_timeout_add_seconds(NUM2UINT(interval),
                               (GSourceFunc)invoke_source_func,
                               (gpointer)info, g_free);
    info->id = id;
    rb_id = UINT2NUM(id);
    G_RELATIVE2(mGLibSource, func, id__callbacks__, rb_id);
    g_hash_table_insert(callbacks_table, (gpointer)func, info);
    return rb_id;
}
#endif

static VALUE
idle_source_new(self)
    VALUE self;
{
    return BOXED2RVAL(g_idle_source_new(), G_TYPE_SOURCE);
}

static VALUE
idle_add(argc, argv, self)
    gint argc;
    VALUE* argv;
    VALUE self;
{
    VALUE arg1, arg2, func, rb_id;
    callback_info_t *info;
    guint id;
    gint priority;

    rb_scan_args(argc, argv, "02", &arg1, &arg2);

    if (RVAL2CBOOL(rb_obj_is_kind_of(arg1, rb_cProc))) {
        priority = G_PRIORITY_DEFAULT;
        func = arg1;
    } else if (RVAL2CBOOL(rb_obj_is_kind_of(arg1, rb_cInteger))) {
        priority = NUM2INT(arg1);
        func = rb_block_proc();
    } else {
        priority = G_PRIORITY_DEFAULT;
        func = rb_block_proc();
    }

    info = ALLOC(callback_info_t);
    info->callback = func;
    id = g_idle_add_full(priority, (GSourceFunc)invoke_source_func,
                         (gpointer)info, g_free);
    info->id = id;
    rb_id = UINT2NUM(id);
    G_RELATIVE2(mGLibSource, func, id__callbacks__, rb_id);
    g_hash_table_insert(callbacks_table, (gpointer)func, info);
    return rb_id;
}

static VALUE
idle_remove(self, func)
    VALUE self, func;
{
    callback_info_t *info;

    info = g_hash_table_lookup(callbacks_table, (gpointer)func);
    G_REMOVE_RELATIVE(mGLibSource, id__callbacks__, UINT2NUM(info->id));
    g_hash_table_remove(callbacks_table, (gpointer)func);
    return CBOOL2RVAL(g_idle_remove_by_data((gpointer)info));
}

#if GLIB_CHECK_VERSION(2,4,0)
static VALUE
child_watch_source_new(self, pid)
    VALUE self, pid;
{
    return BOXED2RVAL(g_child_watch_source_new((GPid)NUM2INT(pid)), G_TYPE_SOURCE);
}

static void
child_watch_func(pid, status, func)
    GPid pid;
    gint status;
    gpointer func;
{
    rb_funcall((VALUE)func, id_call, 2, INT2NUM((long)pid), INT2NUM(status));
}

static VALUE
child_watch_add(self, pid)
    VALUE self, pid;
{
    VALUE func = rb_block_proc();
    G_RELATIVE(self, func);
    return UINT2NUM(g_child_watch_add((GPid)NUM2INT(pid), 
                                      (GChildWatchFunc)child_watch_func, (gpointer)func));
}
#endif

void
Init_glib_main_context()
{
    VALUE mc = G_DEF_CLASS(G_TYPE_MAIN_CONTEXT, "MainContext", mGLib); 

    VALUE timeout = rb_define_module_under(mGLib, "Timeout");
    VALUE idle = rb_define_module_under(mGLib, "Idle");
#if GLIB_CHECK_VERSION(2,4,0)
    VALUE child_watch = rb_define_module_under(mGLib, "ChildWatch");
#endif

    id_call = rb_intern("call");
    id__callbacks__ = rb_intern("__callbacks__");
    callbacks_table = g_hash_table_new(NULL, NULL);

    mGLibSource = rb_const_get(mGLib, rb_intern("Source"));
    rb_define_singleton_method(mGLibSource, "remove", source_remove, 1);
#if GLIB_CHECK_VERSION(2,12,0)
    rb_define_singleton_method(mGLibSource, "current", source_current_source, 0);
#endif
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
#if GLIB_CHECK_VERSION(2,10,0)
    rb_define_method(mc, "owner?", mc_is_owner, 0);
#endif
    rb_define_method(mc, "prepare", mc_prepare, 0);
    rb_define_method(mc, "query", mc_query, 1);
/*
    rb_define_method(mc, "check", mc_check, 1);
*/
    rb_define_method(mc, "dispatch", mc_dispatch, 0);
/*
    rb_define_method(mc, "set_poll_func", mc_set_poll_func, 0);
*/
    rb_define_method(mc, "add_poll", mc_add_poll, 2);
    rb_define_method(mc, "remove_poll", mc_remove_poll, 1);
#ifdef HAVE_G_MAIN_DEPTH
    rb_define_singleton_method(mc, "depth", mc_s_depth, 0);
#endif
    rb_define_module_function(timeout, "source_new", timeout_source_new, 1);
#if GLIB_CHECK_VERSION(2,14,0)
    rb_define_module_function(timeout, "source_new_seconds", timeout_source_new_seconds, 1);
#endif
    rb_define_module_function(timeout, "add", timeout_add, -1);
#if GLIB_CHECK_VERSION(2,14,0)
    rb_define_module_function(timeout, "add_seconds", timeout_add_seconds, 1);
#endif
    rb_define_module_function(idle, "source_new", idle_source_new, 0);
    rb_define_module_function(idle, "add", idle_add, -1);
    rb_define_module_function(idle, "remove", idle_remove, 1);

#if GLIB_CHECK_VERSION(2,4,0)
    rb_define_module_function(child_watch, "source_new", child_watch_source_new, 1);
    rb_define_module_function(child_watch, "add", child_watch_add, 1);
#endif
}
