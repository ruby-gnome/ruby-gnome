
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
                                      RVAL2CSTR(username),  
                                      RVAL2CSTR(password),  
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

void Init_gda_client(void) {
    VALUE c = G_DEF_CLASS(GDA_TYPE_CLIENT, "Client", mGda);

    rb_define_method(c, "initialize", rb_gda_client_new, 0);

    rb_define_method(c, "open_connection", rb_gda_client_open_connection, -1);
    rb_define_method(c, "connections",     rb_gda_client_get_connections,  0);
    rb_define_method(c, "each_connection", rb_gda_client_each_connection,  0);
    rb_define_method(c, "find_connection", rb_gda_client_find_connection,  3);
}

