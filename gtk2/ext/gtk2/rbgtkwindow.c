/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkwindow.c -

  $Author: ktou $
  $Date: 2007/09/08 12:49:10 $

  Copyright (C) 2002-2006 Ruby-GNOME2 Project Team
  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

#define _SELF(s) (GTK_WINDOW(RVAL2GOBJ(s)))

static VALUE
gwin_initialize(int argc, VALUE *argv, VALUE self)
{
    VALUE arg;
    GtkWindowType tp = GTK_WINDOW_TOPLEVEL;
    gchar* title = NULL;
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

    if (title)
        gtk_window_set_title(GTK_WINDOW(window), title);

    RBGTK_INITIALIZE(self, window);
    return Qnil;
}

static VALUE
gwin_set_wmclass(VALUE self, VALUE wmclass_name, VALUE wmclass_class)
{
    gtk_window_set_wmclass(_SELF(self),
                           NIL_P(wmclass_name)?NULL:RVAL2CSTR(wmclass_name),
                           NIL_P(wmclass_class)?NULL:RVAL2CSTR(wmclass_class));
    return self;
}

static VALUE
gwin_add_accel_group(VALUE self, VALUE accel)
{
    gtk_window_add_accel_group(_SELF(self),
                               GTK_ACCEL_GROUP(RVAL2GOBJ(accel)));
    return self;
}

