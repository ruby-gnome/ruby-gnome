
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
 * Class: Gda::Time
 * This structure symbolizes a time snapshot, via the following fields: 'hour', 
 * 'minute', 'second' and 'timezone'.
 *
 *	# 5h 25min 30sec
 *	t = Gda::Time.new
 *	t.hour = 5
 *	t.minute = 25
 *	t.second = 30
 */
VALUE cGdaTime;

void Init_gda_time(void) {
    cGdaTime = rb_struct_define("Time", "hour", "minute", "second", "timezone", NULL);
    rb_set_class_path(cGdaTime, mGda, "Time");
    rb_const_set(mGda, rb_intern("Time"), cGdaTime);
}
