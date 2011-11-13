/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2002-2006 Masao Mutoh
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

#define RG_TARGET_NAMESPACE mDrag

#define RVAL2DC(c) (GDK_DRAG_CONTEXT(RVAL2GOBJ(c)))
#define RVAL2WIDGET(w) (GTK_WIDGET(RVAL2GOBJ(w)))

struct rbgtk_rval2gtktargetentries_args {
    VALUE ary;
    long n;
    GtkTargetEntry *result;
};

static VALUE
rbgtk_rval2gtktargetentries_body(VALUE value)
{
    long i;
    struct rbgtk_rval2gtktargetentries_args *args = (struct rbgtk_rval2gtktargetentries_args *)value;

    for (i = 0; i < args->n; i++) {
        VALUE entry = rb_ary_to_ary(RARRAY_PTR(args->ary)[i]);
        VALUE flags = RARRAY_PTR(entry)[1];
        VALUE info = RARRAY_PTR(entry)[2];

        args->result[i].target = (gchar *)RVAL2CSTR_ACCEPT_NIL(RARRAY_PTR(entry)[0]);
        args->result[i].flags = NIL_P(flags) ? 0 : RVAL2GFLAGS(flags, GTK_TYPE_TARGET_FLAGS);
        args->result[i].info = NIL_P(info) ? 0 : NUM2INT(info);
    }

    return Qnil;
}

static G_GNUC_NORETURN VALUE
rbgtk_rval2gtktargetentries_rescue(VALUE value)
{
    g_free(((struct rbgtk_rval2gtktargetentries_args *)value)->result);

    rb_exc_raise(rb_errinfo());
}

GtkTargetEntry *
rbgtk_rval2gtktargetentries(VALUE value, long *n)
{
    struct rbgtk_rval2gtktargetentries_args args;

    args.ary = rb_ary_to_ary(value);
    args.n = RARRAY_LEN(args.ary);
    args.result = g_new(GtkTargetEntry, args.n + 1);

    rb_rescue(rbgtk_rval2gtktargetentries_body, (VALUE)&args,
              rbgtk_rval2gtktargetentries_rescue, (VALUE)&args);

    *n = args.n;

    return args.result;
}

GtkTargetEntry *
rbgtk_rval2gtktargetentries_accept_nil(VALUE value, long *n)
{
    if (!NIL_P(value))
        return rbgtk_rval2gtktargetentries(value, n);

    *n = 0;

    return NULL;
}

GtkTargetEntry *
rbgtk_get_target_entry(VALUE targets)
{
    long n;

    return RVAL2GTKTARGETENTRIES(targets, &n);
}

static VALUE
rg_m_dest_set(VALUE self, VALUE rbwidget, VALUE rbflags, VALUE rbtargets, VALUE rbactions)
{
    GtkWidget *widget = RVAL2WIDGET(rbwidget);
    GtkDestDefaults flags = RVAL2GFLAGS(rbflags, GTK_TYPE_DEST_DEFAULTS);
    GdkDragAction actions = RVAL2GFLAGS(rbactions, GDK_TYPE_DRAG_ACTION);
    long n;
    GtkTargetEntry *targets = RVAL2GTKTARGETENTRIES_ACCEPT_NIL(rbtargets, &n);

    gtk_drag_dest_set(widget, flags, targets, n, actions);

    g_free(targets);

    return self;
}

static VALUE
rg_m_dest_set_proxy(VALUE self, VALUE widget, VALUE proxy_window, VALUE protocol, VALUE use_coordinates)
{
    gtk_drag_dest_set_proxy(RVAL2WIDGET(widget), 
                            GDK_WINDOW(RVAL2GOBJ(proxy_window)),
                            RVAL2GENUM(protocol, GDK_TYPE_DRAG_PROTOCOL), 
                            RVAL2CBOOL(use_coordinates)); 
    return self;
}

static VALUE
rg_m_dest_unset(VALUE self, VALUE widget)
{
    gtk_drag_dest_unset(RVAL2WIDGET(widget));
    return self;
}

static VALUE
rg_m_dest_find_target(int argc, VALUE *argv, G_GNUC_UNUSED VALUE self)
{
    VALUE widget, context, target_list;
    GdkAtom ret;
    rb_scan_args(argc, argv, "21", &widget, &context, &target_list);

    ret = gtk_drag_dest_find_target(
        RVAL2WIDGET(widget), RVAL2DC(context),
        NIL_P(target_list) ? NULL : RVAL2BOXED(target_list, GTK_TYPE_TARGET_LIST));

    return BOXED2RVAL(ret, GDK_TYPE_ATOM);
}

