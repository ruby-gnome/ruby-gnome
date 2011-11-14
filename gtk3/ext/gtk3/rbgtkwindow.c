/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2002-2011 Ruby-GNOME2 Project Team
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

#define RG_TARGET_NAMESPACE cWindow
#define _SELF(s) (GTK_WINDOW(RVAL2GOBJ(s)))

static VALUE
rg_initialize(int argc, VALUE *argv, VALUE self)
{
    VALUE arg;
    GtkWindowType tp = GTK_WINDOW_TOPLEVEL;
    const gchar *title = NULL;
    GtkWidget* window = NULL;

    if (rb_scan_args(argc, argv, "01", &arg) == 1) {
        if (TYPE(arg) == T_STRING) {
            StringValue(arg);
            title = RVAL2CSTR(arg);
        } else {
            tp = RVAL2GENUM(arg, GTK_TYPE_WINDOW_TYPE);
        }
    }
    window = gtk_window_new(tp);

    if (title != NULL)
        gtk_window_set_title(GTK_WINDOW(window), title);

    RBGTK_INITIALIZE(self, window);
    return Qnil;
}

static VALUE
rg_set_wmclass(VALUE self, VALUE wmclass_name, VALUE wmclass_class)
{
    gtk_window_set_wmclass(_SELF(self),
                           NIL_P(wmclass_name)?NULL:RVAL2CSTR(wmclass_name),
                           NIL_P(wmclass_class)?NULL:RVAL2CSTR(wmclass_class));
    return self;
}

static VALUE
rg_add_accel_group(VALUE self, VALUE accel)
{
    gtk_window_add_accel_group(_SELF(self),
                               GTK_ACCEL_GROUP(RVAL2GOBJ(accel)));
    G_CHILD_ADD(self, accel);
    return self;
}

static VALUE
rg_remove_accel_group(VALUE self, VALUE accel)
{
    gtk_window_remove_accel_group(_SELF(self),
                                  GTK_ACCEL_GROUP(RVAL2GOBJ(accel)));
    G_CHILD_REMOVE(self, accel);
    return self;
}

static VALUE
gwin_activate_focus(VALUE self)
{
    return CBOOL2RVAL(gtk_window_activate_focus(_SELF(self)));
}

static VALUE
gwin_activate_default(VALUE self)
{
    return CBOOL2RVAL(gtk_window_activate_default(_SELF(self)));
}

static VALUE
rg_active_focus(VALUE self)
{
    rb_warning("Gtk::Window#active_focus is deprecated. Use Gtk::Window#activate_focus");
    gtk_window_activate_focus(_SELF(self));
    return self;
}

static VALUE
rg_active_default(VALUE self)
{
    rb_warning("Gtk::Window#active_default is deprecated. Use Gtk::Window#activate_default");
    gtk_window_activate_default(_SELF(self));
    return self;
}

static VALUE
rg_set_default_size(VALUE self, VALUE w, VALUE h)
{
    gtk_window_set_default_size(_SELF(self),
                                NUM2INT(w), NUM2INT(h));
    return self;
}

static VALUE
rg_set_geometry_hints(VALUE self, VALUE geometry_widget, VALUE geometry, VALUE geom_mask)
{
    gtk_window_set_geometry_hints(_SELF(self),
                                  GTK_WIDGET(RVAL2GOBJ(geometry_widget)),
                                  (GdkGeometry*)RVAL2BOXED(geometry, GDK_TYPE_GEOMETRY),
                                  RVAL2GFLAGS(geom_mask, GDK_TYPE_WINDOW_HINTS));
    return self;
}

static VALUE
rg_s_toplevels(G_GNUC_UNUSED VALUE self)
{
    GList* list = gtk_window_list_toplevels();
    VALUE ret = GLIST2ARY(list);
    g_list_foreach(list, (GFunc)g_object_ref, NULL);
    g_list_free(list);
    return ret;
}

static VALUE
rg_add_mnemonic(VALUE self, VALUE keyval, VALUE target)
{
    gtk_window_add_mnemonic(_SELF(self), NUM2INT(keyval), GTK_WIDGET(RVAL2GOBJ(target)));
    return self;
}

