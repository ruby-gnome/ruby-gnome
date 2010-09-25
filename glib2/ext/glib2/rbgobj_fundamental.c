/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

   rbgobj_fundamental.c -

   Copyright (C) 2007 Ruby-GNOME2 Project Team
************************************************/

#include "rbgobject.h"
#include "rbgprivate.h"

static GList *tables = NULL;

void
rbgobj_fund_define_fundamental(RGFundamental *fundamental)
{
    RGConvertTable *table;

    table = g_new0(RGConvertTable, 1);
    table->type = fundamental->type;
    table->get_superclass = fundamental->get_superclass;
    table->type_init_hook = fundamental->type_init_hook;
    table->rvalue2gvalue = fundamental->rvalue2gvalue;
    table->gvalue2rvalue = fundamental->gvalue2rvalue;
    table->initialize = fundamental->initialize;
    table->robj2instance = fundamental->robj2instance;
    table->instance2robj = fundamental->instance2robj;
    RG_DEF_CONVERSION(table);
    tables = g_list_prepend(tables, table);
}
