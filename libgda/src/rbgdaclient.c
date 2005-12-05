
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
 * Class: Gda::Client
 *
 * This class is the main entry point for Ruby/Libgda client applications.
 * It provides the way by which client applications open connections. 
 * Thus, before using any other database-oriented function in libgda,
 * applications must create a Gda::Client object (via Gda::Client.new), 
 * and, once created, open the connections from it.
 *
 * Gda::Client also provides a way to treat several connections as if they were
 * only one (a connection pool), which allows applications to, for instance,
 * commit/rollback a transaction in all the connections being managed by a
 * unique GdaClient object, or obtain the list of all tables in all opened
 * connections.
 */
VALUE cGdaClient;

/*
 * Class method: new
 * 
 * Creates a new Gda::Client object, which is the entry point for libgda client
 * applications. This object, once created, can be used for opening new database
 * connections and activating other services available to GDA clients.
 *
 * Returns: the newly created object.
 */
static VALUE rb_gda_client_new(self)
    VALUE self;
{
    G_INITIALIZE(self, gda_client_new()); 
    return Qnil;
}

/*
 * Method: open_connection(dsn, username, password, options=0)
 * dsn: data source name.
 * username: user name.
 * password: password for user name.
 * options: options for the connection (see Gda::Connection::Options).
 *
 * Establishes a connection to a data source. The connection will be opened if
 * no identical connection is available in the GdaClient connection pool, and
 * re-used if available. If you dont want to share the connection, specify
 * Gda::Connection::OPTIONS_DONT_SHARE as one of the flags in the options
 * parameter.
 *
 * When provided with a block of code, Gda::Client#open_connection will then
 * call the block when the connection will be enabled, passing a reference to
 * a Gda::Connection object as parameter.  At the end of the block, it will
 * automatically disconnect from the data source, calling Gda::Connection#close
 * on the connection object.
 *
 * 	client = Gda::Client.new
 * 	client.open_connection ('arrakis', nil, nil) do |conn|
 * 	# ...
 * 	end
 *
 * When no block code is provided, Gda::Client#open_connection simply returns
 * a reference to a Gda::Connection object.  You should take care to disconnect
 * by yourself, calling Gda::Connection#close manually.
 *
 * 	client = Gda::Client.new
 * 	conn = client.open_connection ('arrakis', nil, nil)
 * 	# ...
 * 	conn.close
 * 
 * Returns: when used with a block of code, always nil, when used without a
 * a block of code, a Gda::Connection object on success, nil if there is an
 * error.
 */
static VALUE rb_gda_client_open_connection(argc, argv, self)
    int argc;
    VALUE *argv, self;
{
    VALUE dsn, username, password, options;
    GdaConnection *conn;    
    GError *error;
    
    rb_scan_args(argc, argv, "31", &dsn, &username, &password, &options);

    error = NULL;
    
    conn = gda_client_open_connection(RGDA_CLIENT(self),
                                      RVAL2CSTR(dsn),
                                      NIL_P(username) ? NULL : RVAL2CSTR(username),  
                                      NIL_P(password) ? NULL : RVAL2CSTR(password),  
                                      NIL_P(options) 
                                        ? 0 
                                        : RVAL2GFLAGS(options, 
                                                      GDA_TYPE_CONNECTION_OPTIONS)
#if defined(GDA_AT_LEAST_1_3)
                                      , &error
#endif
                                      );
    if (rb_block_given_p()) {
        if (conn == NULL) {
            if (error != NULL)
                RAISE_GERROR(error);
            
            rb_raise(rb_eRuntimeError, "Could not create connection...");
        }
        else {
            VALUE r_conn = RGDA_CONNECTION_NEW(conn);
            rb_ensure(rb_yield, r_conn, rb_gda_connection_close, r_conn);
        }
        return Qnil;
    }
    return conn != NULL
        ? RGDA_CONNECTION_NEW(conn)
        : Qnil;
}

/*
 * Method: connections
 *
 * Gets the list of all opened connections in the Gda::Client object.
 *
 * Returns: an Array of Gda::Connection objects.
 */
static VALUE rb_gda_client_get_connections(self)
    VALUE self;
{
    GList *list, *node;
    VALUE arr;

#if defined(GDA_AT_LEAST_1_3)
    list = (GList *) gda_client_get_connections(RGDA_CLIENT(self));
#else 
    list = (GList *) gda_client_get_connection_list(RGDA_CLIENT(self));
#endif
    arr = rb_ary_new();

    for (node = g_list_first(list); 
         node != NULL;
         node = g_list_next(node))
    {
        rb_ary_push(arr, RGDA_CONNECTION_NEW(node->data));
    }

    return arr;
}

/*
 * Method: each_connection { |conn| ... }
 *
 * Calls the block for each opened connection by the client, passing
 * a reference to a Gda::Connection object as parameter.
 *
 * Returns: always nil.
 */
static VALUE rb_gda_client_each_connection(self)
    VALUE self;
{
    rb_ary_each(rb_gda_client_get_connections(self));
    return Qnil;
}

