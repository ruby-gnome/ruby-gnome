
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

/*
 * Class: Gda::Select
 * Filtering data from Gda::DataModel.
 */
VALUE cGdaSelect;

/*
 * Class method: new
 *
 * Creates a new Gda::Select object, which allows programs to filter
 * Gda::DataModel's based on a given SQL SELECT command.
 *
 * A Gda::Select is just another Gda::DataModel based class, so it can be used
 * in the same way any other data model class is.
 *
 * Returns: the newly created Gda::Select object.
 */
static VALUE rb_gda_select_new(self)
    VALUE self;
{
    GdaDataModel *dm = gda_select_new();
    if (dm != NULL) {
        G_INITIALIZE(self, dm);
    }
    return Qnil;
}

/*
 * Method: add_source(name, source)
 * name: name to identify the data model (usually a table name).
 * source: a Gda::DataModel object from which to get data.
 *
 * Adds a data model as a source of data for the Gda::Select object.
 *
 * When the select object is run (via Gda::Select#run), it will parse the SQL
 * and get the required data from the source data models.
 *
 * Returns: self.
 */
static VALUE rb_gda_select_add_source(self, name, source)
    VALUE self, name, source;
{
    gda_select_add_source(RGDA_SELECT(self),
                          RVAL2CSTR(name),
                          RGDA_DATAMODEL(source));
    return self;
       
}

/*
 * Method: set_sql(sql)
 * sql: the SQL command to be used for filtering rows.
 *
 * Sets the SQL command to be used on the Gda::Select object for filtering rows
 * from the source data model (which is set with Gda::Select#set_source).
 *
 * Returns: self.
 */
static VALUE rb_gda_select_set_sql(self, sql)
    VALUE self, sql;
{
    gda_select_set_sql(RGDA_SELECT(self), RVAL2CSTR(sql));
    return sql;
}

/*
 * Method: run
 *
 * Runs the query and fills in the Gda::Select object with the rows that
 * matched the SQL command (which can be set with Gda::Select#set_sql)
 * associated with this Gda::Select object.
 *
 * After calling this function, if everything is successful, the Gda::Select
 * object will contain the matched rows, which can then be accessed like a
 * normal Gda::DataModel.
 *
 * Returns: true if successful, false if there was an error.
 */
static VALUE rb_gda_select_run(self)
    VALUE self;
{
    return CBOOL2RVAL(gda_select_run(RGDA_SELECT(self)));
}

void Init_gda_select(void) {
    VALUE c = G_DEF_CLASS(GDA_TYPE_SELECT, "Select", mGda);
    
    rb_define_method(c, "initialize", rb_gda_select_new,        0);
    rb_define_method(c, "add_source", rb_gda_select_add_source, 2);
    rb_define_method(c, "set_sql",    rb_gda_select_set_sql,    1);
    rb_define_method(c, "run",        rb_gda_select_run,        0);

    G_DEF_SETTER(c, "sql");

    cGdaSelect = c;
}

