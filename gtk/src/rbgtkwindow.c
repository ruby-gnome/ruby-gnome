/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkwindow.c -

  $Author: mutoh $
  $Date: 2002/10/30 13:34:36 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

#define _SELF(s) (GTK_WINDOW(RVAL2GOBJ(s)))

static VALUE
gwin_initialize(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE type;
    GtkWindowType tp;

    if (rb_scan_args(argc, argv, "01", &type) == 1) {
        tp = NUM2INT(type);
    } else {
        tp = GTK_WINDOW_TOPLEVEL;
    }

    RBGTK_INITIALIZE(self, gtk_window_new(tp));
    return Qnil;
}

static VALUE
gwin_set_wmclass(self, wmclass_name, wmclass_class)
    VALUE self, wmclass_name, wmclass_class;
{
    gtk_window_set_wmclass(_SELF(self),
                           NIL_P(wmclass_name)?NULL:RVAL2CSTR(wmclass_name),
                           NIL_P(wmclass_class)?NULL:RVAL2CSTR(wmclass_class));
    return self;
}

static VALUE
gwin_set_policy(self, allow_shrink, allow_grow, auto_shrink)
    VALUE self, allow_shrink, allow_grow, auto_shrink;
{
    gtk_window_set_policy(_SELF(self), RTEST(allow_shrink), 
                          RTEST(allow_grow), RTEST(auto_shrink));
    return self;
}

static VALUE
gwin_add_accel_group(self, accel)
    VALUE self, accel;
{
    gtk_window_add_accel_group(_SELF(self),
                               GTK_ACCEL_GROUP(RVAL2GOBJ(accel)));
    return self;
}

static VALUE
gwin_remove_accel_group(self, accel)
    VALUE self, accel;
{
    gtk_window_remove_accel_group(_SELF(self),
                                  GTK_ACCEL_GROUP(RVAL2GOBJ(accel)));
    return self;
}

static VALUE
gwin_activate_focus(self)
    VALUE self;
{
    gtk_window_activate_focus(_SELF(self));
    return self;
}

static VALUE
gwin_activate_default(self)
    VALUE self;
{
    gtk_window_activate_default(_SELF(self));
    return self;
}

static VALUE
gwin_set_default_size(self, w, h)
    VALUE self, w, h;
{
    gtk_window_set_default_size(_SELF(self),
                                NUM2INT(w), NUM2INT(h));
    return self;
}

static VALUE
gwin_set_geometry_hints(self, geometry_widget, geometry, geom_mask)
    VALUE self, geometry_widget, geometry, geom_mask;
{
    gtk_window_set_geometry_hints(_SELF(self),
                                  GTK_WIDGET(RVAL2GOBJ(geometry_widget)),
                                  (GdkGeometry*)RVAL2BOXED(geometry, GDK_TYPE_GEOMETRY),
                                  NUM2INT(geom_mask));

    return self;
}

static VALUE
gwin_set_gravity(self, gravity)
    VALUE self, gravity;
{
    gtk_window_set_gravity(_SELF(self), NUM2INT(gravity));
    return self;
}

static VALUE
gwin_get_gravity(self)
    VALUE self;
{
    return INT2NUM(gtk_window_get_gravity(_SELF(self)));
}

static VALUE
gwin_set_transient_for(self, parent)
    VALUE self, parent;
{
    gtk_window_set_transient_for(_SELF(self), _SELF(parent));
    return self;
}

/*
void        gtk_window_set_destroy_with_parent
                                            (GtkWindow *window,
                                             gboolean setting);
GList*      gtk_window_list_toplevels       (void);
void        gtk_window_add_mnemonic         (GtkWindow *window,
                                             guint keyval,
                                             GtkWidget *target);
void        gtk_window_remove_mnemonic      (GtkWindow *window,
                                             guint keyval,
                                             GtkWidget *target);
gboolean    gtk_window_mnemonic_activate    (GtkWindow *window,
                                             guint keyval,
                                             GdkModifierType modifier);
*/

