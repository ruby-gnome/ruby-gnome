/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkdrag.c -

  $Author: sakai $
  $Date: 2003/07/20 05:05:08 $

  Copyright (C) 2002,2003 Masao Mutoh
************************************************/


#include "global.h"

#define RVAL2DC(c) (GDK_DRAG_CONTEXT(RVAL2GOBJ(c)))
#define RVAL2WIDGET(w) (GTK_WIDGET(RVAL2GOBJ(w)))

GtkTargetEntry*
rbgtk_get_target_entry(targets)
	VALUE targets;
{
    VALUE ary;
    VALUE e_target, e_flags, e_info;
    GtkTargetEntry *entries;
    int i, n_targets;
    
    if (NIL_P(targets)) return NULL;
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
    int num;
    GtkTargetEntry* entries = rbgtk_get_target_entry(targets);
    if (entries){
        num = RARRAY(targets)->len;
        
        gtk_drag_dest_set(RVAL2WIDGET(widget), NUM2INT(flags), entries, 
                          num, NUM2INT(actions));
    }
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
    gtk_drag_get_data(RVAL2WIDGET(widget), RVAL2DC(context), RVAL2ATOM(target),
                      NUM2ULONG(time));
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
gtkdrag_set_icon(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
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

static VALUE
gtkdrag_set_icon_default(self, context)
    VALUE self, context;
{
    gtk_drag_set_icon_default(RVAL2DC(context));
    return Qnil;
}

static VALUE
gtkdrag_check_threshold(self, widget, start_x, start_y, current_x, current_y)
    VALUE self, widget, start_x, start_y, current_x, current_y;
{
    return gtk_drag_check_threshold(RVAL2WIDGET(widget), 
                                    NUM2INT(start_x), NUM2INT(start_y),
                                    NUM2INT(current_x), NUM2INT(current_y)) ? Qtrue : Qfalse;
}

static VALUE
gtkdrag_drag_source_set(self, widget, flags, targets, actions)
    VALUE self, flags, targets, actions;
{
    gtk_drag_source_set(RVAL2WIDGET(widget), NUM2INT(flags), 
                        rbgtk_get_target_entry(targets), 
                        RARRAY(targets)->len, NUM2INT(actions));
    return self;
}

static VALUE
gtkdrag_drag_source_set_icon(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
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

static VALUE
gtkdrag_drag_source_unset(self, widget)
    VALUE self, widget;
{
    gtk_drag_source_unset(RVAL2WIDGET(widget));
    return self;
}

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
    rb_define_module_function(mGtkDrag, "threshold?", gtkdrag_check_threshold, 5);
    rb_define_module_function(mGtkDrag, "set_icon", gtkdrag_set_icon, -1);
    rb_define_module_function(mGtkDrag, "set_icon_default", gtkdrag_set_icon_default, 1);
    rb_define_module_function(mGtkDrag, "source_set", gtkdrag_drag_source_set, 4);
    rb_define_module_function(mGtkDrag, "source_set_icon", gtkdrag_drag_source_set_icon, -1);
    rb_define_module_function(mGtkDrag, "source_unset", gtkdrag_drag_source_unset, 1);

    G_DEF_SETTERS(mGtkDrag);

    /* GtkDestDefaults */
    G_DEF_CONSTANTS(mGtkDrag, GTK_TYPE_DEST_DEFAULTS, "GTK_");

    /* GtkTargetFlags */
    G_DEF_CONSTANTS(mGtkDrag, GTK_TYPE_TARGET_FLAGS, "GTK_");
}
