
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
 * Class: Gda::Connection
 *
 * The Gda::Connection class offers access to all operations involving an
 * opened connection to a database. Gda::Connection objects are obtained via
 * the Gda::Client class.
 *
 * Once obtained, applications can use GdaConnection to execute commands, run
 * transactions, and get information about all objects stored in the
 * underlying database.
 */
VALUE cGdaConnection;

/*
 * Method: close
 *
 * Closes the connection to the underlying data source. After calling this
 * method, you should not use anymore the Gda::Connection object, since it may
 * have been destroyed.
 *
 * Returns: nil.
 */
VALUE rb_gda_connection_close(self)
    VALUE self;
{
    gda_connection_close(RGDA_CONNECTION(self));
    return Qnil;    /* object should be destroyed */
}

/*
 * Method: opened?
 *
 * Checks whether a connection is open or not.
 *
 * Returns: true if the connection is open, false if it is not.
 */
static VALUE rb_gda_connection_is_opened(self)
    VALUE self;
{
    return CBOOL2RVAL(gda_connection_is_open(RGDA_CONNECTION(self))); 
}

/*
 * Method: dsn
 *
 * Returns: the data source name the connection object is connected to.
 */
static VALUE rb_gda_connection_get_dsn(self)
    VALUE self;
{
    return CSTR2RVAL(gda_connection_get_dsn(RGDA_CONNECTION(self)));
}

/*
 * Method: username
 *
 * Returns: the user name used to open this connection.
 */
static VALUE rb_gda_connection_get_username(self)
    VALUE self;
{
    return CSTR2RVAL(gda_connection_get_username(RGDA_CONNECTION(self)));
}

/*
 * Method: password
 *
 * Returns: the pass word used to open this connection.
 */
static VALUE rb_gda_connection_get_password(self)
    VALUE self;
{
    return CSTR2RVAL(gda_connection_get_password(RGDA_CONNECTION(self)));
}

/*
 * Method: cnc_string
 *
 * Gets the connection string used to open this connection.
 *
 * The connection string is the string sent over to the underlying database
 * provider, which describes the parameters to be used to open a connection on
 * the underlying data source.
 *
 * Returns: the connection string used when opening this connection.
 */
static VALUE rb_gda_connection_get_cnc_string(self)
    VALUE self;
{
    return CSTR2RVAL(gda_connection_get_cnc_string(RGDA_CONNECTION(self)));
}

/*
 * Method: server_version
 *
 * Returns: the version string of the underlying database server.
 */
static VALUE rb_gda_connection_get_server_version(self)
    VALUE self;
{
    return CSTR2RVAL(gda_connection_get_server_version(RGDA_CONNECTION(self)));
}

/*
 * Method: options
 *
 * Returns: the Gda::Connection::Options used to open this connection.
 */
static VALUE rb_gda_connection_get_options(self)
    VALUE self;
{
    return INT2FIX(gda_connection_get_options(RGDA_CONNECTION(self)));
}

/*
 * Method: provider
 *
 * Returns: the provider ID used to open this connection.
 */
static VALUE rb_gda_connection_get_provider(self)
    VALUE self;
{
    return CSTR2RVAL(gda_connection_get_provider(RGDA_CONNECTION(self)));
}

/*
 * Method: client
 *
 * Gets the Gda::Client object associated with a connection. This is always the
 * client that created the connection, as returned by
 * Gda::Client#open_connection.
 *
 * Returns: the Gda::Client to which the connection belongs to.
 */
static VALUE rb_gda_connection_get_client(self)
    VALUE self;
{
    GdaClient *client = gda_connection_get_client(RGDA_CONNECTION(self));
    return client != NULL
        ? RGDA_CLIENT_NEW(client)
        : Qnil;
}

/*
 * Method: set_client(client)
 * client: a Gda::Client object.
 *
 * Associates a Gda::Client with this connection. This method is not intended
 * to be called by applications.
 *
 * Returns: self.
 */