/*
 * Method: find_connection(dsn, username, password)
 * dsn: data source name.
 * username: user name.
 * password: password for user name.
 *
 * Looks for an open connection given a data source name (per libgda
 * configuration), a username and a password.
 *
 * This method iterates over the list of open connections in the 
 * Gda::Client object and looks for one that matches the given data source
 * name, username and password.
 *
 * Returns: a reference to the found Gda::Connection object, or nil if it
 * could not be found.
 */
static VALUE rb_gda_client_find_connection(self, dsn, username, password)
    VALUE self, dsn, username, password;
{
    GdaConnection *conn = gda_client_find_connection(RGDA_CLIENT(self),
                                                     RVAL2CSTR(dsn),
                                                     RVAL2CSTR(username),
                                                     RVAL2CSTR(password));
    return conn != NULL
        ? RGDA_CONNECTION_NEW(conn)
        : Qnil;
}

/*
 * Method: close_all_connections
 * 
 * Closes all connections opened by the Gda::Client object.
 *
 * Returns: self.
 */
static VALUE rb_gda_client_close_all_connections(self)
    VALUE self;
{
    gda_client_close_all_connections(RGDA_CLIENT(self));
    return self;
}

/*
 * Method: begin_transaction(xaction)
 * xaction: a Gda::Transaction object.
 *
 * Starts a transaction on all connections being managed by the Gda::Client.
 * It is important to note that this operates on all connections opened within
 * a Gda::Client, which could not be what you're looking for.
 *
 * To execute a transaction on a unique connection, use
 * Gda::Connection#begin_transaction, Gda::Connection#commit_transaction and
 * Gda::Connection#rollback_transaction.
 *
 * Returns: true if all transactions could be started successfully,
 * or false if one of them fails.
 */
static VALUE rb_gda_client_begin_transaction(self, tr)
    VALUE self, tr;
{
    gda_client_begin_transaction(RGDA_CLIENT(self),
                                 RGDA_TRANSACTION(tr));
    return self;
}

/*
 * Method: commit_transaction(xaction)
 * xaction: a Gda::Transaction object.
 *
 * Commits a transaction on all connections being managed by the Gda::Client.
 * It is important to note that this operates on all connections opened within
 * a Gda::Client, which could not be what you're looking for.
 *
 * To execute a transaction on a unique connection, use
 * Gda::Connection#begin_transaction, Gda::Connection#commit_transaction and
 * Gda::Connection#rollback_transaction.
 *
 * Returns: true if all transactions could be committed successfully,
 * or false if one of them fails.
 */
static VALUE rb_gda_client_commit_transaction(self, tr)
    VALUE self, tr;
{
    gda_client_commit_transaction(RGDA_CLIENT(self),
                                  RGDA_TRANSACTION(tr));
    return self;
}

/*
 * Method: rollback_transaction(xaction)
 * xaction: a Gda::Transaction object.
 *
 * Cancels a transaction on all connections being managed by the Gda::Client.
 * It is important to note that this operates on all connections opened within
 * a Gda::Client, which could not be what you're looking for.
 *
 * To execute a transaction on a unique connection, use
 * Gda::Connection#begin_transaction, Gda::Connection#commit_transaction and
 * Gda::Connection#rollback_transaction.
 *
 * Returns: true if all transactions could be cancelled successfully,
 * or false if one of them fails.
 */
static VALUE rb_gda_client_rollback_transaction(self, tr)
    VALUE self, tr;
{
    gda_client_rollback_transaction(RGDA_CLIENT(self),
                                    RGDA_TRANSACTION(tr));
    return self;
}

/*
 * Method: notify_event(conn, event, params)
 * conn: a Gda::Connection object were the event has occured.
 * event: an event ID (see Gda::Client::Event).
 * params: parameters associated with the event, as a Gda::ParameterList object.
 *
 * Notifies an event to the Gda::Client's listeners. The event can be anything
 * (see Gda::Client::Event) ranging from a connection opening operation, to
 * changes made to a table in an underlying database.
 *
 * Returns: self.
 */
static VALUE rb_gda_client_notify_event(self, conn, event, params)
    VALUE self, conn, event, params;
{
    gda_client_notify_event(RGDA_CLIENT(self),
                            RGDA_CONNECTION(conn),
                            RVAL2GENUM(event, GDA_TYPE_CLIENT_EVENT),
                            NIL_P(params) ? NULL : RGDA_PARAMETER_LIST(params));
    return self;
}

/*
 * Method: notify_error_event(conn, error)
 * conn: a Gda::Connection object were the event has occured.
 * error: the error to be notified, as a Gda::Error object.
 *
 * Notifies the Gda::Client of the Gda::Client::EVENT_ERROR event.
 *
 * Returns: self.
 */
static VALUE rb_gda_client_notify_error_event(self, conn, error)
    VALUE self, conn, error;
{
    gda_client_notify_error_event(RGDA_CLIENT(self),
                                  RGDA_CONNECTION(conn),
                                  RGDA_ERROR(error));
    return self;
}