static VALUE
rg_remove_mnemonic(VALUE self, VALUE keyval, VALUE target)
{
    gtk_window_remove_mnemonic(_SELF(self), NUM2INT(keyval), GTK_WIDGET(RVAL2GOBJ(target)));
    return self;
}

static VALUE
rg_mnemonic_activate(VALUE self, VALUE keyval, VALUE modifier)
{
    return CBOOL2RVAL(gtk_window_mnemonic_activate(_SELF(self), 
                                        NUM2INT(keyval), 
                                        RVAL2GFLAGS(modifier, GDK_TYPE_MODIFIER_TYPE)));
}

static VALUE
rg_focus(VALUE self)
{
    return GOBJ2RVAL(gtk_window_get_focus(_SELF(self)));
}

static VALUE
rg_set_focus(VALUE self, VALUE win)
{
    gtk_window_set_focus(_SELF(self), 
                         NIL_P(win) ? NULL : GTK_WIDGET(RVAL2GOBJ(win)));
    return self;
}

static VALUE
rg_set_default(VALUE self, VALUE win)
{
    gtk_window_set_default(_SELF(self), 
                           NIL_P(win) ? NULL : GTK_WIDGET(RVAL2GOBJ(win)));
    return self;
}

#if GTK_CHECK_VERSION(2,8,0)
static VALUE
rg_present(int argc, VALUE *argv, VALUE self)
{
    VALUE timestamp;
    if (rb_scan_args(argc, argv, "01", &timestamp) == 1) {
        gtk_window_present_with_time(_SELF(self), NUM2UINT(timestamp));
    } else {
        gtk_window_present(_SELF(self));
    }
    return self;
}
#else
static VALUE
gwin_present(VALUE self)
{
    gtk_window_present(_SELF(self));
    return self;
}
#endif

static VALUE
rg_iconify(VALUE self)
{
    gtk_window_iconify(_SELF(self));
    return self;
}

static VALUE
rg_deiconify(VALUE self)
{
    gtk_window_deiconify(_SELF(self));
    return self;
}

static VALUE
rg_stick(VALUE self)
{
    gtk_window_stick(_SELF(self));
    return self;
}

static VALUE
rg_unstick(VALUE self)
{
    gtk_window_unstick(_SELF(self));
    return self;
}

static VALUE
rg_maximize(VALUE self)
{
    gtk_window_maximize(_SELF(self));
    return self;
}

static VALUE
rg_unmaximize(VALUE self)
{
    gtk_window_unmaximize(_SELF(self));
    return self;
}

#if GTK_CHECK_VERSION(2,2,0) 
static VALUE
rg_fullscreen(VALUE self)
{
    gtk_window_fullscreen(_SELF(self));
    return self;
}
static VALUE
rg_unfullscreen(VALUE self)
{
    gtk_window_unfullscreen(_SELF(self));
    return self;
}
#endif
#if GTK_CHECK_VERSION(2,4,0)
static VALUE
rg_set_keep_above(VALUE self, VALUE setting)
{
    gtk_window_set_keep_above(_SELF(self), RVAL2CBOOL(setting));
    return self;
}
static VALUE
rg_set_keep_below(VALUE self, VALUE setting)
{
    gtk_window_set_keep_below(_SELF(self), RVAL2CBOOL(setting));
    return self;
}
#endif

static VALUE
rg_begin_resize_drag(VALUE self, VALUE edge, VALUE button, VALUE root_x, VALUE root_y, VALUE timestamp)
{
    gtk_window_begin_resize_drag(_SELF(self), RVAL2GENUM(edge, GDK_TYPE_WINDOW_EDGE),
                                 NUM2INT(button), NUM2INT(root_x),
                                 NUM2INT(root_y), NUM2UINT(timestamp));
    return self;
}

static VALUE
rg_begin_move_drag(VALUE self, VALUE button, VALUE root_x, VALUE root_y, VALUE timestamp)
{
    gtk_window_begin_move_drag(_SELF(self), 
                               NUM2INT(button), NUM2INT(root_x),
                               NUM2INT(root_y), NUM2UINT(timestamp));
    return self;
}

