/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkdrag.c -

  $Author: mutoh $
  $Date: 2002/10/30 13:34:36 $

  Copyright (C) 2002 Masao Mutoh
************************************************/


#include "global.h"

#define RVAL2DC(c) (GDK_DRAG_CONTEXT(RVAL2GOBJ(c)))
#define RVAL2WIDGET(w) (GTK_WIDGET(RVAL2GOBJ(w)))

static GtkTargetEntry*
get_target_entry(targets)
	VALUE targets;
{
    VALUE ary;
    VALUE e_target, e_flags, e_info;
    GtkTargetEntry *entries;
    int i, n_targets;
    
    Check_Type(targets, T_ARRAY);
    
    n_targets = RARRAY(targets)->len;
    entries = ALLOC_N(GtkTargetEntry, n_targets);
    
    for (i = 0; i < n_targets; i++) {
        ary = rb_ary_entry(targets, i);
        Check_Type(ary, T_ARRAY);
        e_target = rb_ary_entry(ary, 0);
        e_flags = rb_ary_entry(ary, 1);
        e_info = rb_ary_entry(ary, 2);
        
        entries[i].target = NIL_P(e_target) ? NULL:RVAL2CSTR(e_target);
        entries[i].flags = NIL_P(e_flags) ? 0:NUM2INT(e_flags);
        entries[i].info = NIL_P(e_info) ? 0:NUM2INT(e_info);

    }
    return entries;
}

static VALUE
gtkdrag_drag_dest_set(self, widget, flags, targets, actions)
    VALUE self, widget, flags, targets, actions;
{
    GtkTargetEntry *entries = get_target_entry(targets);
    int num = RARRAY(targets)->len;

    gtk_drag_dest_set(RVAL2WIDGET(widget), NUM2INT(flags), entries, 
                      num, NUM2INT(actions));
    return self;
}

static VALUE
gtkdrag_drag_dest_set_proxy(self, widget, proxy_window, protocol, use_coordinates)
    VALUE self, widget, proxy_window, protocol, use_coordinates;
{
    gtk_drag_dest_set_proxy(RVAL2WIDGET(widget), 
                            GDK_WINDOW(RVAL2GOBJ(proxy_window)),
                            NUM2INT(protocol), RTEST(use_coordinates)); 
    return self;
}

static VALUE
gtkdrag_drag_dest_unset(self, widget)
    VALUE self,widget;
{
    gtk_drag_dest_unset(RVAL2WIDGET(widget));
    return self;
}

/*
GdkAtom     gtk_drag_dest_find_target       (GtkWidget *widget,
                                             GdkDragContext *context,
                                             GtkTargetList *target_list);
GtkTargetList* gtk_drag_dest_get_target_list
                                            (GtkWidget *widget);
void        gtk_drag_dest_set_target_list   (GtkWidget *widget,
                                             GtkTargetList *target_list);
*/

static VALUE
gtkdrag_finish(self, context, success, del, time)
    VALUE self, context, success, del, time;
{
    gtk_drag_finish(RVAL2DC(context), RTEST(success),
                    RTEST(del), NUM2INT(time));
    return self;
}

static VALUE
gtkdrag_drag_get_data(self, widget, context, target, time)
    VALUE self, widget, context, target, time;
{
    gtk_drag_get_data(RVAL2WIDGET(widget), RVAL2DC(context),
                      (((GdkAtomData*)RVAL2BOXED(target, GDK_TYPE_ATOM))->atom), 
                      NUM2INT(time));
    return self;
}

static VALUE
gtkdrag_get_source_widget(self, context)
    VALUE self, context;
{
    return GOBJ2RVAL(gtk_drag_get_source_widget(RVAL2DC(context)));
}

static VALUE
gtkdrag_highlight(self, widget)
    VALUE self, widget;
{
    gtk_drag_highlight(RVAL2WIDGET(widget));
    return self;
}

static VALUE
gtkdrag_unhighlight(self, widget)
    VALUE self, widget;
{
    gtk_drag_unhighlight(RVAL2WIDGET(widget));
    return self;
}

static VALUE
gtkdrag_begin(self, widget, targets, actions, button, event)
    VALUE self, widget, targets, actions, button, event;
{
/*	return GOBJ2RVAL(gtk_drag_begin(RVAL2GOBJ(self), );
GdkDragContext* gtk_drag_begin              (GtkWidget *widget,
                                             GtkTargetList *targets,
                                             GdkDragAction actions,
                                             gint button,
                                             GdkEvent *event); */

    rb_notimplement();
    return self;
}

static VALUE
gtkdrag_set_icon_widget(self, context, widget, hot_x, hot_y)
    VALUE self, context, widget, hot_x, hot_y;
{
    gtk_drag_set_icon_widget(RVAL2DC(context), RVAL2WIDGET(widget),
                             NUM2INT(hot_x), NUM2INT(hot_y));
    return self;
}

