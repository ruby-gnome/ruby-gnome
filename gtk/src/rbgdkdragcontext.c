/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgdkdnd.c -

  $Author: mutoh $
  $Date: 2002/10/14 17:24:14 $

  Copyright (C) 2002 Masao Mutoh
************************************************/
#include "global.h"

#define _SELF(self) (GDK_DRAG_CONTEXT(RVAL2GOBJ(self)))

static VALUE
gdkdragcontext_protocol(self)
    VALUE self;
{
    return INT2NUM(_SELF(self)->protocol);
}

static VALUE
gdkdragcontext_is_source(self)
    VALUE self;
{
    return _SELF(self)->is_source ? Qtrue : Qfalse;
}

static VALUE
gdkdragcontext_source_window(self)
    VALUE self;
{
    return GOBJ2RVAL(_SELF(self)->source_window);
}

static VALUE
gdkdragcontext_dest_window(self)
    VALUE self;
{
    return GOBJ2RVAL(_SELF(self)->dest_window);
}

static VALUE
gdkdragcontext_targets(self)
    VALUE self;
{
    GList *list = _SELF(self)->targets, *cur;
    VALUE ary = rb_ary_new();

    for (cur = list; cur != NULL; cur = cur->next) {
        rb_ary_push(ary, BOXED2RVAL((GdkAtom)cur->data, GDK_TYPE_ATOM));
    }
    return ary;
}

static VALUE
gdkdragcontext_actions(self)
    VALUE self;
{
    return INT2NUM(_SELF(self)->actions);
}

static VALUE
gdkdragcontext_suggested_action(self)
    VALUE self;
{
    return INT2NUM(_SELF(self)->suggested_action);
}

static VALUE
gdkdragcontext_action(self)
    VALUE self;
{
    return INT2NUM(_SELF(self)->action);
}

static VALUE
gdkdragcontext_start_time(self)
    VALUE self;
{
    return INT2NUM(_SELF(self)->start_time);
}

static VALUE
gdkdragcontext_initialize(self)
    VALUE self;
{   
    G_INITIALIZE(self, gdk_drag_context_new());
    return Qnil;
}

static VALUE
gdkdragcontext_s_get_protocol(self, xid, protocol)
    VALUE self, xid, protocol;
{
    int prot = NUM2INT(protocol);
    return INT2NUM(gdk_drag_get_protocol(NUM2INT(xid),(GdkDragProtocol*)&prot)); 
}

/* Instance Methods */
static VALUE
gdkdragcontext_get_selection(self)
    VALUE self;
{
    return BOXED2RVAL(gdk_drag_get_selection(_SELF(self)), GDK_TYPE_ATOM);
}

static VALUE
gdkdragcontext_drag_abort(self, time)
    VALUE self, time;
{
    gdk_drag_abort(_SELF(self), NUM2INT(time));
    return Qnil;
}

static VALUE
gdkdragcontext_drop_reply(self, ok, time)
    VALUE self, ok, time;
{
    gdk_drop_reply(_SELF(self), RTEST(ok), NUM2INT(time));
    return Qnil;
}

static VALUE
gdkdragcontext_drag_drop(self, time)
    VALUE self, time;
{
    gdk_drag_drop(_SELF(self), NUM2INT(time));
    return Qnil;
}

static VALUE
gdkdragcontext_find_window(self, drag_window, x_root, y_root, protocol)
    VALUE self, drag_window, x_root, y_root, protocol;
{
    GdkWindow *dest_window;
    int prot = NUM2INT(protocol);
    gdk_drag_find_window(_SELF(self),
                         GDK_WINDOW(RVAL2GOBJ(drag_window)), 
                         NUM2INT(x_root),
                         NUM2INT(y_root),
                         &dest_window, 
                         (GdkDragProtocol*)&prot);
    return GOBJ2RVAL(dest_window);
}

static VALUE
gdkdragcontext_motion(self, dest_window, protocol, x_root, y_root, 
					  suggested_action, possible_actions, time)
    VALUE self, dest_window, protocol, x_root, y_root, 
  		  suggested_action, possible_actions, time;
{
    gboolean ret = gdk_drag_motion(_SELF(self), 
								   GDK_WINDOW(RVAL2GOBJ(dest_window)), 
                                   NUM2INT(protocol), NUM2INT(x_root), NUM2INT(y_root), 
                                   NUM2INT(suggested_action), NUM2INT(possible_actions), 
                                   NUM2INT(time));
    return ret ? Qtrue : Qfalse;
}

