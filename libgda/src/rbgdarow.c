
/*
 * Copyright (C) 2003 Laurent Sansonetti <laurent@datarescue.be>
 *
 * This file is part of Ruby/Gda.
 *
 * Ruby/Gda is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * Ruby/Gda is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with Ruby/Gda; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307  USA
 */

#include "rbgda.h"

static GdaRow* row_copy(GdaRow* row) {
    GList *entries = NULL;
    GdaRow* new_row;
    int i;

    g_return_val_if_fail (row != NULL, NULL);
    for (i = 0; i < gda_row_get_length(row); i++) {
        entries = g_list_append(entries, 
                                (gpointer) gda_row_get_value(row, i));
    }
    new_row = gda_row_new_from_list(gda_row_get_model(row),
                                    entries);
    return new_row;
}

GType gda_row_get_type(void) {
    static GType our_type = 0;
    if (our_type == 0) {
        our_type = g_boxed_type_register_static ("GdaRow",
            (GBoxedCopyFunc)row_copy,
            (GBoxedFreeFunc)gda_row_free);
    }
    return our_type;
}

static VALUE rb_gda_row_new(argc, argv, self)
    int argc;   
    VALUE *argv, self;
{
    VALUE model, values;
    GdaRow *row;

    rb_scan_args(argc, argv, "1*", &model, &values);

    /* Using gda_row_new() */
    if (RARRAY(values)->len == 1) {
        row = gda_row_new(RGDA_DATAMODEL(model),
                          INT2FIX(rb_ary_entry(values, 0)));
    }
    /* Using gda_row_new_from_list() */
    else {
        GList *vals = NULL;
        int i;

        for (i = 0; i < RARRAY(values)->len; i++) {
            vals = g_list_append(vals,
                                 RGDA_VALUE(rb_ary_entry(values, i)));
        }
        row = gda_row_new_from_list(RGDA_DATAMODEL(model), vals);
    }
    
    if (row != NULL) { 
        G_INITIALIZE(self, row); 
    }
    return Qnil;
}

static VALUE rb_gda_row_get_model(self)
    VALUE self;
{
    return RGDA_DATAMODEL_NEW(gda_row_get_model(RGDA_ROW(self)));
}

static VALUE rb_gda_row_get_number(self)
    VALUE self;
{
    return INT2FIX(gda_row_get_number(RGDA_ROW(self)));
}

static VALUE rb_gda_row_get_unique_id(self)
    VALUE self;
{
    return CSTR2RVAL(gda_row_get_id(RGDA_ROW(self)));
}

static VALUE rb_gda_row_set_number(self, number)
    VALUE self, number;
{
    gda_row_set_number(RGDA_ROW(self), FIX2INT(number));
    return Qnil;
}

static VALUE rb_gda_row_set_unique_id(self, id)
    VALUE self, id;
{
    gda_row_set_id(RGDA_ROW(self), RVAL2CSTR(id));
    return Qnil;
}

static VALUE rb_gda_row_get_values(self)
    VALUE self;
{
    GdaRow *row;
    VALUE arr;
    int i;

    arr = rb_ary_new();
    row = RGDA_ROW(self);
    for (i = 0; i < gda_row_get_length(row); i++) {
        rb_ary_push(arr, RGDA_VALUE_NEW(gda_row_get_value(row, i)));
    }
    return arr;
}

static VALUE rb_gda_row_each_value(self)
    VALUE self;
{
    rb_ary_each(rb_gda_row_get_values(self));
    return Qnil;
}

static VALUE rb_gda_row_get_length(self)
    VALUE self;
{
    return INT2FIX(gda_row_get_length(RGDA_ROW(self)));
}

static VALUE rb_gda_row_get_value(self, num)
    VALUE self, num;
{
    const GdaValue *val = gda_row_get_value(RGDA_ROW(self),
                                            FIX2INT(num));
    return val != NULL
        ? RGDA_VALUE_NEW(val)
        : Qnil;
}

void Init_gda_row(void) {
    VALUE c = G_DEF_CLASS(GDA_TYPE_ROW, "Row", mGda);
  
    rb_define_method(c, "initialize", rb_gda_row_new,      -1);
    rb_define_method(c, "model",      rb_gda_row_get_model, 0);
 
    rb_define_method(c, "set_number",    rb_gda_row_set_number,    1);
    rb_define_method(c, "number",        rb_gda_row_get_number,    0);
    rb_define_method(c, "set_unique_id", rb_gda_row_set_unique_id, 1);
    rb_define_method(c, "unique_id",     rb_gda_row_get_unique_id, 0);
 
    rb_define_method(c, "values",     rb_gda_row_get_values, 0); 
    rb_define_method(c, "each_value", rb_gda_row_each_value, 0); 
    rb_define_method(c, "length",     rb_gda_row_get_length, 0);
    rb_define_alias(c, "size", "length");

    rb_define_method(c, "get_value", rb_gda_row_get_value, 1);

    G_DEF_SETTERS(c);
}

