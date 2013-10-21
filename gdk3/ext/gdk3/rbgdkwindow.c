/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2002-2006 Ruby-GNOME2 Project Team
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

#include "rbgdk3private.h"

#ifdef HAVE_RB_CAIRO_H
#include <rb_cairo.h>
#endif

#define RG_TARGET_NAMESPACE cWindow
#define _SELF(s) RVAL2GDKWINDOW(s)
#define _DEVICE(s) (RVAL2GDKDEVICE(s))

static VALUE
rg_initialize(VALUE self, VALUE parent, VALUE attributes, VALUE attributes_mask)
{
    GdkWindow* win;
    win = gdk_window_new(NIL_P(parent) ? NULL : _SELF(parent),
                         RVAL2GDKWINDOWATTR(attributes),
                         RVAL2GDKWINDOWATTRIBUTESTYPE(attributes_mask));
    G_INITIALIZE(self, win);

    return Qnil;
}

static VALUE
rg_destroy(VALUE self)
{
    gdk_window_destroy(_SELF(self));
    return Qnil;
}

static VALUE
rg_window_type(VALUE self)
{
    return GDKWINDOWTYPE2RVAL(gdk_window_get_window_type(_SELF(self)));
}

static VALUE
rg_s_at_pointer(G_GNUC_UNUSED VALUE self)
{
    gint x, y;
    GdkDevice*device = _DEVICE(self);
    GdkWindow* win = gdk_device_get_window_at_position(device, &x ,&y);
    return rb_ary_new3(3, GOBJ2RVAL(win), INT2FIX(x), INT2FIX(y));
}

static VALUE
rg_show(VALUE self)
{
    gdk_window_show(_SELF(self));
    return self;
}

static VALUE
rg_show_unraised(VALUE self)
{
    gdk_window_show_unraised(_SELF(self));
    return self;
}

static VALUE
rg_hide(VALUE self)
{
    gdk_window_hide(_SELF(self));
    return Qnil;
}

static VALUE
rg_visible_p(VALUE self)
{
    return CBOOL2RVAL(gdk_window_is_visible(_SELF(self)));
}

static VALUE
rg_viewable_p(VALUE self)
{
    return CBOOL2RVAL(gdk_window_is_viewable(_SELF(self)));
}

static VALUE
rg_state(VALUE self)
{
    return GDKWINDOWSTATE2RVAL(gdk_window_get_state(_SELF(self)));
}

static VALUE
rg_withdraw(VALUE self)
{
    gdk_window_withdraw(_SELF(self));
    return Qnil;
}

static VALUE
rg_iconify(VALUE self)
{
    gdk_window_iconify(_SELF(self));
    return self;
}

static VALUE
rg_deiconify(VALUE self)
{
    gdk_window_deiconify(_SELF(self));
    return self;
}

static VALUE
rg_stick(VALUE self)
{
    gdk_window_stick(_SELF(self));
    return self;
}

static VALUE
rg_unstick(VALUE self)
{
    gdk_window_unstick(_SELF(self));
    return self;
}

static VALUE
rg_maximize(VALUE self)
{
    gdk_window_maximize(_SELF(self));
    return self;
}

static VALUE
rg_unmaximize(VALUE self)
{
    gdk_window_unmaximize(_SELF(self));
    return self;
}

static VALUE
rg_fullscreen(VALUE self)
{
    gdk_window_fullscreen(_SELF(self));
    return self;
}

static VALUE
rg_unfullscreen(VALUE self)
{
    gdk_window_unfullscreen(_SELF(self));
    return self;
}

static VALUE
rg_set_keep_above(VALUE self, VALUE setting)
{
    gdk_window_set_keep_above(_SELF(self), RVAL2CBOOL(setting));
    return self;
}

static VALUE
rg_set_keep_below(VALUE self, VALUE setting)
{
    gdk_window_set_keep_below(_SELF(self), RVAL2CBOOL(setting));
    return self;
}

static VALUE
rg_move(VALUE self, VALUE x, VALUE y)
{
    gdk_window_move(_SELF(self), NUM2INT(x), NUM2INT(y));
    return self;
}