static VALUE
rg_m_dest_get_target_list(G_GNUC_UNUSED VALUE self, VALUE widget)
{
    GtkTargetList* list = gtk_drag_dest_get_target_list(RVAL2WIDGET(widget));
    return BOXED2RVAL(list, GTK_TYPE_TARGET_LIST);
}

static VALUE
rg_m_dest_set_target_list(VALUE self, VALUE widget, VALUE target_list)
{
    gtk_drag_dest_set_target_list(
        RVAL2WIDGET(widget), 
        NIL_P(target_list) ? NULL : RVAL2BOXED(target_list, GTK_TYPE_TARGET_LIST));

    return self;
}

#if GTK_CHECK_VERSION(2,6,0)
static VALUE
rg_m_dest_add_text_targets(VALUE self, VALUE widget)
{
    gtk_drag_dest_add_text_targets(RVAL2WIDGET(widget));
    return self;
}
static VALUE
rg_m_dest_add_image_targets(VALUE self, VALUE widget)
{
    gtk_drag_dest_add_image_targets(RVAL2WIDGET(widget));
    return self;
}
static VALUE
rg_m_dest_add_uri_targets(VALUE self, VALUE widget)
{
    gtk_drag_dest_add_uri_targets(RVAL2WIDGET(widget));
    return self;
}
#endif

#if GTK_CHECK_VERSION(2,10,0)
static VALUE
rg_m_dest_set_track_motion(VALUE self, VALUE widget, VALUE track_motion)
{
    gtk_drag_dest_set_track_motion(RVAL2WIDGET(widget), RVAL2CBOOL(track_motion));
    return self;
}

static VALUE
rg_m_dest_get_track_motion(G_GNUC_UNUSED VALUE self, VALUE widget)
{
    return CBOOL2RVAL(gtk_drag_dest_get_track_motion(RVAL2WIDGET(widget)));
}
#endif

static VALUE
rg_m_finish(VALUE self, VALUE context, VALUE success, VALUE del, VALUE time)
{
    gtk_drag_finish(RVAL2DC(context), RVAL2CBOOL(success),
                    RVAL2CBOOL(del), NUM2UINT(time));
    return self;
}

static VALUE
rg_m_get_data(VALUE self, VALUE widget, VALUE context, VALUE target, VALUE time)
{
    gtk_drag_get_data(RVAL2WIDGET(widget), RVAL2DC(context), RVAL2ATOM(target),
                      NUM2UINT(time));
    return self;
}

static VALUE
rg_m_get_source_widget(G_GNUC_UNUSED VALUE self, VALUE context)
{
    return GOBJ2RVAL(gtk_drag_get_source_widget(RVAL2DC(context)));
}

static VALUE
rg_m_highlight(VALUE self, VALUE widget)
{
    gtk_drag_highlight(RVAL2WIDGET(widget));
    return self;
}

static VALUE
rg_m_unhighlight(VALUE self, VALUE widget)
{
    gtk_drag_unhighlight(RVAL2WIDGET(widget));
    return self;
}

static VALUE
rg_m_begin(G_GNUC_UNUSED VALUE self, VALUE widget, VALUE target_list, VALUE actions, VALUE button, VALUE event)
{
    return GOBJ2RVAL(gtk_drag_begin(RVAL2WIDGET(widget),
                                    RVAL2BOXED(target_list, GTK_TYPE_TARGET_LIST),
                                    RVAL2GFLAGS(actions, GDK_TYPE_DRAG_ACTION),
                                    NUM2INT(button),
                                    RVAL2GEV(event)));
}

