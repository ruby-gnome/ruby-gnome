/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2007  Ruby-GNOME2 Project Team
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
