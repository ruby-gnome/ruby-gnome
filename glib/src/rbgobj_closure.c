/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/**********************************************************************

  rbgobj_closure.c -

  $Author: sakai $
  $Date: 2003/09/18 11:50:33 $

  Copyright (C) 2002,2003  Masahiro Sakai

**********************************************************************/

#include "global.h"

static ID id_call;
static gboolean rclosure_initialized = FALSE;

typedef struct _GRClosure GRClosure;

struct _GRClosure
{
    GClosure closure;
    VALUE callback;
    VALUE extra_args;
    GValToRValSignalFunc g2r_func;
};

static VALUE
rclosure_default_g2r_func(num, values)
    guint num;
    const GValue* values;
{
    int i;
    VALUE args = rb_ary_new2(num);
    for (i = 0; i < num; i++)
        rb_ary_store(args, i, GVAL2RVAL(&values[i]));
    return args;
}

struct marshal_arg
{
    GClosure*       closure;
    GValue*         return_value;
    guint           n_param_values;
    const GValue*   param_values;
    gpointer        invocation_hint;
    gpointer        marshal_data;
};

static VALUE
rclosure_marshal_body(struct marshal_arg* arg)
{
    GClosure*       closure         = arg->closure;
    GValue*         return_value    = arg->return_value;   
    guint           n_param_values  = arg->n_param_values; 
    const GValue*   param_values    = arg->param_values;
    //gpointer        invocation_hint = arg->invocation_hint;
    //gpointer        marshal_data    = arg->marshal_data;

    VALUE ret, args;
    GValToRValSignalFunc func;

    if (((GRClosure*)closure)->g2r_func){
        func = (GValToRValSignalFunc)((GRClosure*)closure)->g2r_func;
    } else { 
        func = (GValToRValSignalFunc)rclosure_default_g2r_func;
    }
    args = (*func)(n_param_values, param_values);

    if (!NIL_P(((GRClosure*)closure)->extra_args)){
        args = rb_ary_concat(args, ((GRClosure*)closure)->extra_args);
    }

    ret = rb_apply(((GRClosure*)closure)->callback, id_call, args);

    if (return_value && G_VALUE_TYPE(return_value))
        rbgobj_rvalue_to_gvalue(ret, return_value);

    return Qnil;
}

static void
rclosure_marshal(GClosure*       closure,
                 GValue*         return_value,
                 guint           n_param_values,
                 const GValue*   param_values,
                 gpointer        invocation_hint,
                 gpointer        marshal_data)
{
    struct marshal_arg arg;
    int state;

    if (!rclosure_initialized) {
        g_closure_invalidate(closure);
        return;
    }
 
    arg.closure         = closure;
    arg.return_value    = return_value;
    arg.n_param_values  = n_param_values;
    arg.param_values    = param_values;
    arg.invocation_hint = invocation_hint;
    arg.marshal_data    = marshal_data;

    rb_protect((VALUE (*)())&rclosure_marshal_body, (VALUE)&arg, &state);

    if (state){
        /* FIXME */
#if 0
        char buf[BUFSIZ];
        snprintf(buf, BUFSIZ, 
                 "%s:%d warning: unexpected jump occured in GClosure invocation\n",
                 ruby_sourcefile, ruby_sourceline);
        rb_write_deferr(buf);
#else
        rb_jump_tag(state);
#endif
    }
}

static GHashTable* rclosure_table;
static VALUE rclosure_table_wrapper;

static void
rclosure_invalidate(gpointer data, GClosure* closure)
{
    g_hash_table_remove(rclosure_table, closure);
    ((GRClosure*)closure)->callback   = Qnil;
    ((GRClosure*)closure)->extra_args = Qnil;
}

static void
rclosure_table_mark_entry(gpointer       key,
                          gpointer       value,
                          gpointer       user_data)
{
    GRClosure* closure = (GRClosure*)key;
    rb_gc_mark(closure->callback);
    rb_gc_mark(closure->extra_args);
}

static void
rclosure_table_mark(GHashTable* table)
{
    g_hash_table_foreach(table, rclosure_table_mark_entry, NULL);
}

GClosure*
g_rclosure_new(VALUE callback_proc, VALUE extra_args, GValToRValSignalFunc g2r_func)
{
    GRClosure* closure;

    closure = (GRClosure*)g_closure_new_simple(sizeof(GRClosure), NULL);

    closure->callback   = callback_proc;
    closure->extra_args = extra_args;
    closure->g2r_func   = g2r_func;

    g_closure_set_marshal((GClosure*)closure, &rclosure_marshal);

    g_hash_table_insert(rclosure_table, closure, NULL);
    g_closure_add_invalidate_notifier((GClosure*)closure, NULL,
                                      &rclosure_invalidate);

    return (GClosure*)closure;
}

static void
rclosure_end_proc(VALUE _)
{
    rclosure_initialized = FALSE;
}

static void
Init_rclosure()
{
    rclosure_table = g_hash_table_new_full(g_direct_hash, g_direct_equal,
                                           NULL, NULL);

    rclosure_table_wrapper =
      Data_Wrap_Struct(rb_cData,
                       rclosure_table_mark, NULL,
                       rclosure_table);
    rb_global_variable(&rclosure_table_wrapper);

    id_call = rb_intern("call");

    rclosure_initialized = TRUE;
    rb_set_end_proc(rclosure_end_proc, Qnil);
}

/**********************************************************************/

static VALUE
closure_initialize(self)
    VALUE self;
{
    GClosure* closure = g_rclosure_new(G_BLOCK_PROC(), Qnil, NULL);
    G_INITIALIZE(self, closure);
    g_closure_sink(closure);
    return self;
}

static VALUE
closure_in_marshal(self)
    VALUE self;
{
    GClosure* closure = RVAL2BOXED(self, G_TYPE_CLOSURE);
    return closure->in_marshal ? Qtrue : Qfalse;
}

static VALUE
closure_is_invalid(self)
    VALUE self;
{
    GClosure* closure = RVAL2BOXED(self, G_TYPE_CLOSURE);
    return closure->is_invalid ? Qtrue : Qfalse;
}

static VALUE
closure_invalidate(self)
    VALUE self;
{
    GClosure* closure = RVAL2BOXED(self, G_TYPE_CLOSURE);
    g_closure_invalidate(closure);
    return self;
}

static void
Init_closure()
{
    VALUE cClosure = G_DEF_CLASS(G_TYPE_CLOSURE, "Closure", mGLib);
    rb_define_method(cClosure, "initialize", closure_initialize, 0);
    rb_define_method(cClosure, "in_marshal?", closure_in_marshal, 0);
    rb_define_method(cClosure, "invalid?", closure_is_invalid, 0);
}

/**********************************************************************/

void Init_gobject_gclosure()
{
    Init_rclosure();
    Init_closure();
}