static VALUE rb_gda_connection_set_client(self, client)
    VALUE self, client;
{
    gda_connection_set_client(RGDA_CONNECTION(self),
                              RGDA_CLIENT(client));
    return self;
}

static GdaParameterList *parse_params(argc, argv, cmd)
    int argc;
    VALUE *argv, *cmd;
{
    VALUE list;
    rb_scan_args(argc, argv, "11", cmd, &list);
    return NIL_P(list) 
        ? NULL 
        : RGDA_PARAMETER_LIST(list);
}

/*
 * Method: execute_single_command(cmd, params=nil)
 * cmd: a Gda::Command object.
 * params: parameters, as a Gda::ParameterList object.
 *
 * Executes a single command on the given connection.
 *
 * This method lets you retrieve a simple data model from the underlying
 * difference, instead of having to retrieve a list of them, as is the case
 * with Gda::Connection#execute_command.
 *
 * Returns: a Gda::DataModel containing the data returned by the data source,
 * or nil on error.
 */
static VALUE rb_gda_connection_execute_single_command(argc, argv, self)
    int argc;
    VALUE *argv, self;
{
    GdaParameterList *plist;
    GdaDataModel *mod;
    VALUE cmd;
    
    plist = parse_params(argc, argv, &cmd);
    mod = gda_connection_execute_single_command(RGDA_CONNECTION(self),
                                                RGDA_COMMAND(cmd),
                                                plist);
    return mod != NULL
        ? RGDA_DATAMODEL_NEW(mod)
        : Qnil;
}

/*
 * Method: execute_command(cmd, params=nil)
 * cmd: a Gda::Command object.
 * params: parameters, as a Gda::ParameterList object.
 *
 * Executes a command on the underlying data source.
 *
 * This method provides the way to send several commands at once to the data
 * source being accessed by the Gda::Connection. The Gda::Command specified
 * can contain a list of commands in its "text" property (usually a set of SQL
 * commands separated by ';').
 *
 * Returns: an Array of Gda::DataModel objects, as returned by the underlying
 * provider.
 */
static VALUE rb_gda_connection_execute_command(argc, argv, self)
    int argc;
    VALUE *argv, self;
{
    GdaParameterList *plist;
    VALUE cmd, arr;
    GList *list;
    
    arr = rb_ary_new();
    plist = parse_params(argc, argv, &cmd);
    for (list = gda_connection_execute_command(RGDA_CONNECTION(self),
                                               RGDA_COMMAND(cmd),
                                               plist);
         list != NULL;
         list = g_list_next(list))
    {
        rb_ary_push(arr, RGDA_DATAMODEL_NEW(list->data));
    }
    return arr;        
}

/*
 * Method: execute_non_query(cmd, params=nil)
 * cmd: a Gda::Command object.
 * params: parameters, as a Gda::ParameterList object.
 *
 * Executes a single command on the underlying database, and gets the number
 * of rows affected.
 *
 * Returns: the number of affected rows by the executed command, or -1 on
 * error.
 */
static VALUE rb_gda_connection_execute_non_query(argc, argv, self)
    int argc;
    VALUE *argv, self;
{
    GdaParameterList *plist;
    VALUE cmd;

    plist = parse_params(argc, argv, &cmd);
    return INT2FIX(gda_connection_execute_non_query(RGDA_CONNECTION(self),
                                                    RGDA_COMMAND(cmd),
                                                    plist));
}

/*
 * Method: errors
 *
 * Retrieves a list of the last errors ocurred in the connection.
 *
 * Returns: an Array of Gda::Error objects.
 */
static VALUE rb_gda_connection_get_errors(self)
    VALUE self;
{
    const GList* list;
    VALUE arr;

    arr = rb_ary_new();
    for (list = gda_connection_get_errors(RGDA_CONNECTION(self));
         list != NULL;
         list = g_list_next(list))
    {
        rb_ary_push(arr, RGDA_ERROR_NEW(list->data));
    }
    return arr;
}

