
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
 * Class: Gda::Money
 * This structure symbolizes a amount of money associated with a currency,
 * via two fields: 'currency', and 'amount'.
 *
 *	euro = Gda::Money.new
 *	euro.currency = 1.5
 *	euro.amount = 100
 */
VALUE cGdaMoney;

void Init_gda_money(void) {
    cGdaMoney = rb_struct_define("Money", "currency", "amount", NULL);
    rb_set_class_path(cGdaMoney, mGda, "Money");
    rb_const_set(mGda, rb_intern("Money"), cGdaMoney);
}
