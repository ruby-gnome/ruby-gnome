
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

static VALUE rb_gda_export_new(self, conn)
    VALUE self, conn;
{
    GdaExport *export = gda_export_new(RGDA_CONNECTION(conn));
    if (export != NULL) {
        G_INITIALIZE(self, export);
    }
    return Qnil;
}

static VALUE tables2array(list)
    GList *list;
{
    VALUE arr = rb_ary_new();
    if (list != NULL) {
        GList *node;
        
        for (node = list;
             node != NULL;
             node = g_list_next(node))
        {
            rb_ary_push(arr, CSTR2RVAL(node->data));
        }
        g_list_free(list);
    }
    return arr;
}

static VALUE rb_gda_export_get_tables(self)
    VALUE self;
{
    return tables2array(gda_export_get_tables(RGDA_EXPORT(self)));
}

static VALUE rb_gda_export_get_sel_tables(self)
    VALUE self;
{
    return tables2array(gda_export_get_selected_tables(RGDA_EXPORT(self)));
}

static VALUE rb_gda_export_select_tables(self, tables)
    VALUE self, tables;
{
    int i;
    
    if (RARRAY(tables)->len == 0) {
        rb_raise(rb_eArgError, "Wrong # of arguments (need at least 1)");
    }

    for (i = 0; i < RARRAY(tables)->len; i++) {
        VALUE v = rb_ary_entry(tables, i);
        gda_export_select_table(RGDA_EXPORT(self), RVAL2CSTR(v));
    }
    return self;
}

static VALUE rb_gda_export_unselect_tables(self, tables)
    VALUE self, tables;
{
    int i;
    
    if (RARRAY(tables)->len == 0) {
        rb_raise(rb_eArgError, "Wrong # of arguments (need at least 1)");
    }

    for (i = 0; i < RARRAY(tables)->len; i++) {
        VALUE v = rb_ary_entry(tables, i);
        gda_export_unselect_table(RGDA_EXPORT(self), RVAL2CSTR(v));
    }
    return self;
}

static VALUE rb_gda_export_run(argc, argv, self)
    int argc;
    VALUE *argv, self;
{
    VALUE flags;

    rb_scan_args(argc, argv, "01", &flags);
    gda_export_run(RGDA_EXPORT(self), 
                   NIL_P(flags) ? GDA_EXPORT_FLAGS_TABLE_DATA : FIX2INT(flags));
    return self;
}

static VALUE rb_gda_export_stop(self)
    VALUE self;
{
    gda_export_stop(RGDA_EXPORT(self));
    return self;
}

static VALUE rb_gda_export_get_connection(self)
    VALUE self;
{
    return RGDA_CONNECTION_NEW(gda_export_get_connection(RGDA_EXPORT(self)));
}

static VALUE rb_gda_export_set_connection(self, conn)
    VALUE self, conn;
{
    gda_export_set_connection(RGDA_EXPORT(self),
                              RGDA_CONNECTION(conn));
    return self;
}

void Init_gda_export(void) {
    VALUE c = G_DEF_CLASS(GDA_TYPE_EXPORT, "Export", mGda);

    rb_define_method(c, "initialize", rb_gda_export_new, 1);

    rb_define_method(c, "tables",          rb_gda_export_get_tables,       0);
    rb_define_method(c, "selected_tables", rb_gda_export_get_sel_tables,   0);
    rb_define_method(c, "select_tables",   rb_gda_export_select_tables,   -2);
    rb_define_method(c, "unselect_tables", rb_gda_export_unselect_tables, -2);

    rb_define_method(c, "run",  rb_gda_export_run,  -1);
    rb_define_method(c, "stop", rb_gda_export_stop,  0);

    rb_define_method(c, "connection",     rb_gda_export_get_connection, 0);
    rb_define_method(c, "set_connection", rb_gda_export_set_connection, 1);

    G_DEF_CONSTANTS(c, GDA_TYPE_EXPORT_FLAGS, "GDA_EXPORT_");
    G_DEF_SETTER(c, "connection");
}