static VALUE
gwin_remove_accel_group(VALUE self, VALUE accel)
{
    gtk_window_remove_accel_group(_SELF(self),
                                  GTK_ACCEL_GROUP(RVAL2GOBJ(accel)));
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
gwin_active_focus(VALUE self)
{
    rb_warning("Gtk::Window#active_focus is deprecated. Use Gtk::Window#activate_focus");
    gtk_window_activate_focus(_SELF(self));
    return self;
}

static VALUE
gwin_active_default(VALUE self)
{
    rb_warning("Gtk::Window#active_default is deprecated. Use Gtk::Window#activate_default");
    gtk_window_activate_default(_SELF(self));
    return self;
}

static VALUE
gwin_set_default_size(VALUE self, VALUE w, VALUE h)
{
    gtk_window_set_default_size(_SELF(self),
                                NUM2INT(w), NUM2INT(h));
    return self;
}

static VALUE
gwin_set_geometry_hints(VALUE self, VALUE geometry_widget, VALUE geometry, VALUE geom_mask)
{
    gtk_window_set_geometry_hints(_SELF(self),
                                  GTK_WIDGET(RVAL2GOBJ(geometry_widget)),
                                  (GdkGeometry*)RVAL2BOXED(geometry, GDK_TYPE_GEOMETRY),
                                  RVAL2GFLAGS(geom_mask, GDK_TYPE_WINDOW_HINTS));
    return self;
}

static VALUE
gwin_set_gravity(VALUE self, VALUE gravity)
{
    gtk_window_set_gravity(_SELF(self), RVAL2GENUM(gravity, GDK_TYPE_GRAVITY));
    return self;
}

static VALUE
gwin_get_gravity(VALUE self)
{
    return GENUM2RVAL(gtk_window_get_gravity(_SELF(self)), GDK_TYPE_GRAVITY);
}

static VALUE
gwin_set_transient_for(VALUE self, VALUE parent)
{
    gtk_window_set_transient_for(_SELF(self), _SELF(parent));
    return self;
}

static VALUE
gwin_set_destroy_with_parent(VALUE self, VALUE setting)
{
    gtk_window_set_destroy_with_parent(_SELF(self), RVAL2CBOOL(setting));
    return self;
}

/* Define as Properties.
void        gtk_window_set_screen           (GtkWindow *window,
                                             GdkScreen *screen);
GdkScreen*  gtk_window_get_screen           (GtkWindow *window);
gboolean    gtk_window_is_active            (GtkWindow *window);
gboolean    gtk_window_has_toplevel_focus   (GtkWindow *window);
*/

static VALUE
gwin_s_list_toplevels(VALUE self)
{
    GList* list = gtk_window_list_toplevels();
    VALUE ret = GLIST2ARY(list);
    g_list_foreach(list, (GFunc)g_object_ref, NULL);
    g_list_free(list);
    return ret;
}

static VALUE
gwin_add_mnemonic(VALUE self, VALUE keyval, VALUE target)
{
    gtk_window_add_mnemonic(_SELF(self), NUM2INT(keyval), GTK_WIDGET(RVAL2GOBJ(target)));
    return self;
}

static VALUE
gwin_remove_mnemonic(VALUE self, VALUE keyval, VALUE target)
{
    gtk_window_remove_mnemonic(_SELF(self), NUM2INT(keyval), GTK_WIDGET(RVAL2GOBJ(target)));
    return self;
}

static VALUE
gwin_mnemonic_activate(VALUE self, VALUE keyval, VALUE modifier)
{
    return CBOOL2RVAL(gtk_window_mnemonic_activate(_SELF(self), 
                                        NUM2INT(keyval), 
                                        RVAL2GFLAGS(modifier, GDK_TYPE_MODIFIER_TYPE)));
}

static VALUE
gwin_get_focus(VALUE self)
{
    return GOBJ2RVAL(gtk_window_get_focus(_SELF(self)));
}

static VALUE
gwin_set_focus(VALUE self, VALUE win)
{
    gtk_window_set_focus(_SELF(self), 
                         NIL_P(win) ? NULL : GTK_WIDGET(RVAL2GOBJ(win)));
    return self;
}

static VALUE
gwin_set_default(VALUE self, VALUE win)
{
    gtk_window_set_default(_SELF(self), 
                           NIL_P(win) ? NULL : GTK_WIDGET(RVAL2GOBJ(win)));
    return self;
}

#if GTK_CHECK_VERSION(2,8,0)
static VALUE
gwin_present(int argc, VALUE *argv, VALUE self)
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
gwin_iconify(VALUE self)
{
    gtk_window_iconify(_SELF(self));
    return self;
}

static VALUE
gwin_deiconify(VALUE self)
{
    gtk_window_deiconify(_SELF(self));
    return self;
}

static VALUE
gwin_stick(VALUE self)
{
    gtk_window_stick(_SELF(self));
    return self;
}

static VALUE
gwin_unstick(VALUE self)
{
    gtk_window_unstick(_SELF(self));
    return self;
}

static VALUE
gwin_maximize(VALUE self)
{
    gtk_window_maximize(_SELF(self));
    return self;
}

static VALUE
gwin_unmaximize(VALUE self)
{
    gtk_window_unmaximize(_SELF(self));
    return self;
}

#if GTK_CHECK_VERSION(2,2,0) 
static VALUE
gwin_fullscreen(VALUE self)
{
    gtk_window_fullscreen(_SELF(self));
    return self;
}
static VALUE
gwin_unfullscreen(VALUE self)
{
    gtk_window_unfullscreen(_SELF(self));
    return self;
}
#endif
#if GTK_CHECK_VERSION(2,4,0)
static VALUE
gwin_set_keep_above(VALUE self, VALUE setting)
{
    gtk_window_set_keep_above(_SELF(self), RVAL2CBOOL(setting));
    return self;
}
static VALUE
gwin_set_keep_below(VALUE self, VALUE setting)
{
    gtk_window_set_keep_below(_SELF(self), RVAL2CBOOL(setting));
    return self;
}
#endif

static VALUE
gwin_begin_resize_drag(VALUE self, VALUE edge, VALUE button, VALUE root_x, VALUE root_y, VALUE timestamp)
{
    gtk_window_begin_resize_drag(_SELF(self), RVAL2GENUM(edge, GDK_TYPE_WINDOW_EDGE),
                                 NUM2INT(button), NUM2INT(root_x),
                                 NUM2INT(root_y), NUM2UINT(timestamp));
    return self;
}

static VALUE
gwin_begin_move_drag(VALUE self, VALUE button, VALUE root_x, VALUE root_y, VALUE timestamp)
{
    gtk_window_begin_move_drag(_SELF(self), 
                               NUM2INT(button), NUM2INT(root_x),
                               NUM2INT(root_y), NUM2UINT(timestamp));
    return self;
}

static VALUE
gwin_set_decorated(VALUE self, VALUE setting)
{
    gtk_window_set_decorated(_SELF(self), RVAL2CBOOL(setting));
    return self;
}

static VALUE
gwin_set_frame_dimensions(VALUE self, VALUE left, VALUE top, VALUE right, VALUE bottom)
{
    gtk_window_set_frame_dimensions(_SELF(self), NUM2INT(left),
                                    NUM2INT(top), NUM2INT(right),
                                    NUM2INT(bottom));
    return self;
}

static VALUE
gwin_set_has_frame(VALUE self, VALUE setting)
{
    gtk_window_set_has_frame(_SELF(self), RVAL2CBOOL(setting));
    return self;
}

static VALUE
gwin_set_mnemonic_modifier(VALUE self, VALUE modifier)
{
    gtk_window_set_mnemonic_modifier(_SELF(self), 
                                     RVAL2GFLAGS(modifier, GDK_TYPE_MODIFIER_TYPE));
    return self;
}

static VALUE
gwin_set_role(VALUE self, VALUE role)
{
    gtk_window_set_role(_SELF(self), RVAL2CSTR(role));
    return self;
}

static VALUE
gwin_set_type_hint(VALUE self, VALUE hint)
{
    gtk_window_set_type_hint(_SELF(self), RVAL2GENUM(hint, GDK_TYPE_WINDOW_TYPE_HINT));
    return self;
}
/* Defined as Properties
void        gtk_window_set_skip_taskbar_hint
                                            (GtkWindow *window,
                                             gboolean setting);
void        gtk_window_set_skip_pager_hint  (GtkWindow *window,
                                             gboolean setting);
void        gtk_window_set_accept_focus     (GtkWindow *window,
                                             gboolean setting);

gchar*      gtk_window_get_icon_name        (GtkWindow *window);
void        gtk_window_set_icon_name        (GtkWindow *window,
                                             const gchar *name);

void        gtk_window_set_focus_on_map     (GtkWindow *window,
                                             gboolean setting);
gboolean    gtk_window_get_focus_on_map     (GtkWindow *window);

void        gtk_window_set_urgency_hint     (GtkWindow *window,
                                             gboolean setting);

gboolean    gtk_window_get_deletable        (GtkWindow *window);

void        gtk_window_set_deletable        (GtkWindow *window,
                                             gboolean setting);
*/


static VALUE
gwin_get_decorated(VALUE self)
{
    return CBOOL2RVAL(gtk_window_get_decorated(_SELF(self)));
}

static VALUE
gwin_s_get_default_icon_list(VALUE self)
{
    return GLIST2ARYF(gtk_window_get_default_icon_list());
}

static VALUE
gwin_get_default_size(VALUE self)
{
    int width, height;
    gtk_window_get_default_size(_SELF(self), &width, &height);
    return rb_ary_new3(2, INT2NUM(width), INT2NUM(height));
}

static VALUE
gwin_get_destroy_with_parent(VALUE self)
{
    return gtk_window_get_destroy_with_parent(_SELF(self)) ? Qtrue :  Qfalse;
}

static VALUE
gwin_get_frame_dimensions(VALUE self)
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
gwin_get_has_frame(VALUE self)
{
    return CBOOL2RVAL(gtk_window_get_has_frame(_SELF(self)));
}

static VALUE
gwin_get_icon_list(VALUE self)
{
    return GLIST2ARYF(gtk_window_get_icon_list(_SELF(self)));
}

static VALUE
gwin_get_mnemonic_modifier(VALUE self)
{
    return GFLAGS2RVAL(gtk_window_get_mnemonic_modifier(_SELF(self)), GDK_TYPE_MODIFIER_TYPE);
}

static VALUE
gwin_get_position(VALUE self)
{
    int root_x, root_y;
    gtk_window_get_position(_SELF(self), &root_x, &root_y);
    return rb_ary_new3(2, INT2NUM(root_x), INT2NUM(root_y));
}

static VALUE
gwin_get_role(VALUE self)
{
    const gchar* role = gtk_window_get_role(_SELF(self));
    return role ? CSTR2RVAL(role) : Qnil;
}

static VALUE
gwin_get_size(VALUE self)
{
    int width, height;
    gtk_window_get_size(_SELF(self), &width, &height);
    return rb_ary_new3(2, INT2NUM(width), INT2NUM(height));
}

static VALUE
gwin_get_transient_for(VALUE self)
{
    GtkWindow* window = gtk_window_get_transient_for(_SELF(self));
    return window ? GOBJ2RVAL(window) : Qnil;
}

static VALUE
gwin_get_type_hint(VALUE self)
{
    return GENUM2RVAL(gtk_window_get_type_hint(_SELF(self)), GDK_TYPE_WINDOW_TYPE_HINT);
}

#if GTK_CHECK_VERSION(2,10,0)
static VALUE
gwin_get_group(VALUE self)
{
    return GOBJ2RVAL(gtk_window_get_group(_SELF(self)));
}
#endif

static VALUE
gwin_move(VALUE self, VALUE x, VALUE y)
{
    gtk_window_move(_SELF(self), NUM2INT(x), NUM2INT(y));
    return self;
}

static VALUE
gwin_parse_geometry(VALUE self, VALUE geometry)
{
    return CBOOL2RVAL(gtk_window_parse_geometry(_SELF(self), RVAL2CSTR(geometry)));
}

static VALUE
gwin_reshow_with_initial_size(VALUE self)
{
    gtk_window_reshow_with_initial_size(_SELF(self));
    return self;
}

static VALUE
gwin_resize(VALUE self, VALUE width, VALUE height)
{
    gtk_window_resize(_SELF(self), NUM2INT(width), NUM2INT(height));
    return self;
}

static VALUE
gwin_s_set_default_icon_list(VALUE self, VALUE list)
{
    int i;
    GList *glist = NULL;
    Check_Type(list, T_ARRAY);
    for (i = 0; i < RARRAY_LEN(list); i++){
        glist = g_list_append(glist, RVAL2GOBJ(RARRAY_PTR(list)[i]));
    }
    gtk_window_set_default_icon_list(glist);
    g_list_free(glist);
    return list;
}

#if GTK_CHECK_VERSION(2,2,0)
static VALUE
gwin_s_set_default_icon(VALUE self, VALUE icon_or_filename)
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
gwin_s_set_default_icon_name(VALUE self, VALUE name)
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
gwin_set_icon_list(VALUE self, VALUE list)
{
    int i;
    GList *glist = NULL;
    Check_Type(list, T_ARRAY);
    for (i = 0; i < RARRAY_LEN(list); i++){
        glist = g_list_append(glist, RVAL2GOBJ(RARRAY_PTR(list)[i]));
    }
    gtk_window_set_icon_list(_SELF(self), glist);
    g_list_free(glist);
    return list;
}
#if GTK_CHECK_VERSION(2,2,0)
static VALUE
gwin_s_set_auto_startup_notification(VALUE self, VALUE setting)
{
    gtk_window_set_auto_startup_notification(RVAL2CBOOL(setting));
    return self;
}
#endif

/* They are not public methods.
static VALUE
gwin_decorated_window_init(VALUE self)
{
    gtk_decorated_window_init(_SELF(self));
    return self;
}

static VALUE
gwin_decorated_window_calculate_frame_size(VALUE self)
{
    gtk_decorated_window_calculate_frame_size(_SELF(self));
    return self;
}

static VALUE
gwin_decorated_window_set_title(VALUE self, VALUE title)
{
    gtk_decorated_window_set_title(_SELF(self), RVAL2CSTR(title));
    return self;
}

static VALUE
gwin_decorated_window_move_resize_window(VALUE self)
{
    int x, y, width, height;
    gtk_decorated_window_move_resize_window(_SELF(self), &x, &y, &width, &height);
    return rb_ary_new3(4, INT2NUM(x), INT2NUM(y), INT2NUM(width), INT2NUM(height));
}
*/

static void
mark_toplevels(void* _)
{
    GList* list = gtk_window_list_toplevels();
    GList* p;
    for (p = list; p; p = g_list_next(p))
        rbgobj_gc_mark_instance(p->data);
    g_list_free(list);
}

void 
Init_gtk_window()
{
    VALUE gWindow = G_DEF_CLASS(GTK_TYPE_WINDOW, "Window", mGtk);

    rb_define_method(gWindow, "initialize", gwin_initialize, -1);
    rb_define_method(gWindow, "set_wmclass", gwin_set_wmclass, 2);
    rb_define_method(gWindow, "add_accel_group", gwin_add_accel_group, 1);
    rb_define_method(gWindow, "remove_accel_group", gwin_remove_accel_group, 1);
    /* active_(focus|default) are deprecated. Use activate_* instead. */
    rb_define_method(gWindow, "active_focus", gwin_active_focus, 0);
    rb_define_method(gWindow, "active_default", gwin_active_default, 0);
    rb_define_method(gWindow, "activate_focus", gwin_activate_focus, 0);
    rb_define_method(gWindow, "activate_default", gwin_activate_default, 0);
    rb_define_method(gWindow, "set_default_size", gwin_set_default_size, 2);
    rb_define_method(gWindow, "set_geometry_hints", gwin_set_geometry_hints, 3);
    rb_define_method(gWindow, "set_gravity", gwin_set_gravity, 1);
    rb_define_method(gWindow, "gravity", gwin_get_gravity, 0);
    rb_define_method(gWindow, "set_transient_for", gwin_set_transient_for, 1);
    rb_define_method(gWindow, "set_destroy_with_parent", gwin_set_destroy_with_parent, 1);
    rb_define_singleton_method(gWindow, "toplevels", gwin_s_list_toplevels, 0);
    rb_define_method(gWindow, "add_mnemonic", gwin_add_mnemonic, 2);
    rb_define_method(gWindow, "remove_mnemonic", gwin_remove_mnemonic, 2);
    rb_define_method(gWindow, "mnemonic_activate", gwin_mnemonic_activate, 2);
    rb_define_method(gWindow, "focus", gwin_get_focus, 0);
    rb_define_method(gWindow, "set_focus", gwin_set_focus, 1);
    rb_define_method(gWindow, "set_default", gwin_set_default, 1);
#if GTK_CHECK_VERSION(2,8,0)
    rb_define_method(gWindow, "present", gwin_present, -1);
#else
    rb_define_method(gWindow, "present", gwin_present, 0);
#endif
    rb_define_method(gWindow, "iconify", gwin_iconify, 0);
    rb_define_method(gWindow, "deiconify", gwin_deiconify, 0);
    rb_define_method(gWindow, "stick", gwin_stick, 0);
    rb_define_method(gWindow, "unstick", gwin_unstick, 0);
    rb_define_method(gWindow, "maximize", gwin_maximize, 0);
    rb_define_method(gWindow, "unmaximize", gwin_unmaximize, 0);
#if GTK_CHECK_VERSION(2,2,0)
    rb_define_method(gWindow, "fullscreen", gwin_fullscreen, 0);
    rb_define_method(gWindow, "unfullscreen", gwin_unfullscreen, 0);
#endif
#if GTK_CHECK_VERSION(2,4,0)
    rb_define_method(gWindow, "set_keep_above", gwin_set_keep_above, 1);
    rb_define_method(gWindow, "set_keep_below", gwin_set_keep_below, 1);
#endif
    rb_define_method(gWindow, "begin_resize_drag", gwin_begin_resize_drag, 5);
    rb_define_method(gWindow, "begin_move_drag", gwin_begin_move_drag, 4);
    rb_define_method(gWindow, "set_decorated", gwin_set_decorated, 1);
    rb_define_method(gWindow, "set_frame_dimensions", gwin_set_frame_dimensions, 4);
    rb_define_method(gWindow, "set_mnemonic_modifier", gwin_set_mnemonic_modifier, 1);
    rb_define_method(gWindow, "set_role", gwin_set_role, 1);
    rb_define_method(gWindow, "set_type_hint", gwin_set_type_hint, 1);
    rb_define_method(gWindow, "decorated?", gwin_get_decorated, 0);
    rb_define_method(gWindow, "default_size", gwin_get_default_size, 0);
    rb_define_singleton_method(gWindow, "default_icon_list", gwin_s_get_default_icon_list, 0);
    rb_define_method(gWindow, "destroy_with_parent?", gwin_get_destroy_with_parent, 0);
    rb_define_method(gWindow, "frame_dimensions", gwin_get_frame_dimensions, 0);
    rb_define_method(gWindow, "set_has_frame", gwin_set_has_frame, 1);
    rb_define_method(gWindow, "has_frame?", gwin_get_has_frame, 0);
    rb_define_method(gWindow, "icon_list", gwin_get_icon_list, 0);
    rb_define_method(gWindow, "mnemonic_modifier", gwin_get_mnemonic_modifier, 0);
    rb_define_method(gWindow, "position", gwin_get_position, 0);
    rb_define_method(gWindow, "role", gwin_get_role, 0);
    rb_define_method(gWindow, "size", gwin_get_size, 0);
    rb_define_method(gWindow, "transient_for", gwin_get_transient_for, 0);
    rb_define_method(gWindow, "type_hint", gwin_get_type_hint, 0);
#if GTK_CHECK_VERSION(2,10,0)
    rb_define_method(gWindow, "group", gwin_get_group, 0);
#endif
    rb_define_method(gWindow, "move", gwin_move, 2);
    rb_define_method(gWindow, "parse_geometry", gwin_parse_geometry, 1);
    rb_define_method(gWindow, "reshow_with_initial_size", gwin_reshow_with_initial_size, 0);
    rb_define_method(gWindow, "resize", gwin_resize, 2);
    rb_define_singleton_method(gWindow, "set_default_icon_list", gwin_s_set_default_icon_list, 1);
#if GTK_CHECK_VERSION(2,2,0)
    rb_define_singleton_method(gWindow, "set_default_icon", gwin_s_set_default_icon, 1);
#endif
#if GTK_CHECK_VERSION(2,6,0)
    rb_define_singleton_method(gWindow, "set_default_icon_name", gwin_s_set_default_icon_name, 1);
#endif
    rb_define_method(gWindow, "set_icon", gwin_set_icon, 1);
    rb_define_method(gWindow, "set_icon_list", gwin_set_icon_list, 1);
#if GTK_CHECK_VERSION(2,2,0)
    rb_define_singleton_method(gWindow, "set_auto_startup_notification", 
                               gwin_s_set_auto_startup_notification, 1);
#endif

/*
    rb_define_method(gWindow, "decorated_window_init", gwin_decorated_window_init, 0);
    rb_define_method(gWindow, "decorated_window_calculate_frame_size", gwin_decorated_window_calculate_frame_size, 0);   
    rb_define_method(gWindow, "decorated_window_set_title", gwin_decorated_window_set_title, 1);
    rb_define_method(gWindow, "decorated_window_move_resize_window", gwin_decorated_window_move_resize_window, 0);
*/
    G_DEF_SETTERS(gWindow);

    /* GtkWindowPosition (from General constants) */
    G_DEF_CLASS(GTK_TYPE_WINDOW_POSITION, "Position", gWindow);
    G_DEF_CONSTANTS(gWindow, GTK_TYPE_WINDOW_POSITION, "GTK_WIN_");

    /* GtkWindowType (from General constants) */
    G_DEF_CLASS(GTK_TYPE_WINDOW_TYPE, "Type", gWindow);
    G_DEF_CONSTANTS(gWindow, GTK_TYPE_WINDOW_TYPE, "GTK_WINDOW_");

    {
        static VALUE toplevels_marker;
        toplevels_marker =
            rb_data_object_alloc(rb_cData, NULL, mark_toplevels, NULL);
        rb_global_variable(&toplevels_marker);
    }
}
