/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2007  Ruby-GNOME2 Project Team
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 *  MA  02110-1301  USA
 */

#ifndef __RBGCOMPAT_H__
#define __RBGCOMPAT_H__

G_BEGIN_DECLS

typedef struct {
    GType type;
    VALUE (*get_superclass)(void);
    void (*type_init_hook)(VALUE);
    void (*rvalue2gvalue)(VALUE val, GValue *result);
    VALUE (*gvalue2rvalue)(const GValue *);
    void (*initialize)(VALUE, gpointer);
    gpointer (*robj2instance)(VALUE);
    VALUE (*instance2robj)(gpointer);
} RGFundamental;

#define G_DEF_FUNDAMENTAL(f) (rbgobj_fund_define_fundamental(f))

extern void rbgobj_fund_define_fundamental(RGFundamental *fundamental);

G_END_DECLS

#endif
