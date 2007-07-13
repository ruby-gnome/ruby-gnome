/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkbox.c -

  $Author: ggc $
  $Date: 2007/07/13 16:07:31 $

  Copyright (C) 2002-2005 Ruby-GNOME2 Project Team
  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

#define _SELF(self) (GTK_BOX(RVAL2GOBJ(self)))
#define RVAL2WIDGET(w) (GTK_WIDGET(RVAL2GOBJ(w)))

static void
box_pack_start_or_end(argc, argv, self, start)
    int argc;
    VALUE *argv;
    VALUE self;
    int start;
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
box_pack_start(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    box_pack_start_or_end(argc, argv, self, 1);
    return self;
}

static VALUE
box_pack_end(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    box_pack_start_or_end(argc, argv, self, 0);
    return self;
}

static VALUE
box_pack_start_defaults(self, widget)
    VALUE self, widget;
{
    gtk_box_pack_start_defaults(_SELF(self), RVAL2WIDGET(widget));
    return self;
}

static VALUE
box_pack_end_defaults(self, widget)
    VALUE self, widget;
{
    gtk_box_pack_end_defaults(_SELF(self), RVAL2WIDGET(widget));
    return self;
}

static VALUE
box_reorder_child(self, child, pos)
    VALUE self, child, pos;
{
    gtk_box_reorder_child(_SELF(self),
                          RVAL2WIDGET(child), NUM2INT(pos));
    return self;
}

static VALUE
box_query_child_packing(self, child)
	VALUE self, child;
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
box_set_child_packing(self, child, expand, fill, padding, pack_type)
	VALUE self, child, expand, fill, padding, pack_type;
{
    gtk_box_set_child_packing(_SELF(self), 
                              RVAL2WIDGET(child),
                              RVAL2CBOOL(expand), RVAL2CBOOL(fill),
                              NUM2UINT(padding), RVAL2GENUM(pack_type, GTK_TYPE_PACK_TYPE));
    return self;
}

void 
Init_gtk_box()
{
    VALUE gBox = G_DEF_CLASS(GTK_TYPE_BOX, "Box", mGtk);

    rb_define_method(gBox, "pack_start", box_pack_start, -1);
    rb_define_method(gBox, "pack_end", box_pack_end, -1);
    rb_define_method(gBox, "pack_start_defaults", box_pack_start_defaults, 1);
    rb_define_method(gBox, "pack_end_defaults", box_pack_end_defaults, 1);
    rb_define_method(gBox, "reorder_child", box_reorder_child, 2);
    rb_define_method(gBox, "query_child_packing", box_query_child_packing, 1);
    rb_define_method(gBox, "set_child_packing", box_set_child_packing, 5);
}