static VALUE
rg_set_frame_dimensions(VALUE self, VALUE left, VALUE top, VALUE right, VALUE bottom)
{
    gtk_window_set_frame_dimensions(_SELF(self), NUM2INT(left),
                                    NUM2INT(top), NUM2INT(right),
                                    NUM2INT(bottom));
    return self;
}

static VALUE
rg_set_has_frame(VALUE self, VALUE setting)
{
    gtk_window_set_has_frame(_SELF(self), RVAL2CBOOL(setting));
    return self;
}

static VALUE
rg_set_mnemonic_modifier(VALUE self, VALUE modifier)
{
    gtk_window_set_mnemonic_modifier(_SELF(self), 
                                     RVAL2GFLAGS(modifier, GDK_TYPE_MODIFIER_TYPE));
    return self;
}

static VALUE
rg_s_default_icon_list(G_GNUC_UNUSED VALUE self)
{
    return GLIST2ARYF(gtk_window_get_default_icon_list());
}

static VALUE
rg_default_size(VALUE self)
{
    int width, height;
    gtk_window_get_default_size(_SELF(self), &width, &height);
    return rb_ary_new3(2, INT2NUM(width), INT2NUM(height));
}

static VALUE
rg_frame_dimensions(VALUE self)
{
    int left, top, right, bottom;
    gtk_window_get_frame_dimensions(_SELF(self), &left, &top, &right, &bottom);
    return rb_ary_new3(4,
                       INT2NUM(left),
                       INT2NUM(top),
                       INT2NUM(right),
                       INT2NUM(bottom));
}

static VALUE
rg_has_frame_p(VALUE self)
{
    return CBOOL2RVAL(gtk_window_get_has_frame(_SELF(self)));
}

static VALUE
rg_icon_list(VALUE self)
{
    return GLIST2ARYF(gtk_window_get_icon_list(_SELF(self)));
}

static VALUE
rg_mnemonic_modifier(VALUE self)
{
    return GFLAGS2RVAL(gtk_window_get_mnemonic_modifier(_SELF(self)), GDK_TYPE_MODIFIER_TYPE);
}

static VALUE
rg_position(VALUE self)
{
    int root_x, root_y;
    gtk_window_get_position(_SELF(self), &root_x, &root_y);
    return rb_ary_new3(2, INT2NUM(root_x), INT2NUM(root_y));
}

static VALUE
rg_size(VALUE self)
{
    int width, height;
    gtk_window_get_size(_SELF(self), &width, &height);
    return rb_ary_new3(2, INT2NUM(width), INT2NUM(height));
}

#if GTK_CHECK_VERSION(2,10,0)
static VALUE
rg_group(VALUE self)
{
    return GOBJ2RVAL(gtk_window_get_group(_SELF(self)));
}
#endif

static VALUE
rg_move(VALUE self, VALUE x, VALUE y)
{
    gtk_window_move(_SELF(self), NUM2INT(x), NUM2INT(y));
    return self;
}

static VALUE
rg_parse_geometry(VALUE self, VALUE geometry)
{
    return CBOOL2RVAL(gtk_window_parse_geometry(_SELF(self), RVAL2CSTR(geometry)));
}

static VALUE
rg_reshow_with_initial_size(VALUE self)
{
    gtk_window_reshow_with_initial_size(_SELF(self));
    return self;
}

static VALUE
rg_resize(VALUE self, VALUE width, VALUE height)
{
    gtk_window_resize(_SELF(self), NUM2INT(width), NUM2INT(height));
    return self;
}

static VALUE
rg_s_set_default_icon_list(G_GNUC_UNUSED VALUE self, VALUE rblist)
{
    GList *list = RVAL2GDKPIXBUFGLIST(rblist);

    gtk_window_set_default_icon_list(list);

    g_list_free(list);

    return rblist;
}

#if GTK_CHECK_VERSION(2,2,0)
static VALUE
rg_s_set_default_icon(VALUE self, VALUE icon_or_filename)
{
    if (TYPE(icon_or_filename) == T_STRING){
        GError* err;
        gboolean ret = gtk_window_set_default_icon_from_file(RVAL2CSTR(icon_or_filename), &err);
        if (! ret)
            RAISE_GERROR(err);
    } else {
#if GTK_CHECK_VERSION(2,4,0)
        gtk_window_set_default_icon(GDK_PIXBUF(RVAL2GOBJ(icon_or_filename)));
#else
        rb_raise(rb_eArgError, "Invalid argument: %s, or you may need to use GTK+-2.4.x", 
                 rb_class2name(CLASS_OF(icon_or_filename)));
#endif
    }
    return self;
}
#endif

