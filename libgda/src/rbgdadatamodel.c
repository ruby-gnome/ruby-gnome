
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

static VALUE rb_gda_datamodel_changed(self)
    VALUE self;
{
    gda_data_model_changed(RGDA_DATAMODEL(self));
    return self;
}

static VALUE rb_gda_datamodel_row_inserted(self, rownum)
    VALUE self, rownum;
{
    gda_data_model_row_inserted(RGDA_DATAMODEL(self), 
                                FIX2INT(rownum));
    return self;
}

static VALUE rb_gda_datamodel_row_updated(self, rownum)
    VALUE self, rownum;
{
    gda_data_model_row_updated(RGDA_DATAMODEL(self),
                               FIX2INT(rownum));
    return self;
}

static VALUE rb_gda_datamodel_row_removed(self, rownum)
    VALUE self, rownum;
{
    gda_data_model_row_removed(RGDA_DATAMODEL(self),
                               FIX2INT(rownum));
    return self;
}

static VALUE rb_gda_datamodel_freeze(self)
    VALUE self;
{
    gda_data_model_freeze(RGDA_DATAMODEL(self));
    return self;
}

static VALUE rb_gda_datamodel_thaw(self)
    VALUE self;
{
    gda_data_model_thaw(RGDA_DATAMODEL(self));
    return self;
}

static VALUE rb_gda_datamodel_get_columns(self)
    VALUE self;
{
    GdaDataModel *datamod;
    VALUE arr;
    int i;

    arr = rb_ary_new();
    datamod = RGDA_DATAMODEL(self);
    for (i = 0; i < gda_data_model_get_n_columns(datamod); i++) {
        rb_ary_push(arr, CSTR2RVAL(gda_data_model_get_column_title(datamod, 
                                                                   i))); 
    }
    return arr;
}

static VALUE rb_gda_datamodel_each_column(self)
    VALUE self;
{
    rb_ary_each(rb_gda_datamodel_get_columns(self));
    return Qnil;
}

static VALUE rb_gda_datamodel_get_n_columns(self)
    VALUE self;
{
    return INT2FIX(gda_data_model_get_n_columns(RGDA_DATAMODEL(self)));
}

static VALUE rb_gda_datamodel_get_column(self, colnum)
    VALUE self, colnum;
{
    const gchar *column = gda_data_model_get_column_title(RGDA_DATAMODEL(self),  
                                                          FIX2INT(colnum));
    return column != NULL
        ? CSTR2RVAL(column) 
        : Qnil;
}

static VALUE rb_gda_datamodel_describe_column(self, colnum)
    VALUE self, colnum;
{
    GdaFieldAttributes *a = gda_data_model_describe_column(RGDA_DATAMODEL(self),
                                                           FIX2INT(colnum));
    return a != NULL
        ? RGDA_FIELD_ATTRIBUTES_NEW(a)
        : Qnil;
}

static VALUE rb_gda_datamodel_get_column_pos(self, col_title)
    VALUE self, col_title;
{
    return INT2FIX(gda_data_model_get_column_position(RGDA_DATAMODEL(self),
                                                      RVAL2CSTR(col_title)));
}

static VALUE rb_gda_datamodel_get_rows(self)
    VALUE self;
{
    GdaDataModel *datamod;
    VALUE arr;
    int i;

    arr = rb_ary_new();
    datamod = RGDA_DATAMODEL(self);
    for (i = 0; i < gda_data_model_get_n_rows(datamod); i++) {
        rb_ary_push(arr, RGDA_ROW_NEW(gda_data_model_get_row(datamod, 
                                                             i))); 
    }
    return arr;
}

static VALUE rb_gda_datamodel_each_row(self)
    VALUE self;
{
    rb_ary_each(rb_gda_datamodel_get_rows(self));
    return Qnil;
}

static VALUE rb_gda_datamodel_get_n_rows(self)
    VALUE self;
{
    return INT2FIX(gda_data_model_get_n_rows(RGDA_DATAMODEL(self)));
}

static VALUE rb_gda_datamodel_get_row(self, row_number)
    VALUE self, row_number;
{
    const GdaRow *row = gda_data_model_get_row(RGDA_DATAMODEL(self),  
                                               FIX2INT(row_number));
    return row != NULL
        ? RGDA_ROW_NEW(row)
        : Qnil;
}

static VALUE rb_gda_datamodel_append_row(self, values)
    VALUE self, values;
{
    GList *list = NULL;
    const GdaRow *row;
    int i;
    
    for (i = 0; i < RARRAY(values)->len; i++) {
        list = g_list_append(list, RGDA_VALUE(rb_ary_entry(values, i)));
    }

    if (list == NULL) {
        rb_raise(rb_eArgError, "Wrong # of arguments (need at least 1)");
    }
    row = gda_data_model_append_row(RGDA_DATAMODEL(self), 
                                    (const GList *)list);
    return row != NULL 
        ? RGDA_ROW_NEW(row)
        : Qnil;
}

static VALUE rb_gda_datamodel_update_row(self, row)
    VALUE self, row;
{
    return CBOOL2RVAL(gda_data_model_update_row(RGDA_DATAMODEL(self),
                                                RGDA_ROW(row)));
}

static VALUE rb_gda_datamodel_remove_row(self, row)
    VALUE self, row;
{
    return CBOOL2RVAL(gda_data_model_remove_row(RGDA_DATAMODEL(self),
                                                RGDA_ROW(row)));
}

static VALUE rb_gda_datamodel_is_editable(self)
    VALUE self;
{
    return CBOOL2RVAL(gda_data_model_is_editable(RGDA_DATAMODEL(self)));
}

static VALUE rb_gda_datamodel_is_editing(self)
    VALUE self;
{
    return CBOOL2RVAL(gda_data_model_is_editing(RGDA_DATAMODEL(self)));
}

