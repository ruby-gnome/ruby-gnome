
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

static VALUE rb_gda_command_new(self, text, command_type, options)
    VALUE self, text, command_type, options;
{
    GdaCommand *cmd = gda_command_new(RVAL2CSTR(text),
                                      FIX2INT(command_type),
                                      FIX2INT(options));
    if (cmd != NULL) {
        G_INITIALIZE(self, cmd);
    }
    return Qnil;
}

static VALUE rb_gda_command_set_text(self, text)
    VALUE self, text;
{
    gda_command_set_text(RGDA_COMMAND(self),
                         RVAL2CSTR(text));
    return self;
}

static VALUE rb_gda_command_get_text(self)
    VALUE self;
{
    return CSTR2RVAL(gda_command_get_text(RGDA_COMMAND(self)));
}

static VALUE rb_gda_command_set_command_type(self, command_type)
    VALUE self, command_type;
{
    gda_command_set_command_type(RGDA_COMMAND(self),
                                 FIX2INT(command_type));
    return self;
}

static VALUE rb_gda_command_get_command_type(self)
    VALUE self;
{
    return INT2FIX(gda_command_get_command_type(RGDA_COMMAND(self)));
}

static VALUE rb_gda_command_set_options(self, options)
    VALUE self, options;
{
    gda_command_set_options(RGDA_COMMAND(self),
                                 FIX2INT(options));
    return self;
}

static VALUE rb_gda_command_get_options(self)
    VALUE self;
{
    return INT2FIX(gda_command_get_options(RGDA_COMMAND(self)));
}

static VALUE rb_gda_command_set_transaction(self, transaction)
    VALUE self, transaction;
{
    gda_command_set_transaction(RGDA_COMMAND(self),
                                RGDA_TRANSACTION(transaction));
    return self;
}

static VALUE rb_gda_command_get_transaction(self)
    VALUE self;
{
    GdaTransaction *tr = gda_command_get_transaction(RGDA_COMMAND(self));
    return tr != NULL
        ? RGDA_TRANSACTION_NEW(tr)
        : Qnil;
}

void Init_gda_command(void) {
    VALUE c = G_DEF_CLASS(GDA_TYPE_COMMAND, "Command", mGda);

    rb_define_method(c, "initialize", rb_gda_command_new, 3);
   
    rb_define_method(c, "set_text", rb_gda_command_set_text, 1);
    rb_define_method(c, "text",     rb_gda_command_get_text, 0);
    rb_define_alias(c, "to_s", "text");
 
    rb_define_method(c, "set_command_type", rb_gda_command_set_command_type, 1);
    rb_define_method(c, "command_type",     rb_gda_command_get_command_type, 0);
    
    rb_define_method(c, "set_options", rb_gda_command_set_options, 1);
    rb_define_method(c, "options",     rb_gda_command_get_options, 0);
    
    rb_define_method(c, "set_transaction", rb_gda_command_set_transaction, 1);
    rb_define_method(c, "transaction",     rb_gda_command_get_transaction, 0);
    
    rb_define_const(c, "OPTION_IGNORE_ERRORS",  INT2FIX(GDA_COMMAND_OPTION_IGNORE_ERRORS));
    rb_define_const(c, "OPTION_STOP_ON_ERRORS", INT2FIX(GDA_COMMAND_OPTION_STOP_ON_ERRORS));
    rb_define_const(c, "OPTION_BAD_OPTION",     INT2FIX(GDA_COMMAND_OPTION_BAD_OPTION));

    rb_define_const(c, "TYPE_SQL",       INT2FIX(GDA_COMMAND_TYPE_SQL));
    rb_define_const(c, "TYPE_XML",       INT2FIX(GDA_COMMAND_TYPE_XML));
    rb_define_const(c, "TYPE_PROCEDURE", INT2FIX(GDA_COMMAND_TYPE_PROCEDURE));
    rb_define_const(c, "TYPE_TABLE",     INT2FIX(GDA_COMMAND_TYPE_TABLE));
    rb_define_const(c, "TYPE_SCHEMA",    INT2FIX(GDA_COMMAND_TYPE_SCHEMA));
    rb_define_const(c, "TYPE_INVALID",   INT2FIX(GDA_COMMAND_TYPE_INVALID));

    G_DEF_SETTERS(c);
}