static VALUE
rg_m_set_icon(int argc, VALUE *argv, VALUE self)
{
    VALUE context, obj, pixmap = Qnil, mask = Qnil, hot_x, hot_y;

    if (argc == 6) {
        rb_scan_args(argc, argv, "60", &context, &obj, &pixmap, &mask, &hot_x, &hot_y);
        gtk_drag_set_icon_pixmap(RVAL2DC(context),
                                 GDK_COLORMAP(RVAL2GOBJ(obj)), 
                                 GDK_PIXMAP(RVAL2GOBJ(pixmap)),
                                 GDK_BITMAP(RVAL2GOBJ(mask)), 
                                 NUM2INT(hot_x), NUM2INT(hot_y));
    } else {
        rb_scan_args(argc, argv, "40", &context, &obj, &hot_x, &hot_y);

        if (TYPE(obj) == T_SYMBOL){
            gtk_drag_set_icon_stock(RVAL2DC(context), rb_id2name(SYM2ID(obj)),
                                    NUM2INT(hot_x), NUM2INT(hot_y));
        } else if (rb_obj_is_kind_of(obj, GTYPE2CLASS(GTK_TYPE_WIDGET))){
            gtk_drag_set_icon_widget(RVAL2DC(context), RVAL2WIDGET(obj),
                                     NUM2INT(hot_x), NUM2INT(hot_y));
        } else if (rb_obj_is_kind_of(obj, GTYPE2CLASS(GDK_TYPE_PIXBUF))){
            gtk_drag_set_icon_pixbuf(RVAL2DC(context),
                                     GDK_PIXBUF(RVAL2GOBJ(obj)),
                                     NUM2INT(hot_x), NUM2INT(hot_y));
        } else {
            rb_raise(rb_eArgError, "invalid argument %s", rb_class2name(CLASS_OF(obj)));
        }
    }
    return self;
}

#if GTK_CHECK_VERSION(2,8,0)
static VALUE
rg_m_set_icon_name(VALUE self, VALUE context, VALUE name, VALUE hot_x, VALUE hot_y)
{
    gtk_drag_set_icon_name(RVAL2DC(context), RVAL2CSTR(name), NUM2INT(hot_x), NUM2INT(hot_y));
    return self;
}
#endif

static VALUE
rg_m_set_icon_default(VALUE self, VALUE context)
{
    gtk_drag_set_icon_default(RVAL2DC(context));
    return self;
}

static VALUE
rg_m_threshold_p(G_GNUC_UNUSED VALUE self, VALUE widget, VALUE start_x, VALUE start_y, VALUE current_x, VALUE current_y)
{
    return CBOOL2RVAL(gtk_drag_check_threshold(RVAL2WIDGET(widget), 
                                               NUM2INT(start_x), NUM2INT(start_y),
                                               NUM2INT(current_x), NUM2INT(current_y)));
}

static VALUE
rg_m_source_set(VALUE self, VALUE rbwidget, VALUE rbstart_button_mask, VALUE rbtargets, VALUE rbactions)
{
    GtkWidget *widget = RVAL2WIDGET(rbwidget);
    GdkModifierType start_button_mask = RVAL2GFLAGS(rbstart_button_mask, GDK_TYPE_MODIFIER_TYPE);
    GdkDragAction actions = RVAL2GFLAGS(rbactions, GDK_TYPE_DRAG_ACTION);
    long n;
    GtkTargetEntry *targets = RVAL2GTKTARGETENTRIES(rbtargets, &n);

    gtk_drag_source_set(widget, start_button_mask, targets, n, actions);

    g_free(targets);

    return self;
}

static VALUE
rg_m_source_set_icon(int argc, VALUE *argv, VALUE self)
{
    VALUE widget, obj, pixmap = Qnil, mask = Qnil;

    rb_scan_args(argc, argv, "22", &widget, &obj, &pixmap, &mask);
    if (argc == 4){
        gtk_drag_source_set_icon(RVAL2WIDGET(widget), GDK_COLORMAP(RVAL2GOBJ(obj)),
                                 GDK_PIXMAP(RVAL2GOBJ(pixmap)), 
                                 GDK_BITMAP(RVAL2GOBJ(mask)));
    } else if (argc == 2){
        if (TYPE(obj) == T_SYMBOL){
            gtk_drag_source_set_icon_stock(RVAL2WIDGET(widget), rb_id2name(SYM2ID(obj)));
        } else {
            gtk_drag_source_set_icon_pixbuf(RVAL2WIDGET(widget), GDK_PIXBUF(RVAL2GOBJ(obj)));
        }
    } else {
        rb_raise(rb_eArgError, "need 2 or 4 arguments");
    }
    return self;
}

#if GTK_CHECK_VERSION(2,8,0)
static VALUE
rg_m_source_set_icon_name(VALUE self, VALUE widget, VALUE icon_name)
{
    gtk_drag_source_set_icon_name(RVAL2WIDGET(widget), RVAL2CSTR(icon_name));
    return self;
}
#endif

static VALUE
rg_m_source_unset(VALUE self, VALUE widget)
{
    gtk_drag_source_unset(RVAL2WIDGET(widget));
    return self;
}

#if GTK_CHECK_VERSION(2,4,0)
static VALUE
rg_m_source_set_target_list(VALUE self, VALUE widget, VALUE targetlist)
{
    GtkTargetList* tlist = NULL;
    if (! NIL_P(targetlist))
        tlist = (GtkTargetList*)RVAL2BOXED(targetlist, GTK_TYPE_TARGET_LIST);

    gtk_drag_source_set_target_list(RVAL2WIDGET(widget),tlist);
    return self;
}

