/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2002-2005 Ruby-GNOME2 Project Team
 *  Copyright (C) 1998-2000 Yukihiro Matsumoto,
 *                          Daisuke Kanda,
 *                          Hiroshi Igarashi
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

#include "global.h"

#define RG_TARGET_NAMESPACE cBox
#define _SELF(self) (GTK_BOX(RVAL2GOBJ(self)))
#define RVAL2WIDGET(w) (GTK_WIDGET(RVAL2GOBJ(w)))

static void
box_pack_start_or_end(int argc, VALUE *argv, VALUE self, int start)
{
    VALUE arg0, arg1, arg2, arg3;
    gint expand, fill, padding;
    GtkWidget *widget, *child;

    expand = fill = Qtrue; padding = 0;
    switch (rb_scan_args(argc, argv, "13", &arg0, &arg1, &arg2, &arg3)) {
      case 4:
        padding = NUM2INT(arg3);
      case 3:
        fill = RVAL2CBOOL(arg2);
      case 2:
        expand = RVAL2CBOOL(arg1);
      default:
        child = GTK_WIDGET(RVAL2GOBJ(arg0));
        G_CHILD_ADD(self, arg0);
        break;
    }
    widget = GTK_WIDGET(RVAL2GOBJ(self));

    if (start)
        gtk_box_pack_start(_SELF(self), child, expand, fill, padding);
    else
        gtk_box_pack_end(_SELF(self), child, expand, fill, padding);
}

static VALUE
rg_pack_start(int argc, VALUE *argv, VALUE self)
{
    box_pack_start_or_end(argc, argv, self, 1);
    return self;
}

static VALUE
rg_pack_end(int argc, VALUE *argv, VALUE self)
{
    box_pack_start_or_end(argc, argv, self, 0);
    return self;
}

static VALUE
rg_pack_start_defaults(VALUE self, VALUE widget)
{
    gtk_box_pack_start_defaults(_SELF(self), RVAL2WIDGET(widget));
    return self;
}

static VALUE
rg_pack_end_defaults(VALUE self, VALUE widget)
{
    gtk_box_pack_end_defaults(_SELF(self), RVAL2WIDGET(widget));
    return self;
}

static VALUE
rg_reorder_child(VALUE self, VALUE child, VALUE pos)
{
    gtk_box_reorder_child(_SELF(self),
                          RVAL2WIDGET(child), NUM2INT(pos));
    return self;
}

static VALUE
rg_query_child_packing(VALUE self, VALUE child)
{
    gboolean expand, fill;
    guint padding;
    GtkPackType pack_type;
    VALUE ary;

    gtk_box_query_child_packing(_SELF(self), 
                                RVAL2WIDGET(child),
                                &expand, &fill, &padding, &pack_type);

    ary = rb_ary_new2(4);
    rb_ary_push(ary, expand == FALSE ? Qfalse : Qtrue);
    rb_ary_push(ary, fill == FALSE ? Qfalse : Qtrue);
    rb_ary_push(ary, INT2NUM(padding));
    rb_ary_push(ary, GENUM2RVAL(pack_type, GTK_TYPE_PACK_TYPE));

    return ary;
}

static VALUE
rg_set_child_packing(VALUE self, VALUE child, VALUE expand, VALUE fill, VALUE padding, VALUE pack_type)
{
    gtk_box_set_child_packing(_SELF(self), 
                              RVAL2WIDGET(child),
                              RVAL2CBOOL(expand), RVAL2CBOOL(fill),
                              NUM2UINT(padding), RVAL2GENUM(pack_type, GTK_TYPE_PACK_TYPE));
    return self;
}

void 
Init_gtk_box(VALUE mGtk)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_BOX, "Box", mGtk);

    RG_DEF_METHOD(pack_start, -1);
    RG_DEF_METHOD(pack_end, -1);
    RG_DEF_METHOD(pack_start_defaults, 1);
    RG_DEF_METHOD(pack_end_defaults, 1);
    RG_DEF_METHOD(reorder_child, 2);
    RG_DEF_METHOD(query_child_packing, 1);
    RG_DEF_METHOD(set_child_packing, 5);
}