static VALUE
gtkdrag_set_icon_pixmap(self, context, colormap, pixmap, mask, hot_x, hot_y)
    VALUE self, context, colormap, pixmap, mask, hot_x, hot_y;
{
    gtk_drag_set_icon_pixmap(RVAL2DC(context),
                             GDK_COLORMAP(RVAL2GOBJ(colormap)), 
                             GDK_PIXMAP(RVAL2GOBJ(pixmap)),
                             GDK_BITMAP(RVAL2GOBJ(mask)), 
                             NUM2INT(hot_x), NUM2INT(hot_y));
    return self;
}

/*
void        gtk_drag_set_icon_pixbuf        (GdkDragContext *context,
                                             GdkPixbuf *pixbuf,
                                             gint hot_x,
                                             gint hot_y);
void        gtk_drag_set_icon_stock         (GdkDragContext *context,
                                             const gchar *stock_id,
                                             gint hot_x,
                                             gint hot_y);
*/

static VALUE
gtkdrag_set_default_icon(self, context)
    VALUE self, context;
{
    gtk_drag_set_icon_default(RVAL2DC(context));
    return Qnil;
}

/*
gboolean    gtk_drag_check_threshold        (GtkWidget *widget,
                                             gint start_x,
                                             gint start_y,
                                             gint current_x,
                                             gint current_y);
*/

static VALUE
gtkdrag_drag_source_set(self, widget, flags, targets, actions)
    VALUE self, flags, targets, actions;
{
    gtk_drag_source_set(RVAL2WIDGET(widget), NUM2INT(flags), 
                        get_target_entry(targets), 
                        RARRAY(targets)->len, NUM2INT(actions));
    return self;
}

static VALUE
gtkdrag_drag_source_set_icon(self, widget, colormap, pixmap, mask)
    VALUE self, widget, colormap, pixmap, mask;
{
    gtk_drag_source_set_icon(RVAL2WIDGET(widget), GDK_COLORMAP(RVAL2GOBJ(colormap)),
                             GDK_PIXMAP(RVAL2GOBJ(pixmap)), 
                             GDK_BITMAP(RVAL2GOBJ(mask)));
    return self;
}

static VALUE
gtkdrag_drag_source_unset(self, widget)
    VALUE self, widget;
{
    gtk_drag_source_unset(RVAL2WIDGET(widget));
    return self;
}

/*
void        gtk_drag_source_set_icon_pixbuf (GtkWidget *widget,
                                             GdkPixbuf *pixbuf);
void        gtk_drag_source_set_icon_stock  (GtkWidget *widget,
                                             const gchar *stock_id);
*/


void
Init_gtk_drag()
{
    VALUE mGtkDrag = rb_define_module_under(mGtk, "Drag");

    rb_define_module_function(mGtkDrag, "dest_set", gtkdrag_drag_dest_set, 4);
    rb_define_module_function(mGtkDrag, "dest_set_proxy", gtkdrag_drag_dest_set_proxy, 4);
    rb_define_module_function(mGtkDrag, "dest_unset", gtkdrag_drag_dest_unset, 1);
    rb_define_module_function(mGtkDrag, "finish", gtkdrag_finish, 4);
    rb_define_module_function(mGtkDrag, "get_data", gtkdrag_drag_get_data, 4);
    rb_define_module_function(mGtkDrag, "get_source_widget", gtkdrag_get_source_widget, 1);
    rb_define_module_function(mGtkDrag, "highlight", gtkdrag_highlight, 1);
    rb_define_module_function(mGtkDrag, "unhighlight", gtkdrag_unhighlight, 1);
    rb_define_module_function(mGtkDrag, "begin", gtkdrag_begin, 5);
    rb_define_module_function(mGtkDrag, "set_icon_widget", gtkdrag_set_icon_widget, 4);
    rb_define_module_function(mGtkDrag, "set_icon_pixmap", gtkdrag_set_icon_pixmap, 6);
    rb_define_module_function(mGtkDrag, "set_default_icon", gtkdrag_set_default_icon, 1);
    rb_define_module_function(mGtkDrag, "source_set", gtkdrag_drag_source_set, 4);
    rb_define_module_function(mGtkDrag, "source_set_icon", gtkdrag_drag_source_set_icon, 4);
    rb_define_module_function(mGtkDrag, "source_unset", gtkdrag_drag_source_unset, 1);

    G_DEF_SETTERS(mGtkDrag);

    /* GtkDestDefaults */
    rb_define_const(mGtkDrag, "DEST_DEFAULT_MOTION", INT2FIX(GTK_DEST_DEFAULT_MOTION));
    rb_define_const(mGtkDrag, "DEST_DEFAULT_HIGHLIGHT", INT2FIX(GTK_DEST_DEFAULT_HIGHLIGHT));
    rb_define_const(mGtkDrag, "DEST_DEFAULT_DROP", INT2FIX(GTK_DEST_DEFAULT_DROP));
    rb_define_const(mGtkDrag, "DEST_DEFAULT_ALL", INT2FIX(GTK_DEST_DEFAULT_ALL));

    /* GtkTargetFlags */
    rb_define_const(mGtkDrag, "TARGET_SAME_APP", INT2FIX(GTK_TARGET_SAME_APP));
    rb_define_const(mGtkDrag, "TARGET_SAME_WIDGET", INT2FIX(GTK_TARGET_SAME_WIDGET));
}
