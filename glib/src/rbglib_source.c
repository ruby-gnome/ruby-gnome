/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbglib_source.c -

  $Author: ggc $
  $Date: 2007/07/13 16:07:28 $

  Copyright (C) 2005 Masao Mutoh
************************************************/

#include "global.h"

static ID id_call;

/*****************************************/
static void
source_free(source)
    GSource* source;
{
    g_source_unref(source);
    g_source_destroy(source);
}

GType
g_source_get_type(void)
{
  static GType our_type = 0;
  if (our_type == 0)
    our_type = g_boxed_type_register_static ("GSource",
                    (GBoxedCopyFunc)g_source_ref,
                    (GBoxedFreeFunc)source_free);
  return our_type;
}
/*****************************************/

#define _SELF(s) ((GSource*)RVAL2BOXED(s, G_TYPE_SOURCE))

/*
GSource*    g_source_new                    (GSourceFuncs *source_funcs,
                                             guint struct_size);
*/

static VALUE
source_attach(self, context)
    VALUE self, context;
{
    return UINT2NUM(g_source_attach(_SELF(self), 
                                    RVAL2BOXED(context, G_TYPE_MAIN_CONTEXT)));
}

#if GLIB_CHECK_VERSION(2,12,0)
static VALUE
source_is_destroyed(self)
    VALUE self;
{
    return CBOOL2RVAL(g_source_is_destroyed(_SELF(self)));
}
#endif

static VALUE
source_set_priority(self, priority)
    VALUE self, priority;
{
    g_source_set_priority(_SELF(self), NUM2INT(priority));
    return self;
}

static VALUE
source_get_priority(self)
    VALUE self;
{
    return INT2NUM(g_source_get_priority(_SELF(self)));
}

static VALUE
source_set_can_recurse(self, can_recurse)
    VALUE self, can_recurse;
{
    g_source_set_can_recurse(_SELF(self), RVAL2CBOOL(can_recurse));
    return self;
}

static VALUE
source_get_can_recurse(self)
    VALUE self;
{
    return CBOOL2RVAL(g_source_get_can_recurse(_SELF(self)));
}

static VALUE
source_get_id(self)
    VALUE self;
{
    return UINT2NUM(g_source_get_id(_SELF(self)));
}

static VALUE
source_get_context(self)
    VALUE self;
{
    GMainContext* context = g_source_get_context(_SELF(self));
    return BOXED2RVAL(context, G_TYPE_MAIN_CONTEXT);
}

static gboolean
source_func(func)
    gpointer func;
{
    return RVAL2CBOOL(rb_funcall((VALUE)func, id_call, 0));
}

static VALUE
source_set_callback(self)
    VALUE self;
{
    VALUE func = rb_block_proc();
    G_RELATIVE(self, func);
    g_source_set_callback(_SELF(self),
                          (GSourceFunc)source_func,
                          (gpointer)func, 
                          (GDestroyNotify)NULL);
    return self;
}

/*
void        g_source_set_callback_indirect  (GSource *source,
                                             gpointer callback_data,
                                             GSourceCallbackFuncs *callback_funcs);
*/

static VALUE
source_add_poll(self, fd)
    VALUE self, fd;
{
    g_source_add_poll(_SELF(self), RVAL2BOXED(fd, G_TYPE_POLL_FD));
    return self;
}

static VALUE
source_remove_poll(self, fd)
    VALUE self, fd;
{
    g_source_remove_poll(_SELF(self), RVAL2BOXED(fd, G_TYPE_POLL_FD));
    return self;
}

static VALUE
source_get_current_time(self)
    VALUE self;
{
    GTimeVal timeval;
    g_source_get_current_time(_SELF(self), &timeval);

    return rb_assoc_new(LONG2NUM(timeval.tv_sec), LONG2NUM(timeval.tv_usec));
}

/* How can I implement them ?
gboolean    g_source_remove_by_funcs_user_data
                                            (GSourceFuncs *funcs,
                                             gpointer user_data);
gboolean    g_source_remove_by_user_data    (gpointer user_data);
*/

void
Init_glib_source()
{
    VALUE src = G_DEF_CLASS(G_TYPE_SOURCE, "Source", mGLib); 

    id_call = rb_intern("call");

    rb_define_method(src, "attach", source_attach, 1);
#if GLIB_CHECK_VERSION(2,12,0)
    rb_define_method(src, "destroyed?", source_is_destroyed, 0);
#endif
    rb_define_method(src, "set_priority", source_set_priority, 1);
    rb_define_method(src, "priority", source_get_priority, 0);
    rb_define_method(src, "set_can_recurse", source_set_can_recurse, 1);
    rb_define_method(src, "can_recurse?", source_get_can_recurse, 0);
    rb_define_method(src, "id", source_get_id, 0);
    rb_define_method(src, "context", source_get_context, 0);
    rb_define_method(src, "set_callback", source_set_callback, 0);
    rb_define_method(src, "add_poll", source_add_poll, 1);
    rb_define_method(src, "remove_poll", source_remove_poll, 1);
    rb_define_method(src, "current_time", source_get_current_time, 0);

    /* GLib::Source.remove is moved to rbglib_maincontext.c */

    G_DEF_SETTERS(src);
}
