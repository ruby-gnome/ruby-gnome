/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtkhandlebox.c -

  $Author: mutoh $
  $Date: 2002/06/23 16:13:32 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

VALUE gHandleBox;

static VALUE
hb_init(self)
    VALUE self;
{
    RBGTK_INITIALIZE(self, gtk_handle_box_new());
    return Qnil;
}

static VALUE
hb_set_shadow_type(self, shadow)
    VALUE self, shadow;
{
    gtk_handle_box_set_shadow_type(GTK_HANDLE_BOX(RVAL2GOBJ(self)),
				   NUM2INT(shadow));
    return self;
}

static VALUE
hb_set_handle_pos(self, pos)
    VALUE self, pos;
{
    gtk_handle_box_set_handle_position(GTK_HANDLE_BOX(RVAL2GOBJ(self)),
				       NUM2INT(pos));
    return self;
}

static VALUE
hb_set_snap_edge(self, edge)
    VALUE self, edge;
{
    gtk_handle_box_set_snap_edge(GTK_HANDLE_BOX(RVAL2GOBJ(self)),
				 NUM2INT(edge));
    return self;
}

static VALUE
hb_shadow_type(self)
    VALUE self;
{
    return INT2FIX(GTK_HANDLE_BOX(RVAL2GOBJ(self))->shadow_type);
}

static VALUE
hb_handle_pos(self)
    VALUE self;
{
    return INT2FIX(GTK_HANDLE_BOX(RVAL2GOBJ(self))->handle_position);
}

static VALUE
hb_snap_edge(self)
    VALUE self;
{
    return INT2FIX(GTK_HANDLE_BOX(RVAL2GOBJ(self))->snap_edge);
}

static VALUE
hb_child_detached(self)
    VALUE self;
{
    return GTK_HANDLE_BOX(RVAL2GOBJ(self))->child_detached ? Qtrue : Qfalse;
}

void Init_gtk_handle_box()
{
    static RGObjClassInfo cinfo;

    gHandleBox = rb_define_class_under(mGtk, "HandleBox", gBin);
    cinfo.klass = gHandleBox;
    cinfo.gtype = GTK_TYPE_HANDLE_BOX;
    cinfo.mark = 0;
    cinfo.free = 0;
    rbgtk_register_class(&cinfo);

    rb_define_method(gHandleBox, "initialize", hb_init, 0);

    /* setter */
    rb_define_method(gHandleBox, "set_shadow_type", hb_set_shadow_type, 1);
    rb_define_method(gHandleBox, "set_handle_position", hb_set_handle_pos, 1);
    rb_define_method(gHandleBox, "set_snap_edge", hb_set_snap_edge, 1);

    /* getter */
    rb_define_method(gHandleBox, "shadow_type", hb_shadow_type, 0);
    rb_define_method(gHandleBox, "handle_position", hb_handle_pos, 0);
    rb_define_method(gHandleBox, "snap_edge", hb_snap_edge, 0);
    rb_define_method(gHandleBox, "child_detached?", hb_child_detached, 0);
    /* FIXME: any documentation? - yashi */
    /* rb_define_method(gHandleBox, "shrink_on_detach?", hb_shrin, 0); */
    /* rb_define_method(gHandleBox, "in_drag?", hb_in_drag, 0); */

    /* aliases */
    rb_define_alias(gHandleBox, "shadow_type=", "set_shadow_type");
    rb_define_alias(gHandleBox, "handle_position=", "set_handle_position");
    rb_define_alias(gHandleBox, "handle_pos=", "set_handle_position");
    rb_define_alias(gHandleBox, "handle_pos", "handle_position");
    rb_define_alias(gHandleBox, "snap_edge=", "set_snap_edge");
}
