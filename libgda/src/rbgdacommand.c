
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
 * Class: Gda::Command
 *
 * The Gda::Command object holds data needed to issue a command to the
 * providers. Applications usually create a Gda::Command (via
 * Gda::Command.new), set its properties and pass it over to the database using
 * the Gda::Connection methods.
 *
 * One interesting thing about Gda::Command's is that they can be reused over
 * and over. That is, applications don't need to create a command every time
 * they want to run something on the connected database. Moreover, the ability
 * to create command strings with placeholders allows the use of parameters to
 * specify the values for those placeholders. Thus, an application can create
 * a command of the form:
 *
 * 	INSERT INTO employees VALUES (id, name, address, salary)
 *
 * and reuse the same command over and over, just using different values for
 * the placeholders.
 *
 * The value for the placeholders is specified when sending the Gda::Command
 * to a database connection, which is done via the
 * Gda::Connection#execute_command method.
 */
VALUE cGdaCommand;

/*
 * Class method: new(text, type, options)
 * text: text of the command.
 * type: type of the command (see Gda::Command::Type).
 * options: options for the command (see Gda::Command::Options).
 *
 * Creates a new Gda::Command from the parameters.  If there are conflicting
 * options, this will set options to Gda::Command::DEFAULT_OPTION.
 * 
 * Returns: a newly allocated Gda::Command object.
 */
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

/*
 * Method: set_text(text)
 * text: the command text.
 *
 * Sets the text of the command.
 *
 * Returns: self.
 */
static VALUE rb_gda_command_set_text(self, text)
    VALUE self, text;
{
    gda_command_set_text(RGDA_COMMAND(self),
                         RVAL2CSTR(text));
    return self;
}

/*
 * Method: text
 *
 * Returns: the text of the command.
 */
static VALUE rb_gda_command_get_text(self)
    VALUE self;
{
    return CSTR2RVAL(gda_command_get_text(RGDA_COMMAND(self)));
}

/*
 * Method: set_command_type(type)
 * type: the command type (see Gda::Command::Type).
 *
 * Sets the command type.
 *
 * Returns: self.
 */
static VALUE rb_gda_command_set_command_type(self, command_type)
    VALUE self, command_type;
{
    gda_command_set_command_type(RGDA_COMMAND(self),
                                 FIX2INT(command_type));
    return self;
}

/*
 * Method: command_type
 *
 * Returns: the command type (see Gda::Command::Type).
 */
static VALUE rb_gda_command_get_command_type(self)
    VALUE self;
{
    return INT2FIX(gda_command_get_command_type(RGDA_COMMAND(self)));
}

/*
 * Method: set_options(options)
 * options: see Gda::Command::Options.
 *
 * Sets command options.
 *
 * Returns: self.
 */
static VALUE rb_gda_command_set_options(self, options)
    VALUE self, options;
{
    gda_command_set_options(RGDA_COMMAND(self),
                                 FIX2INT(options));
    return self;
}

/*
 * Method: options
 *
 * Returns: options of the command (see Gda::Command::Options).
 */
static VALUE rb_gda_command_get_options(self)
    VALUE self;
{
    return INT2FIX(gda_command_get_options(RGDA_COMMAND(self)));
}

/*
 * Method: set_transaction(xaction)
 * xaction: a Gda::Transaction object.
 *
 * Sets the GdaTransaction associated with the command.
 *
 * Returns: self.
 */
static VALUE rb_gda_command_set_transaction(self, transaction)
    VALUE self, transaction;
{
    gda_command_set_transaction(RGDA_COMMAND(self),
                                RGDA_TRANSACTION(transaction));
    return self;
}

/*
 * Method: transaction
 *
 * Returns: the Gda::Transaction object associated with the command.
 */
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

    G_DEF_CLASS(GDA_TYPE_COMMAND_OPTIONS, "Options", c);
    G_DEF_CONSTANTS(c, GDA_TYPE_COMMAND_OPTIONS, "GDA_COMMAND_");
    G_DEF_CLASS(GDA_TYPE_COMMAND_TYPE, "Type", c);
    G_DEF_CONSTANTS(c, GDA_TYPE_COMMAND_TYPE,    "GDA_COMMAND_");
    G_DEF_SETTERS(c);

    cGdaCommand = c;
}