static VALUE
rg_resize(VALUE self, VALUE w, VALUE h)
{
    gdk_window_resize(_SELF(self), NUM2INT(w), NUM2INT(h));
    return self;
}

static VALUE
rg_move_resize(VALUE self, VALUE x, VALUE y, VALUE w, VALUE h)
{
    gdk_window_move_resize(_SELF(self), NUM2INT(x), NUM2INT(y), NUM2INT(w), NUM2INT(h));
    return self;
}

static VALUE
rg_scroll(VALUE self, VALUE dx, VALUE dy)
{
    gdk_window_scroll(_SELF(self), NUM2INT(dx), NUM2INT(dy));
    return self;
}

static VALUE
rg_move_region(VALUE self, VALUE region, VALUE dx, VALUE dy)
{
    gdk_window_move_region(_SELF(self),
                           RVAL2CRREGION(region),
                           NUM2INT(dx),
                           NUM2INT(dy));
    return self;
}

static VALUE
rg_reparent(VALUE self, VALUE new_parent, VALUE x, VALUE y)
{
    gdk_window_reparent(_SELF(self), _SELF(new_parent), NUM2INT(x), NUM2INT(y));
    return self;
}

static VALUE
rg_raise(VALUE self)
{
    gdk_window_raise(_SELF(self));
    return self;
}

static VALUE
rg_lower(VALUE self)
{
    gdk_window_lower(_SELF(self));
    return self;
}

static VALUE
rg_focus(VALUE self, VALUE timestamp)
{
    gdk_window_focus(_SELF(self), NUM2UINT(timestamp));
    return self;
}

static VALUE
rg_register_dnd(VALUE self)
{
    gdk_window_register_dnd(_SELF(self));
    return self;
}

static VALUE
rg_begin_resize_drag(VALUE self, VALUE edge, VALUE button, VALUE root_x, VALUE root_y, VALUE timestamp)
{
    gdk_window_begin_resize_drag(_SELF(self),
                                 RVAL2GDKWINDOWEDGE(edge),
                                 NUM2INT(button),
                                 NUM2INT(root_x), NUM2INT(root_y),
                                 NUM2UINT(timestamp));
    return self;
}

static VALUE
rg_begin_move_drag(VALUE self, VALUE button, VALUE root_x, VALUE root_y, VALUE timestamp)
{
    gdk_window_begin_move_drag(_SELF(self), NUM2INT(button),
                               NUM2INT(root_x), NUM2INT(root_y),
                                 NUM2UINT(timestamp));
    return self;
}

static VALUE
rg_s_constrain_size(G_GNUC_UNUSED VALUE self, VALUE geometry, VALUE flags, VALUE w, VALUE h)
{
    gint new_width, new_height;
    gdk_window_constrain_size(RVAL2GDKGEOMETRY(geometry),
                              RVAL2GDKWINDOWHINTS(flags),
                              NUM2INT(w), NUM2INT(h), &new_width, &new_height);
    return rb_assoc_new(INT2NUM(new_width), INT2NUM(new_height));
}

static VALUE
rg_beep(VALUE self)
{
    gdk_window_beep(_SELF(self));
    return self;
}

static VALUE
rg_begin_paint(VALUE self, VALUE area)
{
    if (rb_obj_is_kind_of(area, GTYPE2CLASS(GDK_TYPE_RECTANGLE))){
        gdk_window_begin_paint_rect(_SELF(self),
                                    RVAL2GDKRECTANGLE(area));
    } else {
        gdk_window_begin_paint_region(_SELF(self), RVAL2CRREGION(area));
    }
    return self;
}

static VALUE
rg_end_paint(VALUE self)
{
    gdk_window_end_paint(_SELF(self));
    return self;
}

