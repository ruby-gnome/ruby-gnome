/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgdkdnd.c -

  $Author: mutoh $
  $Date: 2002/05/19 12:39:05 $

  Copyright (C) 2002 MUTOH Masao
************************************************/
#include "global.h"

VALUE gdkDragContext;
VALUE gdkDragContextConst;

/* GdkDragContext */
static VALUE
gdkdragcontext_protocol(self)
    VALUE self;
{
    return INT2NUM(get_gdkdragcontext(self)->protocol);
}

static VALUE
gdkdragcontext_is_source(self)
    VALUE self;
{
    return get_gdkdragcontext(self)->is_source ? Qtrue : Qfalse;
}

static VALUE
gdkdragcontext_source_window(self)
    VALUE self;
{
    return make_gdkwindow(get_gdkdragcontext(self)->source_window);
}

static VALUE
gdkdragcontext_dest_window(self)
    VALUE self;
{
    return make_gdkwindow(get_gdkdragcontext(self)->dest_window);
}

static VALUE
gdkdragcontext_targets(self)
    VALUE self;
{
    GList *list = get_gdkdragcontext(self)->targets, *cur;
    VALUE ary = rb_ary_new();

    for (cur = list; cur != NULL; cur = cur->next) {
        rb_ary_push(ary, make_gdkatom(GPOINTER_TO_INT(cur->data)));
    }
    return ary;
}

static VALUE
gdkdragcontext_actions(self)
    VALUE self;
{
    return INT2NUM(get_gdkdragcontext(self)->actions);
}

static VALUE
gdkdragcontext_suggested_action(self)
    VALUE self;
{
    return INT2NUM(get_gdkdragcontext(self)->suggested_action);
}

static VALUE
gdkdragcontext_action(self)
    VALUE self;
{
    return INT2NUM(get_gdkdragcontext(self)->action);
}

static VALUE
gdkdragcontext_start_time(self)
    VALUE self;
{
    return INT2NUM(get_gdkdragcontext(self)->start_time);
}

/* Singleton Methods */
static VALUE
gdkdragcontext_s_new(self)
    VALUE self;
{   
    return new_gdkdragcontext(gdk_drag_context_new());
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
    return make_gdkatom(gdk_drag_get_selection(get_gdkdragcontext(self)));
}

static VALUE
gdkdragcontext_drag_abort(self, time)
    VALUE self, time;
{
    gdk_drag_abort(get_gdkdragcontext(self), NUM2INT(time));
    return Qnil;
}

static VALUE
gdkdragcontext_drop_reply(self, ok, time)
    VALUE self, ok, time;
{
    gdk_drop_reply(get_gdkdragcontext(self), RTEST(ok), NUM2INT(time));
    return Qnil;
}

static VALUE
gdkdragcontext_drag_drop(self, time)
    VALUE self, time;
{
    gdk_drag_drop(get_gdkdragcontext(self), NUM2INT(time));
    return Qnil;
}

static VALUE
gdkdragcontext_find_window(self, drag_window, x_root, y_root, protocol)
    VALUE self, drag_window, x_root, y_root, protocol;
{
    GdkWindow *dest_window;
    int prot = NUM2INT(protocol);
    gdk_drag_find_window(get_gdkdragcontext(self),
                         get_gdkwindow(drag_window), 
                         NUM2INT(x_root),
                         NUM2INT(y_root),
                         &dest_window, 
                         (GdkDragProtocol*)&prot);
    return make_gdkwindow(dest_window);
}

static VALUE
gdkdragcontext_motion(self, dest_window, protocol, x_root, y_root, 
					  suggested_action, possible_actions, time)
    VALUE self, dest_window, protocol, x_root, y_root, 
  		  suggested_action, possible_actions, time;
{
    gboolean ret = gdk_drag_motion(get_gdkdragcontext(self), get_gdkwindow(dest_window), 
                                   NUM2INT(protocol), NUM2INT(x_root), NUM2INT(y_root), 
                                   NUM2INT(suggested_action), NUM2INT(possible_actions), 
                                   NUM2INT(time));
    return ret ? Qtrue : Qfalse;
}