static VALUE
gwin_get_focus(self)
    VALUE self;
{
    return GOBJ2RVAL(gtk_window_get_focus(_SELF(self)));
}

static VALUE
gwin_set_focus(self, win)
    VALUE self, win;
{
    gtk_window_set_focus(_SELF(self), 
                         GTK_WIDGET(RVAL2GOBJ(win)));
    return self;
}

static VALUE
gwin_set_default(self, win)
    VALUE self, win;
{
    gtk_window_set_default(_SELF(self), 
                           GTK_WIDGET(RVAL2GOBJ(win)));
    return self;
}

static VALUE
gwin_present(self)
    VALUE self;
{
    gtk_window_present(_SELF(self));
    return self;
}

static VALUE
gwin_iconify(self)
    VALUE self;
{
    gtk_window_iconify(_SELF(self));
    return self;
}

static VALUE
gwin_deiconify(self)
    VALUE self;
{
    gtk_window_deiconify(_SELF(self));
    return self;
}

static VALUE
gwin_stick(self)
    VALUE self;
{
    gtk_window_stick(_SELF(self));
    return self;
}

static VALUE
gwin_unstick(self)
    VALUE self;
{
    gtk_window_unstick(_SELF(self));
    return self;
}

static VALUE
gwin_maximize(self)
    VALUE self;
{
    gtk_window_maximize(_SELF(self));
    return self;
}

static VALUE
gwin_unmaximize(self)
    VALUE self;
{
    gtk_window_unmaximize(_SELF(self));
    return self;
}

/*
void        gtk_window_begin_resize_drag    (GtkWindow *window,
                                             GdkWindowEdge edge,
                                             gint button,
                                             gint root_x,
                                             gint root_y,
                                             guint32 timestamp);
void        gtk_window_begin_move_drag      (GtkWindow *window,
                                             gint button,
                                             gint root_x,
                                             gint root_y,
                                             guint32 timestamp);
*/

static VALUE
gwin_set_decorated(self, setting)
    VALUE self, setting;
{
    gtk_window_set_decorated(_SELF(self), RTEST(setting));
    return self;
}

/*
void        gtk_window_set_frame_dimensions (GtkWindow *window,
                                             gint left,
                                             gint top,
                                             gint right,
                                             gint bottom);
void        gtk_window_set_has_frame        (GtkWindow *window,
                                             gboolean setting);
void        gtk_window_set_mnemonic_modifier
                                            (GtkWindow *window,
                                             GdkModifierType modifier);
*/

static VALUE
gwin_set_role(self, role)
    VALUE self, role;
{
    gtk_window_set_role(_SELF(self), RVAL2CSTR(role));
    return self;
}

static VALUE
gwin_set_type_hint(self, hint)
    VALUE self, hint;
{
    gtk_window_set_type_hint(_SELF(self), NUM2INT(hint));
    return self;
}

static VALUE
gwin_get_decorated(self)
    VALUE self;
{
    return gtk_window_get_decorated(_SELF(self)) ? Qtrue : Qfalse;
}

/*
GList*      gtk_window_get_default_icon_list
                                            (void);
*/

static VALUE
gwin_get_default_size(self)
    VALUE self;
{
    int width, height;
    gtk_window_get_default_size(_SELF(self), &width, &height);
    return rb_ary_new3(2, INT2NUM(width), INT2NUM(height));
}

static VALUE
gwin_get_destroy_with_parent(self)
    VALUE self;
{
    return gtk_window_get_destroy_with_parent(_SELF(self)) ? Qtrue :  Qfalse;
}

static VALUE
gwin_get_frame_dimensions(self)
    VALUE self;
{
    int left, top, right, bottom;
    gtk_window_get_frame_dimensions(_SELF(self), &left, &top, &right, &bottom);
    return rb_ary_new3(2, INT2NUM(left), INT2NUM(top), INT2NUM(right), INT2NUM(bottom));
}