static VALUE
rg_invalidate(VALUE self, VALUE area, VALUE invalidate_children)
{
    if (rb_obj_is_kind_of(area, GTYPE2CLASS(GDK_TYPE_RECTANGLE))){
        gdk_window_invalidate_rect(_SELF(self),
                                   RVAL2GDKRECTANGLE(area),
                                   RVAL2CBOOL(invalidate_children));
    } else {
        gdk_window_invalidate_region(_SELF(self),
                                     RVAL2CRREGION(area),
                                     RVAL2CBOOL(invalidate_children));
    }
    return self;
}

static gboolean
invalidate_child_func_wrap(GdkWindow *window, VALUE func)
{
    VALUE result = rb_funcall(func, id_call, 1, GOBJ2RVAL(window));
    return RVAL2CBOOL(result);
}

static VALUE
rg_invalidate_maybe_recurse(VALUE self, VALUE region)
{
    VALUE func = (VALUE)NULL;
    if (rb_block_given_p()){
        func = rb_block_proc();
        G_RELATIVE(self, func);
    }
    gdk_window_invalidate_maybe_recurse(_SELF(self),
                                        RVAL2CRREGION(region),
                                        (GdkWindowChildFunc)invalidate_child_func_wrap,
                                        (gpointer)func);
    return self;
}

static VALUE
rg_update_area(VALUE self)
{
    return CRREGION2RVAL(gdk_window_get_update_area(_SELF(self)));
}

static VALUE
rg_freeze_updates(VALUE self)
{
    gdk_window_freeze_updates(_SELF(self));
    return self;
}

static VALUE
rg_thaw_updates(VALUE self)
{
    gdk_window_thaw_updates(_SELF(self));
    return self;
}

static VALUE
rg_s_process_all_updates(VALUE self)
{
    gdk_window_process_all_updates();
    return self;
}

static VALUE
rg_process_updates(VALUE self, VALUE update_children)
{
    gdk_window_process_updates(_SELF(self), RVAL2CBOOL(update_children));
    return self;
}

static VALUE
rg_s_set_debug_updates(VALUE self, VALUE setting)
{
    gdk_window_set_debug_updates(RVAL2CBOOL(setting));
    return self;
}

static VALUE
rg_configure_finished(VALUE self)
{
    gdk_window_configure_finished(_SELF(self));
    return self;
}

static VALUE
rg_enable_synchronized_configure(VALUE self)
{
    gdk_window_enable_synchronized_configure(_SELF(self));

    if (rb_block_given_p()) {
        rb_ensure(rb_yield, self, rg_configure_finished, self);
    }
    return self;
}

static VALUE
rg_set_user_data(VALUE self, VALUE user_data)
{
    gdk_window_set_user_data(_SELF(self), (gpointer)RVAL2GOBJ(user_data));
    return self;
}

static VALUE
rg_set_override_redirect(VALUE self, VALUE override_redirect)
{
    gdk_window_set_override_redirect(_SELF(self),
                                     RVAL2CBOOL(override_redirect));
    return self;
}

static VALUE
rg_set_accept_focus(VALUE self, VALUE accept_focus)
{
    gdk_window_set_accept_focus(_SELF(self), RVAL2CBOOL(accept_focus));
    return self;
}

static VALUE
rg_set_focus_on_map(VALUE self, VALUE focus_on_map)
{
    gdk_window_set_focus_on_map(_SELF(self), RVAL2CBOOL(focus_on_map));
    return self;
}

static VALUE
rg_shape_combine_region(VALUE self, VALUE shape_region, VALUE offset_x, VALUE offset_y)
{
    gdk_window_shape_combine_region(_SELF(self),
                                    NIL_P(shape_region) ? NULL : RVAL2CRREGION(shape_region),
                                    INT2NUM(offset_x), INT2NUM(offset_y));
    return self;
}

static VALUE
rg_set_child_shapes(VALUE self)
{
    gdk_window_set_child_shapes(_SELF(self));
    return self;
}

static VALUE
rg_merge_child_shapes(VALUE self)
{
    gdk_window_merge_child_shapes(_SELF(self));
    return self;
}

static VALUE
rg_input_shape_combine_region(VALUE self, VALUE shape_region, VALUE offset_x, VALUE offset_y)
{
    gdk_window_input_shape_combine_region(_SELF(self),
                                          RVAL2CRREGION(shape_region),
                                          NUM2INT(offset_x),
                                          NUM2INT(offset_y));
    return self;
}