static VALUE rb_gda_datamodel_begin_edit(self)
    VALUE self;
{
    return CBOOL2RVAL(gda_data_model_begin_edit(RGDA_DATAMODEL(self)));
}

static VALUE rb_gda_datamodel_cancel_edit(self)
    VALUE self;
{
    return CBOOL2RVAL(gda_data_model_cancel_edit(RGDA_DATAMODEL(self)));
}

static VALUE rb_gda_datamodel_end_edit(self)
    VALUE self;
{
    return CBOOL2RVAL(gda_data_model_end_edit(RGDA_DATAMODEL(self)));
}

static VALUE rb_gda_datamodel_to_comma_separated(self)
    VALUE self;
{
    return CSTR2RVAL(gda_data_model_to_comma_separated(RGDA_DATAMODEL(self)));
}

static VALUE rb_gda_datamodel_to_tab_separated(self)
    VALUE self;
{
    return CSTR2RVAL(gda_data_model_to_tab_separated(RGDA_DATAMODEL(self)));
}

static VALUE rb_gda_datamodel_to_xml(argc, argv, self)
    int argc;
    VALUE *argv, self;
{
    VALUE standalone;

    rb_scan_args(argc, argv, "01", &standalone);
    
    return CSTR2RVAL(gda_data_model_to_xml(RGDA_DATAMODEL(self), 
                                           NIL_P(standalone) ? FALSE : RVAL2CBOOL(standalone)));
}

static VALUE rb_gda_datamodel_get_command_text(self)
    VALUE self;
{
    return CSTR2RVAL(gda_data_model_get_command_text(RGDA_DATAMODEL(self)));
}

static VALUE rb_gda_datamodel_set_command_text(self, text)
    VALUE self, text;
{
    gda_data_model_set_command_text(RGDA_DATAMODEL(self),
                                    RVAL2CSTR(text));
    return self;
}

static VALUE rb_gda_datamodel_get_command_type(self)
    VALUE self;
{
    return INT2FIX(gda_data_model_get_command_type(RGDA_DATAMODEL(self)));
}

static VALUE rb_gda_datamodel_set_command_type(self, type)
    VALUE self, type;
{
    gda_data_model_set_command_type(RGDA_DATAMODEL(self),
                                    FIX2INT(type));
    return self;
}

void Init_gda_datamodel(void) {
    VALUE c = G_DEF_CLASS(GDA_TYPE_DATA_MODEL, "DataModel", mGda);

    rb_define_method(c, "emit_changed",      rb_gda_datamodel_changed,      0); 
    rb_define_method(c, "emit_row_inserted", rb_gda_datamodel_row_inserted, 1); 
    rb_define_method(c, "emit_row_updated",  rb_gda_datamodel_row_updated,  1); 
    rb_define_method(c, "emit_row_removed",  rb_gda_datamodel_row_removed,  1); 

    rb_define_method(c, "freeze!", rb_gda_datamodel_freeze, 0);
    rb_define_method(c, "thaw!",   rb_gda_datamodel_thaw,   0);

    rb_define_method(c, "columns",          rb_gda_datamodel_get_columns,   0);
    rb_define_method(c, "each_column",      rb_gda_datamodel_each_column,   0);
    rb_define_method(c, "n_columns",        rb_gda_datamodel_get_n_columns, 0);
    rb_define_method(c, "get_column_title", rb_gda_datamodel_get_column,    1);

    rb_define_method(c, "describe_column",     rb_gda_datamodel_describe_column, 1);
    rb_define_method(c, "get_column_position", rb_gda_datamodel_get_column_pos,  1);

    rb_define_method(c, "rows",     rb_gda_datamodel_get_rows,   0);
    rb_define_method(c, "each_row", rb_gda_datamodel_each_row,   0);
    rb_define_method(c, "n_rows",   rb_gda_datamodel_get_n_rows, 0);
    rb_define_method(c, "get_row",  rb_gda_datamodel_get_row,    1);

    rb_define_method(c, "append_row", rb_gda_datamodel_append_row, -2);
    rb_define_method(c, "remove_row", rb_gda_datamodel_remove_row,  1);
    rb_define_method(c, "update_row", rb_gda_datamodel_update_row,  1);
    
    rb_define_method(c, "editable?", rb_gda_datamodel_is_editable, 0);
    rb_define_method(c, "editing?",  rb_gda_datamodel_is_editing,  0);
    
    rb_define_method(c, "begin_edit",  rb_gda_datamodel_begin_edit,  0);
    rb_define_method(c, "cancel_edit", rb_gda_datamodel_cancel_edit, 0);
    rb_define_method(c, "end_edit",    rb_gda_datamodel_end_edit,    0);

    rb_define_method(c, "to_comma_separated", rb_gda_datamodel_to_comma_separated, 0);
    rb_define_method(c, "to_tab_separated",   rb_gda_datamodel_to_tab_separated,   0);
    rb_define_method(c, "to_xml",             rb_gda_datamodel_to_xml,            -1);

/*  TODO:
xmlNodePtr  gda_data_model_to_xml_node      (GdaDataModel *model,
                                             const gchar *name);
gboolean    gda_data_model_add_data_from_xml_node
                                            (GdaDataModel *model,
                                             xmlNodePtr node);
*/

    rb_define_method(c, "command_text", rb_gda_datamodel_get_command_text, 0);
    rb_define_method(c, "set_command_text", 
                     rb_gda_datamodel_set_command_text, 1);

    rb_define_method(c, "command_type", rb_gda_datamodel_get_command_type, 0);
    rb_define_method(c, "set_command_type", 
                     rb_gda_datamodel_set_command_type, 1);

    G_DEF_SETTERS(c);
}

