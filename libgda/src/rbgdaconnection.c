
/*
 * Copyright (C) 2003 Laurent Sansonetti <laurent@datarescue.be>
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

static VALUE rb_gda_connection_get_client(self)
    VALUE self;
{
    GdaClient *client = gda_connection_get_client(RGDA_CONNECTION(self));
    return client != NULL
        ? RGDA_CLIENT_NEW(client)
        : Qnil;
}

/* 
 *  FIXME 
 *  #execute_command, #execute_single_command and #execute_non_query should accept
 *  parameters (as Gda::Parameter objects).
 */
static VALUE rb_gda_connection_execute_single_command(self, cmd)
    VALUE self, cmd;
{
    GdaDataModel *mod = gda_connection_execute_single_command(RGDA_CONNECTION(self),
                                                              RGDA_COMMAND(cmd),
                                                              NULL);
    return mod != NULL
        ? RGDA_DATAMODEL_NEW(mod)
        : Qnil;
}

static VALUE rb_gda_connection_execute_command(self, cmd)
    VALUE self, cmd;
{
    GList *list;
    VALUE arr;
    
    arr = rb_ary_new();
    for (list = gda_connection_execute_command(RGDA_CONNECTION(self),
                                               RGDA_COMMAND(cmd),
                                               NULL);
         list != NULL;
         list = g_list_next(list))
    {
        rb_ary_push(arr, RGDA_DATAMODEL_NEW(list->data));
    }
    return arr;        
}

static VALUE rb_gda_connection_execute_non_query(self, cmd)
    VALUE self, cmd;
{
    return INT2FIX(gda_connection_execute_non_query(RGDA_CONNECTION(self),
                                                    RGDA_COMMAND(cmd),
                                                    NULL));
                                               
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
    rb_define_method(c, "client",         rb_gda_connection_get_client,         0);

    rb_define_method(c, "execute_command",        rb_gda_connection_execute_command,        1);
    rb_define_method(c, "execute_single_command", rb_gda_connection_execute_single_command, 1);
    rb_define_method(c, "execute_non_query",      rb_gda_connection_execute_non_query,      1);

    rb_define_method(c, "errors", rb_gda_connection_get_errors, 0);

    rb_define_const(c, "OPTIONS_READ_ONLY",  INT2FIX(GDA_CONNECTION_OPTIONS_READ_ONLY));
    rb_define_const(c, "OPTIONS_DONT_SHARE", INT2FIX(GDA_CONNECTION_OPTIONS_DONT_SHARE));
}