static VALUE
rg_set_child_input_shapes(VALUE self)
{
    gdk_window_set_child_input_shapes(_SELF(self));
    return self;
}

static VALUE
rg_merge_child_input_shapes(VALUE self)
{
    gdk_window_merge_child_input_shapes(_SELF(self));
    return self;
}

static VALUE
rg_set_static_gravities(VALUE self, VALUE use_static)
{
    gboolean ret = gdk_window_set_static_gravities(_SELF(self),
                                                   RVAL2CBOOL(use_static));
    if (! ret)
        rb_raise(rb_eRuntimeError, "couldn't turn on static gravity");

    return self;
}

static VALUE
rg_set_title(VALUE self, VALUE title)
{
    gdk_window_set_title(_SELF(self),RVAL2CSTR(title));
    return self;
}

static VALUE
rg_set_background(VALUE self, VALUE color)
{
    gdk_window_set_background(_SELF(self), RVAL2GDKCOLOR(color));
    return self;
}

static VALUE
rg_user_data(VALUE self)
{
    GObject *data = NULL;
    gdk_window_get_user_data(_SELF(self), (gpointer)&data);
    return GOBJ2RVAL(data);
}

static VALUE
rg_geometry(VALUE self)
{
    gint x, y, w, h;
    gdk_window_get_geometry(_SELF(self), &x, &y, &w, &h);
    return rb_ary_new3(4, INT2NUM(x), INT2NUM(y),
                       INT2NUM(w), INT2NUM(h));
}

static VALUE
rg_set_geometry_hints(VALUE self, VALUE geometry, VALUE geom_mask)
{
    gdk_window_set_geometry_hints(_SELF(self),
                                  NIL_P(geometry) ? (GdkGeometry*)NULL : RVAL2GDKGEOMETRY(geometry),
                                  RVAL2GDKWINDOWHINTS(geom_mask));
    return self;
}

static VALUE
rg_width(VALUE self)
{
    return INT2NUM(gdk_window_get_width(_SELF(self)));
}

static VALUE
rg_height(VALUE self)
{
    return INT2NUM(gdk_window_get_height(_SELF(self)));
}

static VALUE
rg_set_icon_list(VALUE self, VALUE rbpixbufs)
{
    GdkWindow *window = _SELF(self);
    GList *pixbufs = RVAL2GDKPIXBUFGLIST(rbpixbufs);

    gdk_window_set_icon_list(window, pixbufs);

    g_list_free(pixbufs);

    return self;
}

static VALUE
rg_set_modal_hint(VALUE self, VALUE modal)
{
    gdk_window_set_modal_hint(_SELF(self), RVAL2CBOOL(modal));
    return self;
}

static VALUE
rg_set_type_hint(VALUE self, VALUE hint)
{
    gdk_window_set_type_hint(_SELF(self), RVAL2GDKWINDOWHINT(hint));
    return self;
}

static VALUE
rg_type_hint(VALUE self)
{
    return GDKWINDOWHINT2RVAL(gdk_window_get_type_hint(_SELF(self)));
}

static VALUE
rg_set_skip_taskbar_hint(VALUE self, VALUE hint)
{
    gdk_window_set_skip_taskbar_hint(_SELF(self), RVAL2CBOOL(hint));
    return self;
}

static VALUE
rg_set_skip_pager_hint(VALUE self, VALUE hint)
{
    gdk_window_set_skip_pager_hint(_SELF(self), RVAL2CBOOL(hint));
    return self;
}

static VALUE
rg_set_urgency_hint(VALUE self, VALUE hint)
{
    gdk_window_set_urgency_hint(_SELF(self), RVAL2CBOOL(hint));
    return self;
}

static VALUE
rg_position(VALUE self)
{
    gint x, y;
    gdk_window_get_position(_SELF(self), &x, &y);
    return rb_assoc_new(INT2NUM(x), INT2NUM(y));
}

