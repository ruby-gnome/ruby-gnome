
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

static VALUE rb_gda_client_new(self)
    VALUE self;
{
    G_INITIALIZE(self, gda_client_new()); 
    return Qnil;
}

static VALUE rb_gda_client_open_connection(argc, argv, self)
    int argc;
    VALUE *argv, self;
{
    VALUE dsn, username, password, options;
    GdaConnection *conn;    

    rb_scan_args(argc, argv, "31", &dsn, &username, &password, &options);

    conn = gda_client_open_connection(RGDA_CLIENT(self),
                                      RVAL2CSTR(dsn),  
                                      NIL_P(username) ? NULL : RVAL2CSTR(username),  
                                      NIL_P(password) ? NULL : RVAL2CSTR(password),  
                                      NIL_P(options) ? 0 : FIX2INT(options));
    if (rb_block_given_p()) {
        if (conn == NULL) {
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

static VALUE rb_gda_client_get_connections(self)
    VALUE self;
{
    GList *list, *node;
    VALUE arr;

    list = (GList *) gda_client_get_connection_list(RGDA_CLIENT(self));
    arr = rb_ary_new();

    for (node = g_list_first(list); 
         node != NULL;
         node = g_list_next(node))
    {
        rb_ary_push(arr, RGDA_CONNECTION_NEW(node->data));
    }

    return arr;
}

static VALUE rb_gda_client_each_connection(self)
    VALUE self;
{
    rb_ary_each(rb_gda_client_get_connections(self));
    return Qnil;
}

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

static VALUE rb_gda_client_close_all_connections(self)
    VALUE self;
{
    gda_client_close_all_connections(RGDA_CLIENT(self));
    return self;
}

static VALUE rb_gda_client_begin_transaction(self, tr)
    VALUE self, tr;
{
    gda_client_begin_transaction(RGDA_CLIENT(self),
                                 RGDA_TRANSACTION(tr));
    return self;
}

static VALUE rb_gda_client_commit_transaction(self, tr)
    VALUE self, tr;
{
    gda_client_commit_transaction(RGDA_CLIENT(self),
                                  RGDA_TRANSACTION(tr));
    return self;
}

static VALUE rb_gda_client_rollback_transaction(self, tr)
    VALUE self, tr;
{
    gda_client_rollback_transaction(RGDA_CLIENT(self),
                                    RGDA_TRANSACTION(tr));
    return self;
}

static VALUE rb_gda_client_notify_event(self, conn, event, params)
    VALUE self, conn, event, params;
{
    gda_client_notify_event(RGDA_CLIENT(self),
                            RGDA_CONNECTION(conn),
                            FIX2INT(event),
                            NIL_P(params) ? NULL : RGDA_PARAMETER_LIST(params));
    return self;
}

static VALUE rb_gda_client_notify_error_event(self, conn, error)
    VALUE self, conn, error;
{
    gda_client_notify_error_event(RGDA_CLIENT(self),
                                  RGDA_CONNECTION(conn),
                                  RGDA_ERROR(error));
    return self;
}

static VALUE rb_gda_client_notify_connection_opened_event(self, conn)
    VALUE self, conn;
{
    gda_client_notify_connection_opened_event(RGDA_CLIENT(self),
                                              RGDA_CONNECTION(conn));
    return self;
}

static VALUE rb_gda_client_notify_connection_closed_event(self, conn)
    VALUE self, conn;
{
    /* not exported in libgda <= 1.0.0 */
    void gda_client_notify_connection_closed_event(GdaClient *, 
                                                   GdaConnection *);

    gda_client_notify_connection_closed_event(RGDA_CLIENT(self),
                                              RGDA_CONNECTION(conn));
    return self;
}

static VALUE rb_gda_client_notify_transaction_started_event(self, conn, tr)
    VALUE self, conn, tr;
{
    gda_client_notify_transaction_started_event(RGDA_CLIENT(self),
                                                RGDA_CONNECTION(conn),
                                                RGDA_TRANSACTION(tr));
    return self;
}

static VALUE rb_gda_client_notify_transaction_committed_event(self, conn, tr)
    VALUE self, conn, tr;
{
    gda_client_notify_transaction_committed_event(RGDA_CLIENT(self),
                                                  RGDA_CONNECTION(conn),
                                                  RGDA_TRANSACTION(tr));
    return self;
}

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

    rb_define_const(c, "EVENT_ERROR", 
                    INT2FIX(GDA_CLIENT_EVENT_ERROR));
    rb_define_const(c, "EVENT_CONNECTION_OPENED", 
                    INT2FIX(GDA_CLIENT_EVENT_CONNECTION_OPENED));
    rb_define_const(c, "EVENT_CONNECTION_CLOSED", 
                    INT2FIX(GDA_CLIENT_EVENT_CONNECTION_CLOSED));
    rb_define_const(c, "EVENT_TRANSACTION_STARTED", 
                    INT2FIX(GDA_CLIENT_EVENT_TRANSACTION_STARTED));
    rb_define_const(c, "EVENT_TRANSACTION_COMMITTED", 
                    INT2FIX(GDA_CLIENT_EVENT_TRANSACTION_COMMITTED));
    rb_define_const(c, "EVENT_TRANSACTION_CANCELLED", 
                    INT2FIX(GDA_CLIENT_EVENT_TRANSACTION_CANCELLED));
}

