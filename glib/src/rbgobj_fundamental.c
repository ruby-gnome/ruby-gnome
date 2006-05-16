/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

   rbgobj_fundamental.c -

   $Author: mutoh $
   $Date: 2006/05/16 14:52:45 $

   Copyright (C) 2006 Sjoerd Simons, Masao Mutoh
************************************************/

#include "rbgobject.h"

static GHashTable *fundamentals;

void 
Init_gobject_fundamental(void) {
  fundamentals = g_hash_table_new(g_int_hash, g_int_equal);
}

void
rbgobj_fund_define_fundermental(RGFundamental *f) {
  g_hash_table_insert(fundamentals, &(f->type), f);
}

gboolean 
rbgobj_fund_has_type(GType type) {
  RGFundamental *f = g_hash_table_lookup(fundamentals, &type);
  return f != NULL;
}

VALUE 
rbgobj_fund_get_superclass(GType type) {
  RGFundamental *f = g_hash_table_lookup(fundamentals, &type);
  if (f == NULL || f->get_superclass == NULL) {
    return Qnil;
  }
  return f->get_superclass();
}

void 
rbgobj_fund_type_init_hook(GType type, VALUE klass) {
  RGFundamental *f = g_hash_table_lookup(fundamentals, &type);
  if (f == NULL || f->type_init_hook == NULL) {
    return;
  }
  f->type_init_hook(klass);
}

gboolean 
rbgobj_fund_rvalue2gvalue(GType type, VALUE val, GValue *result) {
  RGFundamental *f = g_hash_table_lookup(fundamentals, &type);
  if (f == NULL ||  f->rvalue2gvalue == NULL) {
    return FALSE;
  }
  f->rvalue2gvalue(val, result);
  return TRUE;
}

VALUE 
rbgobj_fund_gvalue2rvalue(GType type, const GValue *value) {
  RGFundamental *f = g_hash_table_lookup(fundamentals, &type);
  if (f == NULL ||  f->gvalue2rvalue == NULL) {
    return Qnil;
  }
  return f->gvalue2rvalue(value);
}

void 
rbgobj_fund_initialize(GType type, VALUE obj, gpointer cobj) {
  RGFundamental *f = g_hash_table_lookup(fundamentals, &type);
  if (f == NULL ||  f->initialize == NULL) {
    /* error out ? */
    return;
  }
  f->initialize(obj, cobj);
}

gpointer 
rbgobj_fund_robj2instance(GType type, VALUE obj) {
  RGFundamental *f = g_hash_table_lookup(fundamentals, &type);
  if (f == NULL ||  f->robj2instance == NULL) {
    return NULL;
  }
  return f->robj2instance(obj);
}

VALUE 
rbgobj_fund_instance2robj(GType type, gpointer instance) {
  RGFundamental *f = g_hash_table_lookup(fundamentals, &type);
  if (f == NULL ||  f->instance2robj == NULL) {
    return Qnil;
  }
  return f->instance2robj(instance);
}
