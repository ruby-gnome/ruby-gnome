/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2002,2003 Masao Mutoh
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

#define RG_TARGET_NAMESPACE cDragContext
#define _SELF(self) (GDK_DRAG_CONTEXT(RVAL2GOBJ(self)))

static VALUE
rg_protocol(VALUE self)
{
    return GENUM2RVAL(_SELF(self)->protocol, GDK_TYPE_DRAG_PROTOCOL);
}

static VALUE
rg_source_p(VALUE self)
{
    return CBOOL2RVAL(_SELF(self)->is_source);
}

static VALUE
rg_source_window(VALUE self)
{
    return GOBJ2RVAL(_SELF(self)->source_window);
}

static VALUE
rg_dest_window(VALUE self)
{
    return GOBJ2RVAL(_SELF(self)->dest_window);
}

static VALUE
rg_targets(VALUE self)
{
    GList *list = _SELF(self)->targets, *cur;
    VALUE ary = rb_ary_new();

    for (cur = list; cur != NULL; cur = cur->next) {
        rb_ary_push(ary, BOXED2RVAL((GdkAtom)cur->data, GDK_TYPE_ATOM));
    }
    return ary;
}

static VALUE
rg_actions(VALUE self)
{
    return GFLAGS2RVAL(_SELF(self)->actions, GDK_TYPE_DRAG_ACTION);
}

static VALUE
rg_suggested_action(VALUE self)
{
    return GFLAGS2RVAL(_SELF(self)->suggested_action, GDK_TYPE_DRAG_ACTION);
}

static VALUE
rg_action(VALUE self)
{
    return GFLAGS2RVAL(_SELF(self)->action, GDK_TYPE_DRAG_ACTION);
}

static VALUE
rg_start_time(VALUE self)
{
    return UINT2NUM(_SELF(self)->start_time);
}

static VALUE
rg_initialize(VALUE self)
{   
    G_INITIALIZE(self, gdk_drag_context_new());
    return Qnil;
}

static VALUE
rg_s_get_protocol(int argc, VALUE *argv, G_GNUC_UNUSED VALUE self)
{
    VALUE xid;
    GdkDragProtocol prot;
    GdkNativeWindow ret;

    if (argc == 1) {
        rb_scan_args(argc, argv, "10", &xid);
        ret = gdk_drag_get_protocol(RVAL2GDKNATIVEWINDOW(xid), &prot);
    } else {
#if GTK_CHECK_VERSION(2,2,0)
        VALUE display;
        rb_scan_args(argc, argv, "20", &display, &xid);
        ret = gdk_drag_get_protocol_for_display(GDK_DISPLAY_OBJECT(RVAL2GOBJ(display)),
                                                RVAL2GDKNATIVEWINDOW(xid), &prot);
#else
        rb_warn("Not supported arguments for Gdk::Display in GTK+-2.0.x.");
        ret = gdk_drag_get_protocol(NUM2UINT(xid), &prot);
#endif
    }

    return rb_ary_new3(2, GENUM2RVAL(prot, GDK_TYPE_DRAG_PROTOCOL), GDKNATIVEWINDOW2RVAL(ret));
}

/* Instance Methods */
static VALUE
rg_selection(VALUE self)
{
    return BOXED2RVAL(gdk_drag_get_selection(_SELF(self)), GDK_TYPE_ATOM);
}

static VALUE
rg_drag_abort(VALUE self, VALUE time)
{
    gdk_drag_abort(_SELF(self), NUM2UINT(time));
    return self;
}

static VALUE
rg_drop_reply(VALUE self, VALUE ok, VALUE time)
{
    gdk_drop_reply(_SELF(self), RVAL2CBOOL(ok), NUM2UINT(time));
    return self;
}

static VALUE
rg_drag_drop(VALUE self, VALUE time)
{
    gdk_drag_drop(_SELF(self), NUM2UINT(time));
    return self;
}

static VALUE
rg_find_window(int argc, VALUE *argv, VALUE self)
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

struct rbgdk_rval2gdkatomglist_args {
    VALUE ary;
    long n;
    GList *result;
};

static VALUE
rbgdk_rval2gdkatomglist_body(VALUE value)
{
    long i;
    struct rbgdk_rval2gdkatomglist_args *args = (struct rbgdk_rval2gdkatomglist_args *)value;

    for (i = 0; i < args->n; i++)
        args->result = g_list_append(args->result, GINT_TO_POINTER(RVAL2ATOM(RARRAY_PTR(args->ary)[i])));

    return Qnil;
}

static G_GNUC_NORETURN VALUE
rbgdk_rval2gdkatomglist_rescue(VALUE value)
{
    g_free(((struct rbgdk_rval2gdkatomglist_args *)value)->result);

    rb_exc_raise(rb_errinfo());
}