static VALUE
rg_root_origin(VALUE self)
{
    int x, y;
    gdk_window_get_root_origin(_SELF(self), &x, &y);
    return rb_assoc_new(INT2FIX(x), INT2FIX(y));
}

static VALUE
rg_frame_extents(VALUE self)
{
    GdkRectangle rect;
    gdk_window_get_frame_extents(_SELF(self), &rect);
    return GDKRECTANGLE2RVAL(&rect);
}

static VALUE
rg_origin(VALUE self)
{
    gint x, y;
    gdk_window_get_origin(_SELF(self), &x, &y);
    return rb_assoc_new(INT2NUM(x), INT2NUM(y));
}

static VALUE
rg_pointer(VALUE self)
{
    gint x, y;
    GdkModifierType state;
    GdkDevice *device = _DEVICE(self);
    GdkWindow* ret = gdk_window_get_device_position(_SELF(self), device, &x, &y, &state);
    return rb_ary_new3(4, GOBJ2RVAL(ret), INT2NUM(x), INT2NUM(y), GDKMODIFIERTYPE2RVAL(state));
}

static VALUE
rg_parent(VALUE self)
{
    return GOBJ2RVAL(gdk_window_get_parent(_SELF(self)));
}

static VALUE
rg_toplevel(VALUE self)
{
    return GOBJ2RVAL(gdk_window_get_toplevel(_SELF(self)));
}

static VALUE
rg_children(VALUE self)
{
    /* Don't use gdk_window_get_children() here */
    GList* list = gdk_window_peek_children(_SELF(self));
    VALUE ary = rb_ary_new();
    while (list) {
        rb_ary_push(ary, GOBJ2RVAL(list->data));
        list = list->next;
    }
    return ary;
}

static VALUE
rg_events(VALUE self)
{
    return GDKEVENTMASK2RVAL(gdk_window_get_events(_SELF(self)));
}

static VALUE
rg_set_events(VALUE self, VALUE mask)
{
    gdk_window_set_events(_SELF(self), RVAL2GDKEVENTMASK(mask));
    return self;
}

/* deprecated
static VALUE
rg_set_icon(VALUE self, VALUE icon, VALUE pixmap, VALUE mask)
{
    gdk_window_set_icon(_SELF(self), NIL_P(icon) ? NULL :_SELF(icon),
                        NIL_P(pixmap) ? NULL : RVAL2GDKPIXMAP(pixmap),
                        NIL_P(mask) ? NULL : RVAL2GDKBITMAP(mask));
    return self;
}
*/

static VALUE
rg_set_icon_name(VALUE self, VALUE name)
{
    gdk_window_set_icon_name(_SELF(self), RVAL2CSTR(name));
    return self;
}

static VALUE
rg_set_transient_for(VALUE self, VALUE parent)
{
    gdk_window_set_transient_for(_SELF(self), _SELF(parent));
    return self;
}

static VALUE
rg_set_role(VALUE self, VALUE role)
{
    gdk_window_set_role(_SELF(self), RVAL2CSTR(role));
    return self;
}

static VALUE
rg_set_group(VALUE self, VALUE leader)
{
    gdk_window_set_group(_SELF(self), _SELF(leader));
    return self;
}

static VALUE
rg_group(VALUE self)
{
    return GOBJ2RVAL(gdk_window_get_group(_SELF(self)));
}

static VALUE
rg_set_decorations(VALUE self, VALUE decor)
{
    gdk_window_set_decorations(_SELF(self), RVAL2GDKWMDECORATION(decor));
    return self;
}

static VALUE
rg_decorations(VALUE self)
{
    GdkWMDecoration decorations;
    gboolean ret = gdk_window_get_decorations(_SELF(self), &decorations);
    return ret ? GDKWMDECORATION2RVAL(decorations) : Qnil;
}

static VALUE
rg_set_functions(VALUE self, VALUE func)
{
    gdk_window_set_functions(_SELF(self), RVAL2GDKWMFUNCTION(func));
    return self;
}

