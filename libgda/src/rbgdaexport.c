
/*
 * Copyright (C) 2003, 2004 Laurent Sansonetti <lrz@gnome.org>
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

/*
 * Class: Gda::Export
 * Exporting of databases.
 */
VALUE cGdaExport;

/*
 * Class method: new(conn)
 * conn: a Gda::Connection object.
 *
 * Creates a new Gda::Export object, which allows you to easily add exporting
 * functionality to your programs.
 *
 * It works by first having a Gda::Connection object associated to it, and then
 * allowing you to retrieve information about all the objects present in the
 * database, and also to add/remove those objects from a list of selected
 * objects.
 *
 * When you're done, you just run the export (Gda::Export#run), first
 * connecting to the different signals that will let you be informed of the
 * export process progress.
 *
 * Returns: a newly created Gda::Export object.
 */
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

/*
 * Method: tables
 *
 * Returns a list of all tables that exist in the Gda::Connection being used by
 * the Gda::Export object. This method is useful when you're building, for
 * example, a list for the user to select which tables he/she wants in the
 * export process.
 *
 * Returns: an Array containing the name of all the tables.
 */
static VALUE rb_gda_export_get_tables(self)
    VALUE self;
{
    return tables2array(gda_export_get_tables(RGDA_EXPORT(self)));
}

/*
 * Method: selected_tables
 *
 * Returns a list with the names of all the currently selected objects in the
 * Gda::Export object.
 *
 * Returns: an Array containing the names of the selected tables.
 */
static VALUE rb_gda_export_get_sel_tables(self)
    VALUE self;
{
    return tables2array(gda_export_get_selected_tables(RGDA_EXPORT(self)));
}

/*
 * Method: select_tables(*names)
 * names: list of table names to be selected.
 *
 * Adds all the given tables to the list of selected tables.
 *
 * Returns: self.
 */
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

/*
 * Method: unselect_tables(*names)
 * names: list of table names to be unselected.
 *
 * Removes the given table names from the list of selected tables.
 *
 * Returns: self.
 */
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

/*
 * Method: run(flags=Gda::Export::FLAGS_TABLE_DATA)
 * flags: executing flags (see Gda::Export::Flags).
 *
 * Starts the execution of the given export object. This means that, after
 * calling this method, your application will lose control about the
 * export process and will only receive notifications via the class signals.
 *
 * Returns: self.
 */
static VALUE rb_gda_export_run(argc, argv, self)
    int argc;
    VALUE *argv, self;
{
    VALUE flags;

    rb_scan_args(argc, argv, "01", &flags);
    gda_export_run(RGDA_EXPORT(self), 
                   NIL_P(flags) ? GDA_EXPORT_FLAGS_TABLE_DATA : RVAL2GENUM(flags, GDA_TYPE_EXPORT_FLAGS));
    return self;
}

/*
 * Method: stop
 *
 * Stops execution of the export object.
 *
 * Returns: self.
 */
static VALUE rb_gda_export_stop(self)
    VALUE self;
{
    gda_export_stop(RGDA_EXPORT(self));
    return self;
}

/*
 * Method: connection
 *
 * Returns: the Gda::Connection object associated with the export object.
 */
static VALUE rb_gda_export_get_connection(self)
    VALUE self;
{
    return RGDA_CONNECTION_NEW(gda_export_get_connection(RGDA_EXPORT(self)));
}

/*
 * Method: set_connection(conn)
 * conn: a Gda::Connection object.
 *
 * Associates the given Gda::Connection object with the export object.
 *
 * Returns: self.
 */ 
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

    G_DEF_CLASS(GDA_TYPE_EXPORT_FLAGS, "Flags", c);
    G_DEF_CONSTANTS(c, GDA_TYPE_EXPORT_FLAGS, "GDA_EXPORT_");
    G_DEF_SETTER(c, "connection");

    cGdaExport = c;
}