static VALUE
gdkdragcontext_drop_finish(self, success, time)
    VALUE self, success, time;
{
    gdk_drop_finish(_SELF(self), RTEST(success), NUM2INT(time));
    return Qnil;
}

static VALUE
gdkdragcontext_drag_status(self, action, time)
    VALUE self, action, time;
{
    gdk_drag_status(_SELF(self), NUM2INT(action), NUM2INT(time));
    return Qnil;
}

void
Init_gtk_gdk_dragcontext()
{
    VALUE gdkDragContext = G_DEF_CLASS(GDK_TYPE_DRAG_CONTEXT, "DragContext", mGdk);

    rb_define_singleton_method(gdkDragContext, "protocol", gdkdragcontext_s_get_protocol, 2);

    rb_define_method(gdkDragContext, "initialize", gdkdragcontext_initialize, 0);
    rb_define_method(gdkDragContext, "protocol", gdkdragcontext_protocol, 0);
    rb_define_method(gdkDragContext, "source?", gdkdragcontext_is_source, 0);
    rb_define_method(gdkDragContext, "source_window", gdkdragcontext_source_window, 0);
    rb_define_method(gdkDragContext, "dest_window", gdkdragcontext_dest_window, 0);
    rb_define_method(gdkDragContext, "targets", gdkdragcontext_targets, 0);
    rb_define_method(gdkDragContext, "actions", gdkdragcontext_actions, 0);
    rb_define_method(gdkDragContext, "suggested_action", gdkdragcontext_suggested_action, 0);
    rb_define_method(gdkDragContext, "action", gdkdragcontext_action, 0);
    rb_define_method(gdkDragContext, "start_time", gdkdragcontext_start_time, 0);

    rb_define_method(gdkDragContext, "selection", gdkdragcontext_get_selection, 0);
    rb_define_method(gdkDragContext, "drag_abort", gdkdragcontext_drag_abort, 1);
    rb_define_method(gdkDragContext, "drop_reply", gdkdragcontext_drop_reply, 2);
    rb_define_method(gdkDragContext, "drag_drop", gdkdragcontext_drag_drop, 1);
    rb_define_method(gdkDragContext, "find_window", gdkdragcontext_find_window, 4);
    rb_define_method(gdkDragContext, "motion", gdkdragcontext_motion, 7);
    rb_define_method(gdkDragContext, "drop_finish", gdkdragcontext_drop_finish, 2);
    rb_define_method(gdkDragContext, "drag_status", gdkdragcontext_drag_status, 2);

	/* Constants */
    rb_define_const(gdkDragContext, "PROTO_MOTIF", INT2FIX(GDK_DRAG_PROTO_MOTIF));
    rb_define_const(gdkDragContext, "PROTO_XDND", INT2FIX(GDK_DRAG_PROTO_XDND));
    rb_define_const(gdkDragContext, "PROTO_ROOTWIN", INT2FIX(GDK_DRAG_PROTO_ROOTWIN));
    rb_define_const(gdkDragContext, "PROTO_NONE", INT2FIX(GDK_DRAG_PROTO_NONE));
    rb_define_const(gdkDragContext, "PROTO_WIN32_DROPFILES", INT2FIX(GDK_DRAG_PROTO_WIN32_DROPFILES));
    rb_define_const(gdkDragContext, "PROTO_OLE2", INT2FIX(GDK_DRAG_PROTO_OLE2));
    rb_define_const(gdkDragContext, "PROTO_LOCAL", INT2FIX(GDK_DRAG_PROTO_LOCAL));

    rb_define_const(gdkDragContext, "ACTION_DEFAULT", INT2FIX(GDK_ACTION_DEFAULT));
    rb_define_const(gdkDragContext, "ACTION_COPY", INT2FIX(GDK_ACTION_COPY));
    rb_define_const(gdkDragContext, "ACTION_MOVE", INT2FIX(GDK_ACTION_MOVE));
    rb_define_const(gdkDragContext, "ACTION_LINK", INT2FIX(GDK_ACTION_LINK));
    rb_define_const(gdkDragContext, "ACTION_PRIVATE", INT2FIX(GDK_ACTION_PRIVATE));
    rb_define_const(gdkDragContext, "ACTION_ASK", INT2FIX(GDK_ACTION_ASK));
}