static VALUE
gdkdragcontext_drop_finish(self, success, time)
    VALUE self, success, time;
{
    gdk_drop_finish(get_gdkdragcontext(self), RTEST(success), NUM2INT(time));
    return Qnil;
}

static VALUE
gdkdragcontext_drag_status(self, action, time)
    VALUE self, action, time;
{
    gdk_drag_status(get_gdkdragcontext(self), NUM2INT(action), NUM2INT(time));
    return Qnil;
}

void
Init_gtk_gdk_dnd()
{
    gdkDragContext = rb_define_class_under(mGdk, "DragContext", rb_cData);

    rb_define_singleton_method(gdkDragContext, "new", gdkdragcontext_s_new, 0);
    rb_define_singleton_method(gdkDragContext, "get_protocol", gdkdragcontext_s_get_protocol, 2);

    rb_define_method(gdkDragContext, "protocol", gdkdragcontext_protocol, 0);
    rb_define_method(gdkDragContext, "is_source", gdkdragcontext_is_source, 0);
    rb_define_method(gdkDragContext, "source_window", gdkdragcontext_source_window, 0);
    rb_define_method(gdkDragContext, "dest_window", gdkdragcontext_dest_window, 0);
    rb_define_method(gdkDragContext, "targets", gdkdragcontext_targets, 0);
    rb_define_method(gdkDragContext, "actions", gdkdragcontext_actions, 0);
    rb_define_method(gdkDragContext, "suggested_action", gdkdragcontext_suggested_action, 0);
    rb_define_method(gdkDragContext, "action", gdkdragcontext_action, 0);
    rb_define_method(gdkDragContext, "start_time", gdkdragcontext_start_time, 0);

    rb_define_method(gdkDragContext, "get_selection", gdkdragcontext_get_selection, 0);
    rb_define_alias(gdkDragContext, "selection", "get_selection");
    rb_define_method(gdkDragContext, "drag_abort", gdkdragcontext_drag_abort, 1);
    rb_define_method(gdkDragContext, "drop_reply", gdkdragcontext_drop_reply, 2);
    rb_define_method(gdkDragContext, "drag_drop", gdkdragcontext_drag_drop, 1);
    rb_define_method(gdkDragContext, "find_window", gdkdragcontext_find_window, 4);
    rb_define_method(gdkDragContext, "motion", gdkdragcontext_motion, 7);
    rb_define_method(gdkDragContext, "drop_finish", gdkdragcontext_drop_finish, 2);
    rb_define_method(gdkDragContext, "drag_status", gdkdragcontext_drag_status, 2);

    gdkDragContextConst = rb_define_module_under(gdkDragContext, "Constants");
    rb_define_const(gdkDragContextConst, "PROTO_MOTIF", INT2FIX(GDK_DRAG_PROTO_MOTIF));
    rb_define_const(gdkDragContextConst, "PROTO_XDND", INT2FIX(GDK_DRAG_PROTO_XDND));
    rb_define_const(gdkDragContextConst, "PROTO_ROOTWIN", INT2FIX(GDK_DRAG_PROTO_ROOTWIN));
    rb_define_const(gdkDragContextConst, "PROTO_NONE", INT2FIX(GDK_DRAG_PROTO_NONE));
    rb_define_const(gdkDragContextConst, "ACTION_DEFAULT", INT2FIX(GDK_ACTION_DEFAULT));
    rb_define_const(gdkDragContextConst, "ACTION_COPY", INT2FIX(GDK_ACTION_COPY));
    rb_define_const(gdkDragContextConst, "ACTION_MOVE", INT2FIX(GDK_ACTION_MOVE));
    rb_define_const(gdkDragContextConst, "ACTION_LINK", INT2FIX(GDK_ACTION_LINK));
    rb_define_const(gdkDragContextConst, "ACTION_PRIVATE", INT2FIX(GDK_ACTION_PRIVATE));
    rb_define_const(gdkDragContextConst, "ACTION_ASK", INT2FIX(GDK_ACTION_ASK));

    rb_include_module(gdkDragContext, gdkDragContextConst);
}