#if GTK_CHECK_VERSION(2,6,0)
static VALUE
rg_s_set_default_icon_name(VALUE self, VALUE name)
{
    gtk_window_set_default_icon_name(RVAL2CSTR(name));
    return self;
}
#endif

static VALUE
gwin_set_icon(VALUE self, VALUE icon_or_filename)
{
    if (TYPE(icon_or_filename) == T_STRING){
#if GTK_CHECK_VERSION(2,2,0)
        GError* err;
        gboolean ret = gtk_window_set_icon_from_file(_SELF(self),
                                                     RVAL2CSTR(icon_or_filename), &err);
        if (! ret)
            RAISE_GERROR(err);
#else
        rb_raise(rb_eArgError, "Invalid argument: %s, or you may need to use GTK+-2.4.x", 
                 rb_class2name(CLASS_OF(icon_or_filename)));
#endif
    } else {
        gtk_window_set_icon(_SELF(self), GDK_PIXBUF(RVAL2GOBJ(icon_or_filename)));
    }
    return self;
}

static VALUE
rg_set_icon_list(VALUE self, VALUE rblist)
{
    GtkWindow *window = _SELF(self);
    GList *list = RVAL2GDKPIXBUFGLIST(rblist);

    gtk_window_set_icon_list(window, list);

    g_list_free(list);

    /* TODO: Shouldn't we return self? */
    return rblist;
}

#if GTK_CHECK_VERSION(2,2,0)
static VALUE
rg_s_set_auto_startup_notification(VALUE self, VALUE setting)
{
    gtk_window_set_auto_startup_notification(RVAL2CBOOL(setting));
    return self;
}
#endif

/* They are not public methods.
static VALUE
rg_decorated_window_init(VALUE self)
{
    gtk_decorated_window_init(_SELF(self));
    return self;
}

static VALUE
rg_decorated_window_calculate_frame_size(VALUE self)
{
    gtk_decorated_window_calculate_frame_size(_SELF(self));
    return self;
}

static VALUE
rg_decorated_window_set_title(VALUE self, VALUE title)
{
    gtk_decorated_window_set_title(_SELF(self), RVAL2CSTR(title));
    return self;
}

static VALUE
rg_decorated_window_move_resize_window(VALUE self)
{
    int x, y, width, height;
    gtk_decorated_window_move_resize_window(_SELF(self), &x, &y, &width, &height);
    return rb_ary_new3(4, INT2NUM(x), INT2NUM(y), INT2NUM(width), INT2NUM(height));
}
*/

static void
mark_toplevels(G_GNUC_UNUSED void *_)
{
    GList* list = gtk_window_list_toplevels();
    GList* p;
    for (p = list; p; p = g_list_next(p))
        rbgobj_gc_mark_instance(p->data);
    g_list_free(list);
}

void 
Init_gtk_window(VALUE mGtk)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_WINDOW, "Window", mGtk);

    RG_DEF_METHOD(initialize, -1);
    RG_DEF_METHOD(set_wmclass, 2);
    RG_DEF_METHOD(add_accel_group, 1);
    RG_DEF_METHOD(remove_accel_group, 1);
    /* active_(focus|default) are deprecated. Use activate_* instead. */
    RG_DEF_METHOD(active_focus, 0);
    RG_DEF_METHOD(active_default, 0);
    G_REPLACE_ACTION(RG_TARGET_NAMESPACE, "activate_focus", gwin_activate_focus, 0);
    G_REPLACE_ACTION(RG_TARGET_NAMESPACE, "activate_default", gwin_activate_default, 0);
    RG_DEF_METHOD(set_default_size, 2);
    RG_DEF_METHOD(set_geometry_hints, 3);
    RG_DEF_SMETHOD(toplevels, 0);
    RG_DEF_METHOD(add_mnemonic, 2);
    RG_DEF_METHOD(remove_mnemonic, 2);
    RG_DEF_METHOD(mnemonic_activate, 2);
    RG_DEF_METHOD(focus, 0);
    RG_DEF_METHOD(set_focus, 1);
    RG_DEF_METHOD(set_default, 1);