/*
 * Method: add_errors(*errors)
 * errors: one or several Gda::Error objects.
 * 
 * Adds one or more errors to the connection. This method is usually called by
 * providers, to inform clients of errors that happened during some operation.
 *
 * As soon as this method is called, the connection object (and the associated
 * Gda::Client object) emits the "error" signal, to which clients can connect
 * to be informed of errors.
 *
 * Returns: self.
 */
static VALUE rb_gda_connection_add_errors(self, errors)
    VALUE self, errors;
{
    GList *list = NULL;
    int i;

    for (i = 0; i < RARRAY(errors)->len; i++) {
        list = g_list_append(list, RGDA_ERROR(rb_ary_entry(errors, i)));
    }
    gda_connection_add_error_list(RGDA_CONNECTION(self), list);
    /* the list is automatically freed */
    return self;
}

/*
 * Method: begin_transaction(xaction)
 * xaction: a Gda::Transaction object.
 *
 * Starts a transaction on the data source, identified by the xaction
 * parameter.
 *
 * Before starting a transaction, you can check whether the underlying provider
 * does support transactions or not by using the Gda::Connection#supports?
 * method.
 *
 * Returns: true if the transaction was started successfully, false otherwise.
 */
static VALUE rb_gda_connection_begin_transaction(self, tr)
    VALUE self, tr;
{
    gda_connection_begin_transaction(RGDA_CONNECTION(self),
                                     RGDA_TRANSACTION(tr));
    return self;
}

/*
 * Method: commit_transaction(xaction)
 * xaction: a Gda::Transaction object.
 *
 * Commits the given transaction to the backend database. You need to do
 * Gda::Connection#begin_transaction first.
 *
 * Returns: true if the transaction was finished successfully, false otherwise.
 */
static VALUE rb_gda_connection_commit_transaction(self, tr)
    VALUE self, tr;
{
    gda_connection_commit_transaction(RGDA_CONNECTION(self),
                                      RGDA_TRANSACTION(tr));
    return self;
}

/*
 * Method: rollback_transaction(xaction)
 * xaction: a Gda::Transaction object.
 *
 * Rollbacks the given transaction. This means that all changes made to the
 * underlying data source since the last call to 
 * Gda:Connection#begin_transaction or Gda::Connection#commit_transaction
 * will be discarded.
 *
 * Returns: true if the operation was successful, false otherwise.
 */
static VALUE rb_gda_connection_rollback_transaction(self, tr)
    VALUE self, tr;
{
    gda_connection_rollback_transaction(RGDA_CONNECTION(self),
                                        RGDA_TRANSACTION(tr));
    return self;
}

/*
 * Method: database
 *
 * Returns: the name of the currently active database in the connection.
 */
static VALUE rb_gda_connection_get_database(self)
    VALUE self;
{
    return CSTR2RVAL(gda_connection_get_database(RGDA_CONNECTION(self)));
}

/*
 * Method: change_database(name)
 * name: name of the database to switch to.
 *
 * Changes the current database for the given connection. This operation is
 * not available in all providers.
 *
 * Returns: true if successful, false otherwise.
 */
static VALUE rb_gda_connection_change_database(self, db)
    VALUE self, db;
{
    return CBOOL2RVAL(gda_connection_change_database(RGDA_CONNECTION(self),
                                                     RVAL2CSTR(db)));
}

/*
 * Method: create_database(name)
 * name: database name.
 *
 * Creates a new database on the connection.
 *
 * Returns: true if successful, false otherwise.
 */
static VALUE rb_gda_connection_create_database(self, db)
    VALUE self, db;
{
    return CBOOL2RVAL(gda_connection_create_database(RGDA_CONNECTION(self),
                                                     RVAL2CSTR(db)));
}

