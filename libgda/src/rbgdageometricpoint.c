
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
 * Class: Gda::GeometricPoint
 * This structure symbolizes a geometric point, via two fields: 'x' and 'y'.
 *
 *	pt = Gda::GeometricPoint.new
 *	pt.x = 10
 *	pt.y = 20
 *	x, y = pt.to_a
 */
VALUE cGdaGeometricPoint;

void Init_gda_geometric_point(void) {
    cGdaGeometricPoint = rb_struct_define("GeometricPoint", "x", "y", NULL);
    rb_set_class_path(cGdaGeometricPoint, mGda, "GeometricPoint");
    rb_const_set(mGda, rb_intern("GeometricPoint"), cGdaGeometricPoint);
}