static VALUE
rg_set_composited(VALUE self, VALUE composited)
{
    gdk_window_set_composited(_SELF(self), RVAL2CBOOL(composited));
    return self;
}

static VALUE
rg_set_opacity(VALUE self, VALUE opacity)
{
    gdk_window_set_opacity(_SELF(self), NUM2DBL(opacity));
    return self;
}

static VALUE
rg_set_startup_id(VALUE self, VALUE startup_id)
{
    gdk_window_set_startup_id(_SELF(self), RVAL2CSTR_ACCEPT_NIL(startup_id));
    return self;
}

/* deprecated
static VALUE
rg_s_toplevels(G_GNUC_UNUSED VALUE self)
{
    return GOBJGLIST2RVAL_FREE(gdk_window_get_toplevels(), g_list_free, NULL);
}
*/

static VALUE
rg_s_default_root_window(G_GNUC_UNUSED VALUE self)
{
    return GOBJ2RVAL(gdk_get_default_root_window());
}

/* Would you need this?
GdkPointerHooks* gdk_set_pointer_hooks      (const GdkPointerHooks *new_hooks);
*/

/* From X Window System Interaction */
/* deprecated
static VALUE
rg_s_foreign_new(int argc, VALUE *argv, G_GNUC_UNUSED VALUE self)
{
    VALUE arg[2];
    GdkWindow * win = NULL;

    rb_scan_args(argc, argv, "11", &arg[0], &arg[1]);

    switch(argc)
    {
      case 1:
        win = gdk_window_foreign_new(RVAL2GDKNATIVEWINDOW(arg[0]));
        break;
      case 2:
        win = gdk_window_foreign_new_for_display(RVAL2GOBJ(arg[0]),
                                                 RVAL2GDKNATIVEWINDOW(arg[1]));
        break;
    default:
        break;
    }
    if (win == NULL)
        return Qnil;
    else {
        return GOBJ2RVAL(win);
    }
}

static VALUE
rg_s_lookup(int argc, VALUE *argv, G_GNUC_UNUSED VALUE self)
{
    VALUE arg[2];
    GdkWindow * win = NULL;

    rb_scan_args(argc, argv, "11", &arg[0], &arg[1]);

    switch(argc)
    {
      case 1:
        win = gdk_window_lookup(RVAL2GDKNATIVEWINDOW(arg[0]));
        break;
      case 2:
        win = gdk_window_lookup_for_display(RVAL2GOBJ(arg[0]), RVAL2GDKNATIVEWINDOW(arg[1]));
        break;
    default:
        break;
    }
    if (win == NULL)
        return Qnil;
    else {
        return GOBJ2RVAL(win);
    }
}
*/

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
rg_drag_begin(VALUE self, VALUE targets)
{
    GList *list = RVAL2GDKATOMGLIST(targets);
    GdkDragContext *result = gdk_drag_begin(_SELF(self), list);
    g_list_free(list);

    return GOBJ2RVAL(result);
}

static VALUE
rg_drag_protocol(VALUE self)
{
    GdkWindow *target;
    GdkWindow **p;
    GdkDragProtocol prot;
    VALUE ary = rb_ary_new();

    prot = gdk_window_get_drag_protocol(_SELF(self), &target);
    for (p = &target; *p; p++)
        rb_ary_push(ary, GOBJ2RVAL(*p));

    return rb_ary_new3(2, GDKDRAGPROTOCOL2RVAL(prot), ary);
}

#ifdef HAVE_RB_CAIRO_H
static VALUE
rg_create_cairo_context(VALUE self)
{
    VALUE rb_cr;
    cairo_t *cr;
    cr = gdk_cairo_create(_SELF(self));
    rb_cairo_check_status(cairo_status(cr));
    rb_cr = CRCONTEXT2RVAL(cr);
    cairo_destroy (cr);
    return rb_cr;
}
#endif