/*
gboolean    gtk_window_get_has_frame        (GtkWindow *window);
GList*      gtk_window_get_icon_list        (GtkWindow *window);
GdkModifierType gtk_window_get_mnemonic_modifier
                                            (GtkWindow *window);
*/

static VALUE
gwin_get_mnemonic_modifier(self)
    VALUE self;
{
    return INT2NUM(gtk_window_get_mnemonic_modifier(_SELF(self)));
}

static VALUE
gwin_get_position(self)
    VALUE self;
{
    int root_x, root_y;
    gtk_window_get_position(_SELF(self), &root_x, &root_y);
    return rb_ary_new3(2, INT2NUM(root_x), INT2NUM(root_y));
}

static VALUE
gwin_get_role(self)
    VALUE self;
{
    return CSTR2RVAL(gtk_window_get_role(_SELF(self)));
}

static VALUE
gwin_get_size(self)
    VALUE self;
{
    int width, height;
    gtk_window_get_size(_SELF(self), &width, &height);
    return rb_ary_new3(2, INT2NUM(width), INT2NUM(height));
}

static VALUE
gwin_get_transient_for(self)
    VALUE self;
{
    return GOBJ2RVAL(gtk_window_get_transient_for(_SELF(self)));
}

static VALUE
gwin_get_type_hint(self)
    VALUE self;
{
    return INT2NUM(gtk_window_get_type_hint(_SELF(self)));
}

static VALUE
gwin_move(self, x, y)
    VALUE self, x, y;
{
    gtk_window_move(_SELF(self), NUM2INT(x), NUM2INT(y));
    return self;
}

/*
gboolean    gtk_window_parse_geometry       (GtkWindow *window,
                                             const gchar *geometry);
void        gtk_window_reshow_with_initial_size
                                            (GtkWindow *window);
*/

static VALUE
gwin_resize(self, width, height)
    VALUE self, width, height;
{
    gtk_window_resize(_SELF(self), NUM2INT(width), NUM2INT(height));
    return self;
}