/*
 * Method: notify_connection_opened_event(conn)
 * conn: a Gda::Connection object were the event has occured.
 *
 * Notifies the Gda::Client of the Gda::Client::EVENT_CONNECTION_OPENED event.
 *
 * Returns: self.
 */
static VALUE rb_gda_client_notify_connection_opened_event(self, conn)
    VALUE self, conn;
{
    gda_client_notify_connection_opened_event(RGDA_CLIENT(self),
                                              RGDA_CONNECTION(conn));
    return self;
}

/*
 * Method: notify_connection_closed_event(conn)
 * conn: a Gda::Connection object were the event has occured.
 *
 * Notifies the Gda::Client of the Gda::Client::EVENT_CONNECTION_CLOSED event.
 *
 * Returns: self.
 */
static VALUE rb_gda_client_notify_connection_closed_event(self, conn)
    VALUE self, conn;
{
    gda_client_notify_connection_closed_event(RGDA_CLIENT(self),
                                              RGDA_CONNECTION(conn));
    return self;
}

/*
 * Method: notify_transaction_started_event(conn, xaction)
 * conn: a Gda::Connection object were the event has occured.
 * xaction: a Gda::Transaction object.
 *
 * Notifies the Gda::Client of the
 * Gda::Client::EVENT_CONNECTION_TRANSACTION_STARTED event.
 *
 * Returns: self.
 */
static VALUE rb_gda_client_notify_transaction_started_event(self, conn, tr)
    VALUE self, conn, tr;
{
    gda_client_notify_transaction_started_event(RGDA_CLIENT(self),
                                                RGDA_CONNECTION(conn),
                                                RGDA_TRANSACTION(tr));
    return self;
}

/*
 * Method: notify_transaction_committed_event(conn, xaction)
 * conn: a Gda::Connection object were the event has occured.
 * xaction: a Gda::Transaction object.
 *
 * Notifies the Gda::Client of the
 * Gda::Client::EVENT_CONNECTION_TRANSACTION_COMMITTED event.
 *
 * Returns: self.
 */
static VALUE rb_gda_client_notify_transaction_committed_event(self, conn, tr)
    VALUE self, conn, tr;
{
    gda_client_notify_transaction_committed_event(RGDA_CLIENT(self),
                                                  RGDA_CONNECTION(conn),
                                                  RGDA_TRANSACTION(tr));
    return self;
}

/*
 * Method: notify_transaction_cancelled_event(conn, xaction)
 * conn: a Gda::Connection object were the event has occured.
 * xaction: a Gda::Transaction object.
 *
 * Notifies the Gda::Client of the
 * Gda::Client::EVENT_CONNECTION_TRANSACTION_CANCELLED event.
 *
 * Returns: self.
 */
static VALUE rb_gda_client_notify_transaction_cancelled_event(self, conn, tr)
    VALUE self, conn, tr;
{
    gda_client_notify_transaction_cancelled_event(RGDA_CLIENT(self),
                                                  RGDA_CONNECTION(conn),
                                                  RGDA_TRANSACTION(tr));
    return self;
}

void Init_gda_client(void) {
    VALUE c = G_DEF_CLASS(GDA_TYPE_CLIENT, "Client", mGda);

    rb_define_method(c, "initialize", rb_gda_client_new, 0);

    rb_define_method(c, "open_connection", rb_gda_client_open_connection, -1);
    rb_define_method(c, "connections",     rb_gda_client_get_connections,  0);
    rb_define_method(c, "each_connection", rb_gda_client_each_connection,  0);
    rb_define_method(c, "find_connection", rb_gda_client_find_connection,  3);

    rb_define_method(c, "close_all_connections", rb_gda_client_close_all_connections, 0);   
 
    rb_define_method(c, "begin_transaction",    rb_gda_client_begin_transaction,    1);
    rb_define_method(c, "commit_transaction",   rb_gda_client_commit_transaction,   1);
    rb_define_method(c, "rollback_transaction", rb_gda_client_rollback_transaction, 1);

    rb_define_method(c, "notify_event",       rb_gda_client_notify_event,       3);
    rb_define_method(c, "notify_error_event", rb_gda_client_notify_error_event, 2);
    rb_define_method(c, "notify_connection_opened_event", 
                     rb_gda_client_notify_connection_opened_event, 1);
    rb_define_method(c, "notify_connection_closed_event", 
                     rb_gda_client_notify_connection_closed_event, 1);
    rb_define_method(c, "notify_transaction_started_event", 
                     rb_gda_client_notify_transaction_started_event, 2);
    rb_define_method(c, "notify_transaction_committed_event", 
                     rb_gda_client_notify_transaction_committed_event, 2);
    rb_define_method(c, "notify_transaction_cancelled_event", 
                     rb_gda_client_notify_transaction_cancelled_event, 2);

    G_DEF_CLASS(GDA_TYPE_CLIENT_EVENT, "Event", c);
    G_DEF_CONSTANTS(c, GDA_TYPE_CLIENT_EVENT, "GDA_CLIENT_");

    cGdaClient = c;
}