/*
 * Method: drop_database(name)
 * name: database name.
 *
 * Drops a database from the connection.
 *
 * Returns: true if successful, false otherwise.
 */
static VALUE rb_gda_connection_drop_database(self, db)
    VALUE self, db;
{
    return CBOOL2RVAL(gda_connection_drop_database(RGDA_CONNECTION(self),
                                                   RVAL2CSTR(db)));
}

/*
 * Method: supports?(feature)
 * feature: feature to ask for (see Gda::Connection::Feature).
 *
 * Asks the underlying provider for if a specific feature is supported.
 *
 * Returns: true if the provider supports it, false if not.
 */
static VALUE rb_gda_connection_supports(self, feature)
    VALUE self, feature;
{
    return CBOOL2RVAL(gda_connection_supports(RGDA_CONNECTION(self),
                                              FIX2INT(feature)));
}

void Init_gda_connection(void) {
    VALUE c = G_DEF_CLASS(GDA_TYPE_CONNECTION, "Connection", mGda);

    rb_define_method(c, "close",   rb_gda_connection_close,     0);
    rb_define_method(c, "opened?", rb_gda_connection_is_opened, 0);
  
    rb_define_method(c, "dsn",            rb_gda_connection_get_dsn,            0);
    rb_define_method(c, "username",       rb_gda_connection_get_username,       0);
    rb_define_method(c, "password",       rb_gda_connection_get_password,       0);
    rb_define_method(c, "cnc_string",     rb_gda_connection_get_cnc_string,     0);
    rb_define_method(c, "server_version", rb_gda_connection_get_server_version, 0);
    rb_define_method(c, "options",        rb_gda_connection_get_options,        0);
    rb_define_method(c, "provider",       rb_gda_connection_get_provider,       0);

    rb_define_method(c, "client",     rb_gda_connection_get_client, 0);
    rb_define_method(c, "set_client", rb_gda_connection_set_client, 1);

    rb_define_method(c, "execute_command",        rb_gda_connection_execute_command,        -1);
    rb_define_method(c, "execute_single_command", rb_gda_connection_execute_single_command, -1);
    rb_define_method(c, "execute_non_query",      rb_gda_connection_execute_non_query,      -1);

    rb_define_method(c, "begin_transaction",    rb_gda_connection_begin_transaction,    1);
    rb_define_method(c, "commit_transaction",   rb_gda_connection_commit_transaction,   1);
    rb_define_method(c, "rollback_transaction", rb_gda_connection_rollback_transaction, 1);

    rb_define_method(c, "errors",     rb_gda_connection_get_errors,  0);
    rb_define_method(c, "add_errors", rb_gda_connection_add_errors, -2);

    rb_define_method(c, "database",        rb_gda_connection_get_database,    0);
    rb_define_method(c, "change_database", rb_gda_connection_change_database, 0);
    rb_define_method(c, "create_database", rb_gda_connection_create_database, 0);
    rb_define_method(c, "drop_database",   rb_gda_connection_drop_database,   0);

    rb_define_method(c, "supports?", rb_gda_connection_supports, 1);

    G_DEF_CLASS(GDA_TYPE_CONNECTION_OPTIONS, "Options", c);
    G_DEF_CONSTANTS(c, GDA_TYPE_CONNECTION_OPTIONS, "GDA_CONNECTION_");
    G_DEF_CLASS(GDA_TYPE_CONNECTION_FEATURE, "Feature", c);
    G_DEF_CONSTANTS(c, GDA_TYPE_CONNECTION_FEATURE, "GDA_CONNECTION_");
    G_DEF_CLASS(GDA_TYPE_CONNECTION_SCHEMA, "Schema", c);
    G_DEF_CONSTANTS(c, GDA_TYPE_CONNECTION_SCHEMA,  "GDA_CONNECTION_");
    G_DEF_SETTERS(c);

    cGdaConnection = c;
}

