/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbglib_completion.c -

  $Author: mutoh $
  $Date: 2005/10/15 04:31:38 $

  Copyright (C) 2005 Masao Mutoh
************************************************/

#include "global.h"

static ID id_compfunc;
static ID id_call;
static ID id_to_s;
static ID id_clear;
static ID id_items_internal;

/*****************************************/
static GCompletion*
completion_copy(comp)
    GCompletion* comp;
{
  GCompletion* new_comp;
  g_return_val_if_fail (comp != NULL, NULL);

  new_comp = g_new(GCompletion, 1);
  *new_comp = *comp;
  return new_comp;
}

GType
g_completion_get_type(void)
{
    static GType our_type = 0;
    if (our_type == 0)
        our_type = g_boxed_type_register_static ("GCompletion",
                                                 (GBoxedCopyFunc)completion_copy,
                                                 (GBoxedFreeFunc)g_completion_free);
    return our_type;
}
/*****************************************/

#define G_TYPE_COMPLETION (g_completion_get_type())

#define _SELF(s) ((GCompletion*)RVAL2BOXED(s, G_TYPE_COMPLETION))

/* data should be [self, data] */
static gchar*
comp_func(compdata)
    gpointer compdata;
{
    VALUE ret;
    VALUE self = RARRAY((VALUE)compdata)->ptr[0];
    VALUE data = RARRAY((VALUE)compdata)->ptr[1];
    
    VALUE func = rb_ivar_get(self, id_compfunc);
    
    if (NIL_P(func)){
        ret = rb_funcall(data, id_to_s, 0);
    } else {
        ret = rb_funcall(func, id_call, 1, data);
    }
    return RVAL2CSTR(ret);
}

static VALUE
comp_initialize(self)
    VALUE self;
{
    VALUE block = Qnil;

    if (rb_block_given_p()) {
        block = G_BLOCK_PROC();
    }

    rb_ivar_set(self, id_compfunc, block);
    /* This is used for memory management(GC) */
    rb_ivar_set(self, id_items_internal, rb_hash_new());

    G_INITIALIZE(self, g_completion_new((GCompletionFunc)comp_func));

    return Qnil;
}

static VALUE
comp_add_items(self, items)
    VALUE self, items;
{
    gint i, len;
    GList* list = (GList*)NULL;
    VALUE items_internal = rb_ivar_get(self, id_items_internal);

    len = RARRAY(items)->len;
    for (i = 0; i < len; i ++){
        VALUE data = RARRAY(items)->ptr[i];
        VALUE item = rb_assoc_new(self, data);
        list = g_list_append(list, (gpointer)item);
        rb_hash_aset(items_internal, data, item);
    }
    g_completion_add_items(_SELF(self), list);

    return self;
}

static VALUE
comp_remove_items(self, items)
    VALUE self, items;
{
    gint i, len;
    GList* list = (GList*)NULL;
    VALUE items_internal = rb_ivar_get(self, id_items_internal);

    len = RARRAY(items)->len;
    for (i = 0; i < len; i ++){
        VALUE data = RARRAY(items)->ptr[0];
        VALUE item = rb_assoc_new(self, data);
        g_list_append(list, (gpointer)item);
#if RUBY_VERSION_CODE < 180
        rb_funcall(items_internal, rb_intern("delete"), 1, data);
#else
        rb_hash_delete(items_internal, data);
#endif
    }
    g_completion_remove_items(_SELF(self), list);

    return self;
}

static VALUE
comp_clear_items(self)
    VALUE self;
{
    VALUE items_internal = rb_ivar_get(self, id_items_internal);
    rb_funcall(items_internal, id_clear, 0);
    g_completion_clear_items(_SELF(self));
    return self;
}

static VALUE
comp_items(self)
    VALUE self;
{
    return rb_ivar_get(self, id_items_internal);
}

static VALUE
comp_complete(self, prefix)
    VALUE self, prefix;
{
    gchar* new_prefix;
    VALUE ary = rb_ary_new();
#if GLIB_CHECK_VERSION(2,4,0)
    GList* list = g_completion_complete_utf8(_SELF(self),
                                            (const gchar*)RVAL2CSTR(prefix),
                                            &new_prefix);
#else
    GList* list = g_completion_complete(_SELF(self),
                                        RVAL2CSTR(prefix),
                                        &new_prefix);
#endif
    while (list) {
        rb_ary_push(ary, RARRAY((VALUE)list->data)->ptr[1]);
        list = list->next;
    }

    return rb_assoc_new(ary, new_prefix ? CSTR2RVAL(new_prefix) : Qnil);
}

/* We don't use this.
void        g_completion_set_compare        (GCompletion *cmp,
                                             GCompletionStrncmpFunc strncmp_func);
*/

void
Init_glib_completion()
{
    VALUE comp = G_DEF_CLASS(G_TYPE_COMPLETION, "Completion", mGLib); 

    id_call = rb_intern("call");
    id_to_s = rb_intern("to_s");
    id_clear = rb_intern("clear");
    id_compfunc = rb_intern("completion_proc");
    id_items_internal = rb_intern("items_internal");

    rb_define_method(comp, "initialize", comp_initialize, 0);
    rb_define_method(comp, "add_items", comp_add_items, 1);
    rb_define_method(comp, "remove_items", comp_remove_items, 1);
    rb_define_method(comp, "clear_items", comp_clear_items, 0);
    rb_define_method(comp, "complete", comp_complete, 1);

    rb_define_method(comp, "items", comp_items, 0);
}
