
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
 * Class: Gda::Transaction
 * Management of transactions.
 */
VALUE cGdaTransaction;

/*
 * Class method: new(name)
 * name: name for the transaction.
 *
 * Creates a new Gda::Transaction object, which allows a fine-tune and full
 * control of transactions to be used with providers.
 *
 * Returns: a newly created Gda::Transaction object.
 */
static VALUE rb_gda_transaction_new(self, name)
    VALUE self, name;
{
    GdaTransaction *tr = gda_transaction_new(RVAL2CSTR(name));
    if (tr != NULL) {
        G_INITIALIZE(self, tr);
    }
    return Qnil;
}

/*
 * Method: isolation_level
 *
 * Gets the isolation level (see Gda::Transaction::Isolation) for the
 * transaction. This specifies the locking behavior for the database connection
 * during the given transaction.
 *
 * Returns: the isolation level.
 */
static VALUE rb_gda_transaction_get_isolation_level(self)
    VALUE self;
{
    return GENUM2RVAL(gda_transaction_get_isolation_level(RGDA_TRANSACTION(self)),
                      GDA_TYPE_TRANSACTION_ISOLATION);
}

/*
 * Method: set_isolation_level(level)
 * level: the isolation level (see Gda::Transaction::Isolation).
 *
 * Sets the isolation level of the transaction.
 *
 * Returns: self.
 */
static VALUE rb_gda_transaction_set_isolation_level(self, level)
    VALUE self, level;
{
    gda_transaction_set_isolation_level(RGDA_TRANSACTION(self),
                                        RVAL2GENUM(level, GDA_TYPE_TRANSACTION_ISOLATION));
    return self;
}

/*
 * Method: name
 *
 * Retrieves the name of the transaction, as specified by the client
 * application (via a non-nil parameter in the call to Gda::Transaction.new, 
 * or by calling Gda::Transaction#set_name).
 *
 * Note that some providers may set, when you call
 * Gda::Connection#begin_transaction, the name of the transaction if it's not
 * been specified by the client application, so this method may return, for
 * some providers, values that you don't expect.
 *
 * Returns: the name of the transaction.
 */
static VALUE rb_gda_transaction_get_name(self)
    VALUE self;
{
    return CSTR2RVAL(gda_transaction_get_name(RGDA_TRANSACTION(self)));
}

/*
 * Method: set_name(name)
 * name: a new name for the transaction.
 *
 * Sets the name of the transaction. This is very useful when using providers
 * that support named transactions.
 *
 * Returns: self.
 */
static VALUE rb_gda_transaction_set_name(self, name)
    VALUE self, name;
{
    gda_transaction_set_name(RGDA_TRANSACTION(self),
                             RVAL2CSTR(name));
    return self;
}

void Init_gda_transaction(void) {
    VALUE c = G_DEF_CLASS(GDA_TYPE_TRANSACTION, "Transaction", mGda);

    rb_define_method(c, "initialize", rb_gda_transaction_new, 1);
    rb_define_method(c, "isolation_level", 
                     rb_gda_transaction_get_isolation_level, 0);
    rb_define_method(c, "set_isolation_level", 
                     rb_gda_transaction_set_isolation_level, 1);
    rb_define_method(c, "name", rb_gda_transaction_get_name, 0);
    rb_define_method(c, "set_name", rb_gda_transaction_set_name, 1);

    G_DEF_CLASS(GDA_TYPE_TRANSACTION_ISOLATION, "Isolation", c);
    G_DEF_CONSTANTS(c, GDA_TYPE_TRANSACTION_ISOLATION, "GDA_TRANSACTION_");
    G_DEF_SETTERS(c);

    cGdaTransaction = c;
}