/*
void        gtk_window_set_default_icon_list
                                            (GList *list);
void        gtk_window_set_icon_list        (GtkWindow *window,
                                             GList *list);

static VALUE
gwin_decorated_window_init(self)
    VALUE self;
{
    gtk_decorated_window_init(_SELF(self));
    return self;
}

static VALUE
gwin_decorated_window_calculate_frame_size(self)
    VALUE self;
{
    gtk_decorated_window_calculate_frame_size(_SELF(self));
    return self;
}

static VALUE
gwin_decorated_window_set_title(self, title)
    VALUE self, title;
{
    gtk_decorated_window_set_title(_SELF(self), RVAL2CSTR(title));
    return self;
}

static VALUE
gwin_decorated_window_move_resize_window(self)
    VALUE self;
{
    int x, y, width, height;
    gtk_decorated_window_move_resize_window(_SELF(self), &x, &y, &width, &height);
    return rb_ary_new3(2, INT2NUM(x), INT2NUM(y), INT2NUM(width), INT2NUM(height));
}

*/
void 
Init_gtk_window()
{
    VALUE gWindow = G_DEF_CLASS(GTK_TYPE_WINDOW, "Window", mGtk);

    rb_define_method(gWindow, "initialize", gwin_initialize, -1);
    rb_define_method(gWindow, "set_wmclass", gwin_set_wmclass, 2);
    rb_define_method(gWindow, "set_policy", gwin_set_policy, 3);
    rb_define_method(gWindow, "add_accel_group", gwin_add_accel_group, 1);
    rb_define_method(gWindow, "remove_accel_group", gwin_remove_accel_group, 1);
    rb_define_method(gWindow, "active_focus", gwin_activate_focus, 0);
    rb_define_method(gWindow, "active_default", gwin_activate_default, 0);
    rb_define_method(gWindow, "set_default_size", gwin_set_default_size, 2);
    rb_define_method(gWindow, "set_geometry_hints", gwin_set_geometry_hints, 3);
    rb_define_method(gWindow, "set_gravity", gwin_set_gravity, 1);
    rb_define_method(gWindow, "gravity", gwin_get_gravity, 0);
    rb_define_method(gWindow, "set_transient_for", gwin_set_transient_for, 1);
    rb_define_method(gWindow, "focus", gwin_get_focus, 0);
    rb_define_method(gWindow, "set_focus", gwin_set_focus, 1);
    rb_define_method(gWindow, "set_default", gwin_set_default, 1);
    rb_define_method(gWindow, "present", gwin_present, 0);
    rb_define_method(gWindow, "iconify", gwin_iconify, 0);
    rb_define_method(gWindow, "deiconify", gwin_deiconify, 0);
    rb_define_method(gWindow, "stick", gwin_stick, 0);
    rb_define_method(gWindow, "unstick", gwin_unstick, 0);
    rb_define_method(gWindow, "maximize", gwin_maximize, 0);
    rb_define_method(gWindow, "unmaximize", gwin_unmaximize, 0);
    rb_define_method(gWindow, "set_decorated", gwin_set_decorated, 1);
    rb_define_method(gWindow, "set_role", gwin_set_role, 1);
    rb_define_method(gWindow, "set_type_hint", gwin_set_type_hint, 1);
    rb_define_method(gWindow, "decorated?", gwin_get_decorated, 0);
    rb_define_method(gWindow, "default_size", gwin_get_default_size, 0);
    rb_define_method(gWindow, "destroy_with_parent?", gwin_get_destroy_with_parent, 0);
    rb_define_method(gWindow, "frame_dimensions", gwin_get_frame_dimensions, 0);
    rb_define_method(gWindow, "mnemonic_modifier", gwin_get_mnemonic_modifier, 0);
    rb_define_method(gWindow, "position", gwin_get_position, 0);
    rb_define_method(gWindow, "role", gwin_get_role, 0);
    rb_define_method(gWindow, "size", gwin_get_size, 0);
    rb_define_method(gWindow, "transient_for", gwin_get_transient_for, 0);
    rb_define_method(gWindow, "type_hint", gwin_get_type_hint, 0);
    rb_define_method(gWindow, "move", gwin_move, 2);
    rb_define_method(gWindow, "resize", gwin_resize, 2);
/*
    rb_define_method(gWindow, "decorated_window_init", gwin_decorated_window_init, 0);
    rb_define_method(gWindow, "decorated_window_calculate_frame_size", gwin_decorated_window_calculate_frame_size, 0);   
    rb_define_method(gWindow, "decorated_window_set_title", gwin_decorated_window_set_title, 1);
    rb_define_method(gWindow, "decorated_window_move_resize_window", gwin_decorated_window_move_resize_window, 0);
*/    
    G_DEF_SETTERS(gWindow);

    /* GtkWindowPosition (from General constants) */
    rb_define_const(gWindow, "POS_NONE", INT2FIX(GTK_WIN_POS_NONE));
    rb_define_const(gWindow, "POS_CENTER", INT2FIX(GTK_WIN_POS_CENTER));
    rb_define_const(gWindow, "POS_MOUSE", INT2FIX(GTK_WIN_POS_MOUSE));
    rb_define_const(gWindow, "POS_CENTER_ALWAYS", INT2FIX(GTK_WIN_POS_CENTER_ALWAYS));
    rb_define_const(gWindow, "POS_CENTER_ON_PARENT", INT2FIX(GTK_WIN_POS_CENTER_ON_PARENT));

    /* GtkWindowType (from General constants) */
    rb_define_const(gWindow, "TOPLEVEL", INT2FIX(GTK_WINDOW_TOPLEVEL));
    rb_define_const(gWindow, "POPUP", INT2FIX(GTK_WINDOW_POPUP));
}