#if GTK_CHECK_VERSION(2,8,0)
    RG_DEF_METHOD(present, -1);
#else
    RG_DEF_METHOD(present, 0);
#endif
    RG_DEF_METHOD(iconify, 0);
    RG_DEF_METHOD(deiconify, 0);
    RG_DEF_METHOD(stick, 0);
    RG_DEF_METHOD(unstick, 0);
    RG_DEF_METHOD(maximize, 0);
    RG_DEF_METHOD(unmaximize, 0);
#if GTK_CHECK_VERSION(2,2,0)
    RG_DEF_METHOD(fullscreen, 0);
    RG_DEF_METHOD(unfullscreen, 0);
#endif
#if GTK_CHECK_VERSION(2,4,0)
    RG_DEF_METHOD(set_keep_above, 1);
    RG_DEF_METHOD(set_keep_below, 1);
#endif
    RG_DEF_METHOD(begin_resize_drag, 5);
    RG_DEF_METHOD(begin_move_drag, 4);
    RG_DEF_METHOD(set_frame_dimensions, 4);
    RG_DEF_METHOD(set_mnemonic_modifier, 1);
    RG_DEF_METHOD(default_size, 0);
    RG_DEF_SMETHOD(default_icon_list, 0);
    RG_DEF_METHOD(frame_dimensions, 0);
    RG_DEF_METHOD(set_has_frame, 1);
    RG_DEF_METHOD_P(has_frame, 0);
    RG_DEF_METHOD(icon_list, 0);
    RG_DEF_METHOD(mnemonic_modifier, 0);
    RG_DEF_METHOD(position, 0);
    RG_DEF_METHOD(size, 0);
#if GTK_CHECK_VERSION(2,10,0)
    RG_DEF_METHOD(group, 0);
#endif
    RG_DEF_METHOD(move, 2);
    RG_DEF_METHOD(parse_geometry, 1);
    RG_DEF_METHOD(reshow_with_initial_size, 0);
    RG_DEF_METHOD(resize, 2);
    RG_DEF_SMETHOD(set_default_icon_list, 1);
#if GTK_CHECK_VERSION(2,2,0)
    RG_DEF_SMETHOD(set_default_icon, 1);
#endif
#if GTK_CHECK_VERSION(2,6,0)
    RG_DEF_SMETHOD(set_default_icon_name, 1);
#endif
    G_REPLACE_SET_PROPERTY(RG_TARGET_NAMESPACE, "icon", gwin_set_icon, 1);
    RG_DEF_METHOD(set_icon_list, 1);
#if GTK_CHECK_VERSION(2,2,0)
    RG_DEF_SMETHOD(set_auto_startup_notification, 1);
#endif

/*
    RG_DEF_METHOD(decorated_window_init, 0);
    RG_DEF_METHOD(decorated_window_calculate_frame_size, 0);   
    RG_DEF_METHOD(decorated_window_set_title, 1);
    RG_DEF_METHOD(decorated_window_move_resize_window, 0);
*/
    G_DEF_SETTERS(RG_TARGET_NAMESPACE);

    /* GtkWindowPosition (from General constants) */
    G_DEF_CLASS(GTK_TYPE_WINDOW_POSITION, "Position", RG_TARGET_NAMESPACE);
    G_DEF_CONSTANTS(RG_TARGET_NAMESPACE, GTK_TYPE_WINDOW_POSITION, "GTK_WIN_");

    /* GtkWindowType (from General constants) */
    G_DEF_CLASS(GTK_TYPE_WINDOW_TYPE, "Type", RG_TARGET_NAMESPACE);
    G_DEF_CONSTANTS(RG_TARGET_NAMESPACE, GTK_TYPE_WINDOW_TYPE, "GTK_WINDOW_");

    {
        static VALUE toplevels_marker;
        toplevels_marker =
            rb_data_object_alloc(rb_cData, NULL, mark_toplevels, NULL);
        rb_global_variable(&toplevels_marker);
    }
}
