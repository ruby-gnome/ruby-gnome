
/*
 * Copyright (C) 2003 Laurent Sansonetti <lrz@gnome.org>
 *
 * This file is part of Ruby/Libgda.
 *
 * Ruby/Libgda is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * Ruby/Libgda is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with Ruby/Libgda; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307  USA
 */

#include "rbgda.h"

static VALUE rb_gda_datamodel_list_new(self, args)
    VALUE self, args;
{
    GdaDataModel *dm;
    GList *list = NULL;
    int i;
    
    for (i = 0; i < RARRAY(args)->len; i++) {
        VALUE v = rb_ary_entry(args, i);
        list = g_list_append(list, RVAL2CSTR(v));
    }

    dm = list != NULL 
        ? gda_data_model_list_new_from_string_list((const GList *)list) 
        : gda_data_model_list_new();

    if (dm != NULL) {
        G_INITIALIZE(self, dm);
    }

    return Qnil;
}

static VALUE rb_gda_datamodel_list_append_value(self, value)
    VALUE self, value;
{
    const GdaRow *row = gda_data_model_list_append_value(RGDA_DATAMODEL_LIST(self),
                                       (const GdaValue *)RGDA_VALUE(value));
    return row != NULL
        ? RGDA_ROW_NEW(row)
        : Qnil;
}

void Init_gda_datamodel_list(void) {
    VALUE c = G_DEF_CLASS(GDA_TYPE_DATA_MODEL_LIST, "DataModelList", mGda);
    
    rb_define_method(c, "initialize",    rb_gda_datamodel_list_new,         -2);
    rb_define_method(c, "append_value",  rb_gda_datamodel_list_append_value, 1);
}