static VALUE
rg_m_source_get_target_list(G_GNUC_UNUSED VALUE self, VALUE widget)
{
    GtkTargetList* ret = gtk_drag_source_get_target_list(RVAL2WIDGET(widget));
    return NIL_P(ret) ? Qnil : BOXED2RVAL(ret, GTK_TYPE_TARGET_LIST);
}
#endif

#if GTK_CHECK_VERSION(2,6,0)
static VALUE
rg_m_source_add_text_targets(VALUE self, VALUE widget)
{
    gtk_drag_source_add_text_targets(RVAL2WIDGET(widget));
    return self;
}
static VALUE
rg_m_source_add_image_targets(VALUE self, VALUE widget)
{
    gtk_drag_source_add_image_targets(RVAL2WIDGET(widget));
    return self;
}
static VALUE
rg_m_source_add_uri_targets(VALUE self, VALUE widget)
{
    gtk_drag_source_add_uri_targets(RVAL2WIDGET(widget));
    return self;
}
#endif

void
Init_gtk_drag(VALUE mGtk)
{
    VALUE RG_TARGET_NAMESPACE = rb_define_module_under(mGtk, "Drag");

    RG_DEF_MODFUNC(dest_set, 4);
    RG_DEF_MODFUNC(dest_set_proxy, 4);
    RG_DEF_MODFUNC(dest_unset, 1);
    RG_DEF_MODFUNC(dest_find_target, -1);
    RG_DEF_MODFUNC(dest_get_target_list, 1);
    RG_DEF_MODFUNC(dest_set_target_list, 2);
#if GTK_CHECK_VERSION(2,6,0)
    RG_DEF_MODFUNC(dest_add_text_targets, 1);
    RG_DEF_MODFUNC(dest_add_image_targets, 1);
    RG_DEF_MODFUNC(dest_add_uri_targets, 1);
#endif
#if GTK_CHECK_VERSION(2,10,0)
    RG_DEF_MODFUNC(dest_set_track_motion, 2);
    RG_DEF_MODFUNC(dest_get_track_motion, 1);
#endif
    RG_DEF_MODFUNC(finish, 4);
    RG_DEF_MODFUNC(get_data, 4);
    RG_DEF_MODFUNC(get_source_widget, 1);
    RG_DEF_MODFUNC(highlight, 1);
    RG_DEF_MODFUNC(unhighlight, 1);
    RG_DEF_MODFUNC(begin, 5);
    RG_DEF_MODFUNC_P(threshold, 5);
    RG_DEF_MODFUNC(set_icon, -1);
#if GTK_CHECK_VERSION(2,8,0)
    RG_DEF_MODFUNC(set_icon_name, 4);
#endif
    RG_DEF_MODFUNC(set_icon_default, 1);
    RG_DEF_MODFUNC(source_set, 4);
    RG_DEF_MODFUNC(source_set_icon, -1);
#if GTK_CHECK_VERSION(2,8,0)
    RG_DEF_MODFUNC(source_set_icon_name, 2);
#endif
    RG_DEF_MODFUNC(source_unset, 1);
#if GTK_CHECK_VERSION(2,4,0)
    RG_DEF_MODFUNC(source_set_target_list, 2);
    RG_DEF_MODFUNC(source_get_target_list, 1);
#endif
#if GTK_CHECK_VERSION(2,6,0)
    RG_DEF_MODFUNC(source_add_text_targets, 1);
    RG_DEF_MODFUNC(source_add_image_targets, 1);
    RG_DEF_MODFUNC(source_add_uri_targets, 1);
#endif
    G_DEF_SETTERS(RG_TARGET_NAMESPACE);

    /* GtkDestDefaults */
    G_DEF_CLASS(GTK_TYPE_DEST_DEFAULTS, "DestDefaults", RG_TARGET_NAMESPACE);
    G_DEF_CONSTANTS(RG_TARGET_NAMESPACE, GTK_TYPE_DEST_DEFAULTS, "GTK_");

    /* GtkTargetFlags */
    G_DEF_CLASS(GTK_TYPE_TARGET_FLAGS, "TargetFlags", RG_TARGET_NAMESPACE);
    G_DEF_CONSTANTS(RG_TARGET_NAMESPACE, GTK_TYPE_TARGET_FLAGS, "GTK_");
}
