
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

VALUE rb_gda_connection_close(self)
    VALUE self;
{
    gda_connection_close(RGDA_CONNECTION(self));
    return Qnil;    /* object should be destroyed */
}

static VALUE rb_gda_connection_is_opened(self)
    VALUE self;
{
    return CBOOL2RVAL(gda_connection_is_open(RGDA_CONNECTION(self))); 
}

static VALUE rb_gda_connection_get_dsn(self)
    VALUE self;
{
    return CSTR2RVAL(gda_connection_get_dsn(RGDA_CONNECTION(self)));
}

static VALUE rb_gda_connection_get_username(self)
    VALUE self;
{
    return CSTR2RVAL(gda_connection_get_username(RGDA_CONNECTION(self)));
}

static VALUE rb_gda_connection_get_password(self)
    VALUE self;
{
    return CSTR2RVAL(gda_connection_get_password(RGDA_CONNECTION(self)));
}

static VALUE rb_gda_connection_get_cnc_string(self)
    VALUE self;
{
    return CSTR2RVAL(gda_connection_get_cnc_string(RGDA_CONNECTION(self)));
}

static VALUE rb_gda_connection_get_server_version(self)
    VALUE self;
{
    return CSTR2RVAL(gda_connection_get_server_version(RGDA_CONNECTION(self)));
}

static VALUE rb_gda_connection_get_options(self)
    VALUE self;
{
    return INT2FIX(gda_connection_get_options(RGDA_CONNECTION(self)));
}

static VALUE rb_gda_connection_get_provider(self)
    VALUE self;
{
    return CSTR2RVAL(gda_connection_get_provider(RGDA_CONNECTION(self)));
}

static VALUE rb_gda_connection_get_client(self)
    VALUE self;
{
    GdaClient *client = gda_connection_get_client(RGDA_CONNECTION(self));
    return client != NULL
        ? RGDA_CLIENT_NEW(client)
        : Qnil;
}

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

static VALUE rb_gda_connection_begin_transaction(self, tr)
    VALUE self, tr;
{
    gda_connection_begin_transaction(RGDA_CONNECTION(self),
                                     RGDA_TRANSACTION(tr));
    return self;
}


static VALUE rb_gda_connection_commit_transaction(self, tr)
    VALUE self, tr;
{
    gda_connection_commit_transaction(RGDA_CONNECTION(self),
                                      RGDA_TRANSACTION(tr));
    return self;
}

static VALUE rb_gda_connection_rollback_transaction(self, tr)
    VALUE self, tr;
{
    gda_connection_rollback_transaction(RGDA_CONNECTION(self),
                                        RGDA_TRANSACTION(tr));
    return self;
}

static VALUE rb_gda_connection_get_database(self)
    VALUE self;
{
    return CSTR2RVAL(gda_connection_get_database(RGDA_CONNECTION(self)));
}

static VALUE rb_gda_connection_change_database(self, db)
    VALUE self, db;
{
    return CBOOL2RVAL(gda_connection_change_database(RGDA_CONNECTION(self),
                                                     RVAL2CSTR(db)));
}

static VALUE rb_gda_connection_create_database(self, db)
    VALUE self, db;
{
    return CBOOL2RVAL(gda_connection_create_database(RGDA_CONNECTION(self),
                                                     RVAL2CSTR(db)));
}

static VALUE rb_gda_connection_drop_database(self, db)
    VALUE self, db;
{
    return CBOOL2RVAL(gda_connection_drop_database(RGDA_CONNECTION(self),
                                                   RVAL2CSTR(db)));
}

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

    rb_define_const(c, "OPTIONS_READ_ONLY",  INT2FIX(GDA_CONNECTION_OPTIONS_READ_ONLY));
    rb_define_const(c, "OPTIONS_DONT_SHARE", INT2FIX(GDA_CONNECTION_OPTIONS_DONT_SHARE));

    rb_define_const(c, "FEATURE_AGGREGATES",   INT2FIX(GDA_CONNECTION_FEATURE_AGGREGATES));
    rb_define_const(c, "FEATURE_INDEXES",      INT2FIX(GDA_CONNECTION_FEATURE_INDEXES));
    rb_define_const(c, "FEATURE_INHERITANCE",  INT2FIX(GDA_CONNECTION_FEATURE_INHERITANCE));
    rb_define_const(c, "FEATURE_NAMESPACES",   INT2FIX(GDA_CONNECTION_FEATURE_NAMESPACES));
    rb_define_const(c, "FEATURE_PROCEDURES",   INT2FIX(GDA_CONNECTION_FEATURE_PROCEDURES));
    rb_define_const(c, "FEATURE_SEQUENCES",    INT2FIX(GDA_CONNECTION_FEATURE_SEQUENCES));
    rb_define_const(c, "FEATURE_SQL",          INT2FIX(GDA_CONNECTION_FEATURE_SQL));
    rb_define_const(c, "FEATURE_TRANSACTIONS", INT2FIX(GDA_CONNECTION_FEATURE_TRANSACTIONS));
    rb_define_const(c, "FEATURE_TRIGGERS",     INT2FIX(GDA_CONNECTION_FEATURE_TRIGGERS));
    rb_define_const(c, "FEATURE_USERS",        INT2FIX(GDA_CONNECTION_FEATURE_USERS));
    rb_define_const(c, "FEATURE_VIEWS",        INT2FIX(GDA_CONNECTION_FEATURE_VIEWS));
    rb_define_const(c, "FEATURE_XML_QUERIES",  INT2FIX(GDA_CONNECTION_FEATURE_XML_QUERIES));
#if defined(GDA_CONNECTION_FEATURE_BLOBS)
    rb_define_const(c, "FEATURE_BLOBS",        INT2FIX(GDA_CONNECTION_FEATURE_BLOBS));
#endif

    G_DEF_SETTERS(c);
}

