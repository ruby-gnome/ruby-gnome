
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

static VALUE rb_gda_transaction_new(self, name)
    VALUE self, name;
{
    GdaTransaction *tr = gda_transaction_new(RVAL2CSTR(name));
    if (tr != NULL) {
        G_INITIALIZE(self, tr);
    }
    return Qnil;
}

static VALUE rb_gda_transaction_get_isolation_level(self)
    VALUE self;
{
    return INT2FIX(gda_transaction_get_isolation_level(RGDA_TRANSACTION(self)));
}

static VALUE rb_gda_transaction_set_isolation_level(self, level)
    VALUE self, level;
{
    gda_transaction_set_isolation_level(RGDA_TRANSACTION(self),
                                        FIX2INT(level));
    return self;
}

static VALUE rb_gda_transaction_get_name(self)
    VALUE self;
{
    return CSTR2RVAL(gda_transaction_get_name(RGDA_TRANSACTION(self)));
}

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

    rb_define_const(c, "ISOLATION_UNKNOWN", 
                    INT2FIX(GDA_TRANSACTION_ISOLATION_UNKNOWN));
    rb_define_const(c, "ISOLATION_READ_COMMITED", 
                    INT2FIX(GDA_TRANSACTION_ISOLATION_READ_COMMITTED));
    rb_define_const(c, "ISOLATION_READ_UNCOMMITED", 
                    INT2FIX(GDA_TRANSACTION_ISOLATION_READ_UNCOMMITTED));
    rb_define_const(c, "ISOLATION_REPEATABLE_READ", 
                    INT2FIX(GDA_TRANSACTION_ISOLATION_REPEATABLE_READ));
    rb_define_const(c, "ISOLATION_SERIALIZABLE", 
                    INT2FIX(GDA_TRANSACTION_ISOLATION_SERIALIZABLE));

    G_DEF_SETTERS(c);
}

