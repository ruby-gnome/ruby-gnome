/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
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

static ID id_compfunc;
static ID id_call;
static ID id_to_s;
static ID id_clear;
static ID id_items_internal;

/*****************************************/
static GCompletion*
completion_copy(GCompletion *comp)
{
  GCompletion* new_comp;
  g_return_val_if_fail (comp != NULL, NULL);

  new_comp = g_new(GCompletion, 1);
  *new_comp = *comp;
  return new_comp;
}

static GType
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

#define RG_TARGET_NAMESPACE cCompletion
#define _SELF(s) ((GCompletion*)RVAL2BOXED(s, G_TYPE_COMPLETION))

/* data should be [self, data] */
static gchar *
comp_func(gpointer compdata)
{
    VALUE ret;
    VALUE self = RARRAY_PTR((VALUE)compdata)[0];
    VALUE data = RARRAY_PTR((VALUE)compdata)[1];

    VALUE func = rb_ivar_get(self, id_compfunc);

    if (NIL_P(func)){
        ret = rb_funcall(data, id_to_s, 0);
    } else {
        ret = rb_funcall(func, id_call, 1, data);
    }
    return (gchar *)RVAL2CSTR(ret);
}

static VALUE
rg_initialize(VALUE self)
{
    VALUE block = Qnil;

    if (rb_block_given_p()) {
        block = rb_block_proc();
    }

    rb_ivar_set(self, id_compfunc, block);
    /* This is used for memory management(GC) */
    rb_ivar_set(self, id_items_internal, rb_hash_new());

    G_INITIALIZE(self, g_completion_new((GCompletionFunc)comp_func));

    return Qnil;
}

static VALUE
rg_add_items(VALUE self, VALUE items)
{
    gint i, len;
    GList* list = (GList*)NULL;
    VALUE items_internal = rb_ivar_get(self, id_items_internal);

    items = rb_ary_to_ary(items);
    len = RARRAY_LEN(items);
    for (i = 0; i < len; i ++){
        VALUE data = RARRAY_PTR(items)[i];
        VALUE item = rb_assoc_new(self, data);
        list = g_list_append(list, (gpointer)item);
        rb_hash_aset(items_internal, data, item);
    }
    g_completion_add_items(_SELF(self), list);

    return self;
}

static VALUE
rg_remove_items(VALUE self, VALUE items)
{
    gint i, len;
    GList* list = (GList*)NULL;
    VALUE items_internal = rb_ivar_get(self, id_items_internal);

    items = rb_ary_to_ary(items);
    len = RARRAY_LEN(items);
    for (i = 0; i < len; i ++){
        VALUE data = RARRAY_PTR(items)[i];
        VALUE item = rb_hash_aref(items_internal, data);
        list = g_list_append(list, (gpointer)item);
        rb_hash_delete(items_internal, data);
    }
    g_completion_remove_items(_SELF(self), list);

    return self;
}

static VALUE
rg_clear_items(VALUE self)
{
    VALUE items_internal = rb_ivar_get(self, id_items_internal);
    rb_funcall(items_internal, id_clear, 0);
    g_completion_clear_items(_SELF(self));
    return self;
}

static VALUE
rg_items(VALUE self)
{
    return rb_ivar_get(self, id_items_internal);
}

static VALUE
rg_complete(VALUE self, VALUE prefix)
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
        rb_ary_push(ary, RARRAY_PTR((VALUE)list->data)[1]);
        list = list->next;
    }

    return rb_assoc_new(ary, new_prefix ? CSTR2RVAL(new_prefix) : Qnil);
}

/* We don't use this.
void        g_completion_set_compare        (GCompletion *cmp,
                                             GCompletionStrncmpFunc strncmp_func);
*/

void
Init_glib_completion(void)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(G_TYPE_COMPLETION, "Completion", mGLib); 

    id_call = rb_intern("call");
    id_to_s = rb_intern("to_s");
    id_clear = rb_intern("clear");
    id_compfunc = rb_intern("completion_proc");
    id_items_internal = rb_intern("items_internal");

    RG_DEF_METHOD(initialize, 0);
    RG_DEF_METHOD(add_items, 1);
    RG_DEF_METHOD(remove_items, 1);
    RG_DEF_METHOD(clear_items, 0);
    RG_DEF_METHOD(complete, 1);

    RG_DEF_METHOD(items, 0);
}