void
Init_gdk_window(VALUE mGdk)
{
    GObjectClass *g_class;
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GDK_TYPE_WINDOW, "Window", mGdk);

    g_class = g_type_class_peek(GDK_TYPE_WINDOW);

    RG_DEF_METHOD(initialize, 3);
    RG_DEF_METHOD(destroy, 0);
    RG_DEF_METHOD(window_type, 0);
    RG_DEF_SMETHOD(at_pointer, 0);
    RG_DEF_SMETHOD(constrain_size, 4);
    RG_DEF_SMETHOD(process_all_updates, 0);
    RG_DEF_SMETHOD(set_debug_updates, 1);
    RG_DEF_METHOD(show, 0);
    RG_DEF_METHOD(show_unraised, 0);
    RG_DEF_METHOD(hide, 0);
    RG_DEF_METHOD_P(visible, 0);
    RG_DEF_METHOD_P(viewable, 0);
    RG_DEF_METHOD(withdraw, 0);
    RG_DEF_METHOD(state, 0);
    RG_DEF_METHOD(iconify, 0);
    RG_DEF_METHOD(deiconify, 0);
    RG_DEF_METHOD(stick, 0);
    RG_DEF_METHOD(unstick, 0);
    RG_DEF_METHOD(maximize, 0);
    RG_DEF_METHOD(unmaximize, 0);
    RG_DEF_METHOD(fullscreen, 0);
    RG_DEF_METHOD(unfullscreen, 0);
    RG_DEF_METHOD(set_keep_above, 1);
    RG_DEF_METHOD(set_keep_below, 1);
    RG_DEF_METHOD(move, 2);
    RG_DEF_METHOD(resize, 2);
    RG_DEF_METHOD(move_resize, 4);
    RG_DEF_METHOD(scroll, 2);
    RG_DEF_METHOD(move_region, 3);
    RG_DEF_METHOD(reparent, 3);
    RG_DEF_METHOD(raise, 0);
    RG_DEF_METHOD(lower, 0);
    RG_DEF_METHOD(focus, 1);
    RG_DEF_METHOD(register_dnd, 0);
    RG_DEF_METHOD(beep, 0);
    RG_DEF_METHOD(begin_resize_drag, 5);
    RG_DEF_METHOD(begin_move_drag, 4);
    RG_DEF_METHOD(begin_paint, 1);
    RG_DEF_METHOD(end_paint, 0);
    RG_DEF_METHOD(invalidate, 2);
    RG_DEF_METHOD(invalidate_maybe_recurse, 1);
    RG_DEF_METHOD(update_area, 0);
    RG_DEF_METHOD(freeze_updates, 0);
    RG_DEF_METHOD(thaw_updates, 0);
    RG_DEF_METHOD(process_updates, 1);
    RG_DEF_METHOD(configure_finished, 0);
    RG_DEF_METHOD(enable_synchronized_configure, 0);
    RG_DEF_METHOD(set_user_data, 1);
    RG_DEF_METHOD(set_override_redirect, 1);
    RG_DEF_METHOD(set_accept_focus, 1);
    RG_DEF_METHOD(set_focus_on_map, 1);
    RG_DEF_METHOD(shape_combine_region, 3);
    RG_DEF_METHOD(set_child_shapes, 0);
    RG_DEF_METHOD(merge_child_shapes, 0);
    RG_DEF_METHOD(input_shape_combine_region, 3);
    RG_DEF_METHOD(set_child_input_shapes, 0);
    RG_DEF_METHOD(merge_child_input_shapes, 0);
    RG_DEF_METHOD(set_static_gravities, 1);
    RG_DEF_METHOD(set_title, 1);
    RG_DEF_METHOD(set_background, 1);
    RG_DEF_METHOD(user_data, 0);
    RG_DEF_METHOD(geometry, 0);
    RG_DEF_METHOD(set_geometry_hints, 2);
    RG_DEF_METHOD(width, 0);
    RG_DEF_METHOD(height, 0);
    RG_DEF_METHOD(set_icon_list, 1);
    RG_DEF_METHOD(set_modal_hint, 1);
    RG_DEF_METHOD(set_type_hint, 1);
    RG_DEF_METHOD(type_hint, 0);

    RG_DEF_METHOD(set_skip_taskbar_hint, 1);
    RG_DEF_METHOD(set_skip_pager_hint, 1);
    RG_DEF_METHOD(set_urgency_hint, 1);
    RG_DEF_METHOD(position, 0);
    RG_DEF_METHOD(root_origin, 0);
    RG_DEF_METHOD(frame_extents, 0);
    RG_DEF_METHOD(origin, 0);
    RG_DEF_METHOD(pointer, 0);
    RG_DEF_METHOD(parent, 0);
    RG_DEF_METHOD(toplevel, 0);
    RG_DEF_SMETHOD(default_root_window, 0);
    RG_DEF_METHOD(children, 0);
    RG_DEF_METHOD(events, 0);
    RG_DEF_METHOD(set_events, 1);