static GList *
rbgdk_rval2gdkatomglist(VALUE value)
{
    struct rbgdk_rval2gdkatomglist_args args;

    args.ary = rb_ary_to_ary(value);
    args.n = RARRAY_LEN(args.ary);
    args.result = NULL;

    rb_rescue(rbgdk_rval2gdkatomglist_body, (VALUE)&args,
              rbgdk_rval2gdkatomglist_rescue, (VALUE)&args);

    return args.result;
}

#define RVAL2GDKATOMGLIST(value) rbgdk_rval2gdkatomglist(value)

static VALUE
rg_s_drag_begin(G_GNUC_UNUSED VALUE self, VALUE rbwindow, VALUE rbtargets)
{
    GdkWindow *window = GDK_WINDOW(RVAL2GOBJ(rbwindow));
    GList *targets = RVAL2GDKATOMGLIST(rbtargets);
    GdkDragContext *result = gdk_drag_begin(window, targets);

    g_list_free(targets);

    return GOBJ2RVAL(result);
}

static VALUE
rg_drag_motion(VALUE self, VALUE dest_window, VALUE protocol, VALUE x_root, VALUE y_root, VALUE suggested_action, VALUE possible_actions, VALUE time)
{
    gboolean ret = gdk_drag_motion(_SELF(self), 
                                   GDK_WINDOW(RVAL2GOBJ(dest_window)), 
                                   RVAL2GENUM(protocol, GDK_TYPE_DRAG_PROTOCOL), 
                                   NUM2INT(x_root), NUM2INT(y_root), 
                                   RVAL2GFLAGS(suggested_action, GDK_TYPE_DRAG_ACTION), 
                                   RVAL2GFLAGS(possible_actions, GDK_TYPE_DRAG_ACTION), 
                                   NUM2UINT(time));
    return CBOOL2RVAL(ret);
}

static VALUE
rg_drop_finish(VALUE self, VALUE success, VALUE time)
{
    gdk_drop_finish(_SELF(self), RVAL2CBOOL(success), NUM2UINT(time));
    return self;
}

static VALUE
rg_drag_status(VALUE self, VALUE action, VALUE time)
{
    gdk_drag_status(_SELF(self), 
                    RVAL2GFLAGS(action, GDK_TYPE_DRAG_ACTION), NUM2UINT(time));
    return self;
}

#if GTK_CHECK_VERSION(2,6,0)
static VALUE
rg_drag_drop_succeeded_p(VALUE self)
{
    return CBOOL2RVAL(gdk_drag_drop_succeeded(_SELF(self)));
}
#endif

void
Init_gtk_gdk_dragcontext(VALUE mGdk)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GDK_TYPE_DRAG_CONTEXT, "DragContext", mGdk);

    RG_DEF_SMETHOD(get_protocol, -1);
    RG_DEF_SMETHOD(drag_begin, 1);

    RG_DEF_METHOD(initialize, 0);
    RG_DEF_METHOD(protocol, 0);
    RG_DEF_METHOD_P(source, 0);
    RG_DEF_METHOD(source_window, 0);
    RG_DEF_METHOD(dest_window, 0);
    RG_DEF_METHOD(targets, 0);
    RG_DEF_METHOD(actions, 0);
    RG_DEF_METHOD(suggested_action, 0);
    RG_DEF_METHOD(action, 0);
    RG_DEF_METHOD(start_time, 0);

    RG_DEF_METHOD(selection, 0);
    RG_DEF_METHOD(drag_abort, 1);
    RG_DEF_METHOD(drop_reply, 2);
    RG_DEF_METHOD(drag_drop, 1);
    RG_DEF_METHOD(find_window, 4);
    RG_DEF_METHOD(drag_motion, 7);
    RG_DEF_METHOD(drop_finish, 2);
    RG_DEF_METHOD(drag_status, 2);
#if GTK_CHECK_VERSION(2,6,0)
    RG_DEF_METHOD_P(drag_drop_succeeded, 0);
#endif
    /* Constants */
    G_DEF_CLASS(GDK_TYPE_DRAG_PROTOCOL, "Protocol", RG_TARGET_NAMESPACE);
    G_DEF_CLASS(GDK_TYPE_DRAG_ACTION, "Action", RG_TARGET_NAMESPACE);
    G_DEF_CONSTANTS(RG_TARGET_NAMESPACE, GDK_TYPE_DRAG_PROTOCOL, "GDK_DRAG_");
    G_DEF_CONSTANTS(RG_TARGET_NAMESPACE, GDK_TYPE_DRAG_ACTION, "GDK_");
}
