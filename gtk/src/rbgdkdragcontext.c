/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgdkdnd.c -

  $Author: mutoh $
  $Date: 2005/02/07 16:56:39 $

  Copyright (C) 2002,2003 Masao Mutoh
************************************************/
#include "global.h"

#define _SELF(self) (GDK_DRAG_CONTEXT(RVAL2GOBJ(self)))

static VALUE
gdkdragcontext_protocol(self)
    VALUE self;
{
    return GENUM2RVAL(_SELF(self)->protocol, GDK_TYPE_DRAG_PROTOCOL);
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
    return GFLAGS2RVAL(_SELF(self)->actions, GDK_TYPE_DRAG_ACTION);
}

static VALUE
gdkdragcontext_suggested_action(self)
    VALUE self;
{
    return GFLAGS2RVAL(_SELF(self)->suggested_action, GDK_TYPE_DRAG_ACTION);
}

static VALUE
gdkdragcontext_action(self)
    VALUE self;
{
    return GFLAGS2RVAL(_SELF(self)->action, GDK_TYPE_DRAG_ACTION);
}

static VALUE
gdkdragcontext_start_time(self)
    VALUE self;
{
    return UINT2NUM(_SELF(self)->start_time);
}

static VALUE
gdkdragcontext_initialize(self)
    VALUE self;
{   
    G_INITIALIZE(self, gdk_drag_context_new());
    return Qnil;
}

static VALUE
gdkdragcontext_s_get_protocol(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE xid;
    GdkDragProtocol prot;
    guint32 ret;

    if (argc == 1) {
        rb_scan_args(argc, argv, "10", &xid);
        ret = gdk_drag_get_protocol(NUM2UINT(xid), &prot);
    } else {
#if GTK_CHECK_VERSION(2,2,0)
        VALUE display;
        rb_scan_args(argc, argv, "20", &display, &xid);
        ret = gdk_drag_get_protocol_for_display(GDK_DISPLAY_OBJECT(RVAL2GOBJ(display)),
                                                NUM2UINT(xid), &prot);
#else
        rb_warn("Not supported arguments for Gdk::Display in GTK+-2.0.x.");
        ret = gdk_drag_get_protocol(NUM2UINT(xid), &prot);
#endif
    }

    return rb_ary_new3(2, GENUM2RVAL(prot, GDK_TYPE_DRAG_PROTOCOL), UINT2NUM(ret));
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
    return self;
}

static VALUE
gdkdragcontext_drop_reply(self, ok, time)
    VALUE self, ok, time;
{
    gdk_drop_reply(_SELF(self), RTEST(ok), NUM2INT(time));
    return self;
}

static VALUE
gdkdragcontext_drag_drop(self, time)
    VALUE self, time;
{
    gdk_drag_drop(_SELF(self), NUM2INT(time));
    return self;
}

static VALUE
gdkdragcontext_find_window(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE drag_window, x_root, y_root;
    GdkWindow *dest_window;
    GdkDragProtocol prot;

    if (argc == 3) {
        rb_scan_args(argc, argv, "30", &drag_window, &x_root, &y_root);
        gdk_drag_find_window(_SELF(self),
                             GDK_WINDOW(RVAL2GOBJ(drag_window)), 
                             NUM2INT(x_root), NUM2INT(y_root),
                             &dest_window, &prot);
    } else {
#if GTK_CHECK_VERSION(2,2,0)
        VALUE screen;
        rb_scan_args(argc, argv, "40", &drag_window, &screen, &x_root, &y_root);
        gdk_drag_find_window_for_screen(_SELF(self),
                                        GDK_WINDOW(RVAL2GOBJ(drag_window)), 
                                        GDK_SCREEN(RVAL2GOBJ(screen)),
                                        NUM2INT(x_root), NUM2INT(y_root),
                                        &dest_window, &prot);
#else
        rb_raise(rb_eArgError, "Wrong number of arguments: %d", argc);
#endif
    }

    return rb_ary_new3(2, GOBJ2RVAL(dest_window), 
                       GENUM2RVAL(prot, GDK_TYPE_DRAG_PROTOCOL));
}