/* deprecated
    RG_DEF_METHOD(set_icon, 3);
*/
    RG_DEF_METHOD(set_icon_name, 1);
    RG_DEF_METHOD(set_transient_for, 1);
    RG_DEF_METHOD(set_role, 1);
    RG_DEF_METHOD(set_group, 1);
    RG_DEF_METHOD(group, 0);
    RG_DEF_METHOD(set_decorations, 1);
    RG_DEF_METHOD(decorations, 0);
    RG_DEF_METHOD(set_functions, 1);
    RG_DEF_METHOD(set_composited, 1);
    RG_DEF_METHOD(set_opacity, 1);
    RG_DEF_METHOD(set_startup_id, 1);
/* deprecated
    RG_DEF_SMETHOD(toplevels, 0);

    RG_DEF_SMETHOD(foreign_new, -1);
    RG_DEF_SMETHOD(lookup, -1);
*/
    RG_DEF_METHOD(drag_begin, 1);
    RG_DEF_METHOD(drag_protocol, 0);

#ifdef HAVE_RB_CAIRO_H
    RG_DEF_METHOD(create_cairo_context, 0);
#endif

    G_DEF_CLASS(GDK_TYPE_WINDOW_TYPE, "Type", RG_TARGET_NAMESPACE);
    G_DEF_CLASS(GDK_TYPE_WINDOW_WINDOW_CLASS, "WindowClass",
                RG_TARGET_NAMESPACE);
    G_DEF_CLASS(GDK_TYPE_WINDOW_HINTS, "Hints", RG_TARGET_NAMESPACE);
    G_DEF_CLASS(GDK_TYPE_GRAVITY, "Gravity", RG_TARGET_NAMESPACE);
    G_DEF_CLASS(GDK_TYPE_WINDOW_EDGE, "Edge", RG_TARGET_NAMESPACE);
    G_DEF_CLASS(GDK_TYPE_WINDOW_TYPE_HINT, "TypeHint", RG_TARGET_NAMESPACE);
    G_DEF_CLASS(GDK_TYPE_WINDOW_ATTRIBUTES_TYPE, "AttributesType", RG_TARGET_NAMESPACE);
    G_DEF_CLASS(GDK_TYPE_FILTER_RETURN, "FilterReturn", RG_TARGET_NAMESPACE);
    G_DEF_CLASS(GDK_TYPE_MODIFIER_TYPE, "ModifierType", RG_TARGET_NAMESPACE);
    G_DEF_CLASS(GDK_TYPE_WM_DECORATION, "WMDecoration", RG_TARGET_NAMESPACE);
    G_DEF_CLASS(GDK_TYPE_WM_FUNCTION, "WMFunction", RG_TARGET_NAMESPACE);

    rb_define_const(RG_TARGET_NAMESPACE, "PARENT_RELATIVE", INT2FIX(GDK_PARENT_RELATIVE));

#ifdef GDK_WINDOWING_X11
    G_DEF_CLASS3("GdkWindowImplX11", "WindowImplX11", mGdk);
#elif defined(GDK_WINDOWING_WIN32)
    G_DEF_CLASS3("GdkWindowImplWin32", "WindowImplWin32", mGdk);
#elif defined(GDK_WINDOWING_FB)
    G_DEF_CLASS3("GdkWindowFB", "WindowFB", mGdk);
#endif
}