static VALUE
gdkdragcontext_s_drag_begin(self, window, targets)
    VALUE self, window, targets;
{
    GList* list = NULL;
    VALUE ret;
    int i;
    for (i = 0; i < RARRAY(targets)->len; i++){
        list = g_list_append(list, GINT_TO_POINTER(RVAL2ATOM(RARRAY(targets)->ptr[0])));
    }

    ret = GOBJ2RVAL(gdk_drag_begin(GDK_WINDOW(RVAL2GOBJ(window)), list));
    g_list_free(list);

    return ret;
}

static VALUE
gdkdragcontext_drag_motion(self, dest_window, protocol, x_root, y_root, 
                           suggested_action, possible_actions, time)
    VALUE self, dest_window, protocol, x_root, y_root, 
  		  suggested_action, possible_actions, time;
{
    gboolean ret = gdk_drag_motion(_SELF(self), 
                                   GDK_WINDOW(RVAL2GOBJ(dest_window)), 
                                   RVAL2GENUM(protocol, GDK_TYPE_DRAG_PROTOCOL), 
                                   NUM2INT(x_root), NUM2INT(y_root), 
                                   RVAL2GFLAGS(suggested_action, GDK_TYPE_DRAG_ACTION), 
                                   RVAL2GFLAGS(possible_actions, GDK_TYPE_DRAG_ACTION), 
                                   NUM2UINT(time));
    return ret ? Qtrue : Qfalse;
}

static VALUE
gdkdragcontext_drop_finish(self, success, time)
    VALUE self, success, time;
{
    gdk_drop_finish(_SELF(self), RTEST(success), NUM2INT(time));
    return self;
}

static VALUE
gdkdragcontext_drag_status(self, action, time)
    VALUE self, action, time;
{
    gdk_drag_status(_SELF(self), 
                    RVAL2GFLAGS(action, GDK_TYPE_DRAG_ACTION), NUM2UINT(time));
    return self;
}

#if GTK_CHECK_VERSION(2,6,0)
static VALUE
gdkdragcontext_drag_drop_succeeded(self)
    VALUE self;
{
    return CBOOL2RVAL(gdk_drag_drop_succeeded(_SELF(self)));
}
#endif

void
Init_gtk_gdk_dragcontext()
{
    VALUE gdkDragContext = G_DEF_CLASS(GDK_TYPE_DRAG_CONTEXT, "DragContext", mGdk);

    rb_define_singleton_method(gdkDragContext, "get_protocol", gdkdragcontext_s_get_protocol, -1);
    rb_define_singleton_method(gdkDragContext, "drag_begin", gdkdragcontext_s_drag_begin, 1);

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
    rb_define_method(gdkDragContext, "drag_motion", gdkdragcontext_drag_motion, 7);
    rb_define_method(gdkDragContext, "drop_finish", gdkdragcontext_drop_finish, 2);
    rb_define_method(gdkDragContext, "drag_status", gdkdragcontext_drag_status, 2);
#if GTK_CHECK_VERSION(2,6,0)
    rb_define_method(gdkDragContext, "drag_drop_succeeded?", gdkdragcontext_drag_drop_succeeded, 0);
#endif
	/* Constants */
    G_DEF_CLASS(GDK_TYPE_DRAG_PROTOCOL, "Protocol", gdkDragContext);
    G_DEF_CLASS(GDK_TYPE_DRAG_ACTION, "Action", gdkDragContext);
    G_DEF_CONSTANTS(gdkDragContext, GDK_TYPE_DRAG_PROTOCOL, "GDK_DRAG_");
    G_DEF_CONSTANTS(gdkDragContext, GDK_TYPE_DRAG_ACTION, "GDK_");
}
